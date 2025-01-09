//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "UIMeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "LevelLoader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"


///////////////////////////////////////////////////////////////////////////////
// UIMeshComponent class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
UIMeshComponent::UIMeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
	, mIsSkeletal(isSkeletal)

{
	
	mOwner->GetGame()->GetRenderer()->AddUIMeshComp(this);
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
UIMeshComponent::~UIMeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveUIMeshComp(this);
}


//-----------------------------------------------------------------------------
//  �`�揈��
//-----------------------------------------------------------------------------
void UIMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		
		shader->SetMatrixUniform("uModelMatrix",
			mOwner->GetWorldTransform());

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
