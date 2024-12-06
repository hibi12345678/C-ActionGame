#pragma once

#include "Actor.h"
#include <string>

class EffectActor : public Actor
{
public:
    EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale);

    void UpdateActor(float deltaTime) override;

private:
    float mLifeSpan;
    
};