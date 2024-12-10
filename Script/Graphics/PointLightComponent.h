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
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class PointLightComponent : public Component
{
public:
	//=========================================================================
    // public variables.
    //=========================================================================
	Vector3 mPosition;
	Vector3 mDiffuseColor;
	float mInnerRadius;
	float mOuterRadius;

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	PointLightComponent(class Actor* owner);

	//デストラクタ
	~PointLightComponent();

	//Getter,Setter
	TypeID GetType() const override { return TPointLightComponent; }
	void SetCol(Vector3 col) { mDiffuseColor = col; }
	void SetInRad(float rad) { mInnerRadius = rad; }
	void SetOutRad(float rad) { mOuterRadius = rad; }
	void SetPosition(const Vector3& pos) { mPosition = pos;  }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	void Draw(class Shader* shader, class Mesh* mesh);
};
