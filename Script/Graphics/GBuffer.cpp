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
#include "GBuffer.h"
#include <GL/glew.h>
#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////
// Gbuffer class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//----------------------------------------------------------------------------
GBuffer::GBuffer()
	:mBufferID(0)
{
	
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
GBuffer::~GBuffer()
{
	
}


//-----------------------------------------------------------------------------
//  Gbuffer�̐���
//-----------------------------------------------------------------------------
bool GBuffer::Create(int width, int height)
{
	//�t���[���o�b�t�@�I�u�W�F�N�g�̐���
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	
	//�[�x�o�b�t�@�̒ǉ�
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
						  width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, depthBuffer);
	
	//G�o�b�t�@���̊e�o�͂ɑ΂��ăe�N�X�`�����쐬����
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();
		tex->CreateForRendering(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);
		//�e�N�X�`�����J���[�o�͂Ɋ��蓖�Ă�
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
							 tex->GetTextureID(), 0);
	}
	
	//�J���[�A�^�b�`�����g�̃x�N�^�[�𐶐�����
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	
	// �`��ΏۂƂȂ�o�b�t�@�̃��X�g��ݒ肷��
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
				  attachments.data());
	
	//�`��ΏۂƂȂ�o�b�t�@�̃��X�g��ݒ肷��
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();
		return false;
	}
	
	return true;
}


//-----------------------------------------------------------------------------
//  Gbuffer�̍폜
//-----------------------------------------------------------------------------
void GBuffer::Destroy()
{
	glDeleteFramebuffers(1, &mBufferID);
	for (Texture* t : mTextures)
	{
		t->Unload();
		delete t;
	}
}


//-----------------------------------------------------------------------------
//  Texture�̎擾
//-----------------------------------------------------------------------------
Texture* GBuffer::GetTexture(Type type)
{
	if (mTextures.size() > 0)
	{
		return mTextures[type];
	}
	else
	{
		return nullptr;
	}
}


//-----------------------------------------------------------------------------
//  Texture���A�N�e�B�u�����A�o�C���h
//-----------------------------------------------------------------------------
void GBuffer::SetTexturesActive()
{
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		mTextures[i]->SetActive(i);
	}
}
