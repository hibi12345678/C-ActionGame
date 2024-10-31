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
	SetScale(30.0f);
	mc = new MeshComponent(this);

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Arrow3D.gpmesh");
	mc->SetMesh(mesh);
	for (int i = 0; i <= 2; i++) {
		PointLightComponent* pointLight = new PointLightComponent(game->GetPlayer());
		pointLight->SetCol(Vector3(1.0, 0.6, 0.0));
		pointLight->SetInRad(100.0);
		pointLight->SetOutRad(400.0);
		pointLight->SetPosition(Vector3(200.0f, 0.0f, 200.0f));
		mPointLights.emplace_back(pointLight);
	}
	

}

void TorchItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Game* game = GetGame();
;
    Vector3 playerPosition = game->GetPlayer()->GetPosition();
    playerPosition += Vector3(70.0f, 0.0f, 100.0f);
    playerPosition += game->GetPlayer()->GetForward() * 70.0;
    SetPosition(playerPosition);

	if (game->GetPlayer()->GetItemState() != FollowActor::ETorch) {

		for (auto b : mPointLights) {
			delete b;
		}
		mPointLights.clear();
		delete this;
		
	}

}

