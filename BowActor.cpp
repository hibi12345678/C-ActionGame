// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BowActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "MoveComponent.h"
BowActor::BowActor(Game* game, int num)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
	, mState(EActive)


{
	SetScale(1.0f);
	mc = new MeshComponent(this);


	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Bow.gpmesh");
	mc->SetMesh(mesh);
	






}

void BowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Game* game = GetGame();




	Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Shield_joint");
	Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Shield_joint");
	
	Quaternion playerRotation = game->GetPlayer()->GetRotation();
	Rotation = Quaternion::Concatenate(Rotation, playerRotation);

	SetRotation(Rotation);
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	Matrix4 playerTransform = Matrix4::CreateFromQuaternion(playerRotation); // 回転行列を取得
	Vector3 globalWeaponPos = Vector3::Transform(Position, playerTransform); // ローカル座標を変換
	globalWeaponPos += playerPosition; // プレイヤーの位置を加算
	SetPosition(globalWeaponPos);




	if (FollowActor::EBow != game->GetPlayer()->GetItemState()) {

		delete this;

	}

}

