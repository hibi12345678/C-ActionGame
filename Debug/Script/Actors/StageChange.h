//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"
#include "SoundEvent.h"    


///////////////////////////////////////////////////////////////////////////////
//StageChange class
///////////////////////////////////////////////////////////////////////////////
class StageChange : public Actor
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	StageChange(Game* game);

	//�f�X�g���N�^
	~StageChange();

	//Getter,Setter
	class BoxComponent* GetBox() { return mBoxComp; }

	void MoveStage(int stageNum);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	bool mStageFlag;

	SoundEvent mMusicEvent;
	class BoxComponent* mBoxComp;
	class Game* mGame;
};