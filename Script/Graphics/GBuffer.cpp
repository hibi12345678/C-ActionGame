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
//      コンストラクタです.
//----------------------------------------------------------------------------
GBuffer::GBuffer()
	:mBufferID(0)
{
	
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
GBuffer::~GBuffer()
{
	
}


//-----------------------------------------------------------------------------
//  Gbufferの生成
//-----------------------------------------------------------------------------
bool GBuffer::Create(int width, int height)
{
	//フレームバッファオブジェクトの生成
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	
	//深度バッファの追加
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
						  width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, depthBuffer);
	
	//Gバッファ内の各出力に対してテクスチャを作成する
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();
		tex->CreateForRendering(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);
		//テクスチャをカラー出力に割り当てる
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
							 tex->GetTextureID(), 0);
	}
	
	//カラーアタッチメントのベクターを生成する
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	
	// 描画対象となるバッファのリストを設定する
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
				  attachments.data());
	
	//描画対象となるバッファのリストを設定する
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();
		return false;
	}
	
	return true;
}


//-----------------------------------------------------------------------------
//  Gbufferの削除
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
//  Textureの取得
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
//  Textureをアクティブ化し、バインド
//-----------------------------------------------------------------------------
void GBuffer::SetTexturesActive()
{
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		mTextures[i]->SetActive(i);
	}
}
