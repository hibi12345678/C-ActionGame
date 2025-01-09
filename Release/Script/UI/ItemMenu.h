//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include "SoundEvent.h"
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//ItemMenu class
///////////////////////////////////////////////////////////////////////////////
class ItemMenu : public UIScreen
{
public:
	//Enum ItemState
	enum class ItemState
	{
		ESword,
		ETorch,
		EBow,
		EBomb
	};

	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	explicit ItemMenu(class Game* game);

	//�f�X�g���N�^
	~ItemMenu();

	//����
	void HandleKeyPress(int key);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	ItemState menuState; //�A�C�e���X�e�[�g

	SoundEvent mMusicEvent; //�X�e�[�g��MainMenu��GamePaly��
	class UIActor* uiActor;
};
