#pragma once
#include "UIScreen.h"
#include "Game.h"
class GameClear : public UIScreen
{
public:
	GameClear(class Game* game);
	~GameClear();
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EQuit
	};
	void HandleKeyPress(int key) override;
};
