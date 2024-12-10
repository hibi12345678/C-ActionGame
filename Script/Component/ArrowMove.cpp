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
#include "ArrowMove.h"
#include "Actor.h"
#include "ArrowActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "PhysWorld.h"


ArrowMove::ArrowMove(Actor* owner)
	:MoveComponent(owner)
{
}

void ArrowMove::Update(float deltaTime)
{ 
	// Construct segment in direction of travel
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// Create line segment
	LineSegment l(start, end);

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	// (Don't collide vs player)
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		mOwner->SetState(Actor::EDead);

	}
	

	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
