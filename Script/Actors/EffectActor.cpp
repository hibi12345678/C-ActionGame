#include "EffectActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"

EffectActor::EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale)
    : Actor(game)
    , mLifeSpan(lifeSpan)
{
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh(meshPath);
    mc->SetMesh(mesh);
}

void EffectActor::UpdateActor(float deltaTime)
{

    Actor::UpdateActor(deltaTime);

    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0.0f)
    {
        SetState(EDead);
    }
}