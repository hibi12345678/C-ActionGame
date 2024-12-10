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
#include <glm.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecColor;
};

class Renderer
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	Renderer(class Game* game);
	//�f�X�g���N�^
	~Renderer();

	//������
	bool Initialize(float screenWidth, float screenHeight);
	void InitializeImGui(SDL_Window* window, SDL_GLContext context);

	//�I������
	void Shutdown();
	void UnloadData();

	//Add,Remove
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);
	void AddUIMeshComp(class UIMeshComponent* mesh);
	void RemoveUIMeshComp(class UIMeshComponent* mesh);
	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(class PointLightComponent* light);

	//Getter,Setter
	DirectionalLight& GetDirectionalLight() { return mDirLight; }
	const Vector3& GetAmbientLight() const { return mAmbientLight; }
	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
	class GBuffer* GetGBuffer() { return mGBuffer;}
	const std::vector<class PointLightComponent*>& GetPointLights() const { return mPointLights; }
	void SetViewMatrix(const Matrix4& view) { mView = view; }
	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }

	Vector3 Unproject(const Vector3& screenPoint) const;
	void Draw();
	glm::mat4 Renderer::ConvertToGLM(const Matrix4& matrix);
	glm::vec3 ToGlmVec3(const Vector3& vec) {
		return glm::vec3(vec.x, vec.y, vec.z);
	}

private:
	//=========================================================================
	// protected variables.
	//=========================================================================
	bool mBoxFlag; //AABBDraw�p�t���O
	unsigned int cubemapTexture; //skybox�p�e�N�X�`��
	unsigned int skyboxVAO; //Skybox�pVAO
	unsigned int skyboxVBO; //Skybox�pVBO
	unsigned int skyboxEBO; //Skybox�pEBO
	float angle;
	float mScreenWidth; //Width
	float mScreenHeight; //Hieght

	Vector3 mAmbientLight; 	// Lighting data
	Matrix4 mView; //�V�F�[�_�[�ɑ���View
	Matrix4 mProjection; //�V�F�[�_�[�ɑ���ˉe

	SDL_Window* mWindow; // Window
	SDL_GLContext mContext; // OpenGL context
	SDL_Window* mImGuiWindow; // ImGuiWindow
	SDL_GLContext mImGuiContext; //ImGuicontext
	DirectionalLight mDirLight;
	class Game* mGame;
	class Terrain* mTerrain;
	class GBuffer* mGBuffer;
	class VertexArray* mSpriteVerts; //2D�p���_
	class Shader* mSpriteShader; //2D�p�V�F�[�_�[
	class Shader* mMeshShader; //�I�u�W�F�N�g�p�V�F�[�_�[
	class Shader* mUIMeshShader;
	class Shader* mSkinnedShader; //�L�����N�^�[�p�V�F�[�_�[
	class Shader* mSkyboxShader; //skybox�p�V�F�[�_�[
	class Shader* mLineShader; //AABB�`��p�V�F�[�_�[
	class Shader* mTessellationShader; //Terrain�p�V�F�[�_�[
	class Shader* mGGlobalShader; //Gbuffer�p�V�F�[�_�[
	class Shader* mGPointLightShader; //PointLight�p�V�F�[�_�[
	class Mesh* mPointLightMesh;
	std::vector<class PointLightComponent*> mPointLights;
	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class MeshComponent*> mMeshComps;
	std::vector<class SkeletalMeshComponent*> mSkeletalMeshes;
	std::vector<class UIMeshComponent*> mUIMeshComps;

    //=========================================================================
    // private methods.
    //=========================================================================
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, bool lit = true);
	void DrawSkybox();
	void DrawUIObj();
	void DrawFromGBuffer();
	void DrawAABB();
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const Matrix4& view);
};
