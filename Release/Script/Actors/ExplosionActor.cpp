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
#include "ExplosionActor.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// ExplosionActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
ExplosionActor::ExplosionActor(Game* game)
	:Actor(game)
	, mLifeSpan(0.8f)
{
	SetScale(50.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Explosion.gpmesh");
	mc->SetMesh(mesh);
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-1.5f, -1.5f, 0.0f),
		Vector3(1.5f, 1.5f, 50.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	game->AddExplosion(this);
	mAudioComp = new AudioComponent(this);
	mAudioComp->PlayEvent("event:/Explosion");
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
ExplosionActor::~ExplosionActor()
{
	GetGame()->RemoveExplosion(this);
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void ExplosionActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;

	if (mLifeSpan > 0.5f)
	{
		SetScale(125 - mLifeSpan * 100 );
	}
	if (mLifeSpan <= 0.5f)
	{
		SetScale(mLifeSpan * 100 + 25);
		if (mLifeSpan < 0.0f)
		{
			SetState(EDead);
		}
	}
}