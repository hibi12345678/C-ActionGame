// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class ArrowActor : public Actor
{
public:
	ArrowActor(class Game* game);
	~ArrowActor();
	class BoxComponent* GetBox() { return mBoxComp; }
	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);
	void HitTarget();

private:
	class AudioComponent* mAudioComp;
	class BoxComponent* mBoxComp;
	class ArrowMove* mMyMove;
	float mLifeSpan;
	
};
