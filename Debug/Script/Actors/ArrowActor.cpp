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
#include "ArrowActor.h"
#include "ArrowMove.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "EnemyActor.h"
#include "FollowActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
//ArrowActor class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ArrowActor::ArrowActor(Game* game)
	:Actor(game)
	, mLifeSpan(2.0f)
{
	SetScale(80.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Arrow3D.gpmesh");
	mc->SetMesh(mesh);
	mMyMove = new ArrowMove(this);
	mMyMove->SetForwardSpeed(1500.0f);
	mAudioComp = new AudioComponent(this);
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-1.0f, -1.00f, -1.00f),
		Vector3(1.0f, 1.0f, 1.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	game->AddArrow(this);
}


//-----------------------------------------------------------------------------
//  デストラクタ
//-----------------------------------------------------------------------------
ArrowActor::~ArrowActor() {
	GetGame()->RemoveArrow(this);
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void ArrowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(EDead);
	}
}


//-----------------------------------------------------------------------------
// ArrowMoveのSetMove呼び出し
//-----------------------------------------------------------------------------
void ArrowActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
}
