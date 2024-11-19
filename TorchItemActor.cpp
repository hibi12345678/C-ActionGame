// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TorchItemActor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "PointLightComponent.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "Game.h"
TorchItemActor::TorchItemActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num)
{

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Torch3D.gpmesh");
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
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();
	if (FollowActor::ETorch != game->GetPlayer()->GetItemState()) {

		for (auto b : mPointLights) {
			delete b;
		}
		mPointLights.clear();
		SetState(Actor::EDead);

	}


}

