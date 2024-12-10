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
    // スケールを設定
    SetScale(1.0f);
    SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    // スケールを設定
    SetRotation(Quaternion(0.0f,1.0f,0.0f,1.0f));
    // メッシュコンポーネントを作成し、メッシュを設定
    UIMeshComponent* mc = new UIMeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Object/NewTree.gpmesh");
    mc->SetMesh(mesh);

}

void UIActor::UpdateActor(float deltaTime)
{

}