//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "PauseMenu.h"
#include <SDL.h>
#include "DialogBox.h"
#include "Game.h"
#include "GameTimer.h"
#include "Renderer.h"



///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
PauseMenu::PauseMenu(Game* game)
	:UIScreen(game)
	,flag(false)
{
	
	currentState = static_cast<PauseMenu::State>(mGame->GetState());
	flag = false;
	mGame->SetState(Game::GameState::EPaused);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	mBGPos = Vector2(0.0f, 50.0f);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/Texture/DialogBG2.png");
	SetTitle("PauseTitle");  
	//�ĊJ
	AddButton("ResumeButton", [this]() {
		Close();
	});

	//�X�^�[�g���j���[
	if (currentState == EGameplay) {
		AddButton("StartButton", [this]() {
			new DialogBox(mGame, "StartText",
				[this]() {
					flag = true; mGame->SetState(Game::GameState::EMainMenu); Close();
				});
			});
	}
	
	//�Q�[�����I������
	AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::GameState::EQuit);
		});
	});

}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
PauseMenu::~PauseMenu()
{
	mGame->GetTimer()->StartTimer();
	//��ԑJ��
	if(currentState == EMainMenu)
	  mGame->SetState(Game::GameState::EMainMenu);

	else if (currentState == EGameplay)
	{
		if (flag == true) {
			mGame->SetState(Game::GameState::EMainMenu);
		}
		else {
			mGame->SetState(Game::GameState::EGameplay);
			SDL_ShowCursor(SDL_DISABLE);
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_GetRelativeMouseState(nullptr, nullptr);
		}
	}
		

}


//-----------------------------------------------------------------------------
//    ���͏���
//-----------------------------------------------------------------------------
void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
