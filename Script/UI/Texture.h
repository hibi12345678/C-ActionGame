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
	//�R���X�g���N�^
	Texture();
	//�f�X�g���N�^
	~Texture();
	
	//������
	bool Load(const std::string& fileName);

	//�I��
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
    int mTexNum; //�摜�̃T�C�Y���ǂ̂悤�ɕύX���邩�̌��߂�ϐ�
	int mWidth; //�摜��Width
	int mHeight; //�摜��Height
	unsigned int mTextureID; //�e�N�X�`��ID
	Vector2 texPos; //�X�N���[�����W

	std::string mFileName;
};
