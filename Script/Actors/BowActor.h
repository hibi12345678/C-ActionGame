#pragma once
#include "ItemActorBase.h"

class BowActor : public ItemActorBase
{
public:
	BowActor(class Game* game, float scale, int num);

	void UpdateActor(float deltaTime) override;
};