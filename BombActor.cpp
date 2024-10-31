// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BombActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BombMove.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
BombActor::BombActor(Game* game)
	:Actor(game)
	, mLifeSpan(6.0f)
{
	SetScale(20.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb3D.gpmesh");
	mc->SetMesh(mesh);
	mMyMove = new BombMove(this);
	mMyMove->SetForwardSpeed(400.0f);
	mAudioComp = new AudioComponent(this);
	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, 0.0f),
		Vector3(25.0f, 25.0f, 50.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void BombActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(EDead);
	}
}

void BombActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
}

void BombActor::HitTarget()
{
	mAudioComp->PlayEvent("event:/Ding");
}

void BombActor::FixCollisions()
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& bombBox = mBoxComp->GetWorldBox();


	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();

	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(bombBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.mMax.x - bombBox.mMin.x;
			float dx2 = planeBox.mMin.x - bombBox.mMax.x;
			float dy1 = planeBox.mMax.y - bombBox.mMin.y;
			float dy2 = planeBox.mMin.y - bombBox.mMax.y;
			float dz1 = planeBox.mMax.z - bombBox.mMin.z;
			float dz2 = planeBox.mMin.z - bombBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;

			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;

			}
			else
			{
				pos.z += dz;

			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}

}