#pragma once
#include "UIScreen.h"
#include "Game.h"
#include "SoundEvent.h"
class ItemMenu : public UIScreen
{
public:
	ItemMenu(class Game* game);
	~ItemMenu();
	
	void HandleKeyPress(int key);
private:

	
	SoundEvent mMusicEvent;
};
