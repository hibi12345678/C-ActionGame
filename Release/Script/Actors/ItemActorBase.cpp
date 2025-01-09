//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ItemActorBase.h"
#include "FollowActor.h"
#include "Game.h"
#include "Maths.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"


///////////////////////////////////////////////////////////////////////////////
// ItemActorBase class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
ItemActorBase::ItemActorBase(Game* game, float scale, int num)
    : Actor(game)
    , mNum(num)

{
	SetScale(scale);
    mc = new MeshComponent(this);
}



//-----------------------------------------------------------------------------
//  Update
//-----------------------------------------------------------------------------
void ItemActorBase::UpdateActor(float deltaTime)
{

	
    Actor::UpdateActor(deltaTime);
	Game* game = GetGame();
	FollowActor* player= game->GetPlayer();

	bool isVisible = player->GetVisible();
	SetVisible(isVisible);
	if (mNum == 0) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Sword_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Sword_joint");
	}

	else if (mNum == 1) {
		Position = game->GetPlayer()->GetSekltalMesh()->GetBonePosition("Shield_joint");
		Rotation = game->GetPlayer()->GetSekltalMesh()->GetBoneRotation("Shield_joint");
	}

	Quaternion playerRotation = game->GetPlayer()->GetRotation();
	Rotation = Quaternion::Concatenate(Rotation, playerRotation);

	SetRotation(Rotation);
	Vector3 playerPosition = game->GetPlayer()->GetPosition();
	Matrix4 playerTransform = Matrix4::CreateFromQuaternion(playerRotation); 
	Vector3 globalWeaponPos = Vector3::Transform(Position, playerTransform); 
	globalWeaponPos += playerPosition;
	SetPosition(globalWeaponPos);
}

//-----------------------------------------------------------------------------
//  可視状態の処理
//-----------------------------------------------------------------------------
void ItemActorBase::SetVisible(bool visible)
{

	mc->SetVisible(visible);
}