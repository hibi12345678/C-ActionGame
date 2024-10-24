#pragma once
#include "UIScreen.h"
#include "Game.h"
class MainmenuUI : public UIScreen
{
public:
	MainmenuUI(class Game* game);
	~MainmenuUI();

	void HandleKeyPress(int key);
};
