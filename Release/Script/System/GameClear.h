//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//GameClear class
///////////////////////////////////////////////////////////////////////////////
class GameClear : public UIScreen
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
	explicit GameClear(class Game* game);

    //�f�X�g���N�^
	~GameClear();

	//����
	void HandleKeyPress(int key) override;
};
