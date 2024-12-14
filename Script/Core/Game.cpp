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
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
Game::Game()
	:mRenderer(nullptr)
	, mAudioSystem(nullptr)
	, mPhysWorld(nullptr)
	, mGameState(GameState::EMainMenu)  // ������Ԃ����C�����j���[�ɐݒ�)
	, mUpdatingActors(false)
	, mainFlag(true)
	, playFlag(false)
	, gameOverFlag(false)
	, gameClearFlag(false)
	, scoreNumber(0)
    , mBossTime(0.0f)
	, stageNumber(1)
{	
}


//-----------------------------------------------------------------------------
//   �����������ł�
//-----------------------------------------------------------------------------
bool Game::Initialize()
{
	
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//renderer�̐���
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
	
	//audiosystem�̐���
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	//physics world�̐���
	mPhysWorld = new PhysWorld(this);
	
	//SDL_ttf�̏�����
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
//      ���s���܂�.
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
//  ���͏���
//-----------------------------------------------------------------------------
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// ImGui��SDL�C�x���g������������
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				mGameState = GameState::EQuit;
				break;
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


//-----------------------------------------------------------------------------
//  �L�[�{�[�h���͂̏���
//-----------------------------------------------------------------------------
void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		//pause menu�̐���
		new PauseMenu(this);
		mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
		timer->StopTimer();
		break;
	case SDLK_TAB:
		if (mGameState == GameState::EGameplay) {
			//ItemMenu�̐���
			new ItemMenu(this);
			mMusicEvent = mAudioSystem->PlayEvent("event:/Button");
			timer->StopTimer();
		}
		
		break;
	case SDLK_1:
		if (mGameState == GameState::EGameplay) {
			// TreeActor�̐���
			new TreeActor(this);

			mMusicEvent = mAudioSystem->PlayEvent("event:/Button");

		}

		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// �@Update
//-----------------------------------------------------------------------------
void Game::UpdateGame()
{

	FollowActor* followActor = GetPlayer();
	if (mGameState == GameState::EGameOver && gameOverFlag == false) {

		new GameOver(this);
		gameOverFlag = true;
		//GameOverBGM�̐���
		mMusicEvent = mAudioSystem->PlayEvent("event:/GameOver");
				
	}

	BossActor* bossActor = GetBoss();
	if (bossActor != nullptr) {

		if (!bossActor->GetHealth()) {
			float health = bossActor->GetHealth();
			if (health <= 0.0f && gameClearFlag == false) {
				new GameClear(this);
				gameClearFlag = true;
				//GameClearBGM�̐���
				mMusicEvent = mAudioSystem->PlayEvent("event:/GameClear");
			}
		}
	}

	//delta time�̌v�Z
	// �O�̃t���[������16�~���b���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	float mStamina;

	// Update audio system
	mAudioSystem->Update(deltaTime);

	//EGameplay���̏���
	if (mGameState == GameState::EGameplay )
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

	//EGameOver���̏���
	if (mGameState == GameState::EGameOver)
	{

		mFollowActor->Update(deltaTime);
		Vector3 light = this->GetRenderer()->GetAmbientLight();
		
		this->GetRenderer()->SetAmbientLight(light - Vector3(0.003f, 0.003f, 0.003f));

	}

	//EGameClear���̏���
	if (mGameState == GameState::EGameClear)
	{
		mFollowActor->Update(deltaTime);
		mBossActor->Update(deltaTime);

	}


	//EmainMenu���̏���
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

		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(SDL_ENABLE);
	}

	//EGameplay���̏���
	if (mGameState == GameState::ELoadStage)
	{
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
		
		mStageChanges[stageNumber-1]->MoveStage(stageNumber);
		
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

	/*	//�{�X�̐���
	if (scoreNumber == 3) {
		mBossTime -= deltaTime;
		if (mBossTime < 0.0f) {
			BossActor* boss = new BossActor(this);
			boss->SetPosition(Vector3(0.0f, 0.0f, -100.0f));
			mBossTime = 0.0f;
			scoreNumber++;
			SetScore(scoreNumber);
		}	
	}	*/

}

//�`�揈��
void Game::GenerateOutput()
{
	mRenderer->Draw();
}

//-----------------------------------------------------------------------------
//  �I�u�W�F�N�g��ǂݎ��A�z�u����
//-----------------------------------------------------------------------------
void Game::LoadData()
{
	if (!mUIStack.empty()) {
		while (!mUIStack.empty()) {
			delete mUIStack.back();
			mUIStack.pop_back();
		}
	}

	if (mGameState == GameState::EGameplay) {
		
		scoreNumber = 0;
		LoadText("Assets/Text/Main.gptext");
		mHUD = new HUD(this);
		LevelLoader::LoadLevel(this, "Assets/Level/Actor.gplevel");
		mFollowActor->GetMoveComponent()->SetAngularSpeed(90.0f);
		class Tutorial* mTutorial = new Tutorial(this);
		StageChange* sc = new StageChange(this);
		sc->SetPosition(Vector3(0.0f, 1500.0f, -100.0f));
		// Create pause menu
		for (int i = 0; i < 11; ++i) {  // �؂�5�z�u�����
			float xPosition = -2200.0f + (i * 400.0f);  // Y���W��-1300����400�����₷
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3(xPosition, -1500.0f, -100.0f));  // X, Y, Z���W��ݒ�
		}
		for (int i = 0; i < 11; ++i) {  // �؂�5�z�u�����
			float xPosition = -2200.0f + (i * 400.0f);  // Y���W��-1300����400�����₷
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3( 1500.0f, xPosition, -100.0f));  // X, Y, Z���W��ݒ�
		}
		for (int i = 0; i < 11; ++i) {  // �؂�5�z�u�����
			float xPosition = -2200.0f + (i * 400.0f);  // Y���W��-1300����400�����₷
			TreeActor* tree = new TreeActor(this);
			tree->SetPosition(Vector3( -1500.0f, xPosition, -100.0f));  // X, Y, Z���W��ݒ�
		}
		gameOverFlag = false;
		gameClearFlag = false;
		timer->ResetTimer();
		timer->StartTimer();
	}

	else if (mGameState == GameState::EMainMenu) {
		
		LoadText("Assets/Text/Mainmenu.gptext");
		mHUD = new HUD(this);
		LevelLoader::LoadLevel(this, "Assets/Level/Stage.gplevel");
		LevelLoader::LoadLevel(this, "Assets/Level/Light.gplevel");
		new MainmenuUI(this);
		stageNumber = 1;
		this->GetRenderer()->GetTerrain()->SetTranslate(glm::vec3(15000.0f, 10000.0f, -100.0f));
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(SDL_ENABLE);
	}
}


//-----------------------------------------------------------------------------
//  �I������
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
//   �I������
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
//   Actor vector �ɒǉ�
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
//   Actor vector ����폜
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
//   UIStack vector �ɒǉ�
//-----------------------------------------------------------------------------
void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}


//-----------------------------------------------------------------------------
//  Font���t�@�C���p�X����擾
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
//  Text���t�@�C���p�X����擾
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
//  json�t�@�C������key�ƈ�v����text�̎擾
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
//  �X�P���g�����t�@�C���p�X����擾
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
//  �A�j���[�V�������t�@�C���p�X����擾
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
//   Plane vector �ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}


//-----------------------------------------------------------------------------
//   Plane vector����폜
//-----------------------------------------------------------------------------
void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}


//-----------------------------------------------------------------------------
//   Enemy vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddEnemy(EnemyActor* enemy)
{
	mEnemys.emplace_back(enemy);
}


//-----------------------------------------------------------------------------
//   Enemy vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveEnemy(EnemyActor* enemy)
{
	auto iter = std::find(mEnemys.begin(), mEnemys.end(), enemy);
	mEnemys.erase(iter);
}


//-----------------------------------------------------------------------------
//  DropItem vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddDropItem(DropItemActor* dropItem)
{
	mDropItems.emplace_back(dropItem);
}


//-----------------------------------------------------------------------------
//  DropItem vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveDropItem(DropItemActor* dropItem)
{
	auto iter = std::find(mDropItems.begin(), mDropItems.end(), dropItem);
	mDropItems.erase(iter);
}


//-----------------------------------------------------------------------------
//  Arrow vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddArrow(ArrowActor* arrow)
{
	mArrows.emplace_back(arrow);
}


//-----------------------------------------------------------------------------
//  Arrow vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveArrow(ArrowActor* arrow)
{
	auto iter = std::find(mArrows.begin(), mArrows.end(), arrow);
	mArrows.erase(iter);
}


//-----------------------------------------------------------------------------
//   Bomb vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddBomb(BombActor* bomb)
{
	mBombs.emplace_back(bomb);
}


//-----------------------------------------------------------------------------
//  Bomb vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveBomb(BombActor* bomb)
{
	auto iter = std::find(mBombs.begin(), mBombs.end(), bomb);
	mBombs.erase(iter);
}


//-----------------------------------------------------------------------------
//   Explosion vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddExplosion(ExplosionActor* explosion)
{
	mExplosions.emplace_back(explosion);
}


//-----------------------------------------------------------------------------
//  Explosion vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveExplosion(ExplosionActor* explosion)
{
	auto iter = std::find(mExplosions.begin(), mExplosions.end(), explosion);
	mExplosions.erase(iter);
}


//-----------------------------------------------------------------------------
//   Tree vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddTree(TreeActor* tree)
{
	mTrees.emplace_back(tree);
}


//-----------------------------------------------------------------------------
//  Tree vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveTree(TreeActor* tree)
{
	auto iter = std::find(mTrees.begin(), mTrees.end(), tree);
	mTrees.erase(iter);
}


//-----------------------------------------------------------------------------
//  StageChange vector�ɒǉ�
//-----------------------------------------------------------------------------
void Game::AddStageChange(StageChange* sc)
{
	mStageChanges.emplace_back(sc);
}


//-----------------------------------------------------------------------------
//  StageChange vector����폜
//-----------------------------------------------------------------------------
void Game::RemoveStageChange(StageChange* sc)
{
	auto iter = std::find(mStageChanges.begin(), mStageChanges.end(),
		sc);
	mStageChanges.erase(iter);
}


//-----------------------------------------------------------------------------
//  Stage�؂�ւ��邽�߂ɓǂݍ���
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
}

//-----------------------------------------------------------------------------
//  �f�[�^�t�@�C���̎擾
//-----------------------------------------------------------------------------
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

