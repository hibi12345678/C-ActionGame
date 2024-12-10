//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameClear.h"
#include <SDL.h>
#include "DialogBox.h"
#include "Game.h"


GameClear::GameClear(Game* game)
	:UIScreen(game)
{


	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("GameClear");
	
	AddButton("StartButton", [this]() {
		new DialogBox(mGame, "StartText",
			[this]() {
				mGame->SetState(Game::GameState::EMainMenu), Close();
			});
		});

	AddButton("QuitButton", [this]() {
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::GameState::EQuit);
			});
		});

}

GameClear::~GameClear()
{
	mGame->SetState(Game::GameState::EMainMenu);
}

void GameClear::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);


}

