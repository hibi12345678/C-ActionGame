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
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
PointLightComponent::PointLightComponent(Actor* owner)
	:Component(owner)
{
	owner->GetGame()->GetRenderer()->AddPointLight(this);
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
PointLightComponent::~PointLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}


//-----------------------------------------------------------------------------
//   PointLightComponent�̕`��
//-----------------------------------------------------------------------------
void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() *
		mOuterRadius / mesh->GetRadius());
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 worldTransform = scale * trans;
	//���W��f�B�t���[�Y�𑗐M
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);

	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr);
}


//-----------------------------------------------------------------------------
// json�t�@�C������f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void PointLightComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	JsonHelper::GetVector3(inObj, "color", mDiffuseColor);
	JsonHelper::GetFloat(inObj, "innerRadius", mInnerRadius);
	JsonHelper::GetFloat(inObj, "outerRadius", mOuterRadius);
}


//-----------------------------------------------------------------------------
// json�t�@�C���ւ̂�������
//-----------------------------------------------------------------------------
void PointLightComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddVector3(alloc, inObj, "color", mDiffuseColor);
	JsonHelper::AddFloat(alloc, inObj, "innerRadius", mInnerRadius);
	JsonHelper::AddFloat(alloc, inObj, "outerRadius", mOuterRadius);
}
