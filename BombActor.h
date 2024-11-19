#pragma once
#include "ItemActorBase.h"

class BombActor : public ItemActorBase
{
public:
	BombActor(class Game* game, float scale, int num);
	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);

	void HitTarget();
	void FixCollisions();
private:
	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
	class BombMove* mMyMove;
	float mLifeSpan;
	float blinkTime;
	float blinkInterval;

};