#include "ItemMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "UIScreen.h"
#include "HUD.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "FollowActor.h"
#include "TorchItemActor.h"
#include "SwordActor.h"
#include "BombActor.h"
#include "BowActor.h"
ItemMenu::ItemMenu(Game* game)
	:UIScreen(game)
	
{
	mGame->SetState(Game::GameState::EItem);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("Item");
	HUD* hudInstance = mGame->GetHUD();
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.2, 0.2, 0.2));
	FollowActor* player = mGame->GetPlayer();
	menuState = static_cast<ItemMenu::ItemState>(player->GetItemState());
	
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

ItemMenu::~ItemMenu()
{
	mGame->SetState(Game::GameState::EGameplay);
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// 現在のプレイヤーのアイテムステートを取得
	FollowActor::ItemState currentState = mGame->GetPlayer()->GetItemState();

	// アイテム作成処理
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

void ItemMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_TAB || key == SDLK_ESCAPE)
	{
		Close();
	}
}