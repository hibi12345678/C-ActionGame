//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "EnemyActor.h"


///////////////////////////////////////////////////////////////////////////////
//BossActor class
///////////////////////////////////////////////////////////////////////////////
class BossActor : public EnemyActor
{
public:
    //=========================================================================
    // public methods.
    //=========================================================================
    //�R���X�g���N�^
	BossActor(class Game* game);

    //�f�X�g���N�^
    ~BossActor();

    //Update
	void UpdateActor(float deltaTime);
    void UpdateAttackState(float deltaTime) override;

    void HandleDeath() override;
    void FixCollisions() override;
    void AddAttackBox() override;

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    float headTime;
    class BossCamera* bossCam;
    class DefeatCamera* defCam;
    class BoxComponent* headBox;
};