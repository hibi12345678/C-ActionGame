// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class BombActor : public Actor
{
public:
	BombActor(class Game* game,int num);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);
	
	void HitTarget();
	void FixCollisions();
	
private:
	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
	class BombMove* mMyMove;
	float mLifeSpan;
	float blinkTime;
	float blinkInterval; 
	int mNum;
	Vector3 Position;

	Quaternion Rotation;
};
