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
//      コンストラクタです.
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
//      デストラクタです.
//-----------------------------------------------------------------------------
BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}


//-----------------------------------------------------------------------------
// BoxComponentをワールド座標への変換
//-----------------------------------------------------------------------------
void BoxComponent::OnUpdateWorldTransform()
{
	// Reset to object space box
	mWorldBox = mObjectBox;
	// Scale
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// Rotate (if we want to）
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// Translate
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}


//-----------------------------------------------------------------------------
// jsonファイルからデータの読み取り
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
// jsonファイルへの書き込み
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
