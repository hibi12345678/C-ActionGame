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
	//コンストラクタ
	PauseMenu(class Game* game);

	//デストラクタ
	~PauseMenu();

	//入力
	void HandleKeyPress(int key) override;

private:
	State currentState; //現在のゲーム状態
	bool flag;
};
