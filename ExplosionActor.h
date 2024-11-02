#pragma once
#include "Actor.h"
#include "SoundEvent.h"
class ExplosionActor : public Actor
{
public:
	ExplosionActor(class Game* game);
	~ExplosionActor();
	void UpdateActor(float deltaTime) override;
	class BoxComponent* GetBox() { return mBoxComp; }
private:
	class BoxComponent* mBoxComp;
	float mLifeSpan;
	class AudioComponent* mAudioComp;


};

