#include "EffectActor.h"

class SmokeActor : public EffectActor
{
public:
    SmokeActor(Game* game)
        : EffectActor(game, "Assets/Object/Smoke.gpmesh", 0.5f, 100.0f)
    {
    }
};