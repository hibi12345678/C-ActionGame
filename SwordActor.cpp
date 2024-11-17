// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SwordActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "MoveComponent.h"
SwordActor::SwordActor(Game* game,int num)
	:Actor(game)
	, mLifeSpan(10.0f)
	, mNum(num)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
	, mState(EActive)
	

{
	SetScale(1.0f);
	mc = new MeshComponent(this);

	if (mNum == 0) {
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Sword.gpmesh");
		mc->SetMesh(mesh);
	}
	else if(mNum == 1) {
		Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Shield.gpmesh");
		mc->SetMesh(mesh);
	}

	



}

void SwordActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Game* game = GetGame();

	

	if (mNum == 0) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Sword_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Sword_joint");
	}

	else if(mNum == 1) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Shield_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Shield_joint");
	}
	Quaternion playerRotation = game->GetPlayer()->GetRotation();
	Rotation = Quaternion::Concatenate(Rotation,playerRotation);

	SetRotation(Rotation);
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	Matrix4 playerTransform = Matrix4::CreateFromQuaternion(playerRotation); // 回転行列を取得
	Vector3 globalWeaponPos = Vector3::Transform(Position, playerTransform); // ローカル座標を変換
	globalWeaponPos += playerPosition; // プレイヤーの位置を加算
	SetPosition(globalWeaponPos);
	



	if (FollowActor::ESword != game->GetPlayer()->GetItemState()) {

		delete this;

	}

}

