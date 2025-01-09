//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "TreeActor.h"
#include "BoxComponent.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// TreeActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
TreeActor::TreeActor(Game* game)
	:Actor(game)
{
	SetScale(1.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Object/NewTree.gpmesh");
	mc->SetMesh(mesh);
	
	game->AddTree(this);
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
TreeActor::~TreeActor()
{
	GetGame()->RemoveTree(this);
}
