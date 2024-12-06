#include "SmokeActor.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
SmokeActor::SmokeActor(Game* game)
    : Actor(game)
{
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Smoke.gpmesh");
    mc->SetMesh(mesh);
    mLifeSpan = 0.5;
    SetScale(100.0f);
}

void SmokeActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0) {
        SetState(EDead);
    }
}