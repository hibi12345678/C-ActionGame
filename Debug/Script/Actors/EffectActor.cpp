//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "EffectActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"



///////////////////////////////////////////////////////////////////////////////
// EffectActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//  �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
EffectActor::EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale)
    : Actor(game)
    , mLifeSpan(lifeSpan)
{
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh(meshPath);
    mc->SetMesh(mesh);
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void EffectActor::UpdateActor(float deltaTime)
{

    Actor::UpdateActor(deltaTime);

    mLifeSpan -= deltaTime;

    //0�ɂȂ�Ə���
    if (mLifeSpan < 0.0f)
    {
        SetState(EDead);
    }
}