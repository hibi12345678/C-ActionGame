#include "BossActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "TargetComponent.h"
#include "EnemyActor.h"
#include "Renderer.h"
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


void BossActor::UpdateActor(float deltaTime) {

	EnemyActor::UpdateActor(deltaTime);
}
