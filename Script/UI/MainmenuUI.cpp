//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "MainmenuUI.h"
#include <SDL.h>
#include "DialogBox.h"
#include "Game.h"
#include "UIScreen.h"

MainmenuUI::MainmenuUI(Game* game)
	:UIScreen(game)
{
	AddText("Title", Vector2(0.0f,200.0f),72);

	StartButton("StartButton", [this]() {
		Close();
		});
	
}

MainmenuUI::~MainmenuUI()
{
	mGame->SetState(Game::GameState::EGameplay);
}

void MainmenuUI::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

}