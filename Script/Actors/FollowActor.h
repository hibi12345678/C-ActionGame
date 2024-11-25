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
#include "BoxComponent.h"
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

	class FollowCamera* GetCameraComponent() { return mCameraComp; }
	class SkeletalMeshComponent* GetSekltalMesh() { return mMeshComp; }
	class  MoveComponent* GetMoveComponent() { return mMoveComp; }

	void ActorInput(const uint8_t* keys) override;
	void SetVisible(bool visible);
	void UpdateActor(float deltaTime) override;
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
	void Attack();
	void Shoot();
	void Bomb();
	TypeID GetType() const override { return TFollowActor; }
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	ItemState GetItemState() const { return mItemState; }
	void SetItemState(ItemState state) { mItemState = state; }
	float GetStamina()  { return  mStamina; }
	float GetHealth()  { return  mHealth; }
	float GetHealthRed() { return  mHealthRed; }
	int GetArrowNum() { return mArrowCount; }
	int GetBombNum() { return mBombCount; }
	Vector3 GetPos() const { return pos; }
	class BoxComponent* GetBox() { return mBoxComp; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	void FixCollisions();
	void ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent);
	void Block();
	void EquipSword();
	void UpdateDamage(float deltaTime);
	void HandleVisibility(float deltaTime);
	void UpdateState(float deltaTime);
	void UpdateTimers(float deltaTime);
	void UpdateStamina(float deltaTime);
	void UpdateMovement(float deltaTime);

private:
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
	State mState;
	ItemState mItemState;

	int mArrowCount;
	int mBombCount;

	float mAttackTimer; 
	float mBlockTimer; 
	float mBoxTimer;
	float mDamageTimer;
	float changeTimer;
	float blinkTime;
	float jumpSpeed;
	float blinkInterval;
	float mLastFootstep;
	float mStamina;
	float mHealth;
	float mHealthRed;

	bool isShiftPressed;
	bool blockPressed;
	bool deathFlag;
	bool jumpFlag;
	bool isVisible;
	bool mMoving;

	Vector3 pos;

};
