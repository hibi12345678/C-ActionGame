//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "ItemActorBase.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class SwordActor : public ItemActorBase
{
public:
    //=========================================================================
    // public methods.
    //=========================================================================
    //コンストラクタ
    SwordActor(class Game* game,  float scale , int num);

    //Update
    void UpdateActor(float deltaTime) override;
};