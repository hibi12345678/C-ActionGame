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
#include "TorchItemActor.h"
#include "Game.h"
#include "Maths.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
// TorchActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
TorchItemActor::TorchItemActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num)
{

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Torch3D.gpmesh");
	mc->SetMesh(mesh);
	for (int i = 0; i <= 2; i++) {
		PointLightComponent* pointLight = new PointLightComponent(game->GetPlayer());
		pointLight->SetCol(Vector3(1.0, 0.6, 0.0));
		pointLight->SetInRad(0.0);
		pointLight->SetOutRad(300.0);
		pointLight->SetPosition(GetPosition() + Vector3(300.0f,0.0f,400.0f));
		mPointLights.emplace_back(pointLight);
		
	}

	
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void TorchItemActor::UpdateActor(float deltaTime)
{
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();
	if (FollowActor::ItemState::ETorch != game->GetPlayer()->GetItemState()) {

		for (auto b : mPointLights) {
			delete b;
		}
		mPointLights.clear();
		SetState(Actor::EDead);
	}

}

