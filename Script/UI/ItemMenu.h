//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "SoundEvent.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class ItemMenu : public UIScreen
{
public:
	//Enum ItemState
	enum ItemState
	{
		ESword,
		ETorch,
		EBow,
		EBomb
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	ItemMenu(class Game* game);

	//デストラクタ
	~ItemMenu();

	//入力
	void HandleKeyPress(int key);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	ItemMenu::ItemState menuState; //アイテムステート

	SoundEvent mMusicEvent; //ステートがMainMenuかGamePalyか
	class UIActor* uiActor;
};
