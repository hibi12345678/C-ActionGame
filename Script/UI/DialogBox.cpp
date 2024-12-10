// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "DialogBox.h"
#include <SDL.h>
#include "Game.h"
#include "Renderer.h"


DialogBox::DialogBox(Game* game, const std::string& text,
	std::function<void()> onOK)
	:UIScreen(game)
{
	// Adjust positions for dialog box
	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, 100.0f);
	mNextButtonPos = Vector2(0.0f, 0.0f);

	mBackground = mGame->GetRenderer()->GetTexture("Assets/Texture/DialogBG.png");
	SetTitle(text, Vector3::Zero, 30);
	AddButton("OKButton", [onOK]() {

		onOK();
	});
	AddButton("CancelButton", [this]() { 
		Close();
	});
}

DialogBox::~DialogBox()
{

}
