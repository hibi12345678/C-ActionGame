#pragma once
#include "Actor.h"
#include <memory>
#include <cmath>
#include <cstdlib> 
#include <ctime> 
#include <string>
#include "BoxComponent.h"
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
	void UpdateActor(float deltaTime) override;
	void UpdateMoveState(float deltaTime);
	void UpdatePatrolState(float deltaTime);
    void UpdateBattleState(float deltaTime);
	void UpdateAttackState(float deltaTime);
	void SetRandomRotation();
	int GenerateRandomValue(int max){return rand() % max;}
	void CreateReactActor();
	void AlignToTarget();
	void SetVisible(bool visible);
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
	void Attack();
	void AttackGround();
	void AddAttackBox();
	void CleanUpAttackBox();
	void HandleAttackBox();
	void HandleDeath();
	void HandleBlinking(float deltaTime);
	void UpdateTimers(float deltaTime);
	void SetName(std::string enemyname) { name = enemyname; }
	void SetState(State state) { mState = state; }
	State GetState() const { return mState; }
	TypeID GetType() const override { return TEnemyActor; }
	float GetHealth() { return  mHealth; }
	class BoxComponent* GetBox() { return mBox; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	void FixCollisions();
	void ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent);

protected:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBox;
	class BoxComponent* mAttackBoxComp;
	class Actor* mOwner;

	State mState;
	MoveState mMoveState;

	int randomValue;

	float mDamageTimer;
	float mBoxTimer;
	float mMoveTimer;
	float blinkTime;
	float mAttackTimer;
	float blinkInterval; 
	float forwardSpeed;
	float strafeSpeed;
	float jumpSpeed;
	float mHealth;
	
	bool isVisible;
	bool mMoving;
	bool mReactFlag;
	bool groundFlag;
	bool deathFlag;

	Vector3 diff;

	std::string name;

};