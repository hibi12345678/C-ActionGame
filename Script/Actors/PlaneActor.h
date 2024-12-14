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
#include "Actor.h"


///////////////////////////////////////////////////////////////////////////////
//PlaneActor class
///////////////////////////////////////////////////////////////////////////////
class PlaneActor : public Actor
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	PlaneActor(class Game* game);

	//�f�X�g���N�^
	~PlaneActor();

	//Getter,Setter
	class BoxComponent* GetBox() { return mBox; }
	TypeID GetType() const override { return TPlaneActor; }

private:
	//=========================================================================
	// private variables.
	//=========================================================================	
	class BoxComponent* mBox;
};
