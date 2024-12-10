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
#include "FPSCamera.h"
#include "Actor.h"


FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner)
{
}

void FPSCamera::Update(float deltaTime)
{
	Vector3 cameraPos = mOwner->GetPosition() + Vector3(0.0,0.0,130.0);
	mPitch += mPitchSpeed * deltaTime;
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	Quaternion q(mOwner->GetRight(), mPitch);
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	Vector3 target = cameraPos + viewForward * 100.0f;
	// Also rotate up by pitch quaternion
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// Create look at matrix, set as view
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);

}
