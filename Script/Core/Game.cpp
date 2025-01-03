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
#include "Game.h"
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <imgui_impl_sdl2.h>
#include <rapidjson/document.h>
#include "Actor.h"
#include "Animation.h"
#include "AudioSystem.h"
#include "BossActor.h"
#include "DialogBox.h"
#include "DropItemActor.h"
#include "EnemyActor.h"
#include "ExplosionActor.h"
#include "FollowActor.h"
#include "Font.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameTimer.h"
#include "HUD.h"
#include "ItemMenu.h"
#include "LevelLoader.h"
#include "MainmenuUI.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PauseMenu.h"
#include "PlaneActor.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SoundEvent.h"
#include "StageChange.h"
#include "Terrain.h"
#include "Tutorial.h"
#include "TreeActor.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
// Game class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Game::Game()
	:mRenderer(nullptr)
	, mAudioSystem(nullptr)
	, mPhysWorld(nullptr)
	, mGameState(GameState::EMainMenu)  // 初期状態をメインメニューに設定)
	, mUpdatingActors(false)
	, mainFlag(true)
	, playFlag(false)
	, gameOverFlag(false)
	, gameClearFlag(false)
	, scoreNumber(0)
    , mBossTime(0.0f)
	, stageNumber(1)
	, mHitStopTimer(0.0f)
	, mHUD(nullptr)
	, mFollowActor(nullptr)
	, mCrosshair(nullptr)
{	
}


//-----------------------------------------------------------------------------
//   初期化処理です
//-----------------------------------------------------------------------------
bool Game::Initialize()
{
	
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//rendererの生成
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
	
	//audiosystemの生成
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}
	mMusicEvent = mAudioSystem->PlayEvent("event:/BGM1");
	mMusicEvent = mAudioSystem->PlayEvent("event:/BGM2");
	mMusicEvent = mAudioSystem->PlayEvent("event:/BGM3");
	//physics worldの生成
	mPhysWorld = new PhysWorld(this);
	
	//SDL_ttfの初期化
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	LoadData();
	
	GetData();

	timer = new GameTimer();
	return true;
}

//-----------------------------------------------------------------------------
//      実行します.
//-----------------------------------------------------------------------------
void Game::RunLoop()
{
	while (mGameState != GameState::EQuit)
	{		
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

//-----------------------------------------------------------------------------
//  入力処理
//-----------------------------------------------------------------------------
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
#ifdef _DEBUG
		// ImGuiにSDLイベントを処理させる
		ImGui_ImplSDL2_ProcessEvent(&event);
#endif // DEBUG

		switch (event.type)
		{
		case SDL_QUIT:
			mGameState = GameState::EQuit;
			break;
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyInput(event.key.keysym.sym);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:

			HandleMouseInput(event.button.button);
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (mGameState == GameState::EGameplay)
	{
		ProcessGameplayInput(state);
	}
	else if (!mUIStack.empty())
	{
		if (mGameState == GameState::EMainMenu)
		{
			mUIStack.back()->StartInput(state);
		}
		else
		{
			mUIStack.back()->ProcessInput(state);
		}
	}
}


//-----------------------------------------------------------------------------
//  GameState判別
//-----------------------------------------------------------------------------
void Game::HandleKeyInput(int key)
{
	if (mGameState == GameState::EGameplay || mGameState == GameState::EMainMenu)
	{
		HandleKeyPress(key);
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->HandleKeyPress(key);
	}
}


//-----------------------------------------------------------------------------
//  マウス入力処理
//-----------------------------------------------------------------------------
void Game::HandleMouseInput(Uint8 button)
{
	if (mGameState == GameState::EGameplay && !gameOverFlag && !gameClearFlag)
	{
		HandleKeyPress(button);
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->HandleKeyPress(button);
	}
}


//-----------------------------------------------------------------------------
//  GamePlay中にのみ入力処理が反応するように
//-----------------------------------------------------------------------------
void Game::ProcessGameplayInput(const Uint8* state)
{
	if (!gameOverFlag && !gameClearFlag)
	{
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EActive)
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput(state);
	}
}


//-----------------------------------------------------------------------------
//  キーボード入力の処理
//-----------------------------------------------------------------------------
void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		// Pause menu
		new PauseMenu(this);
		mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
		timer->StopTimer();
		break;
	case SDLK_TAB:
		if (mGameState == GameState::EGameplay)
		{
			// Item menu
			new ItemMenu(this);
			mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
			timer->StopTimer();
		}
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// 　Update
//-----------------------------------------------------------------------------
void Game::UpdateGame()
{

	FollowActor* followActor = GetPlayer();
	if (mGameState == GameState::EGameOver && gameOverFlag == false) {

		new GameOver(this);
		gameOverFlag = true;
		//GameOverBGMの生成
		mMusicEvent = mAudioSystem->PlayEvent("event:/GameOver");	

	}
	
	//delta timeの計算
	// DeltaTime計算
	float deltaTime = CalculateDeltaTime();

	// Update audio system
	mAudioSystem->Update(deltaTime);
	// ゲーム状態に応じた処理
	switch (mGameState)
	{
	case GameState::EGameplay:
		UpdateGameplay(deltaTime);
		break;

	case GameState::EBossMovie:
		UpdateBossMovie(deltaTime);
		break;

	case GameState::EBossDefeat:
		UpdateBossDefeat(deltaTime);
		break;

	case GameState::EGameOver:
		UpdateGameOver(deltaTime);
		break;

	case GameState::EGameClear:
		UpdateGameClear();
		break;

	case GameState::EMainMenu:
		UpdateMainMenu();
		break;

	case GameState::ELoadStage:
		UpdateLoadStage(deltaTime);
		break;

	default:
		break;
	}

	// UIの更新
	UpdateUI(deltaTime);

}
//-----------------------------------------------------------------------------
// DeltaTime計算
//-----------------------------------------------------------------------------
float Game::CalculateDeltaTime()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	return deltaTime;
}

//-----------------------------------------------------------------------------
// EGameplay状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateGameplay(float deltaTime)
{
	if (!playFlag)
	{
		ResetGameplay();
	}

	// アクターの更新
	UpdateActors(deltaTime);

	// 新しいアクターの追加
	AddPendingActors();

	// 死亡したアクターの削除
	DeleteDeadActors();
}

//-----------------------------------------------------------------------------
// ゲームプレイのリセット
//-----------------------------------------------------------------------------
void Game::ResetGameplay()
{
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	LoadData();
	playFlag = true;
	mainFlag = false;
}

//-----------------------------------------------------------------------------
// アクターの更新
//-----------------------------------------------------------------------------
void Game::UpdateActors(float deltaTime)
{
	mUpdatingActors = true;
	mHitStopTimer -= deltaTime;

	for (auto actor : mActors)
	{
		if (mHitStopTimer < 0.0f || mHitStopTimer > 0.15f)
		{
			actor->Update(deltaTime);
		}
		else
		{
			actor->Update(deltaTime * 0.1f);
		}
	}

	mUpdatingActors = false;
}

//-----------------------------------------------------------------------------
// 新しいアクターの追加
//-----------------------------------------------------------------------------
void Game::AddPendingActors()
{
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();
}

//-----------------------------------------------------------------------------
// 死亡したアクターの削除
//-----------------------------------------------------------------------------
void Game::DeleteDeadActors()
{
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

//-----------------------------------------------------------------------------
// EBossMovie状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateBossMovie(float deltaTime)
{
	for (auto actor : mBossActor)
	{
		actor->Update(deltaTime);
	}
}

//-----------------------------------------------------------------------------
// EBossDefeat状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateBossDefeat(float deltaTime)
{
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
}

//-----------------------------------------------------------------------------
// EGameOver状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateGameOver(float deltaTime)
{
	mFollowActor->Update(deltaTime);
	Vector3 light = this->GetRenderer()->GetAmbientLight();
	this->GetRenderer()->SetAmbientLight(light - Vector3(0.003f, 0.003f, 0.003f));
}

//-----------------------------------------------------------------------------
// EGameClear状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateGameClear()
{
	if (!gameClearFlag)
	{
		mMusicEvent = mAudioSystem->PlayEvent("event:/GameClear");
		new GameClear(this);
		gameClearFlag = true;
	}
}

//-----------------------------------------------------------------------------
// EMainMenu状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateMainMenu()
{
	if (!mainFlag)
	{
		ResetMainMenu();
	}

	GetRenderer()->GetTerrain()->SetTex(false);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(SDL_ENABLE);
}

//-----------------------------------------------------------------------------
// メインメニューのリセット
//-----------------------------------------------------------------------------
void Game::ResetMainMenu()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	LoadData();
	mainFlag = true;
	playFlag = false;
}

//-----------------------------------------------------------------------------
// ELoadStage状態の処理
//-----------------------------------------------------------------------------
void Game::UpdateLoadStage(float deltaTime)
{
	DeleteDeadActors();
	sc->MoveStage(stageNumber);
}

//-----------------------------------------------------------------------------
// UIの更新
//-----------------------------------------------------------------------------
void Game::UpdateUI(float deltaTime)
{
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::UIState::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	// 閉じられたUIの削除
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::UIState::EClosing)
		{
			delete* iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


//描画処理
void Game::GenerateOutput()
{
	mRenderer->Draw();
}


//-----------------------------------------------------------------------------
//  オブジェクトを読み取り、配置する
//-----------------------------------------------------------------------------
void Game::LoadData()
{
	// UIスタックのクリア
	if (!mUIStack.empty()) {
		while (!mUIStack.empty()) {
			delete mUIStack.back();
			mUIStack.pop_back();
		}
	}

	// ゲームプレイ時の処理
	if (mGameState == GameState::EGameplay) {
		scoreNumber = 0;
		LoadText("Assets/Text/Main.gptext");
		mHUD = new HUD(this);
		LevelLoader::LoadLevel(this, "Assets/Level/Actor.gplevel");

		mFollowActor->GetMoveComponent()->SetAngularSpeed(90.0f);
		new Tutorial(this);
		sc = new StageChange(this);
		sc->SetPosition(Vector3(-200.0f, 2500.0f, -100.0f));

		// 木の配置
		for (int i = 0; i < 9; ++i) {
			float xPosition = -1800.0f + (i * 400.0f);
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3(xPosition, -1500.0f, -100.0f));
		}
		for (int i = 0; i < 7; ++i) {
			float xPosition = -2200.0f + (i * 400.0f);
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3(1500.0f, xPosition, -100.0f));
		}
		for (int i = 0; i < 7; ++i) {
			float xPosition = -2200.0f + (i * 400.0f);
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3(-1500.0f, xPosition, -100.0f));
		}

		// ゲームオーバー・ゲームクリアフラグの初期化
		gameOverFlag = false;
		gameClearFlag = false;
		timer->ResetTimer();
		timer->StartTimer();
	}
	// メインメニュー時の処理
	else if (mGameState == GameState::EMainMenu) {
		LoadText("Assets/Text/Mainmenu.gptext");
		mHUD = new HUD(this);
		LevelLoader::LoadLevel(this, "Assets/Level/Stage.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Light.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Obstacle.gplevel");

		new MainmenuUI(this);
		stageNumber = 1;

		// Terrainの設定
		auto terrain = this->GetRenderer()->GetTerrain();
		terrain->SetTranslate(glm::vec3(15300.0f, 10000.0f, -100.0f));
		terrain->SetAngle(0.0f);
		terrain->SetScale(glm::vec3(30.0f, 30.0f, 30.0f));

		// マウス設定
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(SDL_ENABLE);
	}
}


//-----------------------------------------------------------------------------
//  終了処理
//-----------------------------------------------------------------------------
void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}

	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}

	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	for (auto a : mAnims)
	{
		delete a.second;
	}
}


//-----------------------------------------------------------------------------
//   終了処理
//-----------------------------------------------------------------------------
void Game::Shutdown()
{
	UnloadData();
	TTF_Quit();
	delete mPhysWorld;
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	SDL_Quit();
}


//-----------------------------------------------------------------------------
//   Actor vector に追加
//-----------------------------------------------------------------------------
void Game::AddActor(Actor* actor)
{

	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}


//-----------------------------------------------------------------------------
//   Actor vector から削除
//-----------------------------------------------------------------------------
void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
	
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


//-----------------------------------------------------------------------------
//   UIStack vector に追加
//-----------------------------------------------------------------------------
void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}


//-----------------------------------------------------------------------------
//  Fontをファイルパスから取得
//-----------------------------------------------------------------------------
Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}


//-----------------------------------------------------------------------------
//  Textをファイルパスから取得
//-----------------------------------------------------------------------------
void Game::LoadText(const std::string& fileName)
{

	mText.clear();

	rapidjson::Document doc;
	if (!LevelLoader::LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load text file %s", fileName.c_str());
		return;
	}
	
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}


//-----------------------------------------------------------------------------
//  jsonファイルからkeyと一致したtextの取得
//-----------------------------------------------------------------------------
const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("Loading");
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}


//-----------------------------------------------------------------------------
//  スケルトンをファイルパスから取得
//-----------------------------------------------------------------------------
Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	auto iter = mSkeletons.find(fileName);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
		{
			mSkeletons.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}


//-----------------------------------------------------------------------------
//  アニメーションをファイルパスから取得
//-----------------------------------------------------------------------------
Animation* Game::GetAnimation(const std::string& fileName)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}


//-----------------------------------------------------------------------------
//   Plane vector に追加
//-----------------------------------------------------------------------------
void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}


//-----------------------------------------------------------------------------
//   Plane vectorから削除
//-----------------------------------------------------------------------------
void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}


//-----------------------------------------------------------------------------
//   Enemy vectorに追加
//-----------------------------------------------------------------------------
void Game::AddEnemy(EnemyActor* enemy)
{
	mEnemys.emplace_back(enemy);
}


//-----------------------------------------------------------------------------
//   Enemy vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveEnemy(EnemyActor* enemy)
{
	auto iter = std::find(mEnemys.begin(), mEnemys.end(), enemy);
	mEnemys.erase(iter);
}

//-----------------------------------------------------------------------------
//   BossActor vectorに追加
//-----------------------------------------------------------------------------
void Game::AddBossActor(BossActor* enemy)
{
	mBossActor.emplace_back(enemy);
}


//-----------------------------------------------------------------------------
//   BossActor vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveBossActor(BossActor* enemy)
{
	auto iter = std::find(mBossActor.begin(), mBossActor.end(), enemy);
	mBossActor.erase(iter);
}


//-----------------------------------------------------------------------------
//  DropItem vectorに追加
//-----------------------------------------------------------------------------
void Game::AddDropItem(DropItemActor* dropItem)
{
	mDropItems.emplace_back(dropItem);
}


//-----------------------------------------------------------------------------
//  DropItem vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveDropItem(DropItemActor* dropItem)
{
	auto iter = std::find(mDropItems.begin(), mDropItems.end(), dropItem);
	mDropItems.erase(iter);
}


//-----------------------------------------------------------------------------
//  Arrow vectorに追加
//-----------------------------------------------------------------------------
void Game::AddArrow(ArrowActor* arrow)
{
	mArrows.emplace_back(arrow);
}


//-----------------------------------------------------------------------------
//  Arrow vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveArrow(ArrowActor* arrow)
{
	auto iter = std::find(mArrows.begin(), mArrows.end(), arrow);
	mArrows.erase(iter);
}


//-----------------------------------------------------------------------------
//   Bomb vectorに追加
//-----------------------------------------------------------------------------
void Game::AddBomb(BombActor* bomb)
{
	mBombs.emplace_back(bomb);
}


//-----------------------------------------------------------------------------
//  Bomb vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveBomb(BombActor* bomb)
{
	auto iter = std::find(mBombs.begin(), mBombs.end(), bomb);
	mBombs.erase(iter);
}


//-----------------------------------------------------------------------------
//   Explosion vectorに追加
//-----------------------------------------------------------------------------
void Game::AddExplosion(ExplosionActor* explosion)
{
	mExplosions.emplace_back(explosion);
}


//-----------------------------------------------------------------------------
//  Explosion vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveExplosion(ExplosionActor* explosion)
{
	auto iter = std::find(mExplosions.begin(), mExplosions.end(), explosion);
	mExplosions.erase(iter);
}


//-----------------------------------------------------------------------------
//   Tree vectorに追加
//-----------------------------------------------------------------------------
void Game::AddTree(TreeActor* tree)
{
	mTrees.emplace_back(tree);
}


//-----------------------------------------------------------------------------
//  Tree vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveTree(TreeActor* tree)
{
	auto iter = std::find(mTrees.begin(), mTrees.end(), tree);
	mTrees.erase(iter);
}


//-----------------------------------------------------------------------------
//  StageChange vectorに追加
//-----------------------------------------------------------------------------
void Game::AddStageChange(StageChange* sc)
{
	mStageChanges.emplace_back(sc);
}


//-----------------------------------------------------------------------------
//  StageChange vectorから削除
//-----------------------------------------------------------------------------
void Game::RemoveStageChange(StageChange* sc)
{
	auto iter = std::find(mStageChanges.begin(), mStageChanges.end(),
		sc);
	mStageChanges.erase(iter);
}


//-----------------------------------------------------------------------------
//  Stage切り替えるために読み込み
//-----------------------------------------------------------------------------
void Game::LoadStage() {

	for(auto ac :mEnemys)
	{
		ac->Actor::SetState(Actor::EDead);
	}
	for (auto ac : mTrees)
	{
		ac->SetState(Actor::EDead);
	}
	for (auto ac : mPlanes)
	{
		ac->SetState(Actor::EDead);
	}
}

//-----------------------------------------------------------------------------
//  データファイルの取得
//-----------------------------------------------------------------------------
void Game::GetData() {

	GetSkeleton("Assets/Skel/EnemyBoss.gpskel");
	GetAnimation("Assets/Anim/Player_idle.gpanim");
	GetAnimation("Assets/Anim/Player_walk.gpanim");
	GetAnimation("Assets/Anim/Player_attack.gpanim");
	GetAnimation("Assets/Anim/Player_attack2.gpanim");
	GetAnimation("Assets/Anim/Player_attack3.gpanim");
	GetAnimation("Assets/Anim/Player_block.gpanim");
	GetAnimation("Assets/Anim/Player_jump.gpanim");
	GetAnimation("Assets/Anim/Player_bow.gpanim");
	GetAnimation("Assets/Anim/Player_bowidle.gpanim");
	GetAnimation("Assets/Anim/Player_bomb.gpanim");
	GetAnimation("Assets/Anim/Enemy_idle.gpanim");
	GetAnimation("Assets/Anim/Enemy_walk.gpanim");
	GetAnimation("Assets/Anim/Enemy_attack.gpanim");
	GetAnimation("Assets/Anim/Enemy_jump_attack.gpanim");
	GetAnimation("Assets/Anim/Enemy_dying.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_idle.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_walk.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_attack.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_jump_attack.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_dying.gpanim");
	GetAnimation("Assets/Anim/EnemyBoss_roa.gpanim");
}

