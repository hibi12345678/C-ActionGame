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
#include <string>
#include <vector>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class Texture
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	Texture();
	//デストラクタ
	~Texture();
	
	//初期化
	bool Load(const std::string& fileName);

	//終了
	void Unload();

	//Getter,Setter
	int GetTexNum() const { return mTexNum; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	unsigned int GetTextureID() const { return mTextureID; }
	Vector2 GetPos() { return texPos; }
	const std::string& GetFileName() const { return mFileName; }
	void SetPos(Vector2 pos) { texPos = pos; }
	void SetActive(int index = 0);
	void SetTexNum(int texNumber) { mTexNum = texNumber; }


	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int width, int height, unsigned int format);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
    int mTexNum; //画像のサイズをどのように変更するかの決める変数
	int mWidth; //画像のWidth
	int mHeight; //画像のHeight
	unsigned int mTextureID; //テクスチャID
	Vector2 texPos; //スクリーン座標

	std::string mFileName;
};
