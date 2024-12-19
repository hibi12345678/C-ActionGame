//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "ItemActorBase.h"


///////////////////////////////////////////////////////////////////////////////
// BombActorclass
///////////////////////////////////////////////////////////////////////////////
class BombActor : public ItemActorBase
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	BombActor(class Game* game, float scale, int num);

	//Upadate
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	void SetPlayer(Actor* player);

	void HitTarget();
	void FixCollisions();

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	float mLifeSpan; //�\������
	float blinkTime; //�_�Ŏ���
	float blinkInterval; //�_�ŊԊu

	class BoxComponent* mBoxComp;
	class AudioComponent* mAudioComp;
	class BombMove* mMyMove;
};