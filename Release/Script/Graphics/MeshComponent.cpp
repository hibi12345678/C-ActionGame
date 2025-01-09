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
//      コンストラクタです.
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
//      デストラクタです.
//-----------------------------------------------------------------------------
MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}


//-----------------------------------------------------------------------------
//  メッシュ情報をシェーダーに送って描画する
//-----------------------------------------------------------------------------
void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		//ワールド座標をシェーダーに送信
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		//鏡面反射率をシェーダーに送信
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//textureをアクティブ
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// メッシュを頂点配列に
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//　描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}



//-----------------------------------------------------------------------------
//  jsonファイルからデータの読み取り
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
//  jsonファイルへの書き込み
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
