// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "FollowActor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include <GL/glew.h>
#include "SkeletalMeshComponent.h"
#include "GBuffer.h"
#include "PointLightComponent.h"
#include "stb_image.h"
#include <SOIL/SOIL.h>
#include<iostream>
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "MoveComponent.h"
#include "TorchItemActor.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.h"
Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
	,mMeshShader(nullptr)
	,mSkinnedShader(nullptr)
	,mGBuffer(nullptr)
	,mGGlobalShader(nullptr)
	,mGPointLightShader(nullptr)
	,angle(0.0f)
{
}

Renderer::~Renderer()
{
}

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};
std::string facesCubemap[6] =
{ 
	"Assets/Texture/SkyboxRight.jpg",
	"Assets/Texture/SkyboxLeft.jpg",
	"Assets/Texture/SkyboxTop.jpg",
	"Assets/Texture/SkyboxBottom.jpg",
	"Assets/Texture/SkyboxFront.jpg",
	"Assets/Texture/SkyboxBack.jpg"
};

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	mImGuiWindow = SDL_CreateWindow("ImGui Window", 1150,100,
		480, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	mImGuiContext = SDL_GL_CreateContext(mImGuiWindow);
	
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);
	// SDL_image初期化
	if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	
	// Create G-buffer
	mGBuffer = new GBuffer();
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	// Load point light mesh
	mPointLightMesh = GetMesh("Assets/Object/PointLight.gpmesh");

	InitializeImGui(mImGuiWindow, mImGuiContext);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// シームを防ぐために非常に重要
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{

			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}


	return true;
}

void Renderer::Shutdown()
{

	// Get rid of G-buffer
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}
	// Delete point lights
	while (!mPointLights.empty())
	{
		delete mPointLights.back();
	}
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	delete mSkyboxShader;
	mSkyboxShader->Unload();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	

}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	
	// Draw the 3D scene to the G-buffer
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, false);
	// Set the frame buffer back to zero (screen's frame buffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw from the GBuffer
	DrawFromGBuffer();
	
	DrawSkybox();
	
	glDisable(GL_DEPTH_TEST);
	DrawAABB();
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mSpriteShader);
		}
	}
	
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}


	// ---- ImGui描画のための準備 ----
	SDL_GL_MakeCurrent(mImGuiWindow, mImGuiContext);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::SetWindowSize(ImVec2(300.0f,700.0f), ImGuiCond_Always);
	ImGui::Begin("Hello, ImGui!");

	static float bgColor[3] = { 0.7f, 0.7f, 0.7f };

	if (ImGui::ColorPicker3("BG color", bgColor, ImGuiColorEditFlags_PickerHueWheel)) {
		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);  

	}
	SetAmbientLight(Vector3(bgColor[0],
		bgColor[1],
		bgColor[2]));
	if (mGame->GetPlayer() != nullptr && mGame->GetState() != Game::GameState::EMainMenu ) {
		float health = mGame->GetPlayer()->GetHealth();
		Vector3 pos = mGame->GetPlayer()->Actor::GetPosition();
		Quaternion Rotation = mGame->GetPlayer()->Actor::GetRotation();
		static float values[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		FollowActor::State mState = mGame->GetPlayer()->GetState();
		
		ImGui::Text("Health: %.2f", health);
		ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
		ImGui::Text("Rotation: (%.2f, %.2f, %.2f ,%.2f)", Rotation.x, Rotation.y, Rotation.z, Rotation.w);
		ImGui::Text("State: %d", static_cast<int>(mState));
		ImGui::Checkbox("Draw Boxes ", &mBoxFlag);
		/*		// 各要素を編集
		for (int i = 0; i < 4; ++i)
		{
			ImGui::SliderFloat(("Value " + std::to_string(i)).c_str(), &values[i], -10.0f, 10.0f);
			
		}*/

		const char* stateName = "";
		switch (mState) {
		case FollowActor::State::EJump:
			stateName = "Jump";
			break;
		case FollowActor::State::EFall:
			stateName = "Fall";
			break;
		case FollowActor::State::EGrounded:
			stateName = "Grounded";
			break;
		case FollowActor::State::EDead:
			stateName = "Dead";
			break;
		}
		ImGui::Text("State (Name): %s", stateName);

	}


	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(mImGuiWindow);
	SDL_GL_MakeCurrent(mWindow, mContext);
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComps.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
}

void Renderer::AddPointLight(PointLightComponent * light)
{
	mPointLights.emplace_back(light);
}

void Renderer::RemovePointLight(PointLightComponent * light)
{
	auto iter = std::find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, bool lit)
{
	// Set the current frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// Set the mesh shader active
	mMeshShader->SetActive();
	// Update view-projection matrix
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit)
	{
		SetLightUniforms(mMeshShader, view);
	}
	for (auto mc : mMeshComps)
	{
		if (mc->GetVisible())
		{
			mc->Draw(mMeshShader);
		}
	}

	// Draw any skinned meshes now
	mSkinnedShader->SetActive();
	// Update view-projection matrix
	mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit)
	{
		SetLightUniforms(mSkinnedShader, view);
	}
	for (auto sk : mSkeletalMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mSkinnedShader);
		}
	}
}

void Renderer::DrawSkybox() {

	
	glDepthFunc(GL_LEQUAL);

	mSkyboxShader->SetActive();
	glm::mat4 glmView = ConvertToGLM(mView);
	glm::mat4 glmProjection = ConvertToGLM(mProjection);
	glm::mat4 swappedView = glm::mat4(glmView[1] , glmView[2] , glmView[0] , glmView[3]);
	glm::mat4 view = glm::mat4(glm::mat3(swappedView) );
	glm::mat4 viewProj = glmProjection * view * 0.5f;
	glUniformMatrix4fv(glGetUniformLocation(mSkyboxShader->GetID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(mSkyboxShader->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(viewProj));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);

}


void Renderer::DrawAABB()
{
	if (!mBoxFlag) return;

	// シェーダープログラムを有効化
	mLineShader->SetActive();

	glm::mat4 glmView = ConvertToGLM(mView);
	glm::mat4 glmProjection = ConvertToGLM(mProjection);
	glm::mat4 swappedView = glm::mat4(glmView[0], glmView[1], glmView[2], glmView[3]);
	glm::mat4 view = glm::mat4(glm::mat3(swappedView));

	glm::mat4 viewProj = glmProjection * swappedView;

	glUniformMatrix4fv(glGetUniformLocation(mLineShader->GetID(), "uViewProjection"),
		1, GL_FALSE, glm::value_ptr(viewProj));

	auto& boxes = mGame->GetPhysWorld()->GetBoxComponents();
	for (auto box : boxes) {
		const AABB& mWorldbox = box->GetWorldBox();

		glm::mat4 minmat = ConvertToGLM(Matrix4::CreateTranslation(mWorldbox.mMin));
		glm::vec3 min = glm::vec3(minmat[3]);
		glm::mat4 maxmat = ConvertToGLM(Matrix4::CreateTranslation(mWorldbox.mMax));
		glm::vec3 max = glm::vec3(maxmat[3]);

		// AABBの頂点を定義
		float vertices[] = {
			min.x, min.y, min.z, max.x, min.y, min.z,
			max.x, min.y, min.z, max.x, max.y, min.z,
			max.x, max.y, min.z, min.x, max.y, min.z,
			min.x, max.y, min.z, min.x, min.y, min.z,
			min.x, min.y, max.z, max.x, min.y, max.z,
			max.x, min.y, max.z, max.x, max.y, max.z,
			max.x, max.y, max.z, min.x, max.y, max.z,
			min.x, max.y, max.z, min.x, min.y, max.z,
			min.x, min.y, min.z, min.x, min.y, max.z,
			max.x, min.y, min.z, max.x, min.y, max.z,
			max.x, max.y, min.z, max.x, max.y, max.z,
			min.x, max.y, min.z, min.x, max.y, max.z,
		};

		// VBOとVAOの設定
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 描画
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 24);

		// クリーンアップ
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
}
	
void Renderer::DrawFromGBuffer()
{
	// Clear the current framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Disable depth testing for the global lighting pass
	glDisable(GL_DEPTH_TEST);
	// Activate global G-buffer shader
	mGGlobalShader->SetActive();
	// Activate sprite verts quad
	mSpriteVerts->SetActive();
	// Set the G-buffer textures to sample
	mGBuffer->SetTexturesActive();
	// Set the lighting uniforms
	SetLightUniforms(mGGlobalShader, mView);
	// Draw the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Copy depth buffer from G-buffer to default frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height,
		0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// Enable depth test, but disable writes to depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// Set the point light shader and mesh as active
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();
	// Set the view-projection matrix
	mGPointLightShader->SetMatrixUniform("uViewProj",
		mView * mProjection);
	// Set the G-buffer textures for sampling
	mGBuffer->SetTexturesActive();

	// The point light color should add to existing color
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Draw the point lights
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mGPointLightShader, mPointLightMesh);
	}
}

bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);

	// Create basic mesh shader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mScreenWidth, mScreenHeight, 10.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create skinned shader
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create shader for drawing from GBuffer (global lighting)
	mGGlobalShader = new Shader();
	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag"))
	{
		return false;
	}
	// For the GBuffer, we need to associate each sampler with an index
	mGGlobalShader->SetActive();
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);
	// The view projection is just the sprite one
	mGGlobalShader->SetMatrixUniform("uViewProj", spriteViewProj);
	// The world transform scales to the screen and flips y
	Matrix4 gbufferWorld = Matrix4::CreateScale(mScreenWidth, -mScreenHeight,
		1.0f);
	mGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);

	// Create a shader for point lights from GBuffer
	mGPointLightShader = new Shader();
	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert",
								  "Shaders/GBufferPointLight.frag"))
	{
		return false;
	}
	// Set sampler indices
	mGPointLightShader->SetActive();
	mGPointLightShader->SetIntUniform("uGDiffuse", 0);
	mGPointLightShader->SetIntUniform("uGNormal", 1);
	mGPointLightShader->SetIntUniform("uGWorldPos", 2);
	mGPointLightShader->SetVector2Uniform("uScreenDimensions",
		Vector2(mScreenWidth, mScreenHeight));

	mSkyboxShader = new Shader();
	if (!mSkyboxShader->Load("Shaders/SkyBox.vert","Shaders/SkyBox.frag"))
	{
		return false;
	}
	mSkyboxShader->SetActive();

	mSkyboxShader = new Shader();
	if (!mSkyboxShader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag"))
	{
		return false;
	}
	
	mLineShader = new Shader();
	if (!mLineShader->Load("Shaders/Line.vert", "Shaders/Line.frag"))
	{
		return false;
	}
	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader, const Matrix4& view)
{
	// Camera position is from inverted view
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;

	// Transform vector by unprojection matrix
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);
	// Get direction vector
	outDir = end - outStart;
	outDir.Normalize();
}

void Renderer::InitializeImGui(SDL_Window* window, SDL_GLContext context) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330");
}

glm::mat4 Renderer::ConvertToGLM(const Matrix4& matrix)
{
	glm::mat4 glmMatrix;
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			glmMatrix[row][col] = matrix.mat[row][col];
		}
	}
	return glmMatrix;
}


