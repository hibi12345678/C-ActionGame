#include "Tutorial.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "UIScreen.h"
#include "HUD.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "FollowActor.h"
#include "TorchItemActor.h"
#include "SwordActor.h"
#include "BombActor.h"
#include "BowActor.h"
#include "UIActor.h"
Tutorial::Tutorial(Game* game)
	:UIScreen(game)

{
	mGame->SetState(Game::GameState::ETutorial);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	//SetTitle("Item");
	HUD* hudInstance = mGame->GetHUD();
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.0f, 0.0f, 0.0f));
	DrawButtonRight("A", [this]() {
		AddTutorialNum();
		});
	DrawButtonLeft("A", [this]() {
		RemoveTutorialNum();

		});
	DrawCloseButton("A",[this]() {
		CloseTutorial();
		});
}
Tutorial::~Tutorial()
{
	mGame->SetState(Game::GameState::EGameplay);
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);


}

