// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "GameOver.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

GameOver::GameOver(Game* game)
	:UIScreen(game)
{


	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("GameOver");
	
	AddButton("StartButton", [this]() {
		new DialogBox(mGame, "StartText",
			[this]() {
					mGame->SetState(Game::EMainMenu), Close();
				});
	});
	
	AddButton("QuitButton", [this]() {
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
			});
		});

}

GameOver::~GameOver()
{
	mGame->SetState(Game::EMainMenu);
}

void GameOver::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);


}

