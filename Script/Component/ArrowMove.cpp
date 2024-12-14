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


///////////////////////////////////////////////////////////////////////////////
// ArrowMove class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
ArrowMove::ArrowMove(Actor* owner)
	:MoveComponent(owner)
{
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void ArrowMove::Update(float deltaTime)
{ 
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	//LineSegmentの生成
	LineSegment l(start, end);

	//LineSegmentとBoxの衝突判定を確認
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		mOwner->SetState(Actor::EDead);

	}
	
	//MoveComponentのUpdate
	MoveComponent::Update(deltaTime);
}
