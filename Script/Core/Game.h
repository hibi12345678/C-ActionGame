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
#pragma once
#include <SDL_types.h> 
#include <string>
#include <unordered_map>
#include <vector>
#include "Math.h"       
#include "PhysWorld.h"     
#include "SoundEvent.h"     


///////////////////////////////////////////////////////////////////////////////
//Game class
///////////////////////////////////////////////////////////////////////////////
class Game
{
public:
    //Enum Clus GameState
    enum class GameState {
        EMainMenu,
        EGameplay,
        EPaused,
        ETutorial,
        EItem,
        ELoadStage,
        EBossMovie,
        EBossDefeat,
        EGameOver,
        EGameClear,
        EQuit
    };

    //=========================================================================
    // public methods.
    //=========================================================================
    //コンストラクタ
    Game();

    //初期化
    bool Initialize();

    //実行
    void RunLoop();

    //終了
    void Shutdown();

    //Update
    void UpdateGame();

    //入力
    void ProcessInput();
    void HandleKeyPress(int key);

    //初期化
    void LoadText(const std::string& fileName);
    void LoadData();

    //終了処理
    void UnloadData();

    //Getter,Setter
    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    GameState GetState() const { return mGameState; }
    int GetScore() const { return scoreNumber; }
    class FollowActor* GetPlayer() { return mFollowActor; }
    std::vector<class BossActor*>& GetBoss() { return mBossActor; }
    std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
    std::vector<class EnemyActor*>& GetEnemys() { return mEnemys; }
    std::vector<class DropItemActor*>& GetDropItem() { return mDropItems; }
    std::vector<class ArrowActor*>& GetArrow() { return mArrows; }
    std::vector<class BombActor*>& GetBomb() { return mBombs; }
    std::vector<class ExplosionActor*>& GetExplosion() { return mExplosions; }
    std::vector<class TreeActor*>& GetTree() { return mTrees; }
    std::vector<class StageChange*>& GetStageChange() { return mStageChanges; }
    class Font* GetFont(const std::string& fileName);
    class Skeleton* GetSkeleton(const std::string& fileName);
    class Animation* GetAnimation(const std::string& fileName);
    const std::string& GetText(const std::string& key);
    class Renderer* GetRenderer() { return mRenderer; }
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    class PhysWorld* GetPhysWorld() { return mPhysWorld; }
    class HUD* GetHUD() { return mHUD; }
    int GetStageNumber(){ return stageNumber; }
    class GameTimer* GetTimer() { return timer; }
    void SetState(GameState state) { mGameState = state; }
    void SetScore(int score) { scoreNumber = score; }
    void SetFollowActor(class FollowActor* actor) { mFollowActor = actor; }
    void SetStageNumber(int num) { stageNumber = num; }
    void SetHitStopTimer() { mHitStopTimer = 0.3f; }

    //Add,Remove
    void PushUI(class UIScreen* screen);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    const std::vector<class Actor*>& GetActors() const { return mActors; }
    void AddPlane(class PlaneActor* plane);
    void RemovePlane(class PlaneActor* plane);
    void AddEnemy(class EnemyActor* enemy);
    void RemoveEnemy(class EnemyActor* enemy);
    void AddBossActor(class BossActor* boss);
    void RemoveBossActor(class BossActor* boss);
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
    void AddStageChange(class StageChange* sc);
    void RemoveStageChange(class StageChange* sc);

    void GenerateOutput();
    void LoadStage();
    void GetData();

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    bool mainFlag; //MainMenu判定
    bool playFlag; //GamePlay判定
    bool gameOverFlag; //ゲームオーバー判定
    bool gameClearFlag; //ゲームクリア判定
    bool mUpdatingActors; 
    int scoreNumber; //スコア
    int stageNumber; //ステージナンバー
    float mBossTime; //ボスが出現するまでの時間
    float mHitStopTimer;
    Uint32 mTicksCount;

    GameState mGameState; //ゲームの状態

    SoundEvent mMusicEvent;
    class Renderer* mRenderer;
    class AudioSystem* mAudioSystem;
    class PhysWorld* mPhysWorld;
    class HUD* mHUD;
    class FollowActor* mFollowActor;
    class SpriteComponent* mCrosshair;
    class GameTimer* timer;
    class StageChange* sc;
    std::vector<class Actor*> mActors;
    std::vector<class UIScreen*> mUIStack;
    std::vector<class PlaneActor*> mPlanes;
    std::vector<class EnemyActor*> mEnemys;
    std::vector<class BossActor*> mBossActor;
    std::vector<class DropItemActor*> mDropItems;
    std::vector<class ArrowActor*> mArrows;
    std::vector<class BombActor*> mBombs;
    std::vector<class ExplosionActor*> mExplosions;
    std::vector<class TreeActor*> mTrees;
    std::vector<class Actor*> mPendingActors;
    std::vector <class StageChange* > mStageChanges;
    std::unordered_map<std::string, class Font*> mFonts;
    std::unordered_map<std::string, class Skeleton*> mSkeletons;
    std::unordered_map<std::string, class Animation*> mAnims;
    std::unordered_map<std::string, std::string> mText;
};
