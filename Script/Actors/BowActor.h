//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "ItemActorBase.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class BowActor : public ItemActorBase
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	BowActor(class Game* game, float scale, int num);

	//Update
	void UpdateActor(float deltaTime) override;
};