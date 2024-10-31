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
#include "FPSCamera.h"
#include "MoveComponent.h"
#include "LevelLoader.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "BossActor.h"
#include "DropItemActor.h"
#include "PointLightComponent.h"
#include "ArrowActor.h"
#include "BombActor.h"
FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
	, mState(EGrounded)
	, mItemState(ESword)
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
	, mBombCount(9)
	, mArrowCount(9)
	, mFPSCamera(nullptr)
	, changeTimer(0.0f)
{
	mMeshComp = new SkeletalMeshComponent(this);
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
	else if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_SPACE] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f && mAttackBoxComp == nullptr )
	{
		delete mBlockBoxComp;  // �������̉��
		mBlockBoxComp = nullptr;  // �|�C���^���N���A
		blockPressed = false;
		mState = EJump;
		
		
		jumpSpeed += -20000.0f;
		mStamina -= 0.1f;
	}

	if (keys[SDL_SCANCODE_R] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f && mAttackBoxComp == nullptr && mItemState == ESword)
	{
		Block();
		mStamina += 0.007f;
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
		
	}
	else if(mItemState == ESword){
		
		if (mBlockBoxComp != nullptr) {
			delete mBlockBoxComp;  // �������̉��
			mBlockBoxComp = nullptr;  // �|�C���^���N���A
		}
		
	}
	if (keys[SDL_SCANCODE_R] && mHealth > 0.0f && mItemState == ETorch) {

		
	}
	

	if (keys[SDL_SCANCODE_R] && mHealth > 0.0f && mItemState == EBow && changeTimer >0.5f) {
		changeTimer = 0.0f;
		if (!mCameraComp && mFPSCamera) {
			mMeshComp->SetVisible(true);
			mCameraComp = new FollowCamera(this);
			delete mFPSCamera;
			mFPSCamera = nullptr; 
		}
		else if (!mFPSCamera && mCameraComp) {
			mMeshComp->SetVisible(false);
			mFPSCamera = new FPSCamera(this);
			delete mCameraComp;
			mCameraComp = nullptr; 
		}
	}
	
	if (mItemState != EBow) {
		if (!mCameraComp && mFPSCamera) {
			mMeshComp->SetVisible(true);
			mCameraComp = new FollowCamera(this);
			delete mFPSCamera;
			mFPSCamera = nullptr;
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
	// �}�E�X���N���b�N�������ꂽ���m�F
	if (mAttackTimer < 0.0f && buttons && SDL_BUTTON(SDL_BUTTON_LEFT) && mState == EGrounded && mStamina >= 0.2f) {

		delete mBlockBoxComp;  // �������̉��
		mBlockBoxComp = nullptr;  // �|�C���^���N���A
		blockPressed = false;
		
		mStamina -= 0.2f;
		if (mItemState == ESword) {
			Attack();
		}
		if (mItemState == EBow) {
			Shoot();
		}
		if (mItemState == EBomb) {
			Bomb();
		}
	}

	if (mHealth > 0.0f) {
		mMoveComp->SetAngularSpeed(angularSpeed);
		if (mCameraComp) {
			mCameraComp->SetPitchSpeed(pitchSpeed);
		}
		else if (mFPSCamera) {
			mFPSCamera->SetPitchSpeed(pitchSpeed);
		}
		

		mMoveComp->SetForwardSpeed(forwardSpeed);
		mMoveComp->SetStrafeSpeed(strafeSpeed);
	}
	
	else{
		mMeshComp->SetVisible(true);
		mMoveComp->SetAngularSpeed(0.0f);
		mCameraComp->SetPitchSpeed(0.0f);
		if (mCameraComp) {
			mCameraComp->SetPitchSpeed(0.0f);
		}
		else if (mFPSCamera) {
			mFPSCamera->SetPitchSpeed(0.0f);
		}
		mMoveComp->SetForwardSpeed(0.0f);
		mMoveComp->SetStrafeSpeed(0.0f);
	}
}

void FollowActor::UpdateActor(float deltaTime) {

	if (mItemState == EBow ) {
		if (mArrowCount <= 0) {
			mItemState = ESword;
			mAudioComp->PlayEvent("event:/Equipped");
		}
		
	}
	if (mItemState == EBomb) {
		if (mBombCount <= 0) {
			mItemState = ESword;
			mAudioComp->PlayEvent("event:/Equipped");
		}

	}
	changeTimer += deltaTime;
	if (mStamina >= 1.0f) {
		mStamina = 1.0f;
	}
	else if (mStamina <= 0.0f) {
		mStamina = 0.0f;
	}
	if (mState == EJump) {
		
		jumpSpeed += 50000.0f * deltaTime; // �d�͂̉e�����󂯂�
		if (jumpSpeed >= 0.0f) {
			mState = EFall;
		}
	}
	else if (mState == EFall) {
		jumpSpeed += 100000.0f * deltaTime; // �d�͂̉e�����󂯂�
	}
	else if(mState == EGrounded) {
		jumpSpeed = 0.0f;
		if (mStamina < 1.0f && isShiftPressed == false) {
			mStamina += 0.005f;
		}
	}
	
	

	mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);

	// �^�C�}�[���i�s���Ă���ꍇ
	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;

	}
	// �^�C�}�[���i�s���Ă���ꍇ
	if (mBlockTimer > 0.0f) {
		mBlockTimer -= deltaTime;

	}
	// �^�C�}�[���i�s���Ă���ꍇ
	if (mBoxTimer > 0.0f) {
		mBoxTimer -= deltaTime;
		// �^�C�}�[��0�ɂȂ�����BoxComponent���폜
		if (mBoxTimer <= 0.0f) {

			delete mAttackBoxComp;  // �������̉��
			mAttackBoxComp = nullptr;  // �|�C���^���N���A
		}
	}
	mState = EFall;
	FixCollisions();
	// �^�C�}�[���i�s���Ă���ꍇ
	if (mDamageTimer > 0.0f) {
		mDamageTimer -= deltaTime;
		if (mDamageTimer >= 3.0) {
			mHealth -= 0.25f * deltaTime;

		}
		// �o�ߎ��Ԃ𑝉�������
		blinkTime += deltaTime;

		// ���Ԋu�𒴂��������Ԃ�؂�ւ�
		if (blinkTime >= blinkInterval)
		{
			// ����Ԃ𔽓]������
			isVisible = !isVisible;
			mMeshComp->SetVisible(isVisible);

			// �^�C�}�[�����Z�b�g
			blinkTime = 0.0f;
		}
	}

	else  {
		if (!mFPSCamera) {
			mMeshComp->SetVisible(true);

		}
		
		blinkTime = 0.0f;

	}
	
	if (mHealth <= 0.0f) {

		mState = EDead;
		mHealth = 0.0f;
	}
	


}
void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void FollowActor::Attack() {
	mArrowCount--;
	// �U������p��BoxComponent��ǉ�
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(25.0f, -25.0f, 100.0f),
		Vector3(75.0f, 25.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);
	
	// �^�C�}�[�����Z�b�g
	mBoxTimer = 0.5f;  // 0.5�b��ɍ폜����
	// �^�C�}�[�����Z�b�g
	mAttackTimer = 2.0f;  // 0.5�b��ɍ폜����
	mAudioComp->PlayEvent("event:/SwordAttack");
}
void FollowActor::Shoot()
{
	mBombCount--;
	// Get direction vector
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	
	ArrowActor* arrow = new ArrowActor(GetGame());
	if (mCameraComp) {
		arrow->SetPosition(this->GetPosition() + Vector3(0.0, 60.0, 120.0) + dir * 500.0f);
	}
	else {
		arrow->SetPosition(start + dir * 100.0f);
	}
	arrow->SetPlayer(this);
	
	arrow->RotateToNewForward(dir);
	// �^�C�}�[�����Z�b�g
	mAttackTimer = 2.0f;  
	// Play shooting sound
	mAudioComp->PlayEvent("event:/Shot");
}

void FollowActor::Bomb()
{

	// Get direction vector
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);

	BombActor* bomb = new BombActor(GetGame());
	bomb->SetPlayer(this);
	bomb->SetPosition(this->GetPosition()+Vector3(0.0,0.0,200.0) + dir * 100.0f);
	bomb->RotateToNewForward(dir);
	// �^�C�}�[�����Z�b�g
	mAttackTimer = 4.0f;
	// Play shooting sound
	mAudioComp->PlayEvent("event:/Shot");
}
void FollowActor::Block() {


	if(mBlockBoxComp == nullptr){
		// �h�䔻��p��BoxComponent��ǉ�
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
	auto& item = GetGame()->GetDropItem();
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


	for (auto it : item)
	{
		if (it != nullptr) {

			const AABB& itemBox = it->GetBox()->GetWorldBox();
			if (Intersect(playerBox, itemBox))
			{
				mAudioComp->PlayEvent("event:/ItemGet");
				if (it->GetItemNum() == 0) {
					
					mArrowCount += 5;
				}
				else if (it->GetItemNum() == 1) {
					mBombCount += 1;
				}

			}


		}


	}
}
