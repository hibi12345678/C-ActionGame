//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GameOver.h"
#include <SDL.h>
#include "DialogBox.h"
#include "GameTimer.h"


///////////////////////////////////////////////////////////////////////////////
// GameOver class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
GameOver::GameOver(Game* game)
	:UIScreen(game)
{

	mGame->GetTimer()->StopTimer();
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	AddText("GameOver", Vector2(0.0f,300.0f),72,Color::Red,1);
	
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
GameOver::~GameOver()
{
	mGame->SetState(Game::GameState::EMainMenu);
}


//-----------------------------------------------------------------------------
//   入力処理
//-----------------------------------------------------------------------------
void GameOver::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);


}

