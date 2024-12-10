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
#include <SDL.h>
#include "Component.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class SpriteComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	
	//コンストラクタ
	SpriteComponent(class Actor* owner, int drawOrder = 100);

	//デストラクタ
	~SpriteComponent();

	//Getter,Setter
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }
	bool GetVisible() const { return mVisible; }
	TypeID GetType() const override { return TSpriteComponent; }
	virtual void SetTexture(class Texture* texture);
	void SetVisible(bool visible) { mVisible = visible; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	virtual void Draw(class Shader* shader);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mVisible;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;

	class Texture* mTexture;
};
