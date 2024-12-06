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
#include "ExplosionActor.h"
#include "SwordActor.h"
#include "Animation.h"
FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
	, mState(EGrounded)
	, mItemState(ESword)
	, jumpSpeed(0.0f)
	, mStamina(1.0f)
	, mHealth(1.0f)
	, mHealthRed(1.0f)
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
	, deathFlag(true)
	, jumpFlag(false)
	, mAnimState(AnimationState::Idle)
	, mAnimTime(0.0f)

{
	mMeshComp = new SkeletalMeshComponent(this);
	mAudioComp = new AudioComponent(this);
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	game->SetFollowActor(this);
	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, 0.0f),
		Vector3(25.0f, 25.0f, 150.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(true);

	blockPressed = false;
	SwordActor* sword1 = new SwordActor(game,1.0f,0);
	SwordActor* sword2 = new SwordActor(game,1.0f,1);
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W] && mAttackTimer <= 0.0f)
	{

		
		forwardSpeed += 200.0f;
		if (jumpFlag)
			forwardSpeed -= 100.f;
		// Check if Shift key is pressed
		isShiftPressed = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
		// Apply speed multiplier if Shift is pressed
		if (isShiftPressed && mStamina > 0.0f && mState == EGrounded)
		{
			mStamina -= 0.01f;
			forwardSpeed *= 1.7f;

			if (mAnimState != AnimationState::Run ) {
				mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_run.gpanim"), 1.0f);
				mAnimState = AnimationState::Run;
			}
		}

		else if (!isShiftPressed && mAnimState != AnimationState::Walk  && mState == EGrounded) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_walk.gpanim"), 1.0f);
			mAnimState = AnimationState::Walk;
		}
	}
	else if (keys[SDL_SCANCODE_S] && mAttackTimer <= 0.0f)
	{
		forwardSpeed -= 200.0f;
		if (jumpFlag)
			forwardSpeed += 100.f;
		if (mAnimState != AnimationState::Back && mState == EGrounded) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_back.gpanim"), 1.0f);
			mAnimState = AnimationState::Back;
		}
	}
	if (keys[SDL_SCANCODE_A] && mAttackTimer <= 0.0f)
	{
		strafeSpeed -= 200.0f;
		if (jumpFlag)
			strafeSpeed += 100.f;
		if (mAnimState != AnimationState::Left && forwardSpeed == 0.0f && mState == EGrounded) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_left.gpanim"), 1.0f);
			mAnimState = AnimationState::Left;
		}
	}
	else if (keys[SDL_SCANCODE_D] && mAttackTimer <= 0.0f)
	{
		strafeSpeed += 200.0f;
		if (jumpFlag)
			strafeSpeed -= 100.f;
		if (mAnimState != AnimationState::Right && forwardSpeed == 0.0f && mState == EGrounded) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_right.gpanim"), 1.0f);
			mAnimState = AnimationState::Right;
		}
	}
	if (keys[SDL_SCANCODE_SPACE] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f  && mAttackTimer <= 0.0f)
	{
		delete mBlockBoxComp;  // メモリの解放
		mBlockBoxComp = nullptr;  // ポインタをクリア
		blockPressed = false;
		mState = EJump;
		
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_jump.gpanim"), 0.9f);
		
		jumpSpeed += - 20000.0f;
		mStamina -= 0.1f;
		jumpFlag = true;
		inertiaStrafe = strafeSpeed;
		inertiaForward = forwardSpeed;
	}
	if (jumpFlag) {
		strafeSpeed += inertiaStrafe;
		forwardSpeed += inertiaForward;
	}
	if (keys[SDL_SCANCODE_R] && mState == EGrounded && mStamina > 0.1f && mHealth > 0.0f && mAttackTimer <= 0.0f && mItemState == ESword)
	{
		Block();
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
		
	}
	else if(mItemState == ESword){
		
		if (mBlockBoxComp != nullptr) {
			mMoving = false;

			if (mItemState == EBow) {
				mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bowidle.gpanim"), 1.0f);
				mAnimState = AnimationState::Idle;
			}
			else {
				mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_idle.gpanim"), 1.0f);
				mAnimState = AnimationState::Idle;
			}
			delete mBlockBoxComp;  // メモリの解放
			mBlockBoxComp = nullptr;  // ポインタをクリア
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

	if (mAttackBoxComp != nullptr || mBlockBoxComp != nullptr) {
		forwardSpeed = 0.0f;
		strafeSpeed = 0.0f;
	}

	int x, y;
	
	SDL_GetRelativeMouseState(&x, &y);
	// Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	// Rotation/sec at maximum speed
	const float maxAngularSpeed = Math::Pi * 6;
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
	if (mAttackTimer < 0.0f && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && mState == EGrounded && mStamina >= 0.2f) {

		delete mBlockBoxComp;  // メモリの解放
		mBlockBoxComp = nullptr;  // ポインタをクリア
		blockPressed = false;
		mStamina -= 0.3f;
		
		if (mItemState == ESword || mItemState == ETorch) {
			Attack();
			
		}
		if (mItemState == EBow) {
			Shoot();
		}
		if (mItemState == EBomb) {
			Bomb();
		}
	}
	// Did we just start moving?
	if (!mMoving && (!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)))
	{
		mMoving = true;
		
	}
	// Or did we just stop moving?
	else if (mMoving && Math::NearZero(forwardSpeed) && Math::NearZero(strafeSpeed) && !mAttackBoxComp  &&  !mBlockBoxComp && mState == EGrounded)
	{
		mMoving = false;

		if (mItemState == EBow) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bowidle.gpanim"), 1.0f);
			mAnimState = AnimationState::Idle;
		}
		else {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_idle.gpanim"), 1.0f);
			mAnimState = AnimationState::Idle;
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

	UpdateState(deltaTime);
	UpdateStamina(deltaTime);
	UpdateMovement(deltaTime);
	UpdateTimers(deltaTime);
	mState = EFall;
	FixCollisions();
	HandleVisibility(deltaTime);
}

void FollowActor::UpdateTimers(float deltaTime) {
	changeTimer += deltaTime;
	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;
		if (mItemState == EBow && mAttackTimer <= 0.0f) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bowidle.gpanim"));
			mAnimState == AnimationState::Idle;
		}
		else if (mItemState != EBow &&mAttackTimer <= 0.0f) {
			mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_idle.gpanim"));
			mAnimState == AnimationState::Idle;
		}
	}

	if (mBlockTimer > 0.0f) mBlockTimer -= deltaTime;

	if (mBoxTimer > 0.0f) {
		mBoxTimer -= deltaTime;
		if (mBoxTimer <= 0.0f) {
			delete mAttackBoxComp;
			mAttackBoxComp = nullptr;
		}
	}

	if (mDamageTimer > 0.0f) {
		mDamageTimer -= deltaTime;
		UpdateDamage(deltaTime);
	}
}

void FollowActor::UpdateStamina(float deltaTime) {
	if (mState == EGrounded && mStamina < 1.0f) {
		mStamina = std::min(mStamina + 0.005f, 1.0f);
	}
}

void FollowActor::UpdateMovement(float deltaTime) {
	if (mState == EJump) {
		jumpSpeed += 50000.0f * deltaTime; // 重力
		if (jumpSpeed >= 0.0f) mState = EFall;
	}
	else if (mState == EFall) {
		jumpSpeed += 100000.0f * deltaTime;
	}
	else if (mState == EGrounded) {
		jumpSpeed = 0.0f;
	}
	mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);
}

void FollowActor::UpdateState(float deltaTime) {

	if (mItemState == EBow && mArrowCount <= 0) {
		EquipSword();
	}
	else if (mItemState == EBomb && mBombCount <= 0) {
		EquipSword();
	}

	if (mDamageTimer <= 0.0f && mHealth <= 0.0f && deathFlag) {
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_dying.gpanim"), 1.0f);
		mAnimState = AnimationState::Die;
		deathFlag = false;
		mState = EDead;
	}
}

void FollowActor::EquipSword() {
	mItemState = ESword;
	SwordActor* sword1 = new SwordActor(GetGame(), 1.0f, 0);
	SwordActor* sword2 = new SwordActor(GetGame(), 1.0f, 1);
	mAudioComp->PlayEvent("event:/Equipped");
}

void FollowActor::UpdateDamage(float deltaTime) {

	if (mDamageTimer >= 3.0f) {
		mHealthRed = std::max(mHealthRed - 0.252f * deltaTime, 0.0f);
	}

	blinkTime += deltaTime;
	if (blinkTime >= blinkInterval) {
		isVisible = !isVisible;
		mMeshComp->SetVisible(isVisible);
		blinkTime = 0.0f;
	}
}

void FollowActor::HandleVisibility(float deltaTime) {
	if (mDamageTimer <= 0.0f) {
		if (!mFPSCamera) {
			mMeshComp->SetVisible(true);
		}
		blinkTime = 0.0f;
	}
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void FollowActor::Attack() {
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_attack.gpanim"),1.2f);
	mAnimState = AnimationState::Attack;
	mAttackBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(25.0f, -25.0f, 100.0f),
		Vector3(75.0f, 25.0f, 170.0f));
	mAttackBoxComp->SetObjectBox(myBox);
	mAttackBoxComp->SetShouldRotate(true);
	mBoxTimer = 0.5f; 
	mAttackTimer = 1.9f;  
	mAudioComp->PlayEvent("event:/SwordAttack");
}

void FollowActor::Shoot()
{
	mArrowCount--;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bow.gpanim"), 1.0f);
	mAnimState = AnimationState::Attack;
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
	mAttackTimer = 0.8f;  
	mAudioComp->PlayEvent("event:/Arrow");
}

void FollowActor::Bomb()
{
	mBombCount--;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bomb.gpanim"), 1.0f);
	mAnimState = AnimationState::Attack;
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	BombActor* bomb = new BombActor(GetGame(), 0.15f, 1);
	bomb->SetPlayer(this);
	bomb->SetPosition(this->GetPosition()+Vector3(0.0,0.0,200.0) + dir * 100.0f);
	bomb->RotateToNewForward(dir);
	mAttackTimer = 1.2f;
	mAudioComp->PlayEvent("event:/Throw");
}

void FollowActor::Block() {
	if(mBlockBoxComp == nullptr){
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_block.gpanim"), 1.0f);
		mAnimState = AnimationState::Block;
		mBlockBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(25.0f, -1.0f, 50.0f),
			Vector3(30.0f, 1.0f, 170.0f));
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
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	const AABB& attackBox = mAttackBoxComp->GetWorldBox();
	const AABB& blockBox = mBlockBoxComp->GetWorldBox();

	Vector3 pos = GetPosition();
	auto* boss = GetGame()->GetBoss();
	auto& planes = GetGame()->GetPlanes();
	auto& enemy = GetGame()->GetEnemys();
	auto& item = GetGame()->GetDropItem();
	auto& explosion = GetGame()->GetExplosion();
	for (auto pa : planes)
	{
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		ResolveCollision(playerBox, planeBox, pos, mBoxComp);
	}

	for (auto en : enemy)
	{
		if (en != nullptr) {
			const AABB& enemyBox = en->GetBox()->GetWorldBox();
			ResolveCollision(playerBox,enemyBox,pos,mBoxComp);
						
			if(en->GetAttackBox() != nullptr)
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
							mHealth -= 0.25f;
						}
					}
				}
				else if (mBlockBoxComp == nullptr  && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
				{				
					mAudioComp->PlayEvent("event:/Hit");
					mDamageTimer = 4.0f;
					mHealth -= 0.25f;
				}
			}			
		}		
	}

	if (boss != nullptr) {

		const AABB& enemyBox = boss->GetBox()->GetWorldBox();
		ResolveCollision(playerBox, enemyBox, pos, mBoxComp);
		if (boss->GetAttackBox() != nullptr)
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
						mHealth -= 0.25f;
					}
				}
			}

			else if (mBlockBoxComp == nullptr && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mDamageTimer = 4.0f;
				mHealth -= 0.25f;
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

	for (auto ex : explosion)
	{
		if (ex != nullptr) {
			const AABB& exBox = ex->GetBox()->GetWorldBox();
			if (Intersect(playerBox, exBox) && mDamageTimer <= 0.0f)
			{
				mAudioComp->PlayEvent("event:/Hit");
				mDamageTimer = 4.0f;
				mHealth -= 0.25f;
			}
		}
	}
}

void FollowActor::ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent)
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
			jumpFlag = false;
				
		}

		boxComponent->GetOwner()->SetPosition(pos);
		boxComponent->OnUpdateWorldTransform();
	}
}