//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"


///////////////////////////////////////////////////////////////////////////////
//DropItemActor class
///////////////////////////////////////////////////////////////////////////////
class DropItemActor : public Actor
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	DropItemActor(class Game* game);

	//�f�X�g���N�^
	~DropItemActor();

	//Update
	void UpdateActor(float deltaTime) override;

	//Getter,Setter
	class BoxComponent* GetBox() { return mBox; }
	int GetItemNum() { return itemValue; }

	void FixCollisions();

private:
	//=========================================================================
    // private variables.
    //=========================================================================
	bool isVisible;
	int randomValue;
	int itemValue; 
	float mLifeSpan; //�\������
	float blinkTime; //�_�Ŏ���
	float blinkInterval; //�_�ŊԊu
	
	class MeshComponent* mc;
	class BoxComponent* mBox;
};
