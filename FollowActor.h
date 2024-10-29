// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);
	enum State
	{
		EJump,
		EFall,
		EGrounded,
		EDead
	};
	enum ItemState
	{
		ESword,
		ETorch,
		EBow,
		EBomb
	};
	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);
	void UpdateActor(float deltaTime) override;
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
	void Attack();
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	ItemState GetItemState() const { return mItemState; }
	void SetItemState(ItemState state) { mItemState = state; }
	TypeID GetType() const override { return TFollowActor; }
	float GetStamina()  { return  mStamina; }
	float GetHealth()  { return  mHealth; }
	Vector3 GetPos() { return pos; }
	class BoxComponent* GetBox() { return mBoxComp; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	void FixCollisions();
	void Block();
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBoxComp;
	SoundEvent mFootstep;
	class BoxComponent* mAttackBoxComp;
	class BoxComponent* mBlockBoxComp;
	class AudioSystem* mAudioSystem;
	float mAttackTimer;  // 攻撃判定用のタイマー
	float mBlockTimer;  // 攻撃判定用のタイマー
	float mBoxTimer;  // 攻撃判定用のタイマー
	float mDamageTimer;
	float blinkTime;
	float blinkInterval; // 0.5秒ごとに点滅
	bool isVisible;
	bool mMoving;
	// Actor's state
	State mState;
	ItemState mItemState;
	float mStamina;
	float mHealth;
	float jumpSpeed;
	Vector3 pos;
	bool isShiftPressed;
	bool blockPressed;
	float mLastFootstep;
	int mArrowCount;
	int mBombCount;
};
