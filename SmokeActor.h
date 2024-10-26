#pragma once
#include "Actor.h"

class SmokeActor : public Actor
{
public:
	SmokeActor(class Game* game);

	void UpdateActor(float deltaTime) override;

private:

	float mLifeSpan;
};

