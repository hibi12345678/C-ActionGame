#pragma once
#include "UIScreen.h"
#include "Game.h"
#include "SoundEvent.h"
class ItemMenu : public UIScreen
{
public:
	ItemMenu(class Game* game);
	~ItemMenu();
	enum ItemState
	{
		ESword,
		ETorch,
		EBow,
		EBomb
	};
	void HandleKeyPress(int key);
private:
	ItemMenu::ItemState menuState;
	
	SoundEvent mMusicEvent;
};
