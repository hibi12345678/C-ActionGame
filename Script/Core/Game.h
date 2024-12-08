// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL/SDL_types.h> 
#include <unordered_map>
#include <string>
#include <vector>

#include "PhysWorld.h"     
#include "SoundEvent.h"     
#include "Math.h"           
class Game
{
public:

    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

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


    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    void PushUI(class UIScreen* screen);

    class FollowActor* GetPlayer() { return mFollowActor; }
    class BossActor* GetBoss() { return mBossActor; }
    void SetFollowActor(class FollowActor* actor) { mFollowActor = actor; }
    void SetBossActor(class BossActor* actor) { mBossActor = actor; }

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    const std::vector<class Actor*>& GetActors() const { return mActors; }

    void AddPlane(class PlaneActor* plane);
    void RemovePlane(class PlaneActor* plane);
    void AddEnemy(class EnemyActor* enemy);
    void RemoveEnemy(class EnemyActor* enemy);
    void AddDropItem(class DropItemActor* dropItem);
    void RemoveDropItem(class DropItemActor* dropItem);
    void AddArrow(class ArrowActor* arrow);
    void RemoveArrow(class ArrowActor* arrow);
    void AddBomb(class BombActor* bomb);
    void RemoveBomb(class BombActor* bomb);
    void AddExplosion(class ExplosionActor* explosion);
    void RemoveExplosion(class ExplosionActor* explosion);
    void AddTree(class TreeActor* tree);
    void RemoveTree(class TreeActor* tree);

    std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
    std::vector<class EnemyActor*>& GetEnemys() { return mEnemys; }
    std::vector<class DropItemActor*>& GetDropItem() { return mDropItems; }
    std::vector<class ArrowActor*>& GetArrow() { return mArrows; }
    std::vector<class BombActor*>& GetBomb() { return mBombs; }
    std::vector<class ExplosionActor*>& GetExplosion() { return mExplosions; }
    std::vector<class TreeActor*>& GetTree() { return mTrees; }

    class Font* GetFont(const std::string& fileName);
    class Skeleton* GetSkeleton(const std::string& fileName);
    class Animation* GetAnimation(const std::string& fileName);
    void LoadText(const std::string& fileName);
    const std::string& GetText(const std::string& key);

    class Renderer* GetRenderer() { return mRenderer; }
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    class PhysWorld* GetPhysWorld() { return mPhysWorld; }
    class HUD* GetHUD() { return mHUD; }

    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();
    void GetData();

private:

    std::unordered_map<std::string, class Font*> mFonts;
    std::unordered_map<std::string, class Skeleton*> mSkeletons;
    std::unordered_map<std::string, class Animation*> mAnims;
    std::unordered_map<std::string, std::string> mText;

    class Renderer* mRenderer;
    class AudioSystem* mAudioSystem;
    class PhysWorld* mPhysWorld;
    class HUD* mHUD;

    class FollowActor* mFollowActor;
    class BossActor* mBossActor;
    class SpriteComponent* mCrosshair;

    Uint32 mTicksCount;
    GameState mGameState;
    SoundEvent mMusicEvent;

    std::vector<class Actor*> mActors;
    std::vector<class UIScreen*> mUIStack;
    std::vector<class PlaneActor*> mPlanes;
    std::vector<class EnemyActor*> mEnemys;
    std::vector<class DropItemActor*> mDropItems;
    std::vector<class ArrowActor*> mArrows;
    std::vector<class BombActor*> mBombs;
    std::vector<class ExplosionActor*> mExplosions;
    std::vector<class TreeActor*> mTrees;
    std::vector<class Actor*> mPendingActors;

    int scoreNumber;
    float mBossTime;
    bool mainFlag;
    bool playFlag;
    bool gameOverFlag;
    bool gameClearFlag;
    bool mUpdatingActors;
};
