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
	//�R���X�g���N�^
	ExplosionActor(class Game* game);

	//�f�X�g���N�^
	~ExplosionActor();

	//Update
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	class BoxComponent* GetBox() { return mBoxComp; }

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	float mLifeSpan; //�\������

	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
};

