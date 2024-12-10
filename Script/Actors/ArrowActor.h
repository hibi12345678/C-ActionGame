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
//class
///////////////////////////////////////////////////////////////////////////////
class ArrowActor : public Actor
{
public:
	//=========================================================================
	// public methiods.
	//=========================================================================
	//�R���X�g���N�^
	ArrowActor(class Game* game);

	//�f�X�g���N�^
	~ArrowActor();

	//Upadate
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	void SetPlayer(Actor* player);
	class BoxComponent* GetBox() { return mBoxComp; }

	void HitTarget();

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	float mLifeSpan; //�\������

	class AudioComponent* mAudioComp;
	class BoxComponent* mBoxComp;
	class ArrowMove* mMyMove;
	
	
};
