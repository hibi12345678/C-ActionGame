// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EItem,
		EQuit
	};
	void SetItemNum(int num) { itemNum = num; }
	int GetItemNum() const{return itemNum;}
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
	State currentState;
	
protected:
	void UpdateRadar(float deltaTime);
	class Game* mGame; // ゲームインスタンスのポインタを保持
	class Font* mFont;
	class Texture* mHealthBar;
	class Texture* mHealthRedBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;
	class Texture* mTitle;
	class Texture* mStaminaBar;
	class Texture* mStaminaFrame;
	class Texture* mHighlight;
	class Texture* mSword;
	class Texture* mBow;
	class Texture* mBomb;
	class Texture* mBow2;
	class Texture* mBomb2;
	class Texture* mSword2;
	class Texture* mTorch2;
	class Texture* mTorch;
	class Texture* mCross;
	class Texture* mFrame;
	class Texture* mFrame2;
	
	std::vector<class TargetComponent*> mTargetComps;
	std::vector<Vector2> mBlips;

	float mRadarRange;
	float mRadarRadius;

	bool mTargetEnemy;
	int itemNum;
};
