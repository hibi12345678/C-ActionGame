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
	//�f�X�g���N�^
	FollowActor(class Game* game);

	//Update
	void UpdateActor(float deltaTime) override;
	void UpdateState(float deltaTime);
	void UpdateTimers(float deltaTime);
	void UpdateStamina(float deltaTime);
	void UpdateMovement(float deltaTime);
	void UpdateDamage(float deltaTime);
	void HandleVisibility(float deltaTime);

	//����
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
	bool isShiftPressed; //�V�t�g(LShift)���͔���
	bool blockPressed; //�u���b�N(R)���͔���
	bool deathFlag; //���S����
	bool jumpFlag; //�W�����v(Space)����
	bool isVisible; //�\������
	bool mMoving; 
	bool mAttackFlag;
	bool lowStaminaFlag;
	int mArrowCount; //��̐�
	int mBombCount; //�{���̐�
	float mAttackTimer; //�U���̃N�[���^�C��
	float mBlockTimer; //�u���b�N����N�[���^�C��
	float mBoxTimer; //�U������N�[���^�C��
	float mDamageTimer; //�U���̃N�[���^�C��
	float changeTimer; //����؂�ւ��N�[���^�C��
	float blinkTime; //�_�Ŏ���
	float jumpSpeed; 
	float blinkInterval; //�_�ŊԊu
	float mLastFootstep; 
	float mStamina; //�X�^�~�i
	float mHealth; //�̗�
	float mHealthRed; //�̗͐ԃQ�[�W
	float mAnimTime; //�A�j���[�V��������
	float inertiaStrafe; //�������������x 
	float inertiaForward; //�������ʕ������x
	float mPlayTime;
	float mLastWalkstep; //���s���p
	float mLastRunstep; //���s���p
	Vector3 pos;

	State mState; 
	ItemState mItemState; //�������A�C�e���p�X�e�[�g
	AnimationState mAnimState; //�A�j���[�V�����p�X�e�[�g

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
