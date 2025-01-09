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
#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
// Font class
///////////////////////////////////////////////////////////////////////////////
class Font
{
public:
	//=========================================================================
    // public methods.
    //=========================================================================
	//コンストラクタ
	explicit Font(class Game* game);

	//デストラクタ
	~Font();

	//初期化
	bool Load(const std::string& fileName);

	//終了処理
	void Unload();

	class Texture* RenderText(const std::string& textKey,
							  const Vector3& color = Color::White,
							  int pointSize = 30,
	                          int num = 0);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};
