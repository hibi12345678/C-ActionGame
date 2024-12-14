//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"
#include "SoundEvent.h"


///////////////////////////////////////////////////////////////////////////////
//ExplosionActor class
///////////////////////////////////////////////////////////////////////////////
class ExplosionActor : public Actor
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	ExplosionActor(class Game* game);

	//デストラクタ
	~ExplosionActor();

	//Update
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	class BoxComponent* GetBox() { return mBoxComp; }

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	float mLifeSpan; //表示時間

	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
};

