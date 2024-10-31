// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "ArrowActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "ArrowMove.h"
#include "AudioComponent.h"

ArrowActor::ArrowActor(Game* game)
	:Actor(game)
	, mLifeSpan(5.0f)
{
	SetScale(30.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Arrow3D.gpmesh");
	mc->SetMesh(mesh);
	mMyMove = new ArrowMove(this);
	mMyMove->SetForwardSpeed(800.0f);
	mAudioComp = new AudioComponent(this);
}

void ArrowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(EDead);
	}
}

void ArrowActor::SetPlayer(Actor* player)
{
	mMyMove->SetPlayer(player);
}

void ArrowActor::HitTarget()
{
	mAudioComp->PlayEvent("event:/Ding");
}
