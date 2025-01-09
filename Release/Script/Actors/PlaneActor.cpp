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
#include "PlaneActor.h"
#include "BoxComponent.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"


///////////////////////////////////////////////////////////////////////////////
// PlaneActor class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	// Add collision box
	mBox = new BoxComponent(this);
	game->AddPlane(this);

}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
PlaneActor::~PlaneActor()
{
	GetGame()->RemovePlane(this);
}
