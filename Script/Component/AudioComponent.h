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
#include <string>
#include <vector>
#include "Component.h"
#include "SoundEvent.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class AudioComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	AudioComponent(class Actor* owner, int updateOrder = 200);
	//デストラクタ
	~AudioComponent();

	//Update
	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	//Getter,Setter
	TypeID GetType() const override { return TAudioComponent; }

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};