#pragma once
#include "UIScreen.h"
#include "Game.h"
#include "SoundEvent.h"
class Tutorial : public UIScreen
{
public:
	Tutorial(class Game* game);
	~Tutorial();

private:

	SoundEvent mMusicEvent;
	
	class UIActor* uiActor;
};
