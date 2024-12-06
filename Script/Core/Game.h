// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"
#include <SDL/SDL_types.h>
#include "BossActor.h"
#include "PhysWorld.h"
class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }
	class HUD* GetHUD() { return mHUD; }
	
	// Manage UI stack
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* screen);
	
	enum class GameState {
		EMainMenu,
		EGameplay,
		EPaused,
		ETutorial,
		EItem,
		EQuit
	};

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }
	int GetScore() const { return scoreNumber; }
	void SetScore(int score) { scoreNumber = score; }
	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	class Skeleton* GetSkeleton(const std::string& fileName);
	class Animation* GetAnimation(const std::string& fileName);

	const std::vector<class Actor*>& GetActors() const { return mActors; }
	void SetFollowActor(class FollowActor* actor) { mFollowActor = actor; }
	void SetBossActor(class BossActor* actor) { mBossActor = actor; }

	class FollowActor* GetPlayer() { return mFollowActor; }
	class BossActor* GetBoss() { return mBossActor; }

	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	void AddEnemy(class EnemyActor* enemy);
	void RemoveEnemy(class EnemyActor* enemy);
	void AddDropItem(class DropItemActor* dropItem);
	void RemoveDropItem(class DropItemActor* dropItem);
	void AddArrow(class  ArrowActor* arrow);
	void RemoveArrow(class ArrowActor* arrow);
	void AddBomb(class BombActor* bomb);
	void RemoveBomb(class BombActor* bomb);
	void AddExplosion(class ExplosionActor* explosion);
	void RemoveExplosion(class ExplosionActor* explosion);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
	std::vector<class EnemyActor*>& GetEnemys() { return mEnemys; }
	std::vector<class DropItemActor*>& GetDropItem() { return mDropItems; }
	std::vector<class ArrowActor*>& GetArrow() { return mArrows; }
	std::vector<class BombActor*>& GetBomb() { return mBombs; }
	std::vector<class ExplosionActor*>& GetExplosion() { return mExplosions; }
	
	void GetData();
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class UIScreen*> mUIStack;
	// Map for fonts
	std::unordered_map<std::string, class Font*> mFonts;
	// Map of loaded skeletons
	std::unordered_map<std::string, class Skeleton*> mSkeletons;
	// Map of loaded animations
	std::unordered_map<std::string, class Animation*> mAnims;

	// Map for text localization
	std::unordered_map<std::string, std::string> mText;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	Uint32 mTicksCount;
	GameState mGameState;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	// Game-specific code
	std::vector<class PlaneActor*> mPlanes;
	std::vector<class EnemyActor*> mEnemys;
	std::vector<class DropItemActor*> mDropItems;
	std::vector<class ArrowActor*> mArrows;
	std::vector<class BombActor*> mBombs;
	std::vector<class ExplosionActor*> mExplosions;
	class FollowActor* mFollowActor;
	class BossActor* mBossActor;

	class SpriteComponent* mCrosshair;
	
	bool mainFlag;
	bool playFlag;
	bool gameOverFlag;
	bool gameClearFlag;
	int scoreNumber;
	float mBossTime;
};
