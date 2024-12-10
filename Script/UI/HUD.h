// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class HUD : public UIScreen
{
public:
	//Enum State
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EItem,
		EQuit
	};

	//=========================================================================
	// public variables.
	//=========================================================================
	State currentState;

	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	HUD(class Game* game);

    //デストラクタ
	~HUD();

	//Update
	void Update(float deltaTime) override;


	//Getter,Setter
	int GetItemNum() const { return itemNum; }
	void SetItemNum(int num) { itemNum = num; }
	
	//Add,Remove
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);

	void Draw(class Shader* shader) override;


	
protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mTargetEnemy; //ターゲット
	int itemNum; //装備中のアイテム選択用変数
	float mRadarRange; //レーダー範囲
	float mRadarRadius; //レーダー半径

	class Game* mGame; 
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

	//=========================================================================
	// protected methods.
	//=========================================================================
	void UpdateRadar(float deltaTime);

};
