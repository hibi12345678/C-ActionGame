// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"
#include "BoxComponent.h"
#include "SoundEvent.h"


///////////////////////////////////////////////////////////////////////////////
//FollowActor class
///////////////////////////////////////////////////////////////////////////////
class FollowActor : public Actor
{
public:
	//Enum State
	enum State
	{
		EJump,
		EFall,
		EGrounded,
		EDead
	};

	//Enum ItemState 
	enum ItemState
	{
		ESword,
		ETorch,
		EBow,
		EBomb
	};

	//Enum class AnimationState
	enum class AnimationState {
		Idle,
		Walk,
		Back,
		Right,
		Left,
		Run,
		Jump,
		Die,
		Attack,
		Block
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//デストラクタ
	FollowActor(class Game* game);

	//Update
	void UpdateActor(float deltaTime) override;
	void UpdateState(float deltaTime);
	void UpdateTimers(float deltaTime);
	void UpdateStamina(float deltaTime);
	void UpdateMovement(float deltaTime);
	void UpdateDamage(float deltaTime);
	void HandleVisibility(float deltaTime);

	//入力
	void ActorInput(const uint8_t* keys) override;

	//Getter,Setter
	class FollowCamera* GetCameraComponent() { return mCameraComp; }
	class SkeletalMeshComponent* GetSekltalMesh() { return mMeshComp; }
	class  MoveComponent* GetMoveComponent() { return mMoveComp; }
	TypeID GetType() const override { return TFollowActor; }
	State GetState() const { return mState; }
	ItemState GetItemState() const { return mItemState; }
	float GetStamina() { return  mStamina; }
	float GetHealth() { return  mHealth; }
	float GetHealthRed() { return  mHealthRed; }
	int GetArrowNum() { return mArrowCount; }
	int GetBombNum() { return mBombCount; }
	Vector3 GetPos() const { return pos; }
	class BoxComponent* GetBox() { return mBoxComp; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	float GetPlayTime() { return mPlayTime;}
	bool GetVisible() { return isVisible; }
	void SetVisible(bool visible);
	void SetState(State state) { mState = state; }
	void SetItemState(ItemState state) { mItemState = state; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	void Attack();
	void Shoot();
	void Bomb();
	void FixCollisions();
	void ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent);
	void Block();
	void EquipSword();
	void DeleteCamera();



private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool isShiftPressed; //シフト(LShift)入力判定
	bool blockPressed; //ブロック(R)入力判定
	bool deathFlag; //死亡判定
	bool jumpFlag; //ジャンプ(Space)入力
	bool isVisible; //表示判定
	bool mMoving; 
	bool mAttackFlag;
	bool lowStaminaFlag;
	int mArrowCount; //矢の数
	int mBombCount; //ボムの数
	float mAttackTimer; //攻撃のクールタイム
	float mBlockTimer; //ブロック判定クールタイム
	float mBoxTimer; //攻撃判定クールタイム
	float mDamageTimer; //攻撃のクールタイム
	float changeTimer; //武器切り替えクールタイム
	float blinkTime; //点滅時間
	float jumpSpeed; 
	float blinkInterval; //点滅間隔
	float mLastFootstep; 
	float mStamina; //スタミナ
	float mHealth; //体力
	float mHealthRed; //体力赤ゲージ
	float mAnimTime; //アニメーション時間
	float inertiaStrafe; //慣性横方向速度 
	float inertiaForward; //慣性正面方向速度
	float mPlayTime;
	float mLastWalkstep; //歩行音用
	float mLastRunstep; //歩行音用
	Vector3 pos;

	State mState; 
	ItemState mItemState; //装備中アイテム用ステート
	AnimationState mAnimState; //アニメーション用ステート

	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class FPSCamera* mFPSCamera;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBoxComp;
	class BoxComponent* mAttackBoxComp;
	class BoxComponent* mBlockBoxComp;
	class AudioSystem* mAudioSystem;
	SoundEvent mFootstep;
	SoundEvent mRunstep;

};
