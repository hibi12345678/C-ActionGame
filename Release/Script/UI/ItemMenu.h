//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "SoundEvent.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//ItemMenu class
///////////////////////////////////////////////////////////////////////////////
class ItemMenu : public UIScreen
{
public:
	//Enum ItemState
	enum class ItemState
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
	explicit ItemMenu(class Game* game);

	//デストラクタ
	~ItemMenu();

	//入力
	void HandleKeyPress(int key);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	ItemState menuState; //アイテムステート

	SoundEvent mMusicEvent; //ステートがMainMenuかGamePalyか
	class UIActor* uiActor;
};
