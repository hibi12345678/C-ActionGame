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
#include "Component.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class MeshComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクト
	MeshComponent(class Actor* owner, bool isSkeletal = false);

	//デストラクタ
	~MeshComponent();

	//Getter,Setter
	bool GetVisible() const { return mVisible; }
	bool GetIsSkeletal() const { return mIsSkeletal; }
	TypeID GetType() const override { return TMeshComponent; }
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
	void SetVisible(bool visible) { mVisible = visible; }

	//Load,Svae
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	virtual void Draw(class Shader* shader);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mVisible; //表示判定
	bool mIsSkeletal; //スケルトンを所持しているか

	size_t mTextureIndex;

	class Mesh* mMesh;
};