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
	//Enum�@State
	enum State
	{
		EJump,
		EFall,
		EGrounded,
		EDead
	};

	//Enum�@MoveState
	enum MoveState
	{
		EAttack,
		EPatrol,
		EBattle
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	EnemyActor(class Game* game);

	//�f�X�g���N�^
	~EnemyActor();

	//Update
	void UpdateActor(float deltaTime) override;
	void UpdateMoveState(float deltaTime);
	void UpdatePatrolState(float deltaTime);
    void UpdateBattleState(float deltaTime);
	void UpdateAttackState(float deltaTime);
	void UpdateTimers(float deltaTime);
	void HandleBlinking(float deltaTime);

	//Getterr,Setter
	void SetVisible(bool visible);
	void SetRandomRotation();
	void SetName(std::string enemyname) { name = enemyname; }
	void SetState(State state) { mState = state; }
	State GetState() const { return mState; }
	TypeID GetType() const override { return TEnemyActor; }
	float GetHealth() { return  mHealth; }
	class BoxComponent* GetBox() { return mBox; }
	class BoxComponent* GetAttackBox() { return mAttackBoxComp; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	int GenerateRandomValue(int max) { return rand() % max; }
	void CreateReactActor();
	void AlignToTarget();
	void Attack();
	void AttackGround();
	void AddAttackBox();
	void CleanUpAttackBox();
	void HandleAttackBox();
	void HandleDeath();
	void FixCollisions();
	void ResolveCollision(const AABB& aBox, const AABB& bBox, Vector3& pos, BoxComponent* boxComponent);

protected:

	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	bool mMoving;
	bool mReactFlag;
	bool groundFlag; //�W�����v�ɂ��󒆔���
	bool deathFlag; //���S����
	int randomValue;
	float mDamageTimer; //�_���[�W�̃N�[���^�C��
	float mBoxTimer; //�U�����蔭������
	float mMoveTimer; //�s���ω�����
	float blinkTime; //�_�Ŏ���
	float blinkInterval; //�_�ŊԊu
	float mAttackTimer; //�U���̃N�[���^�C��
	float forwardSpeed; 
	float strafeSpeed;
	float jumpSpeed;
	float mHealth;
	Vector3 diff; //FollowActor�Ƃ̋���
	std::string name; //�L�����N�^�[��

	State mState;
	MoveState mMoveState; //�G�̍s�����

	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBox;
	class BoxComponent* mAttackBoxComp;
	class Actor* mOwner;
};