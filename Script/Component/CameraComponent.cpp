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
#include "CameraComponent.h"
#include "Actor.h"
#include "AudioSystem.h"
#include "Game.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// CameraComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{
}


///////////////////////////////////////////////////////////////////////////////
// View座標をRendererに送る
///////////////////////////////////////////////////////////////////////////////
void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	// Pass view matrix to renderer and audio system
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
	
}

