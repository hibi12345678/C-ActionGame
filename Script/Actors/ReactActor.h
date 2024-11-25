#pragma once
#include "EffectActor.h"

class ReactActor : public EffectActor
{
public:
    ReactActor(Game* game)
        : EffectActor(game, "Assets/Object/!.gpmesh", 0.4f, 15.0f)
    {
    }
};