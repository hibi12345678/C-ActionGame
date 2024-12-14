//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SmokeActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"



///////////////////////////////////////////////////////////////////////////////
// SmokeActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
SmokeActor::SmokeActor(Game* game)
    : Actor(game)
{
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Smoke.gpmesh");
    mc->SetMesh(mesh);
    mLifeSpan = 0.5;
    SetScale(100.0f);
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void SmokeActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0) {
        SetState(EDead);
    }
}