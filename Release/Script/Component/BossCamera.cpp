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
#include "BossCamera.h"
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
//      コンストラクタです.
//-----------------------------------------------------------------------------
BossCamera::BossCamera(Actor* owner)
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

	mCurrentPosition = Vector3(380.0f, 0.0f, 400.0f);
	Vector3 mTargetPosition = mOwner->GetGame()->GetPlayer()->GetPosition();
	mTargetPosition += Vector3::UnitZ * 230;
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
BossCamera::~BossCamera() {
	mOwner->GetGame()->GetPlayer()->SetVisible(true);
	mOwner->GetGame()->GetPlayer()->DeleteCamera();
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void BossCamera::Update(float deltaTime)
{
	// 経過時間を更新
	mElapsedTime += deltaTime;

	// 経過時間を移動時間で正規化（0.0～1.0の範囲）
	float t = mElapsedTime / mMoveTime;
	t = Math::Clamp(t, 0.0f, 1.0f); // tが範囲外に出ないように制限

	// 線形補間で現在位置を更新
	mCurrentPosition.x = Math::Lerp(mCurrentPosition.x, mTargetPosition.x, t);
	mCurrentPosition.y = Math::Lerp(mCurrentPosition.y, mTargetPosition.y, t);
	mCurrentPosition.z = Math::Lerp(mCurrentPosition.z, mTargetPosition.z, t);

	// 終了条件（目標位置に到達したら処理を終了するなど）
	if (t >= 1.0f) {
		mOwner->GetGame()->SetState(Game::GameState::EGameplay);
		// 必要に応じてフラグを立てるなどの処理
		mElapsedTime = 0.0f; 
	}
	// Set camera position behind and above Boss
	auto& bosses = mOwner->GetGame()->GetBoss();
	for (auto boss : bosses) {
		if (boss != nullptr) {
			mPitch += mPitchSpeed * deltaTime;
			mPitch = Math::Clamp(mPitch, -mMaxPitch * 0.7f, mMaxPitch * 0.6f);
			Quaternion q(mOwner->GetRight(), mPitch);
			Vector3 viewForward = Vector3::Transform(
				mOwner->GetForward(), q);
			Vector3 target = boss->GetPosition() + Vector3(0.0,0.0,230.0f);
			Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

			//カメラの注視位置をRendererに送る
			Matrix4 view = Matrix4::CreateLookAt(mCurrentPosition, target, up);
			SetViewMatrix(view);

			//音を効く位置をカメラでは鳴くキャラクター中心にする
			Game* game = mOwner->GetGame();
			Vector3 playerPosition = mOwner->GetPosition();
			game->GetAudioSystem()->SetListener(view, Vector3::Zero, 1.0f, playerPosition);
			
		}
	}

}

