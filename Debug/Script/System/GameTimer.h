#pragma once
#include <SDL.h>
#include <iostream>

class GameTimer {
public:
    void StartTimer() {
        mStartTicks = SDL_GetTicks();
        mRunning = true;
    }

    void StopTimer() {
        if (mRunning) {
            mElapsedTicks += SDL_GetTicks() - mStartTicks;
            mRunning = false;
        }
    }

    void ResetTimer() {
        mElapsedTicks = 0;
        mRunning = false;
    }

    int GetElapsedTime() const {
        if (mRunning) {
            return static_cast<int>((mElapsedTicks + SDL_GetTicks() - mStartTicks) / 1000.0);
        }
        return static_cast<int>(mElapsedTicks / 1000.0);
    }

private:
    Uint32 mStartTicks = 0; // タイマー開始時の時間
    Uint32 mElapsedTicks = 0; // 停止時の累積経過時間
    bool mRunning = false;
};