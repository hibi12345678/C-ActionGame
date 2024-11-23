#include "EnemyActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "LevelLoader.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "ReactActor.h"
#include "SmokeActor.h"
#include "DropItemActor.h"
#include "FollowActor.h"
#include <cmath>
#include <cstdlib> 
#include <ctime>   
#include "Random.h"
#include "TargetComponent.h"
#include "ArrowActor.h"
#include "BombActor.h"
#include "ExplosionActor.h"
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
	, blinkInterval(0.2f)
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
	AABB myBox(Vector3(-50.0f, -50.0f, 0.0f),
		Vector3(50.0f, 50.0f, 240.0f));
	mBox->SetObjectBox(myBox);
	mBox->SetShouldRotate(true);
	srand(static_cast<unsigned int>(time(0)));
    game->AddEnemy(this);
}

EnemyActor::~EnemyActor()
{
	if (name == "Enemy") {
		GetGame()->RemoveEnemy(this);
	}	
}

void EnemyActor::UpdateActor(float deltaTime){

	forwardSpeed = 0.0f;
	strafeSpeed = 0.0f;
	// ポインタを受け取る場合
	auto* player = GetGame()->GetPlayer();
	Vector3 playerPosition = player->GetPosition();
	Vector3 pos = GetPosition();
	diff = playerPosition - pos;
	float distanceSq = diff.LengthSq();

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

	if (!mMoving && (!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)) && groundFlag == true)
	{
		mMoving = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/"+name+"_walk.gpanim"), 1.0f);
	}
	else if (mMoving && Math::NearZero(forwardSpeed) && Math::NearZero(strafeSpeed) && groundFlag == true && !mAttackBoxComp)
	{
		mMoving = false;
		if (deathFlag) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_idle.gpanim"), 1.0f);
		}
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

void EnemyActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

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

void EnemyActor::UpdatePatrolState(float deltaTime) {
	mReactFlag = false;
	mMoveTimer -= deltaTime;

	if (randomValue == 0) {
		forwardSpeed += 100.0f;
	}

	if (mMoveTimer <= 0.0f) {
		SetRandomRotation();
		randomValue = GenerateRandomValue(2); // 0から2のランダムな整数
		mMoveTimer = 3.0f + static_cast<float>(randomValue);
	}
}

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
		else if ((randomValue == 2 || randomValue == 3) && mAttackTimer <= 0.0f) {
			(randomValue == 2) ? Attack() : AttackGround();
		}

		if (mMoveTimer <= 0.0f) {
			randomValue = GenerateRandomValue(4);
			mMoveTimer = 2.0f + static_cast<float>(randomValue);
		}
	}
}

void EnemyActor::SetRandomRotation() {
	SetRotation(Quaternion::CreateFromAxisAngle(Random::GetFloatRange(0.0f, Math::TwoPi)));
}

void EnemyActor::CreateReactActor() {
	ReactActor* react = new ReactActor(GetGame());
	react->SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 250.0f));
}

void EnemyActor::AlignToTarget() {
	float angle = atan2(diff.y, diff.x);
	Quaternion rotation = Quaternion::CreateFromAxisAngle(angle);
	SetRotation(rotation);
}

void EnemyActor::Attack() {

	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(50.0f, -50.0f, 50.0f),
		Vector3(125.0f, 50.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_attack.gpanim"), 1.0f);
	mBoxTimer = 0.5f;
	mAttackTimer = 4.0f;
	groundFlag = true;
	mAudioComp->PlayEvent("event:/EnemyAttack");
}

void EnemyActor::AttackGround() {

	mBoxTimer = 2.1f;
	mAttackTimer = 4.0f;
	groundFlag = false;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_jump_attack.gpanim"), 1.0f);
	mAudioComp->PlayEvent("event:/EnemyAttack2");
}

void EnemyActor::UpdateTimers(float deltaTime) {

	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;
	}

	if (mDamageTimer > 0.0f) {
		mDamageTimer -= deltaTime;
		HandleBlinking(deltaTime);
		HandleDeath();
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

void EnemyActor::HandleBlinking(float deltaTime) {
	blinkTime += deltaTime;

	if (blinkTime >= blinkInterval) {
		isVisible = !isVisible;
		mMeshComp->SetVisible(isVisible);
		blinkTime = 0.0f;
	}
}

void EnemyActor::HandleDeath() {
	if (mHealth <= 0.0f) {
		if (deathFlag) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_dying.gpanim"), 1.0f);
			deathFlag = false;
		}

		if (mDamageTimer <= 0.0f) {
			
			DropItemActor* dropitem = new DropItemActor(GetGame());
			dropitem->SetPosition(GetPosition());
			mState = EDead;
		}
		mHealth = 0.0f;
	}
}

void EnemyActor::HandleAttackBox() {
	if (mBoxTimer <= 0.0f) {
		CleanUpAttackBox();
	}
	else if (mBoxTimer <= 0.3f && !groundFlag) {
		AddAttackBox();
	}
}

void EnemyActor::CleanUpAttackBox() {
	delete mAttackBoxComp;
	mAttackBoxComp = nullptr;

	if (deathFlag) {
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/" + name + "_idle.gpanim"), 1.0f);
	}
}

void EnemyActor::AddAttackBox() {
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-150.0f, -125.0f, 0.0f), Vector3(150.0f, 125.0f, 10.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);

	SmokeActor* smoke = new SmokeActor(GetGame());
	smoke->SetPosition(GetPosition());
	groundFlag = true;
}

void EnemyActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}

void EnemyActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}

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
			if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
				mAudioComp->PlayEvent("event:/EnemyHit");
				mHealth -= 0.5f;
				mDamageTimer = 4.0f;
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
			if (Intersect(playerBox, arBox) && mDamageTimer <= 0.0f)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mHealth -= 0.5f;
				mDamageTimer = 4.0f;
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
				mHealth -= 1.0f;
				mDamageTimer = 5.0f;

			}
		}
	}
}

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