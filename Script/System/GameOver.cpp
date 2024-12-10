//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameOver.h"
#include <SDL.h>
#include "DialogBox.h"


GameOver::GameOver(Game* game)
	:UIScreen(game)
{


	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("GameOver");
	
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

GameOver::~GameOver()
{
	mGame->SetState(Game::GameState::EMainMenu);
}

void GameOver::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);


}

