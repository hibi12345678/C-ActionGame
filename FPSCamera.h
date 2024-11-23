// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

};
