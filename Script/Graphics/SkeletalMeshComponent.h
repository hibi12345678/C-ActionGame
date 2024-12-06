// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include <string>
#include <vector>
#include "Maths.h"
class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// Draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(class Animation* anim, float playRate = 1.0f);
	float GetAnimTime() { return mAnimTime; }
	TypeID GetType() const override { return TSkeletalMeshComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	// �{�[�������炻�̃{�[���̌��݂̈ʒu���擾����֐�
	Vector3 GetBonePosition(const std::string& boneName) const;
	// �{�[�������炻�̃{�[���̌��݂̉�]���擾����֐�
	Quaternion GetBoneRotation(const std::string& boneName) const;
protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;
	class Skeleton* mSkeleton;
	class Animation* mAnimation;
	float mAnimPlayRate;
	float mAnimTime;

private:
	std::vector<Matrix4> mCurrentPoses; 
};
