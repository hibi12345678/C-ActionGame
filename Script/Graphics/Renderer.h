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
	//コンストラクタ
	Renderer(class Game* game);
	//デストラクタ
	~Renderer();

	//初期化
	bool Initialize(float screenWidth, float screenHeight);
	void InitializeImGui(SDL_Window* window, SDL_GLContext context);

	//終了処理
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
	bool mBoxFlag; //AABBDraw用フラグ
	unsigned int cubemapTexture; //skybox用テクスチャ
	unsigned int skyboxVAO; //Skybox用VAO
	unsigned int skyboxVBO; //Skybox用VBO
	unsigned int skyboxEBO; //Skybox用EBO
	float angle;
	float mScreenWidth; //Width
	float mScreenHeight; //Hieght

	Vector3 mAmbientLight; 	// Lighting data
	Matrix4 mView; //シェーダーに送るView
	Matrix4 mProjection; //シェーダーに送る射影

	SDL_Window* mWindow; // Window
	SDL_GLContext mContext; // OpenGL context
	SDL_Window* mImGuiWindow; // ImGuiWindow
	SDL_GLContext mImGuiContext; //ImGuicontext
	DirectionalLight mDirLight;
	class Game* mGame;
	class Terrain* mTerrain;
	class GBuffer* mGBuffer;
	class VertexArray* mSpriteVerts; //2D用頂点
	class Shader* mSpriteShader; //2D用シェーダー
	class Shader* mMeshShader; //オブジェクト用シェーダー
	class Shader* mUIMeshShader;
	class Shader* mSkinnedShader; //キャラクター用シェーダー
	class Shader* mSkyboxShader; //skybox用シェーダー
	class Shader* mLineShader; //AABB描画用シェーダー
	class Shader* mTessellationShader; //Terrain用シェーダー
	class Shader* mGGlobalShader; //Gbuffer用シェーダー
	class Shader* mGPointLightShader; //PointLight用シェーダー
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
