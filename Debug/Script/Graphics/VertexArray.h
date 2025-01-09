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

///////////////////////////////////////////////////////////////////////////////
//VertexArray class
///////////////////////////////////////////////////////////////////////////////
class VertexArray
{
public:
	//Enum Layout
	enum Layout
	{
		PosNormTex,
		PosNormSkinTex
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);

	//�f�X�g���N�^
	~VertexArray();

	//Getter,Setter
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
	static unsigned int GetVertexSize(VertexArray::Layout layout);
	void SetActive();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	unsigned int mNumVerts; //���_�o�b�t�@�̐�
	unsigned int mNumIndices; //�C���f�b�N�X�o�b�t�@�̐�
	unsigned int mVertexBuffer; //���_�o�b�t�@��ID
	unsigned int mIndexBuffer; //�C���f�b�N�X�o�b�t�@��ID
	unsigned int mVertexArray; //VertexArray��ID
};