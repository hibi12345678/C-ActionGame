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
#include "BombMove.h"
#include "Actor.h"
#include "ArrowActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "PhysWorld.h"


BombMove::BombMove(Actor* owner)
	:MoveComponent(owner)
{
	
}

void BombMove::Update(float deltaTime)
{
	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}


