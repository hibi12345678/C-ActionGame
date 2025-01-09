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
#include "Component.h"
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//CameraComponent class
///////////////////////////////////////////////////////////////////////////////
class CameraComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	CameraComponent(class Actor* owner, int updateOrder = 200);

	//Getter,Setter
	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }
	TypeID GetType() const override { return TCameraComponent; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	float mPitchSpeed;
	float mMaxPitch;
	float mPitch;
	Vector3 target;
	Vector3 cameraPos;

	//=========================================================================
	// private methods.
	//=========================================================================
	void SetViewMatrix(const Matrix4& view);
};
