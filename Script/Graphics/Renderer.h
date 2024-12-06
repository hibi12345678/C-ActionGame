// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Maths.h"
#include <GL/glew.h> 
#include <glm.hpp> 
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
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	void AddUIMeshComp(class UIMeshComponent* mesh);
	void RemoveUIMeshComp(class UIMeshComponent* mesh);

	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(class PointLightComponent* light);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	const Vector3& GetAmbientLight() const { return mAmbientLight; }
	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }
	const std::vector<class PointLightComponent*>& GetPointLights() const { return mPointLights; }

	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further
	Vector3 Unproject(const Vector3& screenPoint) const;
	
	// Gets start point and direction of screen vector
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

	class GBuffer* GetGBuffer() { return mGBuffer; }

	void InitializeImGui(SDL_Window* window, SDL_GLContext context);
	glm::mat4 Renderer::ConvertToGLM(const Matrix4& matrix);
	glm::vec3 ToGlmVec3(const Vector3& vec) {
		return glm::vec3(vec.x, vec.y, vec.z);
	}
private:
	// Chapter 14 additions
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, bool lit = true);
	void DrawSkybox();
	void DrawUIObj();
	void DrawFromGBuffer();
	void DrawAABB();
	// End chapter 14 additions
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const Matrix4& view);

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// All (non-skeletal) mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;
	std::vector<class SkeletalMeshComponent*> mSkeletalMeshes;
	std::vector<class UIMeshComponent*> mUIMeshComps;
	// Game
	class Game* mGame;
	class Terrain* mTerrain;
	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// shader
	class Shader* mMeshShader;

	class Shader* mUIMeshShader;

	class Shader* mSkinnedShader;	

	class Shader* mSkyboxShader;
	class Shader* mLineShader;
	class Shader* mTessellationShader;

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;

	// Lighting data
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;

	// ImGuiWindow
	SDL_Window* mImGuiWindow;
	// OpenGL ImGuicontext
	SDL_GLContext mImGuiContext;

	// Width/height
	float mScreenWidth;
	float mScreenHeight;
	
	class GBuffer* mGBuffer;
	// GBuffer shader
	class Shader* mGGlobalShader;
	class Shader* mGPointLightShader;
	std::vector<class PointLightComponent*> mPointLights;
	class Mesh* mPointLightMesh;
	float angle;
	bool mBoxFlag;
	unsigned int cubemapTexture;


	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;


};
