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
//TargetComponent class
///////////////////////////////////////////////////////////////////////////////
class TargetComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	TargetComponent(class Actor* owner);

	//デストラクタ
	~TargetComponent();

	//Getter,Setter
	TypeID GetType() const override { return TTargetComponent; }
};
