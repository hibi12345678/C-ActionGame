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
#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "LevelLoader.h"
#include "PhysWorld.h"



///////////////////////////////////////////////////////////////////////////////
// BoxComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
BoxComponent::BoxComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	,mObjectBox(Vector3::Zero, Vector3::Zero)
	,mWorldBox(Vector3::Zero, Vector3::Zero)
	,mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}


//-----------------------------------------------------------------------------
// BoxComponent�����[���h���W�ւ̕ϊ�
//-----------------------------------------------------------------------------
void BoxComponent::OnUpdateWorldTransform()
{
	// Reset to object space box
	mWorldBox = mObjectBox;
	// Scale
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// Rotate (if we want to�j
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// Translate
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}


//-----------------------------------------------------------------------------
// json�t�@�C������f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void BoxComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "objectMin", mObjectBox.mMin);
	JsonHelper::GetVector3(inObj, "objectMax", mObjectBox.mMax);
	JsonHelper::GetVector3(inObj, "worldMin", mWorldBox.mMin);
	JsonHelper::GetVector3(inObj, "worldMax", mWorldBox.mMax);
	JsonHelper::GetBool(inObj, "shouldRotate", mShouldRotate);
}


//-----------------------------------------------------------------------------
// json�t�@�C���ւ̏�������
//-----------------------------------------------------------------------------
void BoxComponent::SaveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value & inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "objectMin", mObjectBox.mMin);
	JsonHelper::AddVector3(alloc, inObj, "objectMax", mObjectBox.mMax);
	JsonHelper::AddVector3(alloc, inObj, "worldMin", mWorldBox.mMin);
	JsonHelper::AddVector3(alloc, inObj, "worldMax", mWorldBox.mMax);
	JsonHelper::AddBool(alloc, inObj, "shouldRotate", mShouldRotate);
}