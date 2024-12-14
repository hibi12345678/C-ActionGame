
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
#include <string>
#include "BoneTransform.h"


///////////////////////////////////////////////////////////////////////////////
//Animation class
///////////////////////////////////////////////////////////////////////////////
class Animation
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================

	//初期化
	bool Load(const std::string& fileName);

	//Getter,Setter
	size_t GetNumBones() const { return mNumBones; }
	size_t GetNumFrames() const { return mNumFrames; }
	float GetDuration() const { return mDuration; }
	float GetFrameDuration() const { return mFrameDuration; }
	const std::string& GetFileName() const { return mFileName; }
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	float mDuration; //アニメーション時間
	float mFrameDuration; //アニメーションの次の座標までのフレーム

	size_t mNumBones; //アニメーション用ボーン
	size_t mNumFrames; //アニメーションフレーム数

	std::vector<std::vector<BoneTransform>> mTracks;
	std::string mFileName;
};
