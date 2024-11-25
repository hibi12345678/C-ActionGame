// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Maths.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }
	TypeID GetType() const override { return TCameraComponent; }
protected:
	void SetViewMatrix(const Matrix4& view);
	Vector3 target;
	Vector3 cameraPos;
	float mPitchSpeed;
	float mMaxPitch;
	float mPitch;
};
