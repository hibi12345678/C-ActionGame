#pragma once
#include "EnemyActor.h"

class BossActor : public EnemyActor
{
public:
	BossActor(class Game* game);
	void UpdateActor(float deltaTime);
};