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
#include "UIScreen.h"


///////////////////////////////////////////////////////////////////////////////
//DialogBox class
///////////////////////////////////////////////////////////////////////////////
class DialogBox : public UIScreen
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	DialogBox(class Game* game, const std::string& text,
		std::function<void()> onOK);
	//デストラクタ
	~DialogBox();
};
