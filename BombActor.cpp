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
#include "ExplosionActor.h"

BombActor::BombActor(Game* game)
	:Actor(game)
	, mLifeSpan(6.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.4f)
	
{
	SetScale(20.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb3D.gpmesh");
	mc->SetMesh(mesh);
	mMyMove = new BombMove(this);
	mMyMove->SetForwardSpeed(600.0f);
	mMyMove->SetStrafeSpeed(0.0f);
	mMyMove->SetJumpSpeed(600.0f);
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
	
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 3.0f)
	{

		if (mLifeSpan < 0.0f) {
			ExplosionActor* ex = new ExplosionActor(GetGame());
			ex->SetPosition(this->GetPosition());
			
			SetState(EDead);
			
		}
		// 経過時間を増加させる
		blinkTime += deltaTime;

		// 一定間隔を超えたら可視状態を切り替え
		if (blinkTime >= blinkInterval)
		{
			
		    
			mAudioComp->PlayEvent("event:/Alert");
			// タイマーをリセット
			blinkTime = 0.0f;
		}
	}
	mMyMove->SetForwardSpeed(600.0f);
	mMyMove->SetStrafeSpeed(0.0f);
	mMyMove->SetJumpSpeed(600.0f);
	FixCollisions();
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

			
			mMyMove->SetForwardSpeed(0.0f);

			mMyMove->SetStrafeSpeed(0.0f);

			mMyMove->SetJumpSpeed(0.0f);
			
		}
	}

}