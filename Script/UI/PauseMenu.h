#pragma once
#include "UIScreen.h"
#include "Game.h"
class PauseMenu : public UIScreen
{
public:
	PauseMenu(class Game* game);
	~PauseMenu();
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EQuit
	};
	void HandleKeyPress(int key) override;
	State currentState;
	bool flag;
};
