//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//GameOver class
///////////////////////////////////////////////////////////////////////////////
class GameOver : public UIScreen
{
public:
	//Enum State
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EQuit
	};
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	explicit GameOver(class Game* game);
	//�f�X�g���N�^
	~GameOver();

	//����
	void HandleKeyPress(int key) override;
};
