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
#include "MoveComponent.h"
#include "Actor.h"
#include "LevelLoader.h"

///////////////////////////////////////////////////////////////////////////////
// MoveComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}


//-----------------------------------------------------------------------------
//   Update
//-----------------------------------------------------------------------------
void MoveComponent::Update(float deltaTime)
{
	//Transform�@Rotation
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		angle = mAngularSpeed * deltaTime;
		
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		Quaternion inc(Vector3::UnitZ, angle);
		// Concatenate old and new quaternion
		rot = Quaternion::Concatenate(rot, inc);
		
		mOwner->SetRotation(rot);
	}
	
	//Transform�@Position
	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed)|| !Math::NearZero(mJumpSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
		pos += mOwner->GetUp() * mJumpSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}


//-----------------------------------------------------------------------------
//  Json�t�@�C������f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void MoveComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::GetFloat(inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::GetFloat(inObj, "strafeSpeed", mStrafeSpeed);
}


//-----------------------------------------------------------------------------
//  Json�t�@�C���ւ̏�������
//-----------------------------------------------------------------------------
void MoveComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::AddFloat(alloc, inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::AddFloat(alloc, inObj, "strafeSpeed", mStrafeSpeed);
}
