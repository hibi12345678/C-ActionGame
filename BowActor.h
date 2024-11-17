#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include <cmath>
class BowActor : public Actor
{
public:
	BowActor(class Game* game, int num);
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


	float mLifeSpan;
	float blinkTime;
	float blinkInterval; // 0.5•b‚²‚Æ‚É“_–Å
	bool isVisible;
	int randomValue;
	int itemValue;
	State mState;
	Vector3 Position;

	int mNum;
	Quaternion Rotation;
};
