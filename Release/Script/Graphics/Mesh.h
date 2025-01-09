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
#include "Collision.h"
#include "VertexArray.h"


///////////////////////////////////////////////////////////////////////////////
//Mesh class
///////////////////////////////////////////////////////////////////////////////
class Mesh
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	Mesh();

	//デストラクタ
	~Mesh();

	//初期化
	bool Load(const std::string& fileName, class Renderer* renderer);
	bool LoadBinary(const std::string& fileName, class Renderer* renderer);

	//終了処理
	void Unload();

	//Getter,Setter
	VertexArray* GetVertexArray() { return mVertexArray; }
	class Texture* GetTexture(size_t index);
	const std::string& GetShaderName() const { return mShaderName; }
	const std::string& GetFileName() const { return mFileName; }
	float GetRadius() const { return mRadius; }
	const AABB& GetBox() const { return mBox; }
	float GetSpecPower() const { return mSpecPower; }

	//Load,Save
	void SaveBinary(const std::string& fileName, const void* verts, 
		uint32_t numVerts, VertexArray::Layout layout,
		const uint32_t* indices, uint32_t numIndices,
		const std::vector<std::string>& textureNames,
		const AABB& box, float radius,
		float specPower);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	float mRadius; //半径
	float mSpecPower; //鏡面反射率
	AABB mBox;
	VertexArray* mVertexArray;
	std::vector<class Texture*> mTextures;
	std::string mShaderName;
	std::string mFileName;
};