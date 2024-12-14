//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "StageChange.h"
#include "BoxComponent.h"
#include "FollowActor.h"
#include "Game.h"
#include "Terrain.h"
#include "Renderer.h"
#include "LevelLoader.h"


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
	AABB myBox(Vector3(-250.0f, -250.0f, 0.0f),
		Vector3(250.0f, 250.0f, 150.0f));
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
			
			if (stageNum == 0) {
				mGame->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(3000.0f, -5000.0f, -3000.0f));
				mGame->GetRenderer()->GetTerrain()->SetAngle(90.0f);
				LevelLoader::LoadLevel(mGame, "Assets/Level/Stage.gplevel");
				mGame->GetPlayer()->SetPosition(Vector3(-500.0f, 0.0f, -100.0f));
			}
			else if (stageNum == 1) {
				mGame->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(3000.0f, -5000.0f, -3000.0f));
				mGame->GetRenderer()->GetTerrain()->SetAngle(90.0f);
				LevelLoader::LoadLevel(mGame, "Assets/Level/Stage.gplevel");
				StageChange* sc = new StageChange(mGame);
				sc->SetPosition(Vector3(1500.0f, 0.0f, -100.0f));
				mGame->GetPlayer()->SetPosition(Vector3(-1500.0f, 0.0f, -50.0f));
			}
			else if (stageNum == 2) {
				mGame->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
				mGame->GetRenderer()->GetTerrain()->SetAngle(0.0f);
				LevelLoader::LoadLevel(mGame, "Assets/Level/Stage.gplevel");
				mGame->GetPlayer()->SetPosition(Vector3(-500.0f, 0.0f, -100.0f));

			}
			GetGame()->SetState(Game::GameState::EGameplay);
			mStageFlag = true;
		}
	}
	else if (mStageFlag) {
		mGame->GetRenderer()->SetAmbientLight(light + Vector3(0.02f, 0.02f, 0.02f));
		mGame->SetStageNumber(stageNum + 1);
		mStageFlag = false;
		
	}

}