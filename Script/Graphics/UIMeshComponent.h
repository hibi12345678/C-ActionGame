//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Component.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class UIMeshComponent : public Component
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	UIMeshComponent(class Actor* owner, bool isSkeletal = false);

	//�f�X�g���N�^
	~UIMeshComponent();

	//Getter,Setter
	TypeID GetType() const override { return TMeshComponent; }
	bool GetVisible() const { return mVisible; }
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
	void SetVisible(bool visible) { mVisible = visible; }

	virtual void Draw(class Shader* shader);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mVisible; //�\������
	bool mIsSkeletal; //�X�P���g���������Ă��邩

	size_t mTextureIndex;

	class Mesh* mMesh;
};