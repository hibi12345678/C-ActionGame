//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ItemMenu.h"
#include <SDL.h>
#include "AudioSystem.h"
#include "BombActor.h"
#include "BowActor.h"
#include "DialogBox.h"
#include "FollowActor.h"
#include "Game.h"
#include "GameTimer.h"
#include "HUD.h"
#include "Renderer.h"
#include "SwordActor.h"
#include "TorchItemActor.h"
#include "UIScreen.h"
#include "UIActor.h"



///////////////////////////////////////////////////////////////////////////////
//ItemMenu class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
ItemMenu::ItemMenu(Game* game)
	:UIScreen(game)
	
{
	mGame->SetState(Game::GameState::EItem);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("Item");
	HUD* hudInstance = mGame->GetHUD();
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	FollowActor* player = mGame->GetPlayer();
	menuState = static_cast<ItemMenu::ItemState>(player->GetItemState());
	
	uiActor = new UIActor(mGame);
	//選択したボタンに応じたテキスト
	static const std::vector<std::pair<std::string, FollowActor::ItemState>> items = {
		{"SwordText", FollowActor::ESword},
		{"TorchText", FollowActor::ETorch},
		{"BowText", FollowActor::EBow},
		{"BombText", FollowActor::EBomb}
	};

	const int itemNum = hudInstance->GetItemNum();
	if (itemNum >= 0 && itemNum < static_cast<int>(items.size())) {
		AddText(items[itemNum].first, Vector2(0.0f, -160.0f), 30);
	}

	for (size_t i = 0; i < items.size(); ++i) {
		ItemButton(items[i].first, i, [this, hudInstance, itemState = items[i].second, itemText = items[i].first]() {
			CloseText();
			AddText(itemText, Vector2(0.0f, -160.0f), 30);
			hudInstance->SetItemNum(static_cast<int>(itemState));
			mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Equipped");
			mGame->GetPlayer()->SetItemState(itemState);
		});
	}
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
ItemMenu::~ItemMenu()
{
	mGame->SetState(Game::GameState::EGameplay);
	mGame->GetTimer()->StartTimer();
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.7f, 0.7f, 0.7f));
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);


	FollowActor::ItemState currentState = mGame->GetPlayer()->GetItemState();
	uiActor->SetState(Actor::EDead);
	//アイテムメニューを閉じたら武器を生成する
	if (menuState != currentState) {
		switch (currentState) {
		case FollowActor::ESword:
			new SwordActor(mGame, 1.0f, 0);
			new SwordActor(mGame, 1.0f, 1);
			break;
		case FollowActor::ETorch:
			new TorchItemActor(mGame, 50.0f, 0);
			break;
		case FollowActor::EBow:
			new BowActor(mGame, 1.0f, 1);
			break;
		case FollowActor::EBomb:
			new BombActor(mGame, 0.15f, 0);
			break;
		default:
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//   入力処理
//-----------------------------------------------------------------------------
void ItemMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_TAB || key == SDLK_ESCAPE)
	{
		Close();
	}
}