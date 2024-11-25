#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class DropItemActor : public Actor
{
public:
	DropItemActor(class Game* game);
	~DropItemActor();
	class BoxComponent* GetBox() { return mBox; }
	void FixCollisions();
	void UpdateActor(float deltaTime) override;
	int GetItemNum() { return itemValue; }
private:
	class MeshComponent* mc;
	class BoxComponent* mBox;
	float mLifeSpan;
	float blinkTime;
	float blinkInterval; // 0.5•b‚²‚Æ‚É“_–Å
	bool isVisible;
	int randomValue;
	int itemValue;
	
};
