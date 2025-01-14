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
#include "Font.h"
#include <vector>
#include "Game.h"
#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////
// Font class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Font::Font(class Game* game)
	:mGame(game)
{
	
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
Font::~Font()
{
	
}


//-----------------------------------------------------------------------------
//   ファイルからFont情報の取得
//-----------------------------------------------------------------------------
bool Font::Load(const std::string& fileName)
{
	// We support these font sizes
	std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};
	
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}
		mFontData.emplace(size, font);
	}
	return true;
}

//-----------------------------------------------------------------------------
//   Fontメモリの解放
//-----------------------------------------------------------------------------
void Font::Unload()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}


//-----------------------------------------------------------------------------
//  文字の描画処理
//-----------------------------------------------------------------------------
Texture* Font::RenderText(const std::string& textKey,
						  const Vector3& color /*= Color::White*/,
						  int pointSize /*= 24*/
                          ,int num)
{
	Texture* texture = nullptr;
	
	// Convert to SDL_Color
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;
	
	// Find the font data for this point size
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		
		TTF_Font* font = iter->second;

		// 実際に描画するテキストを決定
		const std::string& actualText = (num == 0) ? mGame->GetText(textKey) : textKey;

		// テキストをサーフェスに描画 (アルファブレンド)
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		if (surf != nullptr)
		{
			// サーフェスをテクスチャに変換
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
		else
		{
			SDL_Log("Failed to render text surface: %s", TTF_GetError());
		}


	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}
	
	return texture;
}
