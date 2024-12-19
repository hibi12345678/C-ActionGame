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
class FollowCamera : public CameraComponent
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
    //コンストラクタ
	FollowCamera(class Actor* owner);

	//Update
	void Update(float deltaTime) override;

	//Getter,Setter
	TypeID GetType() const override { return TFollowCamera; }
	float GetHorzDist() { return mHorzDist; }
	float GetVertDist() { return mVertDist; }
	float GetRightDist() { return mRightDist; }
	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetRightDist(float dist) { mRightDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }


	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	float mHorzDist;
	float mVertDist;
	float mRightDist;
	float mTargetDist;
	float mSpringConstant;

	Vector3 mActualPos;
	Vector3 mVelocity;

	//=========================================================================
    // protected methods.
    //=========================================================================
	Vector3 ComputeCameraPos() const; //カメラを配置する頭上後方の位置計算 
};
