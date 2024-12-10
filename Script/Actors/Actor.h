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
#include <vector>
#include "Component.h"
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//Actor class
///////////////////////////////////////////////////////////////////////////////
class Actor
{
public:

    // Enums for Actor Type and State
    enum TypeID
    {
        TActor = 0,
        TBallActor,
        TFollowActor,
        TEnemyActor,
        TPlaneActor,
        TTargetActor,

        NUM_ACTOR_TYPES
    };

    static const char* TypeNames[NUM_ACTOR_TYPES];

	//Enum State
    enum State
    {
        EActive,
        EPaused,
        EDead
    };
	
    //=========================================================================
    // public methods.
    //=========================================================================
	//コンストラクタ 
    Actor(class Game* game);

	//デストラクタ
    virtual ~Actor();

	//Update
    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);      
    virtual void UpdateActor(float deltaTime);

	//入力
    void ProcessInput(const uint8_t* keyState); 
    virtual void ActorInput(const uint8_t* keyState);

	//座標
    const Vector3& GetPosition() const { return mPosition; }
    void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeTransform = true; }
    float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeTransform = true; }
    const Quaternion& GetRotation() const { return mRotation; }
    void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeTransform = true; }
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }
    void ComputeWorldTransform();

	//方向(Xが正面,Yが横,Zが垂直)
    Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
    Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
    Vector3 GetUp() const { return Vector3::Transform(-1 * Vector3::UnitZ, mRotation); }
    void RotateToNewForward(const Vector3& forward);

	//State
    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    // Game
    class Game* GetGame() { return mGame; }

    // Component
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);
    Component* GetComponentOfType(Component::TypeID type)
    {
        for (Component* c : mComponents)
        {
            if (c->GetType() == type)
            {
                return c;
            }
        }
        return nullptr;
    }
    const std::vector<Component*>& GetComponents() const { return mComponents; }

    // Load,Save
    virtual void LoadProperties(const rapidjson::Value& inObj);
    virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    template <typename T>
    static Actor* Create(class Game* game, const rapidjson::Value& inObj)
    {
        T* t = new T(game);       // Dynamically allocate actor of type T
        t->LoadProperties(inObj); // Load properties from JSON
        return t;
    }
    virtual TypeID GetType() const { return TActor; }

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool mRecomputeTransform; //world空間への変換フラグ
	float mScale;
	Vector3 mPosition;
	Quaternion mRotation;
	Matrix4 mWorldTransform; //ワールド座標
	State mState;

	class Game* mGame;
	std::vector<Component*> mComponents;
};