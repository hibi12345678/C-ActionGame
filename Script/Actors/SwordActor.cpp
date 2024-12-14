//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SwordActor.h"
#include "Game.h"
#include "Maths.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
// SwordActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void SwordActor::UpdateActor(float deltaTime)
{
	// 基底クラスのUpdateActor呼び出し
	ItemActorBase::UpdateActor(deltaTime);
	Game* game = GetGame();
	if (FollowActor::ESword != game->GetPlayer()->GetItemState()) {

		SetState(Actor::EDead);

	}
	
}
