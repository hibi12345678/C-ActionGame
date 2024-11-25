#pragma once
#include "UIScreen.h"
#include "Game.h"
class GameOver : public UIScreen
{
public:
	GameOver(class Game* game);
	~GameOver();
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EQuit
	};
	void HandleKeyPress(int key) override;
};
