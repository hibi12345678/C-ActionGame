#pragma once
#include "Component.h"

class UIMeshComponent : public Component
{
public:
	UIMeshComponent(class Actor* owner, bool isSkeletal = false);
	~UIMeshComponent();
	// Draw this mesh component
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }


	TypeID GetType() const override { return TMeshComponent; }

protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
	bool mIsSkeletal;
};