//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include "Actor.h"
#include "BoxComponent.h"


///////////////////////////////////////////////////////////////////////////////
//EnemyActor class
///////////////////////////////////////////////////////////////////////////////
class EnemyActor : public Actor
{
public:
	//Enum　State
	enum class State
	{
		EJump,
		EFall,
		EGrounded,
		EDead
	};

	//Enum　MoveState
	enum class MoveState
	{
		EAttack,
		EPatrol,
		EBattle
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
		Roa,
		Block
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	explicit EnemyActor(class Game* game);

	//デストラクタ
	~EnemyActor();

	//Update
	void UpdateActor(float deltaTime) override;
	void UpdateMoveState(float deltaTime);
	void UpdatePatrolState(float deltaTime);
    void UpdateBattleState(float deltaTime);
	virtual void UpdateAttackState(float deltaTime);
	void UpdateTimers(float deltaTime);
	void HandleBlinking(float deltaTime);

	//Getterr,Setter
	State GetState() const { return mState; }
	TypeID GetType() const override { return TEnemyActor; }
	float GetHealth() { return  mHealth; }
	class BoxComponent* GetBox() { return mBox; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }
	void SetVisible(bool visible);
	void SetRandomRotation();
	void SetName(const std::string& enemyname) { name = enemyname; }
	void SetState(State state) { mState = state; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	int GenerateRandomValue(int max) { return rand() % max; }
	void CreateReactActor();
	void AlignToTarget();
	void Attack();
	void AttackGround();
	virtual void AddAttackBox();
	void CleanUpAttackBox();
	void HandleAttackBox();
	virtual void HandleDeath();
	virtual void FixCollisions();
	void ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent);

protected:

	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	bool mMoving;
	bool mReactFlag;
	bool groundFlag; //ジャンプによる空中判定
	bool normalAttack; //攻撃のBox判定
	bool deathFlag; //死亡判定
	int randomValue;
	float mDamageTimer; //ダメージのクールタイム
	float mBoxTimer; //攻撃判定発生時間
	float mMoveTimer; //行動変化時間
	float blinkTime; //点滅時間
	float blinkInterval; //点滅間隔
	float mAttackTimer; //攻撃のクールタイム
	float forwardSpeed; 
	float strafeSpeed;
	float jumpSpeed;
	float mHealth;
	Vector3 diff; //FollowActorとの距離
	std::string name; //キャラクター名

	State mState;
	MoveState mMoveState; //敵の行動状態
	AnimationState mAnimState; //アニメーション用ステート

	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBox;
	class BoxComponent* mAttackBoxComp;
	class Actor* mOwner;
};