//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "EnemyActor.h"
#include "ArrowActor.h"
#include "AudioComponent.h"
#include "BombActor.h"
#include "BoxComponent.h"
#include "DropItemActor.h"
#include "ExplosionActor.h"
#include "FollowActor.h"
#include "FollowCamera.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PlaneActor.h"
#include "Random.h"
#include "ReactActor.h"
#include "Renderer.h"
#include "SmokeActor.h"
#include "SkeletalMeshComponent.h"
#include "TargetComponent.h"


///////////////////////////////////////////////////////////////////////////////
// EnemyActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
EnemyActor::EnemyActor(Game* game)
	:Actor(game)
	, mMoving(false)
	, mState(EGrounded)
	, jumpSpeed(0.0f)
	, mHealth(1.0f)
	, mAttackBoxComp(nullptr)
	, mAttackTimer(0.0f)
	, mDamageTimer(0.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.1f)
	, isVisible(true)
	, randomValue(0)
	, mMoveTimer(0.0f)
	, mBoxTimer(0.0f)
	, mReactFlag(true)
	, groundFlag(true)
	, deathFlag(true)

{
	name = "Enemy";
	mMeshComp = new SkeletalMeshComponent(this);
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	new TargetComponent(this);
	// Add a box component\	
	mBox = new BoxComponent(this);
	AABB myBox(Vector3(-75.0f, -75.0f, 0.0f),
		Vector3(75.0f, 75.0f, 170.0f));
	mBox->SetObjectBox(myBox);
	mBox->SetShouldRotate(false);
	srand(static_cast<unsigned int>(time(0)));
    game->AddEnemy(this);
}



//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
EnemyActor::~EnemyActor()
{
	
	GetGame()->RemoveEnemy(this);
		
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void EnemyActor::UpdateActor(float deltaTime){

	forwardSpeed = 0.0f;
	strafeSpeed = 0.0f;
	auto* player = GetGame()->GetPlayer();
	Vector3 playerPosition = player->GetPosition();
	Vector3 pos = GetPosition();
	diff = playerPosition - pos;
	float distanceSq = diff.LengthSq();

	//距離によって状態遷移
	if (mHealth > 0.0f) {
		if (diff.LengthSq() > 1000000.0f) {
			mMoveState = EPatrol;
		}
		else if (diff.LengthSq() <= 200000.0f && diff.LengthSq() > 100000.0f) {
			mMoveState = EBattle;
		}
		else if (diff.LengthSq() <= 100000.0f) {
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
	UpdateTimers(deltaTime);

}


//-----------------------------------------------------------------------------
//   可視状態の切り替え
//-----------------------------------------------------------------------------
void EnemyActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}


//-----------------------------------------------------------------------------
//  MoveStateの切り替え処理
//-----------------------------------------------------------------------------
void EnemyActor::UpdateMoveState(float deltaTime) {
	if (mHealth <= 0.0f) {
		return;
	}

	switch (mMoveState){
	case EPatrol:
		UpdatePatrolState(deltaTime);
		break;

	case EBattle:
		UpdateBattleState(deltaTime);
		break;

	case EAttack:
		UpdateAttackState(deltaTime);
		break;
	}
}


//-----------------------------------------------------------------------------
//　PatrolStateの処理
//-----------------------------------------------------------------------------
void EnemyActor::UpdatePatrolState(float deltaTime) {
	mReactFlag = false;
	mMoveTimer -= deltaTime;

	if (randomValue == 0) {
		forwardSpeed += 100.0f;
	}

	if (mMoveTimer <= 0.0f) {
		SetRandomRotation();
		randomValue = GenerateRandomValue(2); // 0から1のランダムな整数
		mMoveTimer = 3.0f + static_cast<float>(randomValue);
	}
}


//-----------------------------------------------------------------------------
//　BattleStateの処理
//-----------------------------------------------------------------------------
void EnemyActor::UpdateBattleState(float deltaTime) {
	if (!mReactFlag) {
		mReactFlag = true;
		CreateReactActor();
	}

	AlignToTarget(); // ターゲットに向けて回転
	mMoveTimer -= deltaTime;

	if (randomValue <= 1) {
		forwardSpeed += 200.0f;
	}
	else if (randomValue == 2) {
		forwardSpeed -= 50.0f;
	}

	if (mMoveTimer <= 0.0f) {
		SetRandomRotation();
		randomValue = GenerateRandomValue(4); // 0から3のランダムな整数
		mMoveTimer = 2.0f + static_cast<float>(randomValue);
	}
}


//-----------------------------------------------------------------------------
//　AttackStateの処理
//-----------------------------------------------------------------------------
void EnemyActor::UpdateAttackState(float deltaTime) {
	forwardSpeed += 100.0f;
	AlignToTarget(); // ターゲットに向けて回転
	mMoveTimer -= deltaTime;

	if (diff.LengthSq() <= 20000.0f) {
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
//　PatrolState時のランダムな回転
//-----------------------------------------------------------------------------
void EnemyActor::SetRandomRotation() {
	SetRotation(Quaternion::CreateFromAxisAngle(Random::GetFloatRange(0.0f, Math::TwoPi)));
}


//-----------------------------------------------------------------------------
//　BattleState時のReactActor生成
//-----------------------------------------------------------------------------
void EnemyActor::CreateReactActor() {
	ReactActor* react = new ReactActor(GetGame());
	react->SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 250.0f));
}


//-----------------------------------------------------------------------------
//　FollowActorの方向を向く
//-----------------------------------------------------------------------------
void EnemyActor::AlignToTarget() {
	float angle = atan2(diff.y, diff.x);
	Quaternion rotation = Quaternion::CreateFromAxisAngle(angle);
	SetRotation(rotation);
}


//-----------------------------------------------------------------------------
//　攻撃処理
//-----------------------------------------------------------------------------
void EnemyActor::Attack() {

	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(50.0f, -50.0f, 50.0f),
		Vector3(125.0f, 50.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_attack.gpanim"), 1.0f);
	mAnimState = EnemyActor::AnimationState::Attack;
	mBoxTimer = 0.5f;
	mAttackTimer = 4.0f;
	groundFlag = true;
	mAudioComp->PlayEvent("event:/EnemyAttack");
}


//-----------------------------------------------------------------------------
//　ジャンプ攻撃処理
//-----------------------------------------------------------------------------
void EnemyActor::AttackGround() {

	mBoxTimer = 2.1f;
	mAttackTimer = 4.0f;
	groundFlag = false;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_jump_attack.gpanim"), 1.0f);
	mAnimState = EnemyActor::AnimationState::Attack;
	mAudioComp->PlayEvent("event:/EnemyAttack2");
}


//-----------------------------------------------------------------------------
//　各Timerの処理
//-----------------------------------------------------------------------------
void EnemyActor::UpdateTimers(float deltaTime) {

	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;
	}
	mDamageTimer -= deltaTime;
	HandleDeath();
	if (mDamageTimer > 0.0f) {	
		HandleBlinking(deltaTime);
	}
	
	else {

		mMeshComp->SetVisible(true);
		blinkTime = 0.0f;

	}
	
	if (mBoxTimer > 0.0f) {
		mBoxTimer -= deltaTime;
		HandleAttackBox();
	}

}


//-----------------------------------------------------------------------------
//　攻撃を受けた時の点滅処理
//-----------------------------------------------------------------------------
void EnemyActor::HandleBlinking(float deltaTime) {
	blinkTime += deltaTime;

	if (blinkTime >= blinkInterval) {
		isVisible = !isVisible;
		mMeshComp->SetVisible(isVisible);
		blinkTime = 0.0f;
	}

}


//-----------------------------------------------------------------------------
//　死亡処理
//-----------------------------------------------------------------------------
void EnemyActor::HandleDeath() {
	if (mHealth <= 0.0f) {
		if (deathFlag) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_dying.gpanim"), 1.0f);
			mAnimState = EnemyActor::AnimationState::Die;
			deathFlag = false;
		}

		if (mDamageTimer <= -4.0f) {
			
			DropItemActor* dropitem = new DropItemActor(GetGame());
			dropitem->SetPosition(GetPosition());
			
			Actor::SetState(Actor::EDead);
		}
		mHealth = 0.0f;
	}
}


//-----------------------------------------------------------------------------
//　攻撃判定管理
//-----------------------------------------------------------------------------
void EnemyActor::HandleAttackBox() {
	if (mBoxTimer <= 0.0f) {
		CleanUpAttackBox();
	}
	else if (mBoxTimer <= 0.3f && !groundFlag) {
		AddAttackBox();
	}
}


//-----------------------------------------------------------------------------
//　攻撃判定削除
//-----------------------------------------------------------------------------
void EnemyActor::CleanUpAttackBox() {
	delete mAttackBoxComp;
	mAttackBoxComp = nullptr;

	if (deathFlag) {
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_idle.gpanim"), 1.0f);
		mAnimState = EnemyActor::AnimationState::Idle;
	}
}


//-----------------------------------------------------------------------------
//　攻撃判定生成処理
//-----------------------------------------------------------------------------
void EnemyActor::AddAttackBox() {
	if (mHealth > 0.0f) {
		mAttackBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(-150.0f, -125.0f, 0.0f), Vector3(150.0f, 125.0f, 10.0f));
		mAttackBoxComp->SetObjectBox(myBox);
		mAttackBoxComp->SetShouldRotate(true);
		SmokeActor* smoke = new SmokeActor(GetGame());
		smoke->SetPosition(GetPosition());

		groundFlag = true;
	}
}


//-----------------------------------------------------------------------------
//　Jsonファイルからのデータの読み取り
//-----------------------------------------------------------------------------
void EnemyActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}


//-----------------------------------------------------------------------------
//　Jsonファイルへのデータの書き込み
//-----------------------------------------------------------------------------
void EnemyActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}


//-----------------------------------------------------------------------------
//　衝突処理
//-----------------------------------------------------------------------------
void EnemyActor::FixCollisions()
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
			if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f && deathFlag) {
				mAudioComp->PlayEvent("event:/EnemyHit");
				GetGame()->SetHitStopTimer();
				mHealth -= 0.5f;
				mDamageTimer = 0.5f;
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
			if (Intersect(playerBox, arBox) && mDamageTimer <= 0.0f && deathFlag)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mHealth -= 0.5f;
				mDamageTimer = 0.5f;
			}
		}
	}

	for (auto ex : explosion)
	{
		if (ex != nullptr) {

			const AABB& exBox = ex->GetBox()->GetWorldBox();
			if (Intersect(playerBox, exBox) && mDamageTimer <= 0.0f && deathFlag)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mHealth -= 1.0f;
				mDamageTimer = 0.5f;

			}
		}
	}
}


//-----------------------------------------------------------------------------
//　衝突時に重ならないように
//-----------------------------------------------------------------------------
void EnemyActor::ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent)
{
	if (Intersect(aBox, bBox))
	{
		float dx1 = bBox.mMax.x - aBox.mMin.x;
		float dx2 = bBox.mMin.x - aBox.mMax.x;
		float dy1 = bBox.mMax.y - aBox.mMin.y;
		float dy2 = bBox.mMin.y - aBox.mMax.y;
		float dz1 = bBox.mMax.z - aBox.mMin.z;
		float dz2 = bBox.mMin.z - aBox.mMax.z;


		float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
		float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
		float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

		if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
		{
			pos.x += dx;
		}
		else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
		{
			pos.y += dy;
		}
		else
		{
			pos.z += dz;
			mState = EGrounded;
		}

		boxComponent->GetOwner()->SetPosition(pos);
		boxComponent->OnUpdateWorldTransform();
	}
}