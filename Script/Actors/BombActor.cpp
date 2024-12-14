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
#include "BombActor.h"
#include "AudioComponent.h"
#include "BombMove.h"
#include "BoxComponent.h"
#include "ExplosionActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
// BombActor class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
BombActor::BombActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num)
	, mLifeSpan(6.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.4f)

{
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb3D.gpmesh");
	mc->SetMesh(mesh);

	if (mNum == 1) {

		mMyMove = new BombMove(this);
		mMyMove->SetForwardSpeed(600.0f);
		mMyMove->SetStrafeSpeed(0.0f);
		mMyMove->SetJumpSpeed(600.0f);
		mAudioComp = new AudioComponent(this);
		// Add a box component
		mBoxComp = new BoxComponent(this);
		AABB myBox(Vector3(-100.0f, -100.0f, 0.0f),
			Vector3(100.0f, 100.0f, 200.0f));
		mBoxComp->SetObjectBox(myBox);
		mBoxComp->SetShouldRotate(false);
	}
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void BombActor::UpdateActor(float deltaTime)
{


	if (mNum == 0) {

		// 基底クラスのUpdateActor呼び出し
		ItemActorBase::UpdateActor(deltaTime);
		Game* game = GetGame();

		if (FollowActor::EBomb != game->GetPlayer()->GetItemState()) {

			SetState(Actor::EDead);
		}
	}
	else if (mNum == 1) {
		// 基底クラスのUpdateActor呼び出し
		Actor::UpdateActor(deltaTime);
		Game* game = GetGame();
		mLifeSpan -= deltaTime;
		if (mLifeSpan < 3.0f)
		{

			if (mLifeSpan < 0.0f) {
				ExplosionActor* ex = new ExplosionActor(GetGame());
				ex->SetPosition(this->GetPosition());

				SetState(Actor::EDead);

			}

			blinkTime += deltaTime;

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

}


//-----------------------------------------------------------------------------
//  BombActorのSetPlayerを呼び出す
//-----------------------------------------------------------------------------
void BombActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
}


//-----------------------------------------------------------------------------
// TargetにHitしたら音を出す
//-----------------------------------------------------------------------------
void BombActor::HitTarget()
{
	mAudioComp->PlayEvent("event:/Ding");
}

//-----------------------------------------------------------------------------
// 衝突処理
//-----------------------------------------------------------------------------
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