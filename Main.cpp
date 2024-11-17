// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiLayer.h"
#include <SDL/SDL.h>
#include <iostream>
int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();
	if (success)
	{
		
		game.RunLoop();
	}
	game.Shutdown();
	return 0;

    
}
