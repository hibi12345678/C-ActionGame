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
#include "CameraComponent.h"


///////////////////////////////////////////////////////////////////////////////
//FollowCamera class
///////////////////////////////////////////////////////////////////////////////
class DefeatCamera : public CameraComponent
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	DefeatCamera(class Actor* owner);

	//Update
	void Update(float deltaTime) override;

	//Getter,Setter
	TypeID GetType() const override { return TFollowCamera; }
	float GetHorzDist() { return mHorzDist; }
	float GetVertDist() { return mVertDist; }
	Vector3 GetCameraPos() { return mCurrentPosition; }
	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }


private:
	//=========================================================================
	// private variables.
	//=========================================================================
	float mHorzDist;
	float mVertDist;
	float mTargetDist;
	float mSpringConstant;
	float mMoveTime;    // 移動にかける時間
	float mElapsedTime; // 経過時間

	Vector3 mActualPos;
	Vector3 mVelocity;
	Vector3 mCurrentPosition;  // 現在のカメラ位置
	Vector3 mTargetPosition;   // 目標位置

};

