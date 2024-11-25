#include "EffectActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"

EffectActor::EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale)
    : Actor(game)
    , mLifeSpan(lifeSpan)
{
    // スケールを設定
    SetScale(scale);

    // メッシュコンポーネントを作成し、メッシュを設定
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh(meshPath);
    mc->SetMesh(mesh);
}

void EffectActor::UpdateActor(float deltaTime)
{
    // 親クラスの更新処理
    Actor::UpdateActor(deltaTime);

    // 寿命を減らす
    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0.0f)
    {
        SetState(EDead);
    }
}