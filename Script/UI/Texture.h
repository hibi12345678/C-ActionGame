// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <string>
#include <vector>
#include "Maths.h"
class Texture
{
public:
	Texture();
	~Texture();
	
	bool Load(const std::string& fileName);
	void Unload();
	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int width, int height, unsigned int format);
	void SetPos(Vector2 pos) { texPos = pos; }
	void SetActive(int index = 0);
	Vector2 GetPos() { return texPos; }
	void SetTexNum(int texNumber) { mTexNum = texNumber; }
	int GetTexNum() const { return mTexNum; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	unsigned int GetTextureID() const { return mTextureID; }

	const std::string& GetFileName() const { return mFileName; }
private:
	std::string mFileName;
	unsigned int mTextureID;
	int mTexNum;
	int mWidth;
	int mHeight;
	Vector2 texPos;
};
