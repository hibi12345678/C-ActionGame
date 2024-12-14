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
#pragma  once
#include <random>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//Random class
///////////////////////////////////////////////////////////////////////////////
class Random
{
public:
	//=========================================================================
    // public methods.
    //=========================================================================
	//èâä˙âª
	static void Init();

	//Getter,Setter
	static float GetFloat();
	static float GetFloatRange(float min, float max);
	static int GetIntRange(int min, int max);
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
	static Vector3 GetVector(const Vector3& min, const Vector3& max);

	static void Seed(unsigned int seed);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	static std::mt19937 sGenerator;
};