// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

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
