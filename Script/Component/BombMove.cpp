// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BombMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "FollowActor.h"
#include "ArrowActor.h"


BombMove::BombMove(Actor* owner)
	:MoveComponent(owner)
{
	
}

void BombMove::Update(float deltaTime)
{
	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}


