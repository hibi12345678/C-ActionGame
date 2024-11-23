// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "DropItemActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "FollowActor.h"
#include <cmath>
#include <cstdlib> 
#include <ctime>   
#include "Random.h"
DropItemActor::DropItemActor(Game* game)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
{
	SetScale(30.0f);
	mc = new MeshComponent(this);
	// Add a box component\	
	mBox = new BoxComponent(this);
	AABB myBox(Vector3(-1.0f, -1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 30.0f));
	mBox->SetObjectBox(myBox);
	mBox->SetShouldRotate(false);
	
	game->AddDropItem(this);
	randomValue = rand() % 2;
	if (randomValue ==0) {
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/ArrowDot.gpmesh");
		mc->SetMesh(mesh);
		itemValue = 0;
	}
	else if (randomValue == 1) {
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bomb.gpmesh");
		mc->SetMesh(mesh);
		itemValue = 1;
	}
}
DropItemActor::~DropItemActor() {
	GetGame()->RemoveDropItem(this);
}
void DropItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	mLifeSpan -= deltaTime;
	if (mLifeSpan < 3.0f)
	{
		
		// 経過時間を増加させる
		blinkTime += deltaTime;

		// 一定間隔を超えたら可視状態を切り替え
		if (blinkTime >= blinkInterval)
		{
			// 可視状態を反転させる
			isVisible = !isVisible;
			mc->SetVisible(isVisible);

			// タイマーをリセット
			blinkTime = 0.0f;
		}
		if (mLifeSpan < 0.0f)
		{
			SetState(EDead);
			// 経過時間を増加させる
			
		}
	}

	float angle = 90.0f * deltaTime ;

	// Y軸（または任意の軸）回転を行うクォータニオンを生成
	Quaternion rotation = Quaternion::CreateFromAxisAngle(Math::ToRadians(angle));

	Quaternion mRotation = GetRotation();
	// 現在の回転に新しい回転を掛け合わせて累積的に回転させる
	mRotation = Quaternion::Concatenate(mRotation, rotation);

	// 回転をセット
	SetRotation(mRotation);

}

void DropItemActor::FixCollisions() {

	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& itemBox = mBox->GetWorldBox();
	Vector3 pos = GetPosition();
	// ポインタを受け取る場合
	auto* player = GetGame()->GetPlayer();
	if (player != nullptr) {

		// デリファレンスしてメンバにアクセス
		const AABB& enemyBox = player->GetBox()->GetWorldBox();
		if (Intersect(itemBox, enemyBox))
		{
			SetState(EDead);
		}

		
	}
}
