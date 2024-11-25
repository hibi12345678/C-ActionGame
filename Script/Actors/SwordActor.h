#pragma once
#include "ItemActorBase.h"

class SwordActor : public ItemActorBase
{
public:
    SwordActor(class Game* game,  float scale , int num);

    void UpdateActor(float deltaTime) override;
};