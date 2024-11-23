#include "BossActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "LevelLoader.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "ReactActor.h"
#include "SmokeActor.h"
#include "FollowActor.h"
#include <cmath>
#include <cstdlib>  
#include <ctime>   
#include "Random.h"
#include "TargetComponent.h"
#include "ArrowActor.h"
#include "BombActor.h"
#include "ExplosionActor.h"
#include "EnemyActor.h"
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
