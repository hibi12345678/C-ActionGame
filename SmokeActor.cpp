// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SmokeActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "AudioComponent.h"

SmokeActor::SmokeActor(Game* game)
	:Actor(game)
	, mLifeSpan(0.5f)
{
	SetScale(100.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Smoke.gpmesh");
	mc->SetMesh(mesh);
}

void SmokeActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(EDead);
	}
}

