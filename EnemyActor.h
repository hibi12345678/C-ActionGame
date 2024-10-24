// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class Game* game);
	~EnemyActor();
	enum State
	{
		EJump,
		EFall,
		EGrounded,
		EDead
	};

	enum MoveState
	{
		EAttack,
		EPatrol,
		EBattle
	};
	void SetVisible(bool visible);
	void UpdateActor(float deltaTime) override;
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
	void Attack();
	void AttackGround();
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	TypeID GetType() const override { return TEnemyActor; }

	float GetHealth() { return  mHealth; }
	class BoxComponent* GetBox() { return mBox; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	void FixCollisions();
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBox;
	class BoxComponent* mAttackBoxComp;

	float mAttackTimer;  // 攻撃判定用のタイマー
	bool mMoving;
	// Actor's state
	State mState;
	MoveState mMoveState;
	float jumpSpeed;
	float mHealth;
	//ダメージ用
	float mDamageTimer;
	float mBoxTimer;
	float mMoveTimer;
	float blinkTime;
	float blinkInterval; // 0.5秒ごとに点滅
	bool isVisible;
	class Actor* mOwner;
	Vector3 delta;
	int randomValue;
	bool mReactFlag;
protected:
	void SetViewMatrix(const Matrix4& view);
};
