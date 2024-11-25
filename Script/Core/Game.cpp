// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "UIScreen.h"
#include "HUD.h"
#include "MeshComponent.h"
#include "FollowActor.h"
#include "EnemyActor.h"
#include "BossActor.h"
#include "PlaneActor.h"
#include "PauseMenu.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <rapidjson/document.h>
#include "Skeleton.h"
#include "Animation.h"
#include "PointLightComponent.h"
#include "LevelLoader.h"
#include "DialogBox.h"
#include "MainmenuUI.h"
#include "ItemMenu.h"
#include "GameOver.h"
#include "GameClear.h"
#include <iostream>
#include "imgui_impl_sdl2.h"
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
{
	
}

bool Game::Initialize()
{
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create the renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
	
	// Create the audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	// Create the physics world
	mPhysWorld = new PhysWorld(this);
	
	// Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}
	LoadData();
	
	mTicksCount = SDL_GetTicks();
	GetData();


	return true;
}

void Game::RunLoop()
{
	while (mGameState != GameState::EQuit)
	{		
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// ImGuiにSDLイベントを処理させる
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				mGameState = GameState::EQuit;
				break;
			// This fires when a key's initially pressed
			case SDL_KEYDOWN:
				if (!event.key.repeat)
				{
					if (mGameState == GameState::EGameplay || mGameState == GameState::EMainMenu)
					{
						HandleKeyPress(event.key.keysym.sym);
					}
					else if (!mUIStack.empty())
					{
						mUIStack.back()->
							HandleKeyPress(event.key.keysym.sym);
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (mGameState == GameState::EGameplay && gameOverFlag == false && gameClearFlag == false)
				{
					HandleKeyPress(event.button.button);
				}
				else if (!mUIStack.empty())
				{
					mUIStack.back()->
						HandleKeyPress(event.button.button);
				}
				break;
			default:
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (mGameState == GameState::EGameplay)
	{
		if (gameOverFlag == false && gameClearFlag == false) {

			for (auto actor : mActors)
			{
				if (actor->GetState() == Actor::EActive)
				{
					actor->ProcessInput(state);
				}
			}
			
		}
		else {
			mUIStack.back()->ProcessInput(state);
		}

		
	}

	else if (!mUIStack.empty() && mGameState != GameState::EMainMenu)
	{
		
		mUIStack.back()->ProcessInput(state);
	}
	else if (mGameState == GameState::EMainMenu) {

		mUIStack.back()->StartInput(state);
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		// Create pause menu
		new PauseMenu(this);
		mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
		break;
	case SDLK_TAB:
		if (mGameState == GameState::EGameplay) {
			// Create pause menu
			new ItemMenu(this);
			mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
			
		}
		
		break;
	case '-':
	{
		// Reduce master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		// Increase master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	default:
		break;
	}
}

void Game::UpdateGame()
{

	FollowActor* followActor = GetPlayer();
	if (followActor != nullptr) {
		
		if (!followActor->GetHealth()) {
			float health = followActor->GetHealth();
			if (health <= 0.0f && gameOverFlag == false) {
				new GameOver(this);
				gameOverFlag = true;
				// Start music
				mMusicEvent = mAudioSystem->PlayEvent("event:/GameOver");
			}
		}
		
	}
	BossActor* bossActor = GetBoss();
	if (bossActor != nullptr) {

		if (!bossActor->GetHealth()) {
			float health = bossActor->GetHealth();
			if (health <= 0.0f && gameClearFlag == false) {
				new GameClear(this);
				gameClearFlag = true;
				// Start music
				mMusicEvent = mAudioSystem->PlayEvent("event:/GameClear");
			}
		}

	}
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	float mStamina;
	
	if (mGameState == GameState::EGameplay)
	{

		if (playFlag == false) {
			// Clear the UI stack
			while (!mUIStack.empty())
			{
				delete mUIStack.back();
				mUIStack.pop_back();
			}
			LoadData();
			playFlag = true;
			mainFlag = false;
		}
		// Update all actors
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update(deltaTime);
		}
		mUpdatingActors = false;

		// Move any pending actors to mActors
		for (auto pending : mPendingActors)
		{
			pending->ComputeWorldTransform();
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// Add any dead actors to a temp vector
		std::vector<Actor*> deadActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		// Delete dead actors (which removes them from mActors)
		for (auto actor : deadActors)
		{
			delete actor;
		}


	}

	// Update audio system
	mAudioSystem->Update(deltaTime);

	if (mGameState == GameState::EMainMenu) {
		
		if (mainFlag == false) {

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

		
		// Disable relative mouse mode to show the cursor
		SDL_SetRelativeMouseMode(SDL_FALSE);
		// Optionally, ensure the cursor is explicitly enabled
		SDL_ShowCursor(SDL_ENABLE);
	}
	// Update UI screens
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}
	// Delete any UIScreens that are closed
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// Add any dead actors to a temp vector
	std::vector<EnemyActor*> deadEnemys;
	for (auto actor : mEnemys)
	{
		if (actor->GetState() == EnemyActor::EDead)
		{
			deadEnemys.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadEnemys)
	{
		delete actor;
	}

	if (scoreNumber ==3) {
		mBossTime -= deltaTime;
		if (mBossTime < 0.0f) {
			BossActor* boss = new BossActor(this);
			boss->SetPosition(Vector3(0.0f, 0.0f, -100.0f));
			mBossTime = 0.0f;
			scoreNumber++;
			SetScore(scoreNumber);
		}
		
	}
	
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// 共通の初期化
	if (!mUIStack.empty()) {
		while (!mUIStack.empty()) {
			delete mUIStack.back();
			mUIStack.pop_back();
		}
	}

	// 状態ごとの処理
	if (mGameState == GameState::EGameplay) {
		scoreNumber = 0;
		LoadText("Assets/Text/Main.gptext");
		mHUD = new HUD(this);
		// ゲームプレイ用のレベルロード
		LevelLoader::LoadLevel(this, "Assets/Level/Stage.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Actor.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Light.gplevel");
		gameOverFlag = false;
		gameClearFlag = false;
		// マウス設定
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_GetRelativeMouseState(nullptr, nullptr); // 相対モード初期化
	}
	else if (mGameState == GameState::EMainMenu) {
		LoadText("Assets/Text/Mainmenu.gptext");
		mHUD = new HUD(this);
		LevelLoader::LoadLevel(this, "Assets/Level/Stage.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Light.gplevel");
		new MainmenuUI(this);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(SDL_ENABLE);
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Clear the UI stack
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}

	// Unload fonts
	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}

	// Unload skeletons
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// Unload animations
	for (auto a : mAnims)
	{
		delete a.second;
	}
}

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

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

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

void Game::LoadText(const std::string& fileName)
{
	// Clear the existing map, if already loaded
	mText.clear();

	rapidjson::Document doc;
	if (!LevelLoader::LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load text file %s", fileName.c_str());
		return;
	}
	
	// Parse the text map
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

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("Loading");
	// Find this text in the map, if it exists
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

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

void Game::AddEnemy(EnemyActor* enemy)
{
	mEnemys.emplace_back(enemy);
}

void Game::RemoveEnemy(EnemyActor* enemy)
{
	auto iter = std::find(mEnemys.begin(), mEnemys.end(), enemy);
	mEnemys.erase(iter);
	scoreNumber++;
	SetScore(scoreNumber);
	if (scoreNumber == 3) {
		
		mBossTime = 5.0f;
	}
	
}

void Game::AddDropItem(DropItemActor* dropItem)
{
	mDropItems.emplace_back(dropItem);
}

void Game::RemoveDropItem(DropItemActor* dropItem)
{
	auto iter = std::find(mDropItems.begin(), mDropItems.end(), dropItem);
	mDropItems.erase(iter);
}

void Game::AddArrow(ArrowActor* arrow)
{
	mArrows.emplace_back(arrow);
}

void Game::RemoveArrow(ArrowActor* arrow)
{
	auto iter = std::find(mArrows.begin(), mArrows.end(), arrow);
	mArrows.erase(iter);
}

void Game::AddBomb(BombActor* bomb)
{
	mBombs.emplace_back(bomb);
}

void Game::RemoveBomb(BombActor* bomb)
{
	auto iter = std::find(mBombs.begin(), mBombs.end(), bomb);
	mBombs.erase(iter);
}

void Game::AddExplosion(ExplosionActor* explosion)
{
	mExplosions.emplace_back(explosion);
}

void Game::RemoveExplosion(ExplosionActor* explosion)
{
	auto iter = std::find(mExplosions.begin(), mExplosions.end(), explosion);
	mExplosions.erase(iter);
}

void Game::GetData() {

	GetSkeleton("Assets/Skel/EnemyBoss.gpskel");
	GetAnimation("Assets/Anim/Player_idle.gpanim");
	GetAnimation("Assets/Anim/Player_walk.gpanim");
	GetAnimation("Assets/Anim/Player_attack.gpanim");
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
}