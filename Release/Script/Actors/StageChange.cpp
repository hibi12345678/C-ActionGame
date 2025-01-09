//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "StageChange.h"
#include "AudioSystem.h"
#include "BossActor.h"
#include "BoxComponent.h"
#include "FollowActor.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Terrain.h"
#include "Renderer.h"
#include "MoveComponent.h"
#include "TreeActor.h"

///////////////////////////////////////////////////////////////////////////////
// StageChange class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
StageChange::StageChange(Game* game)
	:Actor(game) 
	,mGame(game)
	,mStageFlag(false)
{
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-300.0f, -100.0f, 0.0f),
		Vector3(300.0f, 300.0f, 150.0f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	game->AddStageChange(this);
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
StageChange::~StageChange()
{
	mGame->RemoveStageChange(this);
}

void StageChange::MoveStage(int stageNum)
{
	Vector3 light = mGame->GetRenderer()->GetAmbientLight();
	if (!mStageFlag) {
		mGame->GetRenderer()->SetAmbientLight(light - Vector3(0.03f, 0.03f, 0.03f));

		if (light.x<=0.0f) {
			
			if (stageNum == 1) {
				mGame->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(7000.0f, -2000.0f, -2300.0f));
				mGame->GetRenderer()->GetTerrain()->SetAngle(90.0f);
				LevelLoader::LoadLevel(mGame, "Assets/Level/Actor2.gplevel");
				LevelLoader::LoadLevel(mGame, "Assets/Level/Stage2.gplevel");
				LevelLoader::LoadLevel(mGame, "Assets/Level/Light2.gplevel");
				LevelLoader::LoadLevel(mGame, "Assets/Level/Obstacle2.gplevel");
				mGame->GetPlayer()->SetPosition(Vector3(-838.0f, -928.0f, 50.0f));
				SetPosition(Vector3(1115.0f, 380.0f, 0.0f));
				AABB newBox(Vector3(-150.0f, -300.0f, 0.0f),
					Vector3(150.0f, 300.0f, 150.0f));
				mBoxComp->SetObjectBox(newBox);
				mBoxComp->SetShouldRotate(false);
				for (int i = 0; i < 5; ++i) {
					float xPosition = -1500.0f + (i * 400.0f);  // Y座標を-1300から400ずつ増やす
					TreeActor* tree = new TreeActor(mGame);
					tree->SetPosition(Vector3(-1500.0f, xPosition, -100.0f));  // X, Y, Z座標を設定
				}
				for (int i = 0; i < 5; ++i) {
					float xPosition = -1500.0f + (i * 400.0f);
					TreeActor* tree = new TreeActor(mGame);
					tree->SetPosition(Vector3(xPosition, -1500.0f, -100.0f));  // X, Y, Z座標を設定
				}
				mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
				GetGame()->SetState(Game::GameState::EGameplay);
			}
			else if (stageNum == 2) {
				mGame->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(0.0f, 0.0f, -100.0f));
				mGame->GetRenderer()->GetTerrain()->SetAngle(0.0f);
				mGame->GetRenderer()->GetTerrain()->SetScale(glm::vec3(4.0f, 4.0f, 10.0f));
				mGame->GetRenderer()->GetTerrain()->SetTex(true);
				LevelLoader::LoadLevel(mGame, "Assets/Level/Stage3.gplevel");
				LevelLoader::LoadLevel(mGame, "Assets/Level/Light3.gplevel");
				LevelLoader::LoadLevel(mGame, "Assets/Level/Obstacle3.gplevel");
				mGame->GetPlayer()->SetPosition(Vector3(-500.0f, 0.0f, -100.0f));
				Quaternion rotation = Quaternion::CreateFromAxisAngle(540.0f);
				mGame->GetPlayer()->SetRotation(rotation);
				mGame->GetPlayer()->DeleteCamera();
				BossActor* boss = new BossActor(mGame);
				boss->SetPosition(Vector3(500.0f, 0.0f, -100.0f));

				SetPosition(Vector3(3000.0f, 3000.0f, 0.0f));
				mGame->GetRenderer()->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
				auto& bosses = mGame->GetBoss();

				
				mGame->SetState(Game::GameState::EBossMovie);
			}
			
			mStageFlag = true;
			mGame->SetStageNumber(stageNum + 1);
			
		}
	}

	else if (mStageFlag) {
		
		mStageFlag = false;
	}
}