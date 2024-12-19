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
#include "DefeatCamera.h"
#include <vector>
#include "Actor.h"
#include "AudioSystem.h"
#include "BossActor.h"
#include "FollowActor.h"
#include "FollowCamera.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
// FollowCamera class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
DefeatCamera::DefeatCamera(Actor* owner)
	:CameraComponent(owner)
	, mSpringConstant(128.0)
	, mTargetDist(100.0)
	, mVelocity(Vector3(0.0, 0.0, 0.0))
	, mActualPos(Vector3(-350.0, 0.0, 150.0))
	, mHorzDist(300)
	, mVertDist(300)
	, mMoveTime(5.4)
	, mElapsedTime(0.0f)
{
	
    mTargetPosition = Vector3(mCurrentPosition = Vector3(380.0f, 0.0f, 200.0f));
	mCurrentPosition = mOwner->GetPosition();

}

//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void DefeatCamera::Update(float deltaTime)
{
	// �o�ߎ��Ԃ��X�V
	mElapsedTime += deltaTime;

	// �o�ߎ��Ԃ��ړ����ԂŐ��K���i0.0�`1.0�͈̔́j
	float t = mElapsedTime / mMoveTime;
	t = Math::Clamp(t, 0.0f, 1.0f); // t���͈͊O�ɏo�Ȃ��悤�ɐ���

	// ���`��ԂŌ��݈ʒu���X�V
	mCurrentPosition.x = Math::Lerp(mCurrentPosition.x, mTargetPosition.x, t);
	mCurrentPosition.y = Math::Lerp(mCurrentPosition.y, mTargetPosition.y, t);
	mCurrentPosition.z = Math::Lerp(mCurrentPosition.z, mTargetPosition.z, t);

	// �I�������i�ڕW�ʒu�ɓ��B�����珈�����I������Ȃǁj
	if (t >= 1.0f) {
		mOwner->GetGame()->SetState(Game::GameState::EBossDefeat);
		// �K�v�ɉ����ăt���O�𗧂Ă�Ȃǂ̏���
		mElapsedTime = 0.0f;
	}

	mPitch += mPitchSpeed * deltaTime;
	mPitch = Math::Clamp(mPitch, -mMaxPitch * 0.7f, mMaxPitch * 0.6f);

	Vector3 up = Vector3::UnitZ;

	//�J�����̒����ʒu��Renderer�ɑ���
	Matrix4 view = Matrix4::CreateLookAt(mCurrentPosition, mOwner->GetPosition(), up);
	SetViewMatrix(view);

	//���������ʒu���J�����ł͖��L�����N�^�[���S�ɂ���
	Game* game = mOwner->GetGame();
	Vector3 playerPosition = mOwner->GetPosition();
	game->GetAudioSystem()->SetListener(view, Vector3::Zero, 1.0f, playerPosition);


}

