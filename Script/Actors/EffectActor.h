#pragma once

#include "Actor.h"
#include <string>

class EffectActor : public Actor
{
public:
    // コンストラクタ
    EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale);

    // Actorの更新メソッドをオーバーライド
    void UpdateActor(float deltaTime) override;

private:
    float mLifeSpan; // 寿命
};