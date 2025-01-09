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
	//�R���X�g���N�^
	GBuffer();
	//�f�X�g���N�^
	~GBuffer();

	//������
	bool Create(int width, int height);

	//�I������
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
