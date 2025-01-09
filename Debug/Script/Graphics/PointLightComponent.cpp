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
#include "PointLightComponent.h"
#include "Actor.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"


///////////////////////////////////////////////////////////////////////////////
// PointLightComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
PointLightComponent::PointLightComponent(Actor* owner)
	:Component(owner)
{
	owner->GetGame()->GetRenderer()->AddPointLight(this);
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
PointLightComponent::~PointLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}


//-----------------------------------------------------------------------------
//   PointLightComponentの描画
//-----------------------------------------------------------------------------
void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() *
		mOuterRadius / mesh->GetRadius());
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 worldTransform = scale * trans;
	//座標やディフューズを送信
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);

	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr);
}


//-----------------------------------------------------------------------------
// jsonファイルからデータの読み取り
//-----------------------------------------------------------------------------
void PointLightComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	JsonHelper::GetVector3(inObj, "color", mDiffuseColor);
	JsonHelper::GetFloat(inObj, "innerRadius", mInnerRadius);
	JsonHelper::GetFloat(inObj, "outerRadius", mOuterRadius);
}


//-----------------------------------------------------------------------------
// jsonファイルへのかきこみ
//-----------------------------------------------------------------------------
void PointLightComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddVector3(alloc, inObj, "color", mDiffuseColor);
	JsonHelper::AddFloat(alloc, inObj, "innerRadius", mInnerRadius);
	JsonHelper::AddFloat(alloc, inObj, "outerRadius", mOuterRadius);
}
