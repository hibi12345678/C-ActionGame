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
#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"



///////////////////////////////////////////////////////////////////////////////
// MeshComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
	,mVisible(true)
	,mIsSkeletal(isSkeletal)

{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}


//-----------------------------------------------------------------------------
//  ���b�V�������V�F�[�_�[�ɑ����ĕ`�悷��
//-----------------------------------------------------------------------------
void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		//���[���h���W���V�F�[�_�[�ɑ��M
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		//���ʔ��˗����V�F�[�_�[�ɑ��M
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//texture���A�N�e�B�u
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// ���b�V���𒸓_�z���
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//�@�`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}



//-----------------------------------------------------------------------------
//  json�t�@�C������f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
	{
		SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh(meshFile));
	}

	int idx;
	if (JsonHelper::GetInt(inObj, "textureIndex", idx))
	{
		mTextureIndex = static_cast<size_t>(idx);
	}

	JsonHelper::GetBool(inObj, "visible", mVisible);
	JsonHelper::GetBool(inObj, "isSkeletal", mIsSkeletal);
}


//-----------------------------------------------------------------------------
//  json�t�@�C���ւ̏�������
//-----------------------------------------------------------------------------
void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	if (mMesh)
	{
		JsonHelper::AddString(alloc, inObj, "meshFile", mMesh->GetFileName());
	}
	JsonHelper::AddInt(alloc, inObj, "textureIndex", static_cast<int>(mTextureIndex));
	JsonHelper::AddBool(alloc, inObj, "visible", mVisible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", mIsSkeletal);
}
