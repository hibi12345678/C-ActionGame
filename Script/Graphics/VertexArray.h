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
	//コンストラクタ
	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);

	//デストラクタ
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
	unsigned int mNumVerts; //頂点バッファの数
	unsigned int mNumIndices; //インデックスバッファの数
	unsigned int mVertexBuffer; //頂点バッファのID
	unsigned int mIndexBuffer; //インデックスバッファのID
	unsigned int mVertexArray; //VertexArrayのID
};