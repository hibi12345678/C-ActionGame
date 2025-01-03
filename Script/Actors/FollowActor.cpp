//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "FollowActor.h"
#include "Animation.h"
#include "ArrowActor.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "BombActor.h"
#include "BossActor.h"
#include "BoxComponent.h"
#include "DropItemActor.h"
#include "EnemyActor.h"
#include "ExplosionActor.h"
#include "FPSCamera.h"
#include "FollowCamera.h"
#include "Game.h"
#include "LevelLoader.h"
#include "MoveComponent.h"
#include "PlaneActor.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "SwordActor.h"
#include "SkeletalMeshComponent.h"
#include "StageChange.h"

///////////////////////////////////////////////////////////////////////////////
// FollowActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
	, mState(State::EGrounded)
	, mItemState(ItemState::ESword)
	, jumpSpeed(0.0f)
	, mStamina(1.0f)
	, mHealth(1.0f)
	, mHealthRed(1.0f)
	, isShiftPressed(false)
	, mAttackFlag(false)
	, lowStaminaFlag(false)
	, mAttackBoxComp(nullptr)
	, mBlockBoxComp(nullptr)
	, mAttackTimer(1.0f)
	, mBoxTimer(0.0f)
	, mDamageTimer(0.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.1f)
	, isVisible(true)
	, blockPressed(false)
	, mBlockTimer(0.0f)
	, mBombCount(2)
	, mArrowCount(4)
	, mFPSCamera(nullptr)
	, changeTimer(0.0f)
	, deathFlag(true)
	, jumpFlag(false)
	, mAnimState(AnimationState::Idle)
	, mAnimTime(0.0f)
	, mPlayTime(0.0f)
	, forwardSpeed(0.0f)
	, strafeSpeed(0.0f)
	, mAttackNum(1)
	, attackCombo(false)

{
	mMeshComp = new SkeletalMeshComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Walk");
	mFootstep.SetPaused(true);
	mLastRunstep = 0.0f;
	mRunstep = mAudioComp->PlayEvent("event:/Run");
	mRunstep.SetPaused(true);
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	game->SetFollowActor(this);
	// Add a box component
	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-25.0f, -25.0f, 0.0f),
		Vector3(25.0f, 25.0f, 150.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

	EquipSword();

}


//-----------------------------------------------------------------------------
//   入力処理
//-----------------------------------------------------------------------------
void FollowActor::ActorInput(const uint8_t* keys)
{
	if (GetGame()->GetState() == Game::GameState::EBossDefeat ||
		GetGame()->GetState() == Game::GameState::EBossMovie)
	{
		return;
	}

	ProcessMovementInput(keys);
	ProcessJumpInput(keys);
	ProcessBlockInput(keys);
	ProcessMouseInput();
	ProcessAttackInput();

}

//-----------------------------------------------------------------------------
//   移動処理
//-----------------------------------------------------------------------------
void FollowActor::ProcessMovementInput(const uint8_t* keys)
{
	forwardSpeed = 0.0f;
	strafeSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W] && mAttackTimer <= 0.0f)
	{
		forwardSpeed += HandleForwardMovement(keys);
	}
	else if (keys[SDL_SCANCODE_S] && mAttackTimer <= 0.0f)
	{
		forwardSpeed -= HandleBackwardMovement();
	}

	if (keys[SDL_SCANCODE_A] && mAttackTimer <= 0.0f)
	{
		strafeSpeed -= HandleStrafeMovement(AnimationState::Left);
	}
	else if (keys[SDL_SCANCODE_D] && mAttackTimer <= 0.0f)
	{
		strafeSpeed += HandleStrafeMovement(AnimationState::Right);
	}

	if (jumpFlag)
	{
		strafeSpeed += inertiaStrafe;
		forwardSpeed += inertiaForward;
	}

	// Did we just start moving?
	if (!mMoving && (!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)))
		mMoving = true;

	// Or did we just stop moving?
	else if (mMoving && Math::NearZero(forwardSpeed) && Math::NearZero(strafeSpeed) 
		&& !mAttackBoxComp && !mBlockBoxComp && mState == State::EGrounded 
		&& GetGame()->GetState() == Game::GameState::EGameplay && mAttackTimer <= 0.0f)
	{
		mMoving = false;

		if (mItemState == ItemState::EBow )
			PlayAnimation("Assets/Anim/Player_bowidle.gpanim", AnimationState::Idle);
		
		else 
			PlayAnimation("Assets/Anim/Player_idle.gpanim", AnimationState::Idle);
			
	}

	if (mHealth > 0.0f) {
		mMoveComp->SetForwardSpeed(forwardSpeed);
		mMoveComp->SetStrafeSpeed(strafeSpeed);
	}
	else {
		mMoveComp->SetForwardSpeed(0.0f);
		mMoveComp->SetStrafeSpeed(0.0f);
	}

}

float FollowActor::HandleForwardMovement(const uint8_t* keys)
{
	float speed = 200.0f;
	if (jumpFlag)
		speed -= 100.0f;

	isShiftPressed = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
	if (isShiftPressed && mStamina > 0.0f && mState == State::EGrounded)
	{

		mStamina -= 0.01f;
		if (!lowStaminaFlag) {
			speed *= 2.0f;
		}
		if (mAnimState != AnimationState::Run && !lowStaminaFlag) {
			PlayAnimation("Assets/Anim/Player_run.gpanim", AnimationState::Run);
			
		}
		else if (mAnimState != AnimationState::Walk && lowStaminaFlag) {
			PlayAnimation("Assets/Anim/Player_walk.gpanim", AnimationState::Walk);
			
		}
		if (mStamina < 0.0f) {
			lowStaminaFlag = true;
		}
	}
	else if (!isShiftPressed && mAnimState != AnimationState::Walk && mState == State::EGrounded)
	{
		PlayAnimation("Assets/Anim/Player_walk.gpanim", AnimationState::Walk);
	}
	return speed;
}


float FollowActor::HandleBackwardMovement()
{
	if (jumpFlag)
		return 100.0f;
	if (mAnimState != AnimationState::Back && mState == State::EGrounded)
	    PlayAnimation("Assets/Anim/Player_back.gpanim", AnimationState::Back);
	return 200.0f;
}


float FollowActor::HandleStrafeMovement(AnimationState direction)
{
	if (jumpFlag)
		return 100.0f;

	if (mAnimState != direction && forwardSpeed == 0.0f && mState == State::EGrounded)
	{
		std::string animPath = direction == AnimationState::Left ?
			"Assets/Anim/Player_left.gpanim" :
			"Assets/Anim/Player_right.gpanim";
		PlayAnimation(animPath, direction);
	}
	return 200.0f;
}


//-----------------------------------------------------------------------------
//   ジャンプ処理
//-----------------------------------------------------------------------------
void FollowActor::ProcessJumpInput(const uint8_t* keys)
{
	if (keys[SDL_SCANCODE_SPACE] && mState == State::EGrounded &&
		mStamina > 0.1f && mHealth > 0.0f && mAttackTimer <= 0.0f)
	{
		ClearBlockState();
		mState = State::EJump;
		PlayAnimation("Assets/Anim/Player_jump.gpanim", AnimationState::Jump);
		jumpSpeed += -20000.0f;
		mStamina -= 0.1f;
		jumpFlag = true;
		inertiaStrafe = mMoveComp->GetStrafeSpeed();
		inertiaForward = mMoveComp->GetForwardSpeed();
	}
}


//-----------------------------------------------------------------------------
//   ブロック処理
//-----------------------------------------------------------------------------
void FollowActor::ProcessBlockInput(const uint8_t* keys)
{
	if (keys[SDL_SCANCODE_R] && mState == State::EGrounded &&
		mStamina > 0.1f && mHealth > 0.0f && mAttackTimer <= 0.0f
		&& mItemState == ItemState::ESword)
	{
		Block();
	}
	else
	{
		ClearBlockState();
	}

	if (keys[SDL_SCANCODE_R] && mHealth > 0.0f && mItemState == ItemState::EBow && changeTimer > 0.5f) {

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

	if (mItemState != ItemState::EBow) {
		if (!mCameraComp && mFPSCamera) {
			mMeshComp->SetVisible(true);
			mCameraComp = new FollowCamera(this);
			delete mFPSCamera;
			mFPSCamera = nullptr;
		}
	}
}


//-----------------------------------------------------------------------------
//   ブロックのBoxを削除する
//-----------------------------------------------------------------------------
void FollowActor::ClearBlockState()
{
	if (mBlockBoxComp)
	{
		delete mBlockBoxComp;
		mBlockBoxComp = nullptr;
		blockPressed = false;
	}
}

//-----------------------------------------------------------------------------
//   マウスの移動をカメラとキャラクターの回転とする処理
//-----------------------------------------------------------------------------
void FollowActor::ProcessMouseInput() {
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

	if (mHealth > 0.0f) {
		mMoveComp->SetAngularSpeed(angularSpeed);
		if (mCameraComp) {
			mCameraComp->SetPitchSpeed(pitchSpeed);
		}
		else if (mFPSCamera) {
			mFPSCamera->SetPitchSpeed(pitchSpeed);
		}
	}
	else {
		mMeshComp->SetVisible(true);
		mMoveComp->SetAngularSpeed(0.0f);
		mCameraComp->SetPitchSpeed(0.0f);
		if (mCameraComp) {
			mCameraComp->SetPitchSpeed(0.0f);
		}
		else if (mFPSCamera) {
			mFPSCamera->SetPitchSpeed(0.0f);
		}
	}

}


//-----------------------------------------------------------------------------
//   マウス入力処理
//-----------------------------------------------------------------------------
void FollowActor::ProcessAttackInput() {
	int butx = 0, buty = 0;

	Uint32 buttons = SDL_GetMouseState(&butx, &buty);

	// マウス左クリックが押されたか確認
	if (mAttackTimer < 0.0f && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && 
		mState == State::EGrounded && mStamina >= 0.2f) {

		delete mBlockBoxComp;  // メモリの解放
		mBlockBoxComp = nullptr;  // ポインタをクリア
		blockPressed = false;
		mStamina -= 0.2f;

		if (mItemState == ItemState::ESword || mItemState == ItemState::ETorch) {
			Attack();

		}
		if (mItemState == ItemState::EBow) {
			Shoot();
		}
		if (mItemState == ItemState::EBomb) {
			Bomb();
		}
	}
	// 連続攻撃確認
	else if (mAttackTimer <= 0.5f && mAttackTimer >= 0.0f && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
		mState == State::EGrounded && mStamina >= 0.2f
		&& (mItemState == ItemState::ESword || mItemState == ItemState::ETorch) 
		&& mAttackNum <= 3) 
	{
		
		attackCombo = true;
	
	}
}

//-----------------------------------------------------------------------------
//   アニメーションの再生
//-----------------------------------------------------------------------------
void FollowActor::PlayAnimation(const std::string& path, AnimationState state)
{
	float animRate = 1.0f;
	if (state == AnimationState::Attack && mItemState == ItemState::ESword)
		animRate = 1.5f;
	
	mMeshComp->PlayAnimation(GetGame()->GetAnimation(path), animRate);
	mAnimState = state;
}


//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void FollowActor::UpdateActor(float deltaTime) {

	if (GetGame()->GetState() == Game::GameState::EBossDefeat 
		|| GetGame()->GetState() == Game::GameState::EBossMovie) {
		
		if (mCameraComp) {
			delete mCameraComp;
			mCameraComp = nullptr;
		}
		if (mFPSCamera) {
			delete mFPSCamera;
			mFPSCamera = nullptr;
		}
		if (mAnimState != AnimationState::Idle) {
			
			PlayAnimation("Assets/Anim/Player_idle.gpanim", AnimationState::Idle);

		}
		SetPosition(Vector3(-3000.0f, 0.0f, -100.0f));

	}
	else{

		mLastFootstep -= deltaTime;
		mLastRunstep -= deltaTime;
		if ((!Math::NearZero(mMoveComp->GetForwardSpeed()) ||
			!Math::NearZero(mMoveComp->GetStrafeSpeed())) &&
			mLastFootstep <= 0.0f && (mAnimState == AnimationState::Walk 
				|| mAnimState == AnimationState::Left 
				|| mAnimState == AnimationState::Right 
				|| mAnimState == AnimationState::Back))
		{
			mFootstep.SetPaused(false);
			mFootstep.Restart();
			mLastFootstep = 0.5f;
		}
		if ((!Math::NearZero(mMoveComp->GetForwardSpeed()) ||
			!Math::NearZero(mMoveComp->GetStrafeSpeed())) &&
			mLastRunstep <= 0.0f && (mAnimState == AnimationState::Run))
		{
			mRunstep.SetPaused(false);
			mRunstep.Restart();
			mLastRunstep = 0.46f;
		}

		mPlayTime += deltaTime;
		UpdateState(deltaTime);
		UpdateStamina(deltaTime);
		UpdateMovement(deltaTime);
		UpdateTimers(deltaTime);
		mState = State::EFall;
		FixCollisions();
		HandleVisibility(deltaTime);
	}

}


//-----------------------------------------------------------------------------
// UpdateTimer
//-----------------------------------------------------------------------------
void FollowActor::UpdateTimers(float deltaTime) {

	if (mHealth <= 0.0f && deathFlag && GetGame()->GetState()==Game::GameState::EGameplay) {
		PlayAnimation("Assets/Anim/Player_dying.gpanim", AnimationState::Die);
		mHealthRed = 0.0f;
		mHealth = 0.0f;
		GetGame()->SetState(Game::GameState::EGameOver);
		deathFlag = false;
	}

	changeTimer += deltaTime;

	if (mAttackTimer > 0.0f) {
		mAttackTimer -= deltaTime;
		if ((mItemState == ItemState::ESword ||mItemState == ItemState::ETorch) 
			&& mAttackTimer <= 0.8f && !mAttackFlag) {
			mAttackBoxComp = new BoxComponent(this);
			AABB myBox(Vector3(25.0f, -25.0f, 100.0f),
				Vector3(75.0f, 25.0f, 170.0f));
			mAttackBoxComp->SetObjectBox(myBox);
			mAttackBoxComp->SetShouldRotate(true);
			mBoxTimer = 0.5f;
			mAttackFlag = true;
		}
		if (mAttackTimer <= 0.0f && !attackCombo) {
			mAttackNum = 1;
			attackCombo = false;
			
			if (mItemState == ItemState::EBow) {
				PlayAnimation("Assets/Anim/Player_bowidle.gpanim", AnimationState::Idle);
				mAttackFlag = false;
			}
			else{
				PlayAnimation("Assets/Anim/Player_idle.gpanim", AnimationState::Idle);
				mAttackFlag = false;
			}
		}
		else if (mAttackTimer <= 0.0f && attackCombo) {
			
			mStamina -= 0.2f;
			attackCombo = false;
			mAttackFlag = false;
			Attack();
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


//-----------------------------------------------------------------------------
// スタミナ
//-----------------------------------------------------------------------------
void FollowActor::UpdateStamina(float deltaTime) {
	if (mState == State::EGrounded && mStamina < 1.0f && mAttackTimer <= 0.0f) {

		mStamina = std::min(mStamina + 0.005f, 1.0f);
	}
	if (mStamina >= 0.2f)
		lowStaminaFlag = false;
}


//-----------------------------------------------------------------------------
//　垂直方向の処理
//-----------------------------------------------------------------------------
void FollowActor::UpdateMovement(float deltaTime) {
	if (mState == State::EJump) {
		jumpSpeed += 50000.0f * deltaTime; // 重力
		if (jumpSpeed >= 0.0f) mState = State::EFall;
	}
	else if (mState == State::EFall) {
		jumpSpeed += 100000.0f * deltaTime;
	}
	else if (mState == State::EGrounded) {
		jumpSpeed = 0.0f;
	}
	mMoveComp->SetJumpSpeed(jumpSpeed * deltaTime);
}


//-----------------------------------------------------------------------------
//  ItemState処理
//-----------------------------------------------------------------------------
void FollowActor::UpdateState(float deltaTime) {

	if (mItemState == ItemState::EBow && mArrowCount <= 0) {
		EquipSword();
	}
	else if (mItemState == ItemState::EBomb && mBombCount <= 0) {
		EquipSword();
	}


}


//-----------------------------------------------------------------------------
//　剣と盾を装備させる
//-----------------------------------------------------------------------------
void FollowActor::EquipSword() {
	
	sword= new SwordActor(GetGame(), 1.0f, 0);
	shield= new SwordActor(GetGame(), 1.0f, 1);

	if (mItemState != ItemState::ESword) {
		mAudioComp->PlayEvent("event:/Equipped");
		mItemState = ItemState::ESword;
	}

}


//-----------------------------------------------------------------------------
//  ダメージのクールタイ処理と可視状態の切り替え
//-----------------------------------------------------------------------------
void FollowActor::UpdateDamage(float deltaTime) {

	if (mDamageTimer >= 2.0f) {
		mHealthRed = std::max(mHealthRed - 0.102f * deltaTime, 0.0f);
	}

	blinkTime += deltaTime;
	if (blinkTime >= blinkInterval) {
		isVisible = !isVisible;
		mMeshComp->SetVisible(isVisible);
		blinkTime = 0.0f;
	}

}


//-----------------------------------------------------------------------------
//  可視状態をtrueにする
//-----------------------------------------------------------------------------
void FollowActor::HandleVisibility(float deltaTime) {
	if (mDamageTimer <= 2.5f) {
		if (!mFPSCamera) {
			mMeshComp->SetVisible(true);
		}
		blinkTime = 0.0f;
	}
}


//-----------------------------------------------------------------------------
//  可視状態の処理
//-----------------------------------------------------------------------------
void FollowActor::SetVisible(bool visible)
{

	mMeshComp->SetVisible(visible);
}


//-----------------------------------------------------------------------------
//  剣の攻撃処理
//-----------------------------------------------------------------------------
void FollowActor::Attack() {

	mAttackTimer = 1.0f;
	
	if(mAttackNum == 1)
	    PlayAnimation("Assets/Anim/Player_attack.gpanim", AnimationState::Attack);
	if (mAttackNum == 2)
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_attack2.gpanim"), 1.2f);
	if (mAttackNum == 3) {
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_attack3.gpanim"), 1.2f);
		attackCombo = false;
	}
		
	mAttackNum++;
	if (mAttackNum == 4) {
		mAttackNum = 1;
	}
	if (mItemState ==FollowActor::ItemState::ESword) {
		mAudioComp->PlayEvent("event:/SwordAttack");
	}
	else if (mItemState == FollowActor::ItemState::ETorch) {
		mAudioComp->PlayEvent("event:/Torch");
	}
}


//-----------------------------------------------------------------------------
// 　弓の攻撃処理
//-----------------------------------------------------------------------------
void FollowActor::Shoot()
{
	mArrowCount--;
	PlayAnimation("Assets/Anim/Player_bow.gpanim", AnimationState::Attack);
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	ArrowActor* arrow = new ArrowActor(GetGame());
	if (mCameraComp) {
		arrow->SetPosition(this->GetPosition() + Vector3(0.0, 60.0, 120.0));
	}
	else {
		arrow->SetPosition(GetPosition() + Vector3(0.0, 60.0, 120.0));
	}
	arrow->SetPlayer(this);
	arrow->RotateToNewForward(dir);
	mAttackTimer = 0.8f;
	mAudioComp->PlayEvent("event:/Arrow");
}


//-----------------------------------------------------------------------------
//  ボムの攻撃処理
//-----------------------------------------------------------------------------
void FollowActor::Bomb()
{
	mBombCount--;
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_bomb.gpanim"), 1.0f);
	mAnimState = AnimationState::Attack;
	Vector3 start, dir;
	GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	BombActor* bomb = new BombActor(GetGame(), 0.10f, 1);
	bomb->SetPlayer(this);
	bomb->SetPosition(GetPosition() + Vector3(0.0, 0.0, 200.0) + GetForward() * 50.0f);
	bomb->RotateToNewForward(dir);
	mAttackTimer = 1.2f;
	mAudioComp->PlayEvent("event:/Throw");
}


//-----------------------------------------------------------------------------
// ブロック処理
//-----------------------------------------------------------------------------
void FollowActor::Block() {
	if (mBlockBoxComp == nullptr) {
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_block.gpanim"), 1.0f);
		mAnimState = AnimationState::Block;
		mBlockBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(25.0f, -1.0f, 50.0f),
			Vector3(30.0f, 1.0f, 170.0f));
		mBlockBoxComp->SetObjectBox(myBox);
		mBlockBoxComp->SetShouldRotate(true);
	}
}


//-----------------------------------------------------------------------------
// ブロック処理
//-----------------------------------------------------------------------------
void FollowActor::DeleteCamera() {


	if (mCameraComp) {
		delete mCameraComp;
		mCameraComp = nullptr;
	}
	else {
		mCameraComp = new FollowCamera(this);
	}
}


//-----------------------------------------------------------------------------
//  jsonファイルからの読み取り
//-----------------------------------------------------------------------------
void FollowActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}


//-----------------------------------------------------------------------------
//  jsonファイルへの書き込み
//-----------------------------------------------------------------------------
void FollowActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}


//-----------------------------------------------------------------------------
//  衝突処理
//-----------------------------------------------------------------------------
void FollowActor::FixCollisions()
{
 	ComputeWorldTransform();
	mBoxComp->OnUpdateWorldTransform();
	const AABB& playerBox = mBoxComp->GetWorldBox();
	const AABB& blockBox = mBlockBoxComp->GetWorldBox();

	Vector3 pos = GetPosition();
	auto& bosses = GetGame()->GetBoss();
	auto& planes = GetGame()->GetPlanes();
	auto& enemy = GetGame()->GetEnemys();
	auto& item = GetGame()->GetDropItem();
	auto& explosion = GetGame()->GetExplosion();
	auto& stageChange = GetGame()->GetStageChange();

	for (auto pa : planes)
	{
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		ResolveCollision(playerBox, planeBox, pos, mBoxComp);
	}

	for (auto sc : stageChange)
	{
		const AABB& scBox = sc->GetBox()->GetWorldBox();
		if (Intersect(playerBox, scBox))
		{
			sc->GetPosition();
			

			if (Game::GameState::EGameplay == GetGame()->GetState() && mState  == FollowActor::State::EGrounded) {
				if (mItemState != ItemState::EBow) {
					if (!mCameraComp && mFPSCamera) {
						mMeshComp->SetVisible(true);
						mCameraComp = new FollowCamera(this);
						delete mFPSCamera;
						mFPSCamera = nullptr;
					}
				}
				GetGame()->SetState(Game::GameState::ELoadStage);

				GetGame()->LoadStage();
			}
		}

	}

	for (auto en : enemy)
	{
		if (en != nullptr && Game::GameState::EGameOver != GetGame()->GetState()) {
			const AABB& enemyBox = en->GetBox()->GetWorldBox();
			ResolveCollision(playerBox, enemyBox, pos, mBoxComp);

			if (en->GetAttackBox() != nullptr)
			{
				const AABB& enemyAttackBox = en->GetAttackBox()->GetWorldBox();
				if (mBlockBoxComp != nullptr) {

					if (Intersect(blockBox, enemyAttackBox)) {
						if (mBlockTimer <= 0.0f) {
							mBlockTimer = 0.5f;
							mStamina -= 0.2f;						
							if (mDamageTimer < 0.5f) {
								mAudioComp->PlayEvent("event:/Block");
								mDamageTimer = 0.5f;
							}
						}
					}
					else if (!Intersect(blockBox, enemyAttackBox)) {
						if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
							mAudioComp->PlayEvent("event:/Hit");
							mDamageTimer = 3.0f;
							mHealth -= 0.1f;
						}
					}
				}
				else if (mBlockBoxComp == nullptr && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
				{
					mAudioComp->PlayEvent("event:/Hit");
					mDamageTimer = 3.0f;
					mHealth -= 0.1f;
				}
			}
		}
	}

	for (auto boss : bosses) {
		if (boss != nullptr && GetGame()->GetStageNumber() == 2) {

			const AABB& enemyBox = boss->GetBox()->GetWorldBox();
			ResolveCollision(playerBox, enemyBox, pos, mBoxComp);
			if (boss->GetAttackBox() != nullptr)
			{
				const AABB& enemyAttackBox = boss->GetAttackBox()->GetWorldBox();
				if (mBlockBoxComp != nullptr) {

					if (Intersect(blockBox, enemyAttackBox)) {

						if (mBlockTimer <= 0.0f) {
							mBlockTimer = 1.0f;
							mStamina -= 0.2f;
							mAudioComp->PlayEvent("event:/Block");
						}
					}
					else if (!Intersect(blockBox, enemyAttackBox)) {
						if (Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f) {
							mAudioComp->PlayEvent("event:/Hit");
							mDamageTimer = 3.0f;
							mHealth -= 0.1f;
						}
					}
				}

				else if (mBlockBoxComp == nullptr && Intersect(playerBox, enemyAttackBox) && mDamageTimer <= 0.0f)
				{
					mAudioComp->PlayEvent("event:/Hit");
					mDamageTimer = 3.0f;
					mHealth -= 0.1f;
				}
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
					mArrowCount += 2;
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
				mDamageTimer = 3.0f;
				mHealth -= 0.2f;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//  衝突時の重なりを避ける処理
//-----------------------------------------------------------------------------
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
			mState = State::EGrounded;
			if (jumpFlag == true) {
				mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/Anim/Player_idle.gpanim"), 1.0f);
				mAnimState = AnimationState::Idle;
				jumpFlag = false;
			}			
		}

		boxComponent->GetOwner()->SetPosition(pos);
		boxComponent->OnUpdateWorldTransform();
	}
}