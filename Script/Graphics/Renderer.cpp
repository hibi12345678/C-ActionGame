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
#include "Renderer.h"
#include <glm.hpp>
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SOIL/SOIL.h>
#include <stb_image.h>
#include <algorithm>
#include <iostream>
#include "FollowActor.h"
#include "CameraComponent.h"
#include "FollowCamera.h"
#include "Game.h"
#include "GBuffer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PointLightComponent.h"
#include "PlaneActor.h"
#include "Shader.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "Terrain.h"
#include "Texture.h"
#include "TreeActor.h"
#include "TorchItemActor.h"
#include "UIMeshComponent.h"
#include "UIScreen.h"
#include "VertexArray.h"



///////////////////////////////////////////////////////////////////////////////
// Renderer class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
	,mMeshShader(nullptr)
	,mSkinnedShader(nullptr)
	,mGBuffer(nullptr)
	,mGGlobalShader(nullptr)
	,mGPointLightShader(nullptr)
	,angle(0.0f)
	,mPerspective(70.0f)
{
}


//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
Renderer::~Renderer()
{
}


//-----------------------------------------------------------------------------
//   skyboxの頂点情報
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//   skyboxのインデックスバッファ
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//   skyboxの各面の画像
//-----------------------------------------------------------------------------
std::string facesCubemap[6] =
{ 
	"Assets/Texture/SkyboxRight.jpg",
	"Assets/Texture/SkyboxLeft.jpg",
	"Assets/Texture/SkyboxTop.jpg",
	"Assets/Texture/SkyboxBottom.jpg",
	"Assets/Texture/SkyboxFront.jpg",
	"Assets/Texture/SkyboxBack.jpg"
};


//-----------------------------------------------------------------------------
//   初期化処理
//-----------------------------------------------------------------------------
bool Renderer::Initialize(float screenWidth, float screenHeight)
{

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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

	mWindow = SDL_CreateWindow("Game", 10, 150,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);



#ifdef _DEBUG
	mImGuiWindow = SDL_CreateWindow("ImGui Window", 1044, 150,
		866, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	mImGuiContext = SDL_GL_CreateContext(mImGuiWindow);
	//Iｍguiの初期化
	InitializeImGui(mImGuiWindow, mImGuiContext);
#endif // _DEBUG





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

	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// OpenGLのバージョンを取得
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;
	// Create quad for drawing sprites
	CreateSpriteVerts();
	
	// G-buffer生成
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

	
	//Skyboxの初期化
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

	//terrainの初期化
	mTerrain = new Terrain(mTessellationShader,mView,mProjection);

	return true;
}


//-----------------------------------------------------------------------------
//   終了処理
//-----------------------------------------------------------------------------
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
	mSkyboxShader->Unload();
	delete mSkyboxShader;
	mLineShader->Unload();
	delete mLineShader;
	mTessellationShader->Unload();
	delete mTessellationShader;

#ifdef _DEBUG 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);

}


//-----------------------------------------------------------------------------
//   終了処理
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//   描画処理の呼び出し
//-----------------------------------------------------------------------------
void Renderer::Draw()
{
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(mPerspective),
		mScreenWidth, mScreenHeight, 100.0f, 200000.0f);
	//Draw the GameScene
	// Draw the 3D scene to the G-buffer
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, true);
	//Draw Terrain
	mTerrain->GenerateTerrain(mView, mProjection);

	//Draw SkyBox
	DrawSkybox();
	// Set the frame buffer back to zero (screen's frame buffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw from the GBuffer
	DrawFromGBuffer();

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
	//DrawUIObj();

#ifdef _DEBUG // DebugモードのみImGuiを有効化
    // ---- ImGui描画のための準備 ----
	SDL_GL_MakeCurrent(mImGuiWindow, mImGuiContext);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// Docking領域を作成
	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	// 背景色設定ウィンドウ
	ImGui::SetWindowSize(ImVec2(480.0f, 800.0f), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::Begin("SetAmbientLight");

	static float bgColor[3] = { 0.7f, 0.7f, 0.7f };
	if (ImGui::ColorPicker3("BG color", bgColor, ImGuiColorEditFlags_PickerHueWheel)) {
		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
	}
	//SetAmbientLight(Vector3(bgColor[0], bgColor[1], bgColor[2]));
	ImGui::End();  

	auto& trees = mGame->GetTree();

	for (size_t i = 0; i < trees.size(); ++i)
	{
		auto& tree = trees[i];
		ImGui::PushID(i);

		ImGui::Begin("Tree Transform", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Tree %zu", i);


		Vector3 position = tree->GetPosition();
		ImGui::Columns(3, nullptr, false);
		ImGui::InputFloat(("Pos X" + std::to_string(i)).c_str(), &position.x);
		ImGui::NextColumn();
		ImGui::InputFloat(("Pos Y##" + std::to_string(i)).c_str(), &position.y);
		ImGui::NextColumn();
		ImGui::InputFloat(("Pos Z##" + std::to_string(i)).c_str(), &position.z);
		ImGui::Columns(1);

		tree->SetPosition(position);

		Quaternion rotation = tree->GetRotation();
		ImGui::Columns(4, nullptr, false);
		ImGui::InputFloat(("Rot X##" + std::to_string(i)).c_str(), &rotation.x);
		ImGui::NextColumn();
		ImGui::InputFloat(("Rot Y##" + std::to_string(i)).c_str(), &rotation.y);
		ImGui::NextColumn();
		ImGui::InputFloat(("Rot Z##" + std::to_string(i)).c_str(), &rotation.z);
		ImGui::NextColumn();
		ImGui::InputFloat(("Rot W##" + std::to_string(i)).c_str(), &rotation.w);
		ImGui::Columns(1);


		tree->SetRotation(rotation);

		ImGui::End();

		ImGui::PopID();
	}

	if (mTerrain != nullptr && mGame->GetState() != Game::GameState::EMainMenu) {
		ImGui::Begin("Terrain");
		glm::vec3 terrainPos = mTerrain->GetTranslate();
		ImGui::Columns(3, nullptr, false);
		ImGui::InputFloat("Pos X##", &terrainPos.x);
		ImGui::NextColumn();
		ImGui::InputFloat("Pos Y##", &terrainPos.y);
		ImGui::NextColumn();
		ImGui::InputFloat("Pos Z##", &terrainPos.z);
		ImGui::Columns(1);
		mTerrain->SetTranslate(terrainPos);
		ImGui::End();
	}


	if (mGame->GetPlayer() != nullptr && mGame->GetState() != Game::GameState::EMainMenu) {
		ImGui::Begin("Player Info");

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

		/*		if (mGame->GetPlayer()->GetCameraComponent() != nullptr) {
			// Perspective用のUI
			ImGui::InputFloat("Perspective", &mPerspective);

			// HorzDist用のUI
			float horzDist = mGame->GetPlayer()->GetCameraComponent()->GetHorzDist();
			if (ImGui::InputFloat("Horizontal Distance", &horzDist))
			{
				mGame->GetPlayer()->GetCameraComponent()->SetHorzDist(horzDist);
			}

			// VertDist用のUI
			float vertDist = mGame->GetPlayer()->GetCameraComponent()->GetVertDist();
			if (ImGui::InputFloat("Vertical Distance", &vertDist))
			{
				mGame->GetPlayer()->GetCameraComponent()->SetVertDist(vertDist);
			}

			// VertDist用のUI
			float rightDist = mGame->GetPlayer()->GetCameraComponent()->GetRightDist();
			if (ImGui::InputFloat("Right Distance", &rightDist))
			{
				mGame->GetPlayer()->GetCameraComponent()->SetRightDist(rightDist);
			}
		}
        */

		ImGui::End();  
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(mImGuiWindow);
	SDL_GL_MakeCurrent(mWindow, mContext);

#endif // _DEBUG
	SDL_GL_SwapWindow(mWindow);
}


//-----------------------------------------------------------------------------
//   Sprite vectorに追加
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//   Sprite vectorへ追加
//-----------------------------------------------------------------------------
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}


//-----------------------------------------------------------------------------
//   Sprite vectorから削除
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//   SkeltialMesh vectorから削除
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//   UIMeshComps vectorへ追加
//-----------------------------------------------------------------------------
void Renderer::AddUIMeshComp(UIMeshComponent* mesh)
{
     mUIMeshComps.emplace_back(mesh);
}


//-----------------------------------------------------------------------------
//  UIMeshComps vectorから削除
//-----------------------------------------------------------------------------
void Renderer::RemoveUIMeshComp(UIMeshComponent* mesh)
{
	auto iter = std::find(mUIMeshComps.begin(), mUIMeshComps.end(), mesh);
	mUIMeshComps.erase(iter);
}


//-----------------------------------------------------------------------------
//   PointLight vectorへ追加
//-----------------------------------------------------------------------------
void Renderer::AddPointLight(PointLightComponent * light)
{
	mPointLights.emplace_back(light);
}


//-----------------------------------------------------------------------------
//  PointLight vectorから削除
//-----------------------------------------------------------------------------
void Renderer::RemovePointLight(PointLightComponent * light)
{
	auto iter = std::find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(iter);
}


//-----------------------------------------------------------------------------
//  textureの取得
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//  メッシュ情報の取得
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//  3Dオブジェクトの描画
//-----------------------------------------------------------------------------
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



//-----------------------------------------------------------------------------
//  SkyBoxの描画
//-----------------------------------------------------------------------------
void Renderer::DrawSkybox() {

	
	glDepthFunc(GL_LEQUAL);


	mSkyboxShader->SetActive();
	glm::mat4 glmView = ConvertToGLM(mView);
	glm::mat4 glmProjection = ConvertToGLM(mProjection);
	glm::mat4 swappedView = glm::mat4(glmView[1], glmView[2], glmView[0], glmView[3]);
	glm::mat4 view = glm::mat4(glm::mat3(swappedView) );
	glUniformMatrix4fv(glGetUniformLocation(mSkyboxShader->GetID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(mSkyboxShader->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(glmProjection));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);

}


//-----------------------------------------------------------------------------
//  衝突判定デバッグ用のAABBの描画
//-----------------------------------------------------------------------------
void Renderer::DrawAABB()
{
	if (!mBoxFlag) return;

	// シェーダープログラムを有効化
	mLineShader->SetActive();

	glm::mat4 glmView = ConvertToGLM(mView);
	glm::mat4 glmProjection = ConvertToGLM(mProjection);
	glm::mat4 swappedView = glm::mat4(glmView[0], glmView[1], glmView[2], glmView[3]);

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


//-----------------------------------------------------------------------------
//  UI用
//-----------------------------------------------------------------------------
void Renderer::DrawUIObj() {

	// 深度テスト無効化
	glDisable(GL_DEPTH_TEST);  // 深度テストを無効化
	glDepthFunc(GL_ALWAYS); // 常に深度テストに合格
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// オーソグラフィック投影行列を使用
	//Matrix4 orthoProj = Matrix4::CreateOrtho((float)mScreenWidth,(float)mScreenHeight, -1.0f, 1.0f);
	//Matrix4 view = Matrix4::Identity;

	// Set the mesh shader active
	mUIMeshShader->SetActive();
	
	// Update view-projection matrix
	mUIMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);  // ビューとオーソグラフィック行列を掛け合わせて渡す


	for (auto mc : mUIMeshComps) {
		if (mc->GetVisible()) {

			mc->Draw(mUIMeshShader);

		}
	}

	// 深度テストを再有効化
	glEnable(GL_DEPTH_TEST);  // 3D描画用に深度テストを有効化
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS); // 通常の深度テストに戻す
	glUseProgram(0);
}


//-----------------------------------------------------------------------------
//  GBufferの描画
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
// シェーダーの初期化
//-----------------------------------------------------------------------------
bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Script/Shaders/Sprite.vert", "Script/Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);

	// Create basic mesh shader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Script/Shaders/Phong.vert", "Script/Shaders/GBufferWrite.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(mPerspective),
		mScreenWidth, mScreenHeight, 100.0f, 200000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create skinned shader
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Script/Shaders/Skinned.vert", "Script/Shaders/GBufferWrite.frag"))
	{
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create shader for drawing from GBuffer (global lighting)
	mGGlobalShader = new Shader();
	if (!mGGlobalShader->Load("Script/Shaders/GBufferGlobal.vert", "Script/Shaders/GBufferGlobal.frag"))
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
	if (!mGPointLightShader->Load("Script/Shaders/BasicMesh.vert",
								  "Script/Shaders/GBufferPointLight.frag"))
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
	if (!mSkyboxShader->Load("Script/Shaders/SkyBox.vert","Script/Shaders/SkyBox.frag"))
	{
		return false;
	}
	mSkyboxShader->SetActive();

	mSkyboxShader = new Shader();
	if (!mSkyboxShader->Load("Script/Shaders/SkyBox.vert", "Script/Shaders/SkyBox.frag"))
	{
		return false;
	}
	
	mLineShader = new Shader();
	if (!mLineShader->Load("Script/Shaders/Line.vert", "Script/Shaders/Line.frag"))
	{
		return false;
	}

	mUIMeshShader = new Shader();
	if (!mUIMeshShader->Load("Script/Shaders/UIMesh.vert", "Script/Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	mTessellationShader = new Shader();
	if (!mTessellationShader->Load("Script/Shaders/Tessellation.vert","Script/Shaders/Tessellation.frag"))
	{
		return false;
	}



	return true;
}


//-----------------------------------------------------------------------------
// 頂点情報の定義
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
// Light情報をシェーダーに送る
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
// スクリーン座標（2D座標）を3D空間の座標に変換
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ImGui初期化関数
//-----------------------------------------------------------------------------

void Renderer::InitializeImGui(SDL_Window* imguiWindow, SDL_GLContext imguiContext) {
	// ImGuiの設定
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// ImGuiのSDLバックエンドとOpenGL3バックエンドの初期化
	ImGui_ImplSDL2_InitForOpenGL(imguiWindow, imguiContext);
	ImGui_ImplOpenGL3_Init("#version 420 core");

	// Dockingを有効にする
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}



//-----------------------------------------------------------------------------
// Matrix4からGLMに変換
//-----------------------------------------------------------------------------
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


