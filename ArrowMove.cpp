// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "ArrowMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "FollowActor.h"
#include "ArrowActor.h"

ArrowMove::ArrowMove(Actor* owner)
	:MoveComponent(owner)
{
}

void ArrowMove::Update(float deltaTime)
{
	// Construct segment in direction of travel
	const float segmentLength = 30.0f;
	if (mOwner->GetGame()->GetPlayer()->GetCameraComponent() != nullptr) {
		start = mOwner->GetGame()->GetPlayer()->GetPosition() +Vector3(350.0,0.0,-250);
		
	}
	else {
		 start = mOwner->GetPosition();

	}
	
	Vector3 dir = mOwner->GetForward();
	Vector3 up = mOwner->GetUp();
	Vector3 end = start + dir * segmentLength + up * 30.0;

	// Create line segment
	LineSegment l(start, end);

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	// (Don't collide vs player)
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		// If we collided, reflect the ball about the normal
		dir = Vector3::Reflect(dir, info.mNormal);
		mOwner->RotateToNewForward(dir);

	}

	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}
