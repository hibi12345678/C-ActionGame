//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameClear.h"
#include <SDL.h>
#include "DialogBox.h"
#include "Game.h"
#include "GameTimer.h"


///////////////////////////////////////////////////////////////////////////////
// GameClear class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
GameClear::GameClear(Game* game)
	:UIScreen(game)
{

	mGame->GetTimer()->StopTimer();
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	AddText("GameClear", Vector2(0.0f, 250.0f), 72, Color::Yellow, 1);
	AddText("GameClear", Vector2(0.0f, 250.0f), 72, Color::Yellow, 1);
	
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


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
GameClear::~GameClear()
{
	mGame->SetState(Game::GameState::EMainMenu);
}



//-----------------------------------------------------------------------------
//   入力処理
//-----------------------------------------------------------------------------
void GameClear::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

}

