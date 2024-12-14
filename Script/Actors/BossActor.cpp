//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "BossActor.h"
#include "EnemyActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "TargetComponent.h"

///////////////////////////////////////////////////////////////////////////////
// BossActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
BossActor::BossActor(Game* game)
	:EnemyActor(game)
{
	name = "EnemyBoss";
	SetScale(2.0f);
	mMeshComp = new SkeletalMeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/"+ name +".gpmesh");
	mMeshComp->SetMesh(mesh);
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/Skel/" + name + ".gpskel"));
	game->SetBossActor(this);
	new TargetComponent(this);
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void BossActor::UpdateActor(float deltaTime) {

	EnemyActor::UpdateActor(deltaTime);
}
