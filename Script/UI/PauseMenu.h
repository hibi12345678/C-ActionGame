//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class PauseMenu : public UIScreen
{
public:
	//Enum State
	enum State {
	EMainMenu,
		EGameplay,
		EPaused,
		ETutorial,
		EItem,
		EQuit
    };

	//=========================================================================
    // public methods.
    //=========================================================================
	//�R���X�g���N�^
	PauseMenu(class Game* game);

	//�f�X�g���N�^
	~PauseMenu();

	//����
	void HandleKeyPress(int key) override;

private:
	State currentState; //���݂̃Q�[�����
	bool flag;
};
