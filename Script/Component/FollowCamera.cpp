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
#include "FollowCamera.h"
#include <vector>
#include "Actor.h"
#include "AudioSystem.h"
#include "FollowActor.h"
#include "Game.h"
#include "LevelLoader.h"



///////////////////////////////////////////////////////////////////////////////
// FollowCamera class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	, mSpringConstant(128.0)
	, mTargetDist(100.0)
	, mVelocity(Vector3(0.0,0.0,0.0))
	, mActualPos(Vector3(-350.0,0.0,150.0))
	, mHorzDist(320.0f)
	, mVertDist(230.0f)
	, mRightDist(0.0f)
{
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void FollowCamera::Update(float deltaTime)
{
	//カメラ位置の計算(頭上後方）
	Vector3 cameraPos = ComputeCameraPos() ;
	mPitch += mPitchSpeed * deltaTime;
	mPitch = Math::Clamp(mPitch, -mMaxPitch * 0.7f, mMaxPitch*0.6f);
	Quaternion q(mOwner->GetRight(), mPitch);
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	Vector3 target = mOwner->GetPosition() + viewForward * mTargetDist * 10.0f;
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	//カメラの注視位置をRendererに送る
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);

	//音を効く位置をカメラでは鳴くキャラクター中心にする
	Game* game = mOwner->GetGame();
	Vector3 playerPosition = mOwner->GetPosition();
	game->GetAudioSystem()->SetListener(view, Vector3::Zero, 1.0f, playerPosition);
}


//-----------------------------------------------------------------------------
//  Jsonファイルからデータの読み取り
//-----------------------------------------------------------------------------
void FollowCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "actualPos", mActualPos);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
	JsonHelper::GetFloat(inObj, "springConstant", mSpringConstant);
}


//-----------------------------------------------------------------------------
//  Jsonファイルからデータの書き込み
//-----------------------------------------------------------------------------
void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);
	JsonHelper::AddVector3(alloc, inObj, "actualPos", mActualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", mSpringConstant);
}


//-----------------------------------------------------------------------------
// 　カメラ位置の計算
//-----------------------------------------------------------------------------
Vector3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += mOwner->GetRight() * mRightDist;
	cameraPos += Vector3::UnitZ * mVertDist;

	return cameraPos;
}
