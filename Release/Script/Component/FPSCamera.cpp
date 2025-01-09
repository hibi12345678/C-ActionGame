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


///////////////////////////////////////////////////////////////////////////////
// FPSCamera class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner)
{
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void FPSCamera::Update(float deltaTime)
{
    //FollowActor�̈ʒu�ɃJ������z�u
	Vector3 cameraPos = mOwner->GetPosition() + Vector3(0.0,0.0,130.0);
	mPitch += mPitchSpeed * deltaTime;
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	Quaternion q(mOwner->GetRight(), mPitch);
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	Vector3 target = cameraPos + viewForward * 100.0f;

	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	//View��Renderer�ɑ���
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);

}
