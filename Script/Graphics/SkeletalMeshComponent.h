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
#include <string>
#include <vector>
#include "Maths.h"
#include "MeshComponent.h"
#include "MatrixPalette.h"


///////////////////////////////////////////////////////////////////////////////
//SkeletalMeshComponent class
///////////////////////////////////////////////////////////////////////////////
class SkeletalMeshComponent : public MeshComponent
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	SkeletalMeshComponent(class Actor* owner);

	//Update
	void Update(float deltaTime) override;

	// Getter,Setter
	Vector3 GetBonePosition(const std::string& boneName) const;
	Quaternion GetBoneRotation(const std::string& boneName) const;
	float GetAnimTime() { return mAnimTime; }
	TypeID GetType() const override { return TSkeletalMeshComponent; }
	void SetSkeleton(class Skeleton* sk) { mSkeleton = sk; }

	//Load,Save
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	void Draw(class Shader* shader) override;
	float PlayAnimation(class Animation* anim, float playRate = 1.0f);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	MatrixPalette mPalette;
	class Skeleton* mSkeleton;
	class Animation* mAnimation;
	float mAnimPlayRate; //アニメーション再生倍率
	float mAnimTime; //アニメーション時間

	//=========================================================================
	// protected variables.
	//=========================================================================
	void ComputeMatrixPalette();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	std::vector<Matrix4> mCurrentPoses; //アニメーション中のボーンの各ローカル座標
};
