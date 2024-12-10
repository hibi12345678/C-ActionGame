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
#include "Collision.h"
#include "Component.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class BoxComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	BoxComponent(class Actor* owner, int updateOrder = 100);
    //デストラクタ
	~BoxComponent();

	//座標
	void OnUpdateWorldTransform() override;

	//Getter,Setter
	const AABB& GetWorldBox() const { return mWorldBox; }
	TypeID GetType() const override { return TBoxComponent; }
	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	bool mShouldRotate;

	AABB mObjectBox;
	AABB mWorldBox;
};
