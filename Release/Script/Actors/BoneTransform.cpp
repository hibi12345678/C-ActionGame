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
#include "BoneTransform.h"

///////////////////////////////////////////////////////////////////////////////
// BoneTransform class
///////////////////////////////////////////////////////////////////////////////
 

//-----------------------------------------------------------------------------
//  位置と回転を行列に変換
//-----------------------------------------------------------------------------
Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 rot = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mTranslation);

	return rot * trans;
}


//-----------------------------------------------------------------------------
//  ボーンの補完
//-----------------------------------------------------------------------------
BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
	retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);
	return retVal;
}
