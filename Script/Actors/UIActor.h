#pragma once

#include "Actor.h"
#include <string>

class UIActor : public Actor
{
public:
    // �R���X�g���N�^
    UIActor(Game* game);

    // Actor�̍X�V���\�b�h���I�[�o�[���C�h
    void UpdateActor(float deltaTime) override;

private:

};