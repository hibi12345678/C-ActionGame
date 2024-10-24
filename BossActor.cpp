#include "BossActor.h"
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
#include "FollowActor.h"
#include <cmath>

#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()
#include "Random.h"
#include "TargetComponent.h"

BossActor::BossActor(Game* game)
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
	, delta(0.0f, 0.0f, 0.0f)
	, randomValue(0)
	, mMoveTimer(0.0f)
	, mMoveState(EPatrol)
	, mBoxTimer(0.0f)
	, mReactFlag(true)
{
	SetScale(2.0f);
	mMeshComp = new SkeletalMeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Enemy.gpmesh");
	mMeshComp->SetMesh(mesh);
	//mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Paladin WProp J Nordstrom.gpmesh"));
	//mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Paladin WProp J Nordstrom.gpskel"));
	//mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	game->SetBossActor(this);
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	new TargetComponent(this);
	// Add a box component\	
	mBox = new BoxComponent(this);
	AABB myBox(Vector3(-50.0f, -50.0f, 0.0f),
		Vector3(50.0f, 50.0f, 170.0f));
	mBox->SetObjectBox(myBox);
	mBox->SetShouldRotate(false);
	srand(static_cast<unsigned int>(time(0)));
	

}

BossActor::~BossActor()
{
	
}
void BossActor::UpdateActor(float deltaTime) {

	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// ポインタを受け取る場合
	auto* player = GetGame()->GetPlayer();
	Vector3 playerPosition = player->GetPosition();
	Vector3 pos = GetPosition();
	Vector3 diff = playerPosition - pos;

	if (diff.LengthSq() <= 200000.0f && diff.LengthSq() > 100000.0f) {

		mMoveState = EBattle;


	}

	else if (diff.LengthSq() <= 5000000.0f ) {
		mMoveState = EAttack;
	}

	else if (diff.LengthSq() > 5000000.0f) {
		mMoveState = EPatrol;
	}
	if (mMoveState == EPatrol) {

		mReactFlag = false;
		mMoveTimer -= deltaTime;
		// movement
		if (randomValue == 0)
		{
			forwardSpeed += 100.0f;
		}


		if (mMoveTimer <= 0.0f) {


			SetRotation(Quaternion::CreateFromAxisAngle(Random::GetFloatRange(0.0f, Math::TwoPi)));
			// 0 から 2 までのランダムな整数を生成
			randomValue = rand() % 2;
			mMoveTimer = 3.0f + float(randomValue);
		}
	}

	else if (mMoveState == EBattle) {

		if (mReactFlag == false) {
			mReactFlag = true;
			ReactActor* react = new ReactActor(GetGame());
			react->SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 300.0f));
		}

		float angle = atan2(diff.y, diff.x);
		float angleDegrees = angle * (180.0f / Math::Pi);
		Quaternion rotation = Quaternion::CreateFromAxisAngle(angle);
		SetRotation(rotation);

		mMoveTimer -= deltaTime;
		// movement
		if (randomValue == 0 || randomValue == 1)
		{
			forwardSpeed += 200.0f;
		}
		else if (randomValue == 2) {
			forwardSpeed += -50.0f;
		}

		if (mMoveTimer <= 0.0f) {

			SetRotation(Quaternion::CreateFromAxisAngle(Random::GetFloatRange(0.0f, Math::TwoPi)));
			// 0 から 2 までのランダムな整数を生成
			randomValue = rand() % 4;
			mMoveTimer = 2.0f + float(randomValue);
		}
	}

	else if (mMoveState == EAttack) {

		forwardSpeed += 100.0f;
		float angle = atan2(diff.y, diff.x);
		float angleDegrees = angle * (180.0f / Math::Pi);
		Quaternion rotation = Quaternion::CreateFromAxisAngle(angle);
		SetRotation(rotation);
		mMoveTimer -= deltaTime;
		if (diff.LengthSq() <= 20000.0f) {
			forwardSpeed = 0.0f;

			if (randomValue == 0)
			{
				strafeSpeed += 50.0f;
			}
			else if (randomValue == 1) {
				strafeSpeed += -50.0f;
			}
			else if (randomValue == 2 && mAttackTimer <= 0.0f) {
				Attack();
			}
			else if (randomValue == 3 && mAttackTimer <= 0.0f) {
				AttackGround();
			}

			if (mMoveTimer <= 0.0f) {

				randomValue = rand() % 4;
				mMoveTimer = 2.0f + float(randomValue);
			}
		}
	}




	// Did we just start moving?
	if (!mMoving && !Math::NearZero(forwardSpeed))
	{
		mMoving = true;
		//mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving?
	else if (mMoving && Math::NearZero(forwardSpeed))
	{
		mMoving = false;
		//mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	if (mState == EFall) {
		jumpSpeed += 100000.0f * deltaTime; // 重力の影響を受ける
	}
	else if (mState == EGrounded) {
		jumpSpeed = 0.0f;

	}
	mState = EFall;

	if (mAttackBoxComp != nullptr) {
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
	}
	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);
	FixCollisions();
	mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);
	// タイマーが進行している場合
	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;

	}


	// タイマーが進行している場合
	if (mDamageTimer > 0.0f) {
		mDamageTimer -= deltaTime;

		// 経過時間を増加させる
		blinkTime += deltaTime;

		// 一定間隔を超えたら可視状態を切り替え
		if (blinkTime >= blinkInterval)
		{
			// 可視状態を反転させる
			isVisible = !isVisible;
			mMeshComp->SetVisible(isVisible);

			// タイマーをリセット
			blinkTime = 0.0f;
		}
	}

	else {

		mMeshComp->SetVisible(true);
		blinkTime = 0.0f;

	}


	// タイマーが進行している場合
	if (mBoxTimer > 0.0f) {
		mBoxTimer -= deltaTime;
		// タイマーが0になったらBoxComponentを削除
		if (mBoxTimer <= 0.0f) {

			delete mAttackBoxComp;  // メモリの解放
			mAttackBoxComp = nullptr;  // ポインタをクリア
		}
	}
	if (mHealth <= 0.0f) {

		mState = EDead;
	}
}
void BossActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void BossActor::Attack() {
	// 攻撃判定用のBoxComponentを追加
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(50.0f, -75.0f, 25.0f),
		Vector3(750.0f, 75.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);

	// タイマーをリセット
	mBoxTimer = 0.5f;  // 0.5秒後に削除する
	// タイマーをリセット
	mAttackTimer = 4.0f;
	mAudioComp->PlayEvent("event:/EnemyAttack");
}

void BossActor::AttackGround() {
	// 攻撃判定用のBoxComponentを追加
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(50.0f, -125.0f, 0.0f),
		Vector3(75.0f, 125.0f, 5.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);

	// タイマーをリセット
	mBoxTimer = 0.5f;  // 0.5秒後に削除する
	// タイマーをリセット
	mAttackTimer = 4.0f;
	mAudioComp->PlayEvent("event:/EnemyAttack");
}

void BossActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}

void BossActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}

void BossActor::FixCollisions()
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBox->GetWorldBox();
	const AABB& attackBox = mAttackBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	// ポインタを受け取る場合
	auto* player = GetGame()->GetPlayer();
	if (player != nullptr) {

		// デリファレンスしてメンバにアクセス
		const AABB& enemyBox = player->GetBox()->GetWorldBox();
		if (Intersect(playerBox, enemyBox))
		{
			// Calculate all our differences
			float dx1 = enemyBox.mMax.x - playerBox.mMin.x;
			float dx2 = enemyBox.mMin.x - playerBox.mMax.x;
			float dy1 = enemyBox.mMax.y - playerBox.mMin.y;
			float dy2 = enemyBox.mMin.y - playerBox.mMax.y;
			float dz1 = enemyBox.mMax.z - playerBox.mMin.z;
			float dz2 = enemyBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;


			// Whichever is closest, adjust x/y position
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

			// Need to set position and update box component
			SetPosition(pos);
			mBox->OnUpdateWorldTransform();
		}

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
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;


			// Whichever is closest, adjust x/y position
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

			// Need to set position and update box component
			SetPosition(pos);
			mBox->OnUpdateWorldTransform();
		}
	}


}

