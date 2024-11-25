#pragma once
#include "ItemActorBase.h"

class TorchItemActor : public ItemActorBase
{
public:

	TorchItemActor(class Game* game,float scale, int num);
	void UpdateActor(float deltaTime) override;

private:
	std::vector<class PointLightComponent*> mPointLights;
};