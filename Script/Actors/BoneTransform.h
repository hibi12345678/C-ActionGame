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
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class BoneTransform
{
public:
	//=========================================================================
	// public variables.
	//=========================================================================
	Vector3 mTranslation;
	Quaternion mRotation;

	//=========================================================================
	// public methods.
	//=========================================================================
	Matrix4 ToMatrix() const;
	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};
