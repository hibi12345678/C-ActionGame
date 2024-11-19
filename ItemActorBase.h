#pragma once
#include "Actor.h"
#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "Math.h"

class ItemActorBase : public Actor
{
public:
    ItemActorBase(class Game* game, float scale ,int num);
    virtual void UpdateActor(float deltaTime) override;
    enum State
    {
        EActive,
        EPaused,
        EDead
    };
protected:

    class MeshComponent* mc;
    float scale;
    int mNum;
    Vector3 Position;
    Actor::State mState;  // Actor::State ‚ðŽg—p
    Quaternion Rotation;

};