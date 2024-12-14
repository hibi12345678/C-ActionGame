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
#include <vector>


///////////////////////////////////////////////////////////////////////////////
// GBuffer class
///////////////////////////////////////////////////////////////////////////////
class GBuffer
{
public:
	//Enum Type
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	GBuffer();
	//デストラクタ
	~GBuffer();

	//初期化
	bool Create(int width, int height);

	//終了処理
	void Destroy();

	//Getter,Setter
	class Texture* GetTexture(Type type);
	unsigned int GetBufferID() const { return mBufferID; }
	void SetTexturesActive();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	unsigned int mBufferID;
	std::vector<class Texture*> mTextures;
};
