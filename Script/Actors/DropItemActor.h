//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"


///////////////////////////////////////////////////////////////////////////////
//DropItemActor class
///////////////////////////////////////////////////////////////////////////////
class DropItemActor : public Actor
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	DropItemActor(class Game* game);

	//デストラクタ
	~DropItemActor();

	//Update
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	class BoxComponent* GetBox() { return mBox; }
	int GetItemNum() { return itemValue; }

	void FixCollisions();

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	int randomValue;
	int itemValue; 
	float mLifeSpan; //表示時間
	float blinkTime; //点滅時間
	float blinkInterval; //点滅間隔
	
	class MeshComponent* mc;
	class BoxComponent* mBox;
};
