#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class TorchItemActor : public Actor
{
public:
	TorchItemActor(class Game* game);
	void UpdateActor(float deltaTime) override;
	int GetItemNum() { return itemValue; }
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

private:
	class MeshComponent* mc;
	class BoxComponent* mBox;
	class PointLightComponent* pointLight;
	float mLifeSpan;
	float blinkTime;
	float blinkInterval; // 0.5•b‚²‚Æ‚É“_–Å
	bool isVisible;
	int randomValue;
	int itemValue;
	State mState;
};
