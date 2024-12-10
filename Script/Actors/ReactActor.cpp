//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ReactActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"


ReactActor::ReactActor(Game* game)
    : Actor(game)
{
    
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/!.gpmesh");
    mc->SetMesh(mesh);
    mLifeSpan = 0.4;
    SetScale(15.0f);
}

void ReactActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0) {
        SetState(EDead);
    }
}