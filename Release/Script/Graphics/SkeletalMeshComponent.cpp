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
#include "SkeletalMeshComponent.h"
#include "Actor.h"
#include "Animation.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Maths.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skeleton.h"
#include "Texture.h"
#include "VertexArray.h"


///////////////////////////////////////////////////////////////////////////////
// SkeltalMeshComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	,mSkeleton(nullptr)
{
}


//-----------------------------------------------------------------------------
//  �A�j���[�V����������Actor�����V�F�[�_�[�ɑ���
//-----------------------------------------------------------------------------
void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], 
			MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}


//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		// Wrap around anim time if past duration
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// Recompute matrix palette
		ComputeMatrixPalette();
	}
}


//-----------------------------------------------------------------------------
// �A�j���[�V�����̍Đ�
//-----------------------------------------------------------------------------
float SkeletalMeshComponent::PlayAnimation(Animation* anim, float playRate)
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}


//-----------------------------------------------------------------------------
// json�t�@�C������f�[�^�̓ǂݎ��
//-----------------------------------------------------------------------------
void SkeletalMeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	MeshComponent::LoadProperties(inObj);

	std::string skelFile;
	if (JsonHelper::GetString(inObj, "skelFile", skelFile))
	{
		SetSkeleton(mOwner->GetGame()->GetSkeleton(skelFile));
	}

	std::string animFile;
	if (JsonHelper::GetString(inObj, "animFile", animFile))
	{
		PlayAnimation(mOwner->GetGame()->GetAnimation(animFile));
	}

	JsonHelper::GetFloat(inObj, "animPlayRate", mAnimPlayRate);
	JsonHelper::GetFloat(inObj, "animTime", mAnimTime);
}


//-----------------------------------------------------------------------------
// json�t�@�C���ւ̏�������
//-----------------------------------------------------------------------------
void SkeletalMeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	MeshComponent::SaveProperties(alloc, inObj);

	if (mSkeleton)
	{
		JsonHelper::AddString(alloc, inObj, "skelFile", mSkeleton->GetFileName());
	}

	if (mAnimation)
	{
		JsonHelper::AddString(alloc, inObj, "animFile", mAnimation->GetFileName());
	}

	JsonHelper::AddFloat(alloc, inObj, "animPlayRate", mAnimPlayRate);
	JsonHelper::AddFloat(alloc, inObj, "animTime", mAnimTime);
}


//-----------------------------------------------------------------------------
// �A�j���[�V�����f�[�^����ɁA�e�{�[���̕ϊ��s����v�Z���A�}�g���b�N�X�p���b�g���\�z����
//-----------------------------------------------------------------------------
void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
	
	mAnimation->GetGlobalPoseAtTime(mCurrentPoses, mSkeleton, mAnimTime);

	// Setup the palette for each bone
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix
		mPalette.mEntry[i] = globalInvBindPoses[i] * mCurrentPoses[i];
	}
}


//-----------------------------------------------------------------------------
// �w�肵�����O�̃{�[���̃��[�J���ʒu�����擾
//-----------------------------------------------------------------------------
Vector3 SkeletalMeshComponent::GetBonePosition(const std::string& boneName) const
{
	int boneIndex = mSkeleton->GetBoneIndex(boneName);
	if (boneIndex < 0 || boneIndex >= mCurrentPoses.size())
	{
		
		return Vector3::Zero; // �{�[����������Ȃ��ꍇ�̓[���x�N�g����Ԃ�
	}

	// �[���x�N�g���Ɍ��݂̃|�[�Y�s��������ăI�u�W�F�N�g��Ԃ̈ʒu���擾
	return mCurrentPoses[boneIndex].TransformPoint(Vector3::Zero);

}


//-----------------------------------------------------------------------------
// �w�肵�����O�̃{�[���̃��[�J����]�����擾
//-----------------------------------------------------------------------------
Quaternion SkeletalMeshComponent::GetBoneRotation(const std::string& boneName) const
{
	int boneIndex = mSkeleton->GetBoneIndex(boneName);
	if (boneIndex < 0 || boneIndex >= mCurrentPoses.size())
	{

		return Quaternion::Identity; // �{�[����������Ȃ��ꍇ�̓[���x�N�g����Ԃ�
	}

	// �[���x�N�g���Ɍ��݂̃|�[�Y�s��������ăI�u�W�F�N�g��Ԃ̈ʒu���擾
	mCurrentPoses[boneIndex].TransformPoint(Vector3::Zero);

	// �s��̉�]�������N�H�[�^�j�I���ɕϊ�
	Quaternion rot = Quaternion::FromMatrix(mCurrentPoses[boneIndex]);
	return rot;
}