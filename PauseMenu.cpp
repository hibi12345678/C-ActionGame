// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "Renderer.h"
PauseMenu::PauseMenu(Game* game)
	:UIScreen(game)
	,flag(false)
{

	// currentState Ç game ÇÃèÛë‘Ç≈çXêV
	currentState = static_cast<PauseMenu::State>(mGame->GetState());
	flag = false;
	mGame->SetState(Game::EPaused);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	mBGPos = Vector2(0.0f, 50.0f);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/Texture/DialogBG2.png");
	SetTitle("PauseTitle");  

	AddButton("ResumeButton", [this]() {
		Close();
	});
	if (currentState == EGameplay) {
		AddButton("StartButton", [this]() {
			new DialogBox(mGame, "StartText",
				[this]() {
					flag = true; mGame->SetState(Game::EMainMenu); Close();
				});
			});
	}
	
	AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
		});
	});

}

PauseMenu::~PauseMenu()
{
	
	if(currentState == EMainMenu)
	  mGame->SetState(Game::EMainMenu);

	else if (currentState == EGameplay)
	{
		if (flag == true) {
			mGame->SetState(Game::EMainMenu);
		}
		else {
			mGame->SetState(Game::EGameplay);
			// Optionally, ensure the cursor is explicitly disabled
			SDL_ShowCursor(SDL_DISABLE);
			// Enable relative mouse mode for camera look
			SDL_SetRelativeMouseMode(SDL_TRUE);
			// Make an initial call to get relative to clear out
			SDL_GetRelativeMouseState(nullptr, nullptr);
		}
	}
		

}

void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
