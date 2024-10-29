// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TorchItemActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "PointLightComponent.h"
#include "FollowActor.h"

TorchItemActor::TorchItemActor(Game* game)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
	, mState(EActive)
{
	SetScale(100.0f);
	mc = new MeshComponent(this);

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Torch3D.gpmesh");
	mc->SetMesh(mesh);
	pointLight = new PointLightComponent(this);
	pointLight->SetCol(Vector3(0.0,
		1.0,
		0.0));
	pointLight->SetInRad(0.0);
	pointLight->SetOutRad(500.0);
	
}

void TorchItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Game* game = GetGame();
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	playerPosition += Vector3(0.0f,400.0f,100.0f);
	SetPosition(playerPosition);
	pointLight->SetPosition(playerPosition + Vector3(0.0f, 0.0f, 100.0f));
	if (game->GetPlayer()->GetItemState() != FollowActor::ETorch) {
		delete this;
	}

}

