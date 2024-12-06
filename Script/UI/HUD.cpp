// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FollowActor.h"
#include "BossActor.h"
#include <algorithm>
#include "GBuffer.h"
#include "TargetComponent.h"
#include <iostream>
#include "Font.h"
#include "FollowActor.h"

HUD::HUD(Game* game)
	:UIScreen(game)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
	, mGame(game)
	, itemNum(0)

	
{
	Renderer* r = mGame->GetRenderer();
	mRadar = r->GetTexture("Assets/Texture/Radar.png");
	mCrosshair = r->GetTexture("Assets/Texture/CrosshairOrig2.png");
	mBlipTex = r->GetTexture("Assets/Texture/Blip.png");
	mRadarArrow = r->GetTexture("Assets/Texture/RadarArrow.png");
	mTitle = r->GetTexture("Assets/Texture/Title.png");
	mStaminaBar= r->GetTexture("Assets/Texture/Stamina.png");
	mStaminaFrame = r->GetTexture("Assets/Texture/StaminaFrame.png");
	mHealthBar= r->GetTexture("Assets/Texture/Health.png");
	mHealthRedBar = r->GetTexture("Assets/Texture/HealthRed.png");
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mHighlight = mGame->GetRenderer()->GetTexture("Assets/Texture/ItemButtonOrange.png");
	mSword = mGame->GetRenderer()->GetTexture("Assets/Texture/SwordTex.png");
	mTorch = mGame->GetRenderer()->GetTexture("Assets/Texture/TorchTex.png");
	mBow = mGame->GetRenderer()->GetTexture("Assets/Texture/BowTex.png");
	mBomb = mGame->GetRenderer()->GetTexture("Assets/Texture/BombTex.png");
	mSword2 = mGame->GetRenderer()->GetTexture("Assets/Texture/SwordTex2.png");
	mTorch2 = mGame->GetRenderer()->GetTexture("Assets/Texture/TorchTex2.png");
	mBow2 = mGame->GetRenderer()->GetTexture("Assets/Texture/BowTex2.png");
	mBomb2 = mGame->GetRenderer()->GetTexture("Assets/Texture/BombTex2.png");
	mCross = mGame->GetRenderer()->GetTexture("Assets/Texture/~.png");
	mFrame = mGame->GetRenderer()->GetTexture("Assets/Texture/Frame.png");
	mFrame2 = mGame->GetRenderer()->GetTexture("Assets/Texture/Frame2.png");
	mTutorial = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader)
{
	
	
	if (Game::GameState::EGameplay == mGame->GetState() || Game::GameState::EItem == mGame->GetState()) {

		// Radar
		const Vector2 cRadarPos(-390.0f, 275.0f);
		DrawTexture(shader, mRadar, cRadarPos, 1.0f);
		// Blips
		for (Vector2& blip : mBlips)
		{
			DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
		}
		// Radar arrow
		DrawTexture(shader, mRadarArrow, cRadarPos);

	
		// Šù‘¶‚Ì FollowActor ‚ðŽæ“¾
		FollowActor* followActor = mGame->GetPlayer();;
		if (followActor != nullptr) {
			float stamina = followActor->GetStamina();

		    DrawTexture(shader, mStaminaBar, Vector2(-278.0f, 320.0f),  stamina * 0.99 , false, 1);
			DrawTexture(shader, mStaminaFrame, Vector2(-280.0f, 320.0f), 1, false, 1);
			float health = followActor->GetHealth();
			float healthRed = followActor->GetHealthRed();
			DrawTexture(shader, mHealthRedBar, Vector2(-273.0f, 345.0f), healthRed * 0.99, true, 1);
			DrawTexture(shader, mHealthBar, Vector2(-273.0f, 345.0f), health * 0.99, true, 1);	
			DrawTexture(shader, mStaminaFrame, Vector2(-275.0f, 345.0f), 1, true, 1);
			DrawTexture(shader, mRadar, Vector2(420.0f, -320.0f), 0.65f);
			if (mGame->GetPlayer()->GetItemState() == FollowActor::ItemState::ESword) {
				DrawTexture(shader, mSword2, Vector2(420.0f, -325.0f), 1.0f);
			}

			else if (mGame->GetPlayer()->GetItemState() == FollowActor::ItemState::ETorch) {
				DrawTexture(shader, mTorch2, Vector2(420.0f, -320.0f), 1.0f);
			}
			else if (mGame->GetPlayer()->GetItemState() == FollowActor::ItemState::EBow) {

				DrawTexture(shader, mBow2, Vector2(415.0f, -315.0f), 1.0f);
				Texture* cross = mCrosshair;
				DrawTexture(shader, cross, Vector2::Zero, 0.5f);
				DrawTexture(shader, mCross, Vector2(440.0f, -350.0f), 1.0f);
				int arrow = followActor->GetArrowNum();
				std::string str = std::to_string(arrow);
				Texture* tex = new Texture();
				tex = mFont->RenderText(str, Color::White, 24, 1);
				DrawTexture(shader, tex, Vector2(460.0f, -352.0f));
			}
			else if (mGame->GetPlayer()->GetItemState() == FollowActor::ItemState::EBomb) {
				DrawTexture(shader, mBomb2, Vector2(412.0f, -315.0f), 1.0f);
				DrawTexture(shader, mCross, Vector2(440.0f, -350.0f), 1.0f);
				int bomb = followActor->GetBombNum();
				std::string str = std::to_string(bomb);
				Texture* tex = new Texture();
				tex = mFont->RenderText(str, Color::White, 24,1);
				DrawTexture(shader, tex, Vector2(460.0f, -352.0f));
			}
			
		}
		else {
			
		}	

		Texture* tex = new Texture();
		int mNum = mGame->GetScore();
		std::string str = std::to_string(mNum);
		tex = mFont->RenderText(str, Color::White, 24,1);
		DrawTexture(shader, tex, Vector2(385.0, 350.0f));
		Texture* scoretex = new Texture();
		scoretex = mFont->RenderText("Score", Color::White, 24);
		DrawTexture(shader, scoretex, Vector2(300.0, 350.0f));

		if (mGame->GetScore() == 4) {


			Texture* bosstex = new Texture();
			bosstex = mFont->RenderText("BossName", Color::White, 18);
			DrawTexture(shader, bosstex, Vector2(-200.0, -270.0f));
			BossActor* BossActor = mGame->GetBoss();;
			//// Health bar
			float health = BossActor->GetHealth();
			DrawTexture(shader, mHealthBar, Vector2(-250.0f, -300.0f), health * 1.5, false, 1);
			DrawTexture(shader, mStaminaFrame, Vector2(-250.0f, -300.0f), 1.5, false, 1);
		}
		
	}
	if (Game::GameState::EItem == mGame->GetState()) {
		
		
		DrawTexture(shader, mFrame, Vector2(0.0f, -160.0f), 0.7f);
		DrawTexture(shader, mFrame2, Vector2(0.0f, 120.0f), 0.7f);
		DrawTexture(shader, mHighlight, Vector2(-270.0f, -160.0f), 0.7f);
		if (itemNum == 0) {
			DrawTexture(shader, mSword, Vector2(-270.0f, -160.0f), 0.7f);		
			
		}
		else if (itemNum == 1) {
			DrawTexture(shader, mTorch, Vector2(-270.0f, -160.0f), 0.7f);
			
		}
		else if (itemNum == 2) {
			DrawTexture(shader, mBow, Vector2(-270.0f, -160.0f), 0.7f);
			
			
		}
		else if (itemNum == 3) {
			DrawTexture(shader, mBomb, Vector2(-270.0f, -160.0f), 0.7f);
			
		}
	}
	if (Game::GameState::ETutorial == mGame->GetState()) {
		DrawTexture(shader, mTutorial, Vector2(0.0f, 0.0f), 0.5f);
	}
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);

}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}


void HUD::UpdateRadar(float deltaTime)
{
	
	if (Game::GameState::EGameplay == mGame->GetState()) {
		// Clear blip positions from last frame
		mBlips.clear();

		// Convert player position to radar coordinates (x forward, z up)
		Vector3 playerPos = mGame->GetPlayer()->GetPosition();
		Vector2 playerPos2D(playerPos.y, playerPos.x);
		// Ditto for player forward
		Vector3 playerForward = mGame->GetPlayer()->GetForward();
		Vector2 playerForward2D(playerForward.x, playerForward.y);

		// Use atan2 to get rotation of radar
		float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
		// Make a 2D rotation matrix
		Matrix3 rotMat = Matrix3::CreateRotation(angle);
		
		// Get positions of blips
		for (auto tc : mTargetComps)
		{
			
			Vector3 targetPos = tc->GetOwner()->GetPosition();
			Vector2 actorPos2D(targetPos.y, targetPos.x);

			// Calculate vector between player and target
			Vector2 playerToTarget = actorPos2D - playerPos2D;

			// See if within range
			if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
			{
				// Convert playerToTarget into an offset from
				// the center of the on-screen radar
				Vector2 blipPos = playerToTarget;
				blipPos *= mRadarRadius / mRadarRange;

				// Rotate blipPos
				blipPos = Vector2::Transform(blipPos, rotMat);
				mBlips.emplace_back(blipPos);
			}
		}
	}

}
