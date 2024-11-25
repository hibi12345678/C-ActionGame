#include "SwordActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Maths.h"
SwordActor::SwordActor(Game* game, float scale, int num)
	: ItemActorBase(game, scale, num) 

{
	if (mNum == 0) {
		Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Object/Sword.gpmesh");
		mc->SetMesh(mesh);
	}
	else if (mNum == 1) {
		Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Object/Shield.gpmesh");
		mc->SetMesh(mesh);
	}
}

void SwordActor::UpdateActor(float deltaTime)
{
	// Šî’êƒNƒ‰ƒX‚ÌUpdateActorŒÄ‚Ño‚µ
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();
	if (FollowActor::ESword != game->GetPlayer()->GetItemState()) {

		SetState(Actor::EDead);

	}
}
