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
#include "MoveComponent.h"


///////////////////////////////////////////////////////////////////////////////
//ArrowMove class
///////////////////////////////////////////////////////////////////////////////
class ArrowMove : public MoveComponent
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	ArrowMove(class Actor* owner);

	//Update
	void Update(float deltaTime) override;

	//Getter,Setter
	void SetPlayer(Actor* player) { mPlayer = player; }

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	Vector3 start;

	class Actor* mPlayer;
};