// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BowActor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Maths.h"
#include "Game.h"
BowActor::BowActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num)
{	
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bow.gpmesh");
	mc->SetMesh(mesh);
	
}

void BowActor::UpdateActor(float deltaTime)
{

	// Šî’êƒNƒ‰ƒX‚ÌUpdateActorŒÄ‚Ño‚µ
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();

	if (FollowActor::EBow != game->GetPlayer()->GetItemState()) {

		SetState(Actor::EDead);

	}

}

