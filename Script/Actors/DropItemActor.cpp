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
#include "DropItemActor.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "FollowActor.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Random.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
DropItemActor::DropItemActor(Game* game)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
{
	
	mc = new MeshComponent(this);
	game->AddDropItem(this);
	randomValue = rand() % 2;
	if (randomValue ==0) {
		SetScale(30.0f);
		AABB myBox(Vector3(-1.0f, -1.0f, 0.0f),
			Vector3(1.0f, 1.0f, 1.0f));
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/ArrowDot.gpmesh");
		mBox = new BoxComponent(this);
		mBox->SetObjectBox(myBox);
		mBox->SetShouldRotate(false);
		mc->SetMesh(mesh);
		itemValue = 0;
	}
	else if (randomValue == 1) {
		SetScale(0.2f);
		AABB myBox(Vector3(-150.0f, -150.0f, 0.0f),
			Vector3(150.0f, 150.0f, 300.0f));
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb.gpmesh");
		mBox = new BoxComponent(this);
		mBox->SetObjectBox(myBox);
		mBox->SetShouldRotate(false);
		mc->SetMesh(mesh);
		itemValue = 1;
	}
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
DropItemActor::~DropItemActor() {
	GetGame()->RemoveDropItem(this);
}


//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void DropItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 3.0f)
	{
		// �o�ߎ��Ԃ𑝉�������
		blinkTime += deltaTime;

		// ���Ԋu�𒴂��������Ԃ�؂�ւ�
		if (blinkTime >= blinkInterval)
		{
			// ����Ԃ𔽓]������
			isVisible = !isVisible;
			mc->SetVisible(isVisible);

			// �^�C�}�[�����Z�b�g
			blinkTime = 0.0f;
		}

		//����
		if (mLifeSpan < 0.0f)
		{
			SetState(EDead);
		}
	}

	float angle = 90.0f * deltaTime ;

	// Y����]���s���N�H�[�^�j�I���𐶐�
	Quaternion rotation = Quaternion::CreateFromAxisAngle(Math::ToRadians(angle));
	Quaternion mRotation = GetRotation();
	// ���݂̉�]�ɐV������]���|�����킹�ĉ�]������
	mRotation = Quaternion::Concatenate(mRotation, rotation);
	// ��]���Z�b�g
	SetRotation(mRotation);
}


//-----------------------------------------------------------------------------
//  �Փˏ���
//-----------------------------------------------------------------------------
void DropItemActor::FixCollisions() {

	ComputeWorldTransform();

	const AABB& itemBox = mBox->GetWorldBox();
	Vector3 pos = GetPosition();
	auto* player = GetGame()->GetPlayer();
	if (player != nullptr) {
		const AABB& enemyBox = player->GetBox()->GetWorldBox();
		if (Intersect(itemBox, enemyBox))
		{
			SetState(EDead);
		}
	}
}
