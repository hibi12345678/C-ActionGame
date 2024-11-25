// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

class Skeleton
{
public:
	// Definition for each bone in the skeleton
	struct Bone
	{
		BoneTransform mLocalBindPose;
		std::string mName;
		int mParent;
	};

	// Load from a file
	bool Load(const std::string& fileName);

	// Getter functions
	size_t GetNumBones() const { return mBones.size(); }
	int GetBoneIndex(const std::string& boneName) const
	{
		for (size_t i = 0; i < mBones.size(); ++i)
		{
			if (mBones[i].mName == boneName)
			{
				return static_cast<int>(i); // インデックスを返す
			}
		}
		printf("A");
		return -1; // 一致するボーン名がない場合は -1 を返す
	}
	const Bone& GetBone(size_t idx) const { return mBones[idx]; }
	const std::vector<Bone>& GetBones() const { return mBones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }
	const std::string& GetFileName() const { return mFileName; }
protected:
	// Called automatically when the skeleton is loaded
	// Computes the global inverse bind pose for each bone
	void ComputeGlobalInvBindPose();
private:
	// The bones in the skeleton
	std::vector<Bone> mBones;
	// The global inverse bind poses for each bone
	std::vector<Matrix4> mGlobalInvBindPoses;
	std::string mFileName;
	
};
