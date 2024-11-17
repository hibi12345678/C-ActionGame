// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TorchItemActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "PointLightComponent.h"
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "MoveComponent.h"
TorchItemActor::TorchItemActor(Game* game)
	:Actor(game)
	, mLifeSpan(10.0f)
	, blinkTime(0.0f)
	, blinkInterval(0.2f)
	, isVisible(true)
	, randomValue(0)
	, mState(EActive)


{
	SetScale(50.0f);
	mc = new MeshComponent(this);

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/Torch3D.gpmesh");
	mc->SetMesh(mesh);
	for (int i = 0; i <= 2; i++) {
		PointLightComponent* pointLight = new PointLightComponent(game->GetPlayer());
		pointLight->SetCol(Vector3(1.0, 0.6, 0.0));
		pointLight->SetInRad(100.0);
		pointLight->SetOutRad(400.0);
		pointLight->SetPosition(Vector3(200.0f, 0.0f, 200.0f));
		mPointLights.emplace_back(pointLight);
	}

	
}

void TorchItemActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Game* game = GetGame();

	Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Sword_joint");
	Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Sword_joint");

	Quaternion playerRotation = game->GetPlayer()->GetRotation();
	Rotation = Quaternion::Concatenate(Rotation, playerRotation);

	SetRotation(Rotation);
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	Matrix4 playerTransform = Matrix4::CreateFromQuaternion(playerRotation); // 回転行列を取得
	Vector3 globalWeaponPos = Vector3::Transform(Position, playerTransform); // ローカル座標を変換
	globalWeaponPos += playerPosition; // プレイヤーの位置を加算
	SetPosition(globalWeaponPos);

	if (FollowActor::ETorch != game->GetPlayer()->GetItemState()) {

		for (auto b : mPointLights) {
			delete b;
		}
		mPointLights.clear();
		delete this;

	}


}

