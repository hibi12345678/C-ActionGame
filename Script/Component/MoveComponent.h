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


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class MoveComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	MoveComponent(class Actor* owner, int updateOrder = 10);

	//Update
	void Update(float deltaTime) override;
	
	//Getter,Setter
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetAngle() const { return angle; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }
	float GetJumpSpeed() const {return  mJumpSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
	void SetJumpSpeed(float speed) { mJumpSpeed = speed; }
	TypeID GetType() const override { return TMoveComponent; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
	float mJumpSpeed;
	float angle;
};
