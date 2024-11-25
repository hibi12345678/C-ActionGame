#include "MainmenuUI.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "UIScreen.h"
MainmenuUI::MainmenuUI(Game* game)
	:UIScreen(game)
{
	AddText("Title", Vector2(0.0f,200.0f),72);
	AddText("Explain1", Vector2(0.0f,-100.0f),28);
	AddText("Explain2", Vector2(0.0f, -140.0f),28);
	AddText("Explain3", Vector2(0.0f, -180.0f),28);
	AddText("Explain4", Vector2(0.0f, -220.0f),28);
	AddText("Explain5", Vector2(0.0f, -260.0f), 28);
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