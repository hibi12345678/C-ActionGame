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
#include <rapidjson/document.h>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class Component
{
public:
	//Enum TypeID
	enum TypeID
	{
		TComponent = 0,
		TAudioComponent,
		TBallMove,
		TBoxComponent,
		TCameraComponent,
		TFollowCamera,
		TMeshComponent,
		TMoveComponent,
		TSkeletalMeshComponent,
		TSpriteComponent,
		TMirrorCamera,
		TPointLightComponent,
		TTargetComponent,

		NUM_COMPONENT_TYPES
	};

	static const char* TypeNames[NUM_COMPONENT_TYPES];

	//=========================================================================
	// public methods.
	//=========================================================================
    //コンストラクタ
	Component(class Actor* owner, int updateOrder = 100);

	//デストラクタ
	virtual ~Component();

	//Update
	virtual void Update(float deltaTime);

	//入力
	virtual void ProcessInput(const uint8_t* keyState) {}

	//座標
	virtual void OnUpdateWorldTransform();

	//Getter,Setter
	class Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }
	virtual TypeID GetType() const = 0;

	// Load/Save
	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;
	template <typename T>
	static Component* Create(class Actor* actor, const rapidjson::Value& inObj)
	{
	
		T* t = new T(actor);
		t->LoadProperties(inObj);
		return t;
	}

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	// Owning actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
