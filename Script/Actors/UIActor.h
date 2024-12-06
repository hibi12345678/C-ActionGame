#pragma once

#include "Actor.h"
#include <string>

class UIActor : public Actor
{
public:
    // コンストラクタ
    UIActor(Game* game);

    // Actorの更新メソッドをオーバーライド
    void UpdateActor(float deltaTime) override;

private:

};