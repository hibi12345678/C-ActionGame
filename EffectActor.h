#pragma once

#include "Actor.h"
#include <string>

class EffectActor : public Actor
{
public:
    // �R���X�g���N�^
    EffectActor(Game* game, const std::string& meshPath, float lifeSpan, float scale);

    // Actor�̍X�V���\�b�h���I�[�o�[���C�h
    void UpdateActor(float deltaTime) override;

private:
    float mLifeSpan; // ����
};