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
#include "Component.h"
#include "Actor.h"
#include "LevelLoader.h"

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const char* Component::TypeNames[NUM_COMPONENT_TYPES] = {
	"Component",
	"AudioComponent",
	"BallMove",
	"BoxComponent",
	"CameraComponent",
	"FollowCamera",
	"MeshComponent",
	"MoveComponent",
	"SkeletalMeshComponent",
	"SpriteComponent",
	"MirrorCamera",
	"PointLightComponent",
	"TargetComponent"
};



///////////////////////////////////////////////////////////////////////////////
// Component class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	,mUpdateOrder(updateOrder)
{
	//�A�N�^�[�ɂ��̃R���|�[�l���g��ǉ�
	mOwner->AddComponent(this);
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
Component::~Component()
{
	mOwner->RemoveComponent(this);
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void Component::Update(float deltaTime)
{
}


//-----------------------------------------------------------------------------
//�@World���W�ւ̕ϊ�
//-----------------------------------------------------------------------------
void Component::OnUpdateWorldTransform()
{
}


//-----------------------------------------------------------------------------
//�@Json�t�@�C������̃f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void Component::LoadProperties(const rapidjson::Value& inObj)
{
	JsonHelper::GetInt(inObj, "updateOrder", mUpdateOrder);
}


//-----------------------------------------------------------------------------
//�@Json�t�@�C���ւ̏�������
//-----------------------------------------------------------------------------
void Component::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddInt(alloc, inObj, "updateOrder", mUpdateOrder);
}
