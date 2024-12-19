//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"
#include "Maths.h"
#include "MeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
//ItemActorBase class
///////////////////////////////////////////////////////////////////////////////
class ItemActorBase : public Actor
{
public:
    //Enum State
    enum State
    {
        EActive,
        EPaused,
        EDead
    };

    //=========================================================================
    // public methods.
    //=========================================================================   
    //コンストラクタ
    ItemActorBase(class Game* game, float scale ,int num);

    //Update
    virtual void UpdateActor(float deltaTime) override;

    void SetVisible(bool visible);

protected:
    //=========================================================================
    // private variables.
    //=========================================================================
    int mNum; //選択アイテム用整数
    float scale;
    Vector3 Position;
    Quaternion Rotation;

    Actor::State mState;  // Actor::State

    class MeshComponent* mc;
};