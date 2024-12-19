//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "MainmenuUI.h"
#include <SDL.h>
#include "DialogBox.h"
#include "Game.h"
#include "UIScreen.h"
#include "Renderer.h"

///////////////////////////////////////////////////////////////////////////////
//MainmenuUI class
///////////////////////////////////////////////////////////////////////////////
MainmenuUI::MainmenuUI(Game* game)
	:UIScreen(game)
{
	AddText("Title", Vector2(50.0f,100.0f),72);
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	StartButton("StartButton", [this]() {
		Close();
		});
	
}

//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
MainmenuUI::~MainmenuUI()
{
	mGame->GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	mGame->SetState(Game::GameState::EGameplay);
}


//-----------------------------------------------------------------------------
//   入力処理
//-----------------------------------------------------------------------------
void MainmenuUI::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

}