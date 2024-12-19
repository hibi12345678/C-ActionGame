//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "ItemActorBase.h"


///////////////////////////////////////////////////////////////////////////////
// BombActorclass
///////////////////////////////////////////////////////////////////////////////
class BombActor : public ItemActorBase
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	BombActor(class Game* game, float scale, int num);

	//Upadate
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	void SetPlayer(Actor* player);

	void HitTarget();
	void FixCollisions();

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	float mLifeSpan; //表示時間
	float blinkTime; //点滅時間
	float blinkInterval; //点滅間隔

	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
	class BombMove* mMyMove;
};