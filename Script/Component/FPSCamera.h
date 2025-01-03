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
//FPSCamera class
///////////////////////////////////////////////////////////////////////////////
class FPSCamera : public CameraComponent
{
public:
    //=========================================================================
    // public methods.
    //=========================================================================
    //コンストラクタ
    explicit FPSCamera(class Actor* owner);

    //Update
	void Update(float deltaTime) override;
};
