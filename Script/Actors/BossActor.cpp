//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "BossActor.h"
#include "ArrowActor.h"
#include "AudioComponent.h"
#include "BossCamera.h"
#include "DefeatCamera.h"
#include "EnemyActor.h"
#include "ExplosionActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PlaneActor.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "SmokeActor.h"
#include "TargetComponent.h"


///////////////////////////////////////////////////////////////////////////////
// BossActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
BossActor::BossActor(Game* game)
	:EnemyActor(game)
{
	name = "EnemyBoss";
	SetScale(2.0f);
	mMeshComp = new SkeletalMeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/"+ name +".gpmesh");
	mMeshComp->SetMesh(mesh);
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Skel/" + name + ".gpskel"));
	GetGame()->AddBossActor(this);
	GetGame()->AddEnemy(this);
	mAnimState = EnemyActor::AnimationState::Idle;
	new TargetComponent(this);
	PointLightComponent* pointLight = new PointLightComponent(this);
	pointLight->SetCol(Vector3(1.0, 0.6, 0.0));
	pointLight->SetInRad(0.0);
	pointLight->SetOutRad(400.0);
	pointLight->SetPosition(GetPosition() + Vector3(200.0f, 0.0f, 300.0f));
	bossCam = new BossCamera(this);
}


BossActor::~BossActor()
{

	GetGame()->RemoveEnemy(this);
	GetGame()->RemoveBossActor(this);

}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void BossActor::UpdateActor(float deltaTime) {

	if (headBox == nullptr) {
		headBox = new BoxComponent(this);
		AABB hBox(Vector3(-75.0f, -75.0f, 130.0f),
			Vector3(75.0f, 75.0f, 180.0f));
		headBox->SetObjectBox(hBox);
		headTime = 0.1f;
	}
	
	if (headTime <= 0.0f) {
		delete headBox;
		headBox = nullptr;
	}
	headTime -= deltaTime;

	if (GetGame()->GetState() == Game::GameState::EBossMovie) {
		if (mAnimState != EnemyActor::AnimationState::Roa) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_roa.gpanim"), 1.0f);
			mAudioComp->PlayEvent("event:/Boss");
			mAnimState = EnemyActor::AnimationState::Roa;
		}

		Vector3 playerPosition = bossCam->GetCameraPos();
		Vector3 pos = GetPosition();
		diff = playerPosition - pos;
		float distanceSq = diff.LengthSq();
		AlignToTarget(); // ターゲットに向けて回転
	}
	else if (GetGame()->GetState() == Game::GameState::EBossDefeat) {
		mMoveComp->SetForwardSpeed(0.0f);
		mMoveComp->SetStrafeSpeed(0.0f);
		Vector3 playerPosition = defCam->GetCameraPos();
		Vector3 pos = GetPosition();
		diff = playerPosition - pos;
		float distanceSq = diff.LengthSq();
		AlignToTarget(); // ターゲットに向けて回転
	}

	else {
		if (bossCam) {
			delete bossCam;
			bossCam = nullptr;
		}
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
		auto* player = GetGame()->GetPlayer();
		Vector3 playerPosition = player->GetPosition();
		Vector3 pos = GetPosition();
		diff = playerPosition - pos;
		float distanceSq = diff.LengthSq();

		//距離によって状態遷移
		if (mHealth > 0.0f) {
			if (diff.LengthSq() > 5000000.0f) {
				mMoveState = EPatrol;
			}
			else if (diff.LengthSq() <= 5000000.0f && diff.LengthSq() > 250000.0f) {
				mMoveState = EBattle;
			}
			else if (diff.LengthSq() <= 250000.0f) {
				mMoveState = EAttack;
			}
		}

		UpdateMoveState(deltaTime);

		if ((!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)) && groundFlag == true && mAnimState != EnemyActor::AnimationState::Walk)
		{

			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_walk.gpanim"), 1.0f);
			mAnimState = EnemyActor::AnimationState::Walk;

		}

		else if (Math::NearZero(forwardSpeed) && Math::NearZero(strafeSpeed) && groundFlag == true && !mAttackBoxComp && deathFlag && mAnimState != EnemyActor::AnimationState::Idle)
		{

			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_idle.gpanim"), 1.0f);
			mAnimState = EnemyActor::AnimationState::Idle;

		}

		if (mState == EJump) {

			jumpSpeed += 50000.0f * deltaTime;
			if (jumpSpeed >= 0.0f) {
				mState = EFall;
			}
		}

		else if (mState == EFall) {
			jumpSpeed += 15000.0f * deltaTime;
		}

		else if (mState == EGrounded) {
			jumpSpeed = 0.0f;

		}

		//攻撃時静止させる
		if (mAttackBoxComp != nullptr) {
			forwardSpeed = 0.0f;
			strafeSpeed = 0.0f;
		}

		mMoveComp->SetForwardSpeed(forwardSpeed);
		mMoveComp->SetStrafeSpeed(strafeSpeed);
		mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);
		mState = EFall;
		FixCollisions();
		
	}
	UpdateTimers(deltaTime);

}


//-----------------------------------------------------------------------------
//　AttackStateの処理
//-----------------------------------------------------------------------------
void BossActor::UpdateAttackState(float deltaTime) {
	forwardSpeed += 100.0f;
	AlignToTarget(); // ターゲットに向けて回転
	mMoveTimer -= deltaTime;

	if (diff.LengthSq() <= 50000.0f) {
		forwardSpeed = 0.0f;

		if (randomValue == 0) {
			strafeSpeed += 50.0f;
		}
		else if (randomValue == 1) {
			strafeSpeed -= 50.0f;
		}
		else if ((randomValue == 2 || randomValue == 3 || randomValue == 4) && mAttackTimer <= 0.0f) {
			(randomValue == 2 || randomValue == 3) ? Attack() : AttackGround();
		}

		if (mMoveTimer <= 0.0f) {
			randomValue = GenerateRandomValue(5);
			mMoveTimer = 2.0f + static_cast<float>(randomValue);
		}
	}
}


//-----------------------------------------------------------------------------
//　死亡処理
//-----------------------------------------------------------------------------
void BossActor::HandleDeath() {

	if (mHealth <= 0.0f) {
		if (deathFlag) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_dying.gpanim"), 1.0f);
			mAnimState = EnemyActor::AnimationState::Die;
			deathFlag = false;
			mAudioComp->PlayEvent("event:/Boss");
			SetPosition(Vector3(-700.0f, 0.0f, -100.0f));
			defCam = new DefeatCamera(this);
			GetGame()->SetState(Game::GameState::EBossDefeat);
			
			
		}

		if (mDamageTimer <= -2.0f) {
			GetGame()->SetState(Game::GameState::EGameClear);
			Actor::SetState(Actor::EDead);
		}
		mHealth = 0.0f;
	}

}


//-----------------------------------------------------------------------------
//　攻撃判定生成処理
//-----------------------------------------------------------------------------
void BossActor::AddAttackBox() {
	if (mHealth > 0.0f) {
		mAttackBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(-150.0f, -125.0f, 0.0f), Vector3(150.0f, 125.0f, 10.0f));
		mAttackBoxComp->SetObjectBox(myBox);
		mAttackBoxComp->SetShouldRotate(true);
		SmokeActor* smoke = new SmokeActor(GetGame());
		smoke->SetPosition(GetPosition());
		smoke->SetScale(250.0f);
		groundFlag = true;
	}

}


//-----------------------------------------------------------------------------
//　衝突処理
//-----------------------------------------------------------------------------
void BossActor::FixCollisions()
{

	ComputeWorldTransform();
	const AABB& playerBox = mBox->GetWorldBox();
	
	const AABB& attackBox = mAttackBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();
	auto& planes = GetGame()->GetPlanes();
	auto* player = GetGame()->GetPlayer();
	auto& enemy = GetGame()->GetEnemys();
	auto& arrow = GetGame()->GetArrow();
	auto& explosion = GetGame()->GetExplosion();
	if (player != nullptr) {

		const AABB& enemyBox = player->GetBox()->GetWorldBox();
		ResolveCollision(playerBox, enemyBox, pos, mBox);

		if (player->GetAttackBox() != nullptr)
		{
			const AABB& enemyAttackBox = player->GetAttackBox()->GetWorldBox();

			if (headBox != nullptr) {
				const AABB& hBox= headBox->GetWorldBox();
				if (Intersect(hBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
					mAudioComp->PlayEvent("event:/Boss");
					mHealth -= 0.15f;
					GetGame()->SetHitStopTimer();
					mDamageTimer = 0.5f;
				}
				else if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
					mAudioComp->PlayEvent("event:/EnemyHit");
					mHealth -= 0.05f;
					GetGame()->SetHitStopTimer();
					mDamageTimer = 0.5f;
				}
			}

		}
	}
	else
	{
	}

	for (auto pa : planes)
	{
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		ResolveCollision(playerBox, planeBox, pos, mBox);
	}

	for (auto en : enemy)
	{
		if (en != nullptr && en != this) {

			const AABB& enemyBox = en->GetBox()->GetWorldBox();
			ResolveCollision(playerBox, enemyBox, pos, mBox);
		}
	}

	for (auto ar : arrow)
	{
		if (ar != nullptr) {

			const AABB& arBox = ar->GetBox()->GetWorldBox();
			if (headBox != nullptr) {
				const AABB& hBox = headBox->GetWorldBox();
				if (Intersect(hBox, arBox) && mDamageTimer <= 0.0f)
				{
					mAudioComp->PlayEvent("event:/Boss");
					mHealth -= 0.15f;
					mDamageTimer = 0.5f;
				}
				else if (Intersect(playerBox, arBox) && mDamageTimer <= 0.0f)
				{
					mAudioComp->PlayEvent("event:/Hit");
					mHealth -= 0.05f;
					mDamageTimer = 0.5f;
				}
			}



		}
	}

	for (auto ex : explosion)
	{
		if (ex != nullptr) {

			const AABB& exBox = ex->GetBox()->GetWorldBox();
			if (Intersect(playerBox, exBox) && mDamageTimer <= 0.0f)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mHealth -= 0.1f;
				mDamageTimer = 2.0f;

			}
		}
	}
}
