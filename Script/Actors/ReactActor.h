#pragma once
#include "Actor.h"

class ReactActor : public Actor
{
public:
    ReactActor(Game* game);
    void UpdateActor(float deltaTime) override;

private:
    float mLifeSpan;
};