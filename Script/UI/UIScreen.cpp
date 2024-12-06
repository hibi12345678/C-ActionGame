// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include <SOIL/SOIL.h>
#include "AudioSystem.h"
UIScreen::UIScreen(Game* game)
	:mGame(game)
	,mTitle(nullptr)
	,mBackground(nullptr)
	,mTitlePos(0.0f, 200.0f)
	,mNextButtonPos(0.0f, 100.0f)
	, mNextItemButtonPos(-200.0f, 100.0f)
	,mNextTextPos(0.0f, -100.0f)
	,mBGPos(0.0f, 150.0f)
	,mUIState(EActive)
	,TutorialNum(0)
{
	// Add to UI Stack
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/ButtonUIRed.png");
	mButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/ButtonUI.png");
	mItemButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/ItemButtonYellow.png");
	mItemButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/ItemButtonWhite.png");
	mSword = mGame->GetRenderer()->GetTexture("Assets/Texture/SwordTex.png");
	mBow = mGame->GetRenderer()->GetTexture("Assets/Texture/BowTex.png");
	mBomb = mGame->GetRenderer()->GetTexture("Assets/Texture/BombTex.png");
	mTorch = mGame->GetRenderer()->GetTexture("Assets/Texture/TorchTex.png");
	mArrow = mGame->GetRenderer()->GetTexture("Assets/Texture/ItemArrow.png");
	mTutorialRightButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/TutorialRightButtonOn.png");
	mTutorialRightButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/TutorialRightButtonOff.png");
	mTutorialLeftButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/TutorialLeftButtonOn.png");
	mTutorialLeftButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/TutorialLeftButtonOff.png");
	mCloseButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/CloseButtonOff.png");
	mCloseButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/CloseButtonOn.png");
}

UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}
	for (auto b : mButtons)
	{
		delete b;
	}	
	for (auto b : mStartButton)
	{
		delete b;
	}
	for (auto b : mText)
	{
		delete b;
	}
	for (auto b : mItemButton)
	{
		delete b;
	}

	if (mItemText)
	{
		mItemText->Unload();
		delete mItemText;
	}
	if (mTutorialButtonRight)
	{
		delete mTutorialButtonRight;
	}	
	if (mTutorialButtonLeft)
	{	
		delete mTutorialButtonLeft;
	}
	if (mCloseButton)
	{
		delete mCloseButton;
	}
	mItemButton.clear();
	mButtons.clear();
	mStartButton.clear();
	mText.clear();
	
}

void UIScreen::Update(float deltaTime)
{
	if (TutorialNum < 0) {
		TutorialNum = 0;
	}
	else if (TutorialNum > 3) {
		TutorialNum = 3;
	}

}

void UIScreen::Draw(Shader* shader)
{
	// Draw background (if exists)
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// Draw title (if exists)
	if (mTitle)
	{			
		DrawTexture(shader, mTitle, mTitlePos);	
	}

	// Draw buttons
	for (auto b : mButtons)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;	
		DrawTexture(shader, tex, b->GetPosition());
		// Draw text of button
		DrawTexture(shader,b->GetNameTex(), b->GetPosition());

	}	
	// Draw buttons
	for (auto b : mStartButton)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		// Draw text of button
		DrawTexture(shader,b->GetNameTex(), b->GetPosition());	
	}
	for (auto tex : mText)
	{
		DrawTexture(shader, tex, tex->GetPos());	
	}
	
	// Draw buttons
	for (auto b : mItemButton)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mItemButtonOn : mItemButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		
		if (b->GetTexNum() == 0) {
			DrawTexture(shader, mSword, b->GetPosition());		
		}
		else if (b->GetTexNum() == 1) {
			DrawTexture(shader, mTorch, b->GetPosition());	
		}
		else if (b->GetTexNum() == 2) {
			DrawTexture(shader, mBow, b->GetPosition());		
		}
		else if (b->GetTexNum() == 3) {
			DrawTexture(shader, mBomb, b->GetPosition());

		}

	}

	for (auto b : mStartButton)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		// Draw text of button
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
	if (mItemText)
	{
	
		if (mArrowFlag == true) {
			DrawTexture(shader, mArrow, Vector2(0.0f, -70.0f),0.5f);
		}
		DrawTexture(shader, mItemText, mItemText->GetPos());

	}
	if (mTutorialButtonRight)
	{
		// Draw background of button
		Texture* tex = mTutorialButtonRight->GetHighlighted() ? mTutorialRightButtonOn : mTutorialRightButtonOff;
		if (TutorialNum != 3) {
			DrawTexture(shader, tex, mTutorialButtonRight->GetPosition());
		}
		

	}
	if (mTutorialButtonLeft)
	{
		
		// Draw background of button
		Texture* tex = mTutorialButtonLeft->GetHighlighted() ? mTutorialLeftButtonOn : mTutorialLeftButtonOff;
		if (TutorialNum != 0) {
			DrawTexture(shader, tex, mTutorialButtonLeft->GetPosition());
		}
		

	}
	if (mCloseButton)
	{

		// Draw background of button
		Texture* tex = mCloseButton->GetHighlighted() ? mCloseButtonOn : mCloseButtonOff;
		if (TutorialNum == 3) {

			DrawTexture(shader, tex, mCloseButton->GetPosition());
		}


	}
	if (Game::GameState::EItem == mGame->GetState()) {

		Texture* mEquipped = new Texture();
		mEquipped = mFont->RenderText("Equipped", Color::White, 30);
		DrawTexture(shader, mEquipped, Vector2(-250.0f, -80.0f));
	}
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
	// Do we have buttons?
	if (!mButtons.empty())
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		
		// Highlight any buttons
		for (auto b : mButtons)
		{
			
			if (b->ContainsPoint(mousePos))
			{
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}
		}

	}
	// Do we have buttons?
	if (!mItemButton.empty())
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		mArrowFlag = false;
		// Highlight any buttons
		for (auto b : mItemButton)
		{

			if (b->ContainsPoint(mousePos))
			{
				b->SetHighlighted(true);
				
				if (b->GetTexNum() == 0) {
					
					SetItemText("SwordText", Vector2(0.0f, 10.0f), 30);
					mArrowFlag = true;
				}
				else if (b->GetTexNum() == 1) {
					
					SetItemText("TorchText", Vector2(0.0f, 10.0f), 30);
					mArrowFlag = true;
				}
				else if (b->GetTexNum() == 2) {
					
					SetItemText("BowText", Vector2(0.0f, 10.0f), 30);
					mArrowFlag = true;
				}
				else if (b->GetTexNum() == 3) {
					
					SetItemText("BombText", Vector2(0.0f, 10.0f), 30);
					mArrowFlag = true;
				}
				
				
			}
			else
			{
				b->SetHighlighted(false);		
			}
		}

	}

	if (mTutorialButtonRight)
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		if (mTutorialButtonRight->ContainsPoint(mousePos))
		{
			mTutorialButtonRight->SetHighlighted(true);

		}
		else
		{
			mTutorialButtonRight->SetHighlighted(false);
		}
	}

	if (mTutorialButtonLeft)
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		if (mTutorialButtonLeft->ContainsPoint(mousePos))
		{
			mTutorialButtonLeft->SetHighlighted(true);

		}
		else
		{
			mTutorialButtonLeft->SetHighlighted(false);
		}


	}

	if (mCloseButton)
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		if (mCloseButton->ContainsPoint(mousePos))
		{
			mCloseButton->SetHighlighted(true);

		}
		else
		{
			mCloseButton->SetHighlighted(false);
		}


	}
}

void UIScreen::StartInput(const uint8_t* keys)
{	
	// Do we have buttons?
	if (!mStartButton.empty())
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		
		for (auto b : mStartButton)
		{	
			if (b->ContainsPoint(mousePos))
			{	
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}
		}
		
	}
}
void UIScreen::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		if (!mItemButton.empty())
		{
			for (auto b : mItemButton)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		if (!mStartButton.empty())
		{
			for (auto b : mStartButton)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}

		}
		if (mTutorialButtonRight)
		{		
			if (mTutorialButtonRight->GetHighlighted())
			{
				mTutorialButtonRight->OnClick();
				break;
			}
		}
		if (mTutorialButtonLeft)
		{
			if (mTutorialButtonLeft->GetHighlighted())
			{
				mTutorialButtonLeft->OnClick();
				break;
			}
		}
		if (mCloseButton)
		{
			if (mCloseButton->GetHighlighted())
			{
				mCloseButton->OnClick();
				break;
			}
		}
		break;
	default:
		break;
	}
	

}


void UIScreen::Close()
{
	mUIState = EClosing;
}

void UIScreen::CloseTutorial()
{
	if (TutorialNum == 3) {
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		mUIState = EClosing;
	}
	
}
void UIScreen::SetTitle(const std::string& text,
						const Vector3& color,
						int pointSize)
{
	// Clear out previous title texture if it exists
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	
	mTitle = mFont->RenderText(text, color, pointSize);

}

void UIScreen::AddText(const std::string& text, Vector2 pos, int pointSize,
	const Vector3& color, int num
	 )
{
	
	Texture* tex = new Texture();
	tex = mFont->RenderText(text, color, pointSize,num);
	tex->SetPos(pos);
	mText.emplace_back(tex);
	
	
}

void UIScreen::CloseText()
{
	for (auto b : mText) {
		delete b;
	}
	mText.clear();
}

void UIScreen::SetItemText(const std::string& text, Vector2 pos, int pointSize,
	const Vector3& color)
{

	// Clear out previous title texture if it exists
	if (mItemText)
	{
		mItemText->Unload();
		delete mItemText;
		mItemText = nullptr;
	}
	mItemText = mFont->RenderText(text, color, pointSize);
	//mItemText->SetTexNum(itemNum);
	mItemText->SetPos(pos);
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()), 
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims,0);
	mButtons.emplace_back(b);
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
	
}

void UIScreen::StartButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(0.0f, 0.0f), dims,0);	
	mStartButton.emplace_back(b);	
}
void UIScreen::ItemButton(const std::string& name,int texNumber ,std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mItemButtonOn->GetWidth()),
		static_cast<float>(mItemButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextItemButtonPos, dims, texNumber);
	mItemButton.emplace_back(b);
	// Update position of next button
	// Move down by height of button plus padding
	mNextItemButtonPos.x += mItemButtonOff->GetWidth()+ 30.0f;
}

void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
	const Vector2& offset, float scale, bool flipY, int a)
{
	float yScale = static_cast<float>(texture->GetHeight()) * scale;
	if (a == 1) {
		yScale = static_cast<float>(texture->GetHeight()) * 1.3f;
	}
	if (flipY) {
		yScale *= -1.0f;
	}
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		yScale,
		1.0f);

	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	texture->SetActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
void UIScreen::DrawButtonRight(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialRightButtonOn->GetWidth()),
		static_cast<float>(mTutorialRightButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(120.0f, -170.0f), dims, 0);
	mTutorialButtonRight = b;
}

void UIScreen::DrawButtonLeft(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialLeftButtonOn->GetWidth()),
		static_cast<float>(mTutorialLeftButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(-120.0f, -170.0f), dims, 0);
	mTutorialButtonLeft = b;
}

void UIScreen::DrawCloseButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mCloseButtonOn->GetWidth()),
		static_cast<float>(mCloseButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(250.0f, 200.0f), dims, 0);
	mCloseButton = b;
}
void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void UIScreen::AddTutorialNum()
{ 
	if (TutorialNum < 4) {
		TutorialNum++;
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		
	}
	
}
void UIScreen::RemoveTutorialNum()
{ 
	if (TutorialNum > 0) {
		TutorialNum--;
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
	}
}
Button::Button(const std::string& name, Font* font,
	std::function<void()> onClick,
	const Vector2& pos, const Vector2& dims, int TexNum)
	:mOnClick(onClick)
	,mNameTex(nullptr)
	,mFont(font)
	,mPosition(pos)
	,mDimensions(dims)
	,mHighlighted(false)
	,mTexNum(TexNum)
{
	SetName(name);
}

Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y > (mPosition.y + mDimensions.y / 2.0f);
	return !no;
}

void Button::OnClick()
{
	if (mOnClick)
	{		
		mOnClick();
	}
}


