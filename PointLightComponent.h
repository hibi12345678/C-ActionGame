// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Math.h"
#include "Component.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	// Draw this point light as geometry
	void Draw(class Shader* shader, class Mesh* mesh);
	Vector3 mPosition;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Radius of light
	float mInnerRadius;
	float mOuterRadius;
	void SetCol(Vector3 col) { mDiffuseColor = col; }
	void SetInRad(float rad) { mInnerRadius = rad; }
	void SetOutRad(float rad) { mOuterRadius = rad; }
	void SetPosition(const Vector3& pos) { mPosition = pos;  }
	TypeID GetType() const override { return TPointLightComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
};
