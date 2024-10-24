// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowActor.h"
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
#include "AudioSystem.h"
#include "BossActor.h"


FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
	, mState(EGrounded)
	, jumpSpeed(0.0f)
	, mStamina(1.0f)
	, mHealth(1.0f)
	, isShiftPressed(false)
	, mAttackBoxComp(nullptr)
	, mBlockBoxComp(nullptr)
	, mAttackTimer(1.0f)
	, mBoxTimer(0.0f)
	, mDamageTimer(0.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, blockPressed(false)
	, mBlockTimer(0.0f)

	
{
	mMeshComp = new SkeletalMeshComponent(this);
	//mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Paladin WProp J Nordstrom.gpmesh"));
	//mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Paladin WProp J Nordstrom.gpskel"));
	
	//mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	mAudioComp = new AudioComponent(this);
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	


	game->SetFollowActor(this);
	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, 0.0f),
		Vector3(25.0f, 25.0f, 170.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	
	blockPressed = false;
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_SPACE] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f && mAttackBoxComp == nullptr)
	{
		delete mBlockBoxComp;  // メモリの解放
		mBlockBoxComp = nullptr;  // ポインタをクリア
		blockPressed = false;
		mState = EJump;
		
		
		jumpSpeed += -20000.0f;
		mStamina -= 0.1f;
	}

	if (keys[SDL_SCANCODE_R] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f && mAttackBoxComp == nullptr)
	{
		Block();
		mStamina += 0.007f;
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
		
	}
	else {
		
		if (mBlockBoxComp != nullptr) {
			delete mBlockBoxComp;  // メモリの解放
			mBlockBoxComp = nullptr;  // ポインタをクリア
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

	if (mAttackBoxComp != nullptr || mBlockBoxComp != nullptr) {
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
	}
	// Check if Shift key is pressed
	isShiftPressed = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
	// Apply speed multiplier if Shift is pressed
	if (isShiftPressed && mStamina > 0.0f && forwardSpeed != 0.0f)
	{
		mStamina -= 0.007f;
		forwardSpeed *= 1.3f;
		strafeSpeed *= 1.3f;
	}
	int x, y;
	
	SDL_GetRelativeMouseState(&x, &y);
	// Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	// Rotation/sec at maximum speed
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		// Convert to ~[-1.0, 1.0]
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// Multiply by rotation/sec
		angularSpeed *= maxAngularSpeed;
	}

	
	// Compute pitch
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// Convert to ~[-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	int butx = 0, buty = 0;
	
	Uint32 buttons = SDL_GetMouseState(&butx, &buty);
	// マウス左クリックが押されたか確認
	if (mAttackTimer < 0.0f && buttons && SDL_BUTTON(SDL_BUTTON_LEFT) && mState == EGrounded && mStamina >= 0.2f) {

		delete mBlockBoxComp;  // メモリの解放
		mBlockBoxComp = nullptr;  // ポインタをクリア
		blockPressed = false;
		Attack();
		mStamina -= 0.2f;
		
	}

	if (mHealth > 0.0f) {
		mMoveComp->SetAngularSpeed(angularSpeed);
		mCameraComp->SetPitchSpeed(pitchSpeed);

		mMoveComp->SetForwardSpeed(forwardSpeed);
		mMoveComp->SetStrafeSpeed(strafeSpeed);
	}
	else {
		mMoving = false;
		
	}

}

void FollowActor::UpdateActor(float deltaTime) {


	
	if (mStamina >= 1.0f) {
		mStamina = 1.0f;
	}
	else if (mStamina <= 0.0f) {
		mStamina = 0.0f;
	}
	if (mState == EJump) {
		
		jumpSpeed += 50000.0f * deltaTime; // 重力の影響を受ける
		if (jumpSpeed >= 0.0f) {
			mState = EFall;
		}
	}
	else if (mState == EFall) {
		jumpSpeed += 100000.0f * deltaTime; // 重力の影響を受ける
	}
	else if(mState == EGrounded) {
		jumpSpeed = 0.0f;
		if (mStamina < 1.0f && isShiftPressed == false) {
			mStamina += 0.005f;
		}
	}
	mState = EFall;
	FixCollisions();

	mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);

	// タイマーが進行している場合
	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;

	}
	// タイマーが進行している場合
	if (mBlockTimer > 0.0f) {
		mBlockTimer -= deltaTime;

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

	// タイマーが進行している場合
	if (mDamageTimer > 0.0f) {
		mDamageTimer -= deltaTime;
		if(mDamageTimer >= 3.75) {
			mHealth -= 0.0168;
		}
		
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

	if (mHealth <= 0.0f) {
		mMeshComp->SetVisible(true);
		mHealth = 0.0f;
		
	   
	}
	


}
void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void FollowActor::Attack() {
	
	// 攻撃判定用のBoxComponentを追加
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(25.0f, -25.0f, 100.0f),
		Vector3(75.0f, 25.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);
	
	// タイマーをリセット
	mBoxTimer = 0.5f;  // 0.5秒後に削除する
	// タイマーをリセット
	mAttackTimer = 2.0f;  // 0.5秒後に削除する
	mAudioComp->PlayEvent("event:/SwordAttack");
}

void FollowActor::Block() {


	if(mBlockBoxComp == nullptr){
		// 防御判定用のBoxComponentを追加
		mBlockBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(25.0f, -1.0f, 0.0f),
			Vector3(30.0f, 1.0f, 20.0f));

		mBlockBoxComp->SetObjectBox(myBox);
		mBlockBoxComp->SetShouldRotate(true);
		
	}
	
	

}

void FollowActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}

void FollowActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}

void FollowActor::FixCollisions()
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	const AABB& attackBox = mAttackBoxComp->GetWorldBox();
	const AABB& blockBox = mBlockBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	auto& enemy = GetGame()->GetEnemys();
	auto* boss = GetGame()->GetBoss();
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
			mBoxComp->OnUpdateWorldTransform();
		}
	}
	

	for (auto en : enemy)
	{
		if (en != nullptr) {

			const AABB& enemyBox = en->GetBox()->GetWorldBox();
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
				}

				// Need to set position and update box component
				SetPosition(pos);
				mBoxComp->OnUpdateWorldTransform();
			}
			
			else if(en->GetAttackBox() != nullptr)
			{
				const AABB& enemyAttackBox = en->GetAttackBox()->GetWorldBox();
				if (mBlockBoxComp != nullptr ) {

					if (Intersect(blockBox, enemyAttackBox)) {
						
						if (mBlockTimer <= 0.0f) {
							mBlockTimer = 0.5f;
							mStamina -= 0.3f;
							mAudioComp->PlayEvent("event:/Block");
						}
					}
					else if (!Intersect(blockBox, enemyAttackBox)) {
						if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
							
							mAudioComp->PlayEvent("event:/Hit");

							mDamageTimer = 4.0f;
						}
					}
					

				}
	
				else if (mBlockBoxComp == nullptr  && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
				{
					
					mAudioComp->PlayEvent("event:/Hit");
					
					mDamageTimer = 4.0f;
				}
			}
			
		}
		
		
	}
	if (boss != nullptr) {

		const AABB& enemyBox = boss->GetBox()->GetWorldBox();
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
			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}




		else if (boss->GetAttackBox() != nullptr)
		{
			const AABB& enemyAttackBox = boss->GetAttackBox()->GetWorldBox();
			if (mBlockBoxComp != nullptr) {

				if (Intersect(blockBox, enemyAttackBox)) {
					
					if (mBlockTimer <= 0.0f) {
						mBlockTimer = 1.0f;
						mStamina -= 0.3f;
						mAudioComp->PlayEvent("event:/Block");
					}
				}
				else if (!Intersect(blockBox, enemyAttackBox)) {
					if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
						
						mAudioComp->PlayEvent("event:/Hit");

						mDamageTimer = 4.0f;
					}
				}


			}

			else if (mBlockBoxComp == nullptr && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
			{
				
				mAudioComp->PlayEvent("event:/Hit");

				mDamageTimer = 4.0f;
			}
		}
	}
}
