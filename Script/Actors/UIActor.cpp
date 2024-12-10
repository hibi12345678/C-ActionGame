//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "UIActor.h"
#include "Game.h"
#include "Mesh.h"
#include "Renderer.h"
#include "UIMeshComponent.h"


UIActor::UIActor(Game* game)
    : Actor(game)
{
    // �X�P�[����ݒ�
    SetScale(1.0f);
    SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    // �X�P�[����ݒ�
    SetRotation(Quaternion(0.0f,1.0f,0.0f,1.0f));
    // ���b�V���R���|�[�l���g���쐬���A���b�V����ݒ�
    UIMeshComponent* mc = new UIMeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/NewTree.gpmesh");
    mc->SetMesh(mesh);

}

void UIActor::UpdateActor(float deltaTime)
{

}