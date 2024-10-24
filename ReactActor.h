#pragma once
#include "Actor.h"

class ReactActor : public Actor
{
public:
	ReactActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	
private:

	float mLifeSpan;
};
