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
#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include "LevelLoader.h"


//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const char* Actor::TypeNames[NUM_ACTOR_TYPES] = {
	"Actor",
	"BallActor",
	"FollowActor",
	"EnemyActor",
	"PlaneActor",
	"TargetActor",
};


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Actor::Actor(Game* game)
	:mState(EActive)
	,mPosition(Vector3::Zero)
	,mRotation(Quaternion::Identity)
	,mScale(1.0f)
	,mGame(game)
	,mRecomputeTransform(true)
{
	mGame->AddActor(this);
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Actor::~Actor()
{
	mGame->RemoveActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		if (mRecomputeTransform)
		{
			ComputeWorldTransform();
		}
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}


//-----------------------------------------------------------------------------
//  ComponentのUpdate処理
//-----------------------------------------------------------------------------
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}


//-----------------------------------------------------------------------------
//  ActorのUpdate処理
//-----------------------------------------------------------------------------
void Actor::UpdateActor(float deltaTime)
{
}


//-----------------------------------------------------------------------------
//  入力処理
//-----------------------------------------------------------------------------
void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		// First process input for components
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}


//-----------------------------------------------------------------------------
//  入力処理
//-----------------------------------------------------------------------------
void Actor::ActorInput(const uint8_t* keyState)
{

}


//-----------------------------------------------------------------------------
//  ワールド座標へ変換します
//-----------------------------------------------------------------------------
void Actor::ComputeWorldTransform()
{
	mRecomputeTransform = false;
	// Scale,rot,pos
	mWorldTransform = Matrix4::CreateScale(mScale);
	mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
	mWorldTransform *= Matrix4::CreateTranslation(mPosition);

	//componentのOnUpdateWorldTransformを呼び出す
	for (auto comp : mComponents)
	{
		comp->OnUpdateWorldTransform();
	}
}


//-----------------------------------------------------------------------------
//   正面方向への回転処理
//-----------------------------------------------------------------------------
void Actor::RotateToNewForward(const Vector3& forward)
{
	// Figure out difference between original (unit x) and new
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// Facing down X
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// Facing down -X
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// Rotate about axis from cross product
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}


//-----------------------------------------------------------------------------
//  コンポーネントの追加
//-----------------------------------------------------------------------------
void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mComponents.insert(iter, component);
}

//-----------------------------------------------------------------------------
// コンポーネントの削除
//-----------------------------------------------------------------------------
void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

//-----------------------------------------------------------------------------
//  jsonファイルから読み取り
//-----------------------------------------------------------------------------
void Actor::LoadProperties(const rapidjson::Value& inObj)
{
	// Use strings for different states
	std::string state;
	if (JsonHelper::GetString(inObj, "state", state))
	{
		if (state == "active")
		{
			SetState(EActive);
		}
		else if (state == "paused")
		{
			SetState(EPaused);
		}
		else if (state == "dead")
		{
			SetState(EDead);
		}
	}

	// Load position, rotation, and scale, and compute transform
	JsonHelper::GetVector3(inObj, "position", mPosition);
	JsonHelper::GetQuaternion(inObj, "rotation", mRotation);
	JsonHelper::GetFloat(inObj, "scale", mScale);
	ComputeWorldTransform();
}

//-----------------------------------------------------------------------------
//  jasonファイルへの書き込み
//-----------------------------------------------------------------------------
void Actor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	std::string state = "active";
	if (mState == EPaused)
	{
		state = "paused";
	}
	else if (mState == EDead)
	{
		state = "dead";
	}

	JsonHelper::AddString(alloc, inObj, "state", state);
	JsonHelper::AddVector3(alloc, inObj, "position", mPosition);
	JsonHelper::AddQuaternion(alloc, inObj, "rotation", mRotation);
	JsonHelper::AddFloat(alloc, inObj, "scale", mScale);

}
