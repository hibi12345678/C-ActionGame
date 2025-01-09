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
#include "UIScreen.h"
#include <SOIL/SOIL.h>
#include "AudioSystem.h"
#include "Font.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////
// UIScreen class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
UIScreen::UIScreen(Game* game)
	:mGame(game)
	,mTitle(nullptr)
	,mBackground(nullptr)
	,mTitlePos(0.0f, 200.0f)
	,mNextButtonPos(0.0f, 100.0f)
	,mNextItemButtonPos(-200.0f, 100.0f)
	,mNextTextPos(0.0f, -100.0f)
	,mBGPos(0.0f, 150.0f)
	,mUIState(UIState::EActive)
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
	mTutorial = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial.jpg");
	mTutorialTex0 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial0.png");
	mTutorialTex1_1 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial1-1.png");
	mTutorialTex1_2 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial1-2.png");
	mTutorialTex1_3 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial1-3.png");
	mTutorialTex2 = mGame->GetRenderer()->GetTexture("Assets/Texture/Tutorial2.png");
}


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
UIScreen::~UIScreen()
{
	// mTitle�̉��
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	// mItemText�̉��
	if (mItemText)
	{
		mItemText->Unload();
		delete mItemText;
	}

	// �e�{�^����e�L�X�g�̉��
	for (auto b : mButtons) { delete b; }
	for (auto b : mStartButton) { delete b; }
	for (auto b : mText) { delete b; }
	for (auto b : mItemButton) { delete b; }

	// �`���[�g���A���{�^���̉��
	delete mTutorialButtonRight;
	delete mTutorialButtonLeft;
	delete mCloseButton;

	// �R���e�i�̃N���A
	mItemButton.clear();
	mButtons.clear();
	mStartButton.clear();
	mText.clear();
}


//-----------------------------------------------------------------------------
//   Update
//-----------------------------------------------------------------------------
void UIScreen::Update(float deltaTime)
{
	if (TutorialNum < 0) {
		TutorialNum = 0;
	}
	else if (TutorialNum > 3) {
		TutorialNum = 3;
	}

}


//-----------------------------------------------------------------------------
//   �`�揈��
//-----------------------------------------------------------------------------
void UIScreen::Draw(Shader* shader)
{
	// �w�i�̕`��
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}

	// �^�C�g���̕`��
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}

	// �{�^���̕`��
	DrawUIButtons(shader, mButtons, mButtonOn, mButtonOff);
	DrawUIButtons(shader, mStartButton, mButtonOn, mButtonOff);
	DrawUIButtons(shader, mItemButton, mItemButtonOn, mItemButtonOff);

	// �e�L�X�g�̕`��
	for (auto tex : mText)
	{
		DrawTexture(shader, tex, tex->GetPos());
	}

	// �A�C�e���{�^���̕`��
	DrawUIItemButtons(shader);
	
	// �A�C�e���e�L�X�g�̕`��
	if (mItemText)
	{
		if (mArrowFlag) {
			DrawTexture(shader, mArrow, Vector2(0.0f, -70.0f), 0.5f);
		}
		DrawTexture(shader, mItemText, mItemText->GetPos());
	}

	// �`���[�g���A����ʂ̕`��
	if (Game::GameState::ETutorial == mGame->GetState())
	{
		DrawUITutorial(shader);
	}

	// �`���[�g���A���{�^���̕`��
	DrawUITutorialButtons(shader);

	// ����{�^���̕`��
	DrawUICloseButton(shader);

	// �A�C�e����ʂ̕`��
	if (Game::GameState::EItem == mGame->GetState())
	{
		Texture* mEquipped = mFont->RenderText("Equipped", Color::White, 30);
		DrawTexture(shader, mEquipped, Vector2(-250.0f, -80.0f));

	}
}


//-----------------------------------------------------------------------------
// �{�^���̕`�揈��
//-----------------------------------------------------------------------------
void UIScreen::DrawUIButtons(Shader* shader, const std::vector<Button*>& buttons, Texture* buttonOn, Texture* buttonOff)
{
	for (auto b : buttons)
	{
		Texture* tex = b->GetHighlighted() ? buttonOn : buttonOff;
		DrawTexture(shader, tex, b->GetPosition());
		if(Game::GameState::EItem != mGame->GetState())
			DrawTexture(shader, b->GetNameTex(), b->GetPosition());	
	}
}


//-----------------------------------------------------------------------------
// �A�C�e���{�^���̕`�揈��
//-----------------------------------------------------------------------------
void UIScreen::DrawUIItemButtons(Shader* shader)
{
	for (auto b : mItemButton)
	{
		Texture* tex = b->GetHighlighted() ? mItemButtonOn : mItemButtonOff;
		DrawTexture(shader, tex, b->GetPosition());

		switch (b->GetTexNum())
		{
		case 0:
			DrawTexture(shader, mSword, b->GetPosition());
			break;
		case 1:
			DrawTexture(shader, mTorch, b->GetPosition());
			break;
		case 2:
			DrawTexture(shader, mBow, b->GetPosition());
			break;
		case 3:
			DrawTexture(shader, mBomb, b->GetPosition());
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// �`���[�g���A����ʂ̕`�揈��
//-----------------------------------------------------------------------------
void UIScreen::DrawUITutorial(Shader* shader)
{
	DrawTexture(shader, mTutorial, Vector2(0.0f, 0.0f), 0.5f);

	Texture* tex = mFont->RenderText("Guide", Color::Black, 60, 1);
	DrawTexture(shader, tex, Vector2(0.0f, 240.0f));

	switch (TutorialNum)
	{
	case 0:
		tex = mFont->RenderText("Defeat the Boss!!", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(0.0f, -150.0f));
		DrawTexture(shader, mTutorialTex0, Vector2(0.0f, 50.0f));
		break;

	case 1:
		DrawTexture(shader, mTutorialTex1_1, Vector2(0.0f, 130.0f));
		DrawTexture(shader, mTutorialTex1_2, Vector2(-120.0f, -40.0f));
		DrawTexture(shader, mTutorialTex1_3, Vector2(120.0f, -40.0f));
		tex = mFont->RenderText("Health bar, ", Color::White, 24, 1);
		DrawTexture(shader, tex, Vector2(20.0f, 140.0f));
		tex = mFont->RenderText("Stamina bar, ", Color::White, 24, 1);
		DrawTexture(shader, tex, Vector2(20.0f, 120.0f));
		tex = mFont->RenderText("Radar", Color::Black, 24, 1);
		DrawTexture(shader, tex, Vector2(-120.0f, 40.0f));
		tex = mFont->RenderText("Equipped Items", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(0.0f, -150.0f));
		break;

	case 2:
		tex = mFont->RenderText("Press Tab to select an item", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(0.0f, -150.0f));
		DrawTexture(shader, mTutorialTex2, Vector2(0.0f, 50.0f));
		break;

	case 3:
		tex = mFont->RenderText("WASD : Move", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(-200.0f, 150.0f));
		tex = mFont->RenderText("Space : Jump", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(-205.0f, 70.0f));
		tex = mFont->RenderText("Left Click : Attack", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(-170.0f, -10.0f));
		tex = mFont->RenderText("Shift : Run", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(-200.0f, -90.0f));
		tex = mFont->RenderText("E : Skill", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(110.0f, 150.0f));
		tex = mFont->RenderText("Esc : Menu", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(145.0f, 70.0f));
		tex = mFont->RenderText("Tab : ItemMenu", Color::Black, 42, 1);
		DrawTexture(shader, tex, Vector2(190.0f, -10.0f));
		break;
	}
}


//-----------------------------------------------------------------------------
// �`���[�g���A���{�^���̕`�揈��
//-----------------------------------------------------------------------------
void UIScreen::DrawUITutorialButtons(Shader* shader)
{
	if (mTutorialButtonRight && TutorialNum != 3)
	{
		Texture* tex = mTutorialButtonRight->GetHighlighted() ? mTutorialRightButtonOn : mTutorialRightButtonOff;
		DrawTexture(shader, tex, mTutorialButtonRight->GetPosition());
	}

	if (mTutorialButtonLeft && TutorialNum != 0)
	{
		Texture* tex = mTutorialButtonLeft->GetHighlighted() ? mTutorialLeftButtonOn : mTutorialLeftButtonOff;
		DrawTexture(shader, tex, mTutorialButtonLeft->GetPosition());
	}
}


//-----------------------------------------------------------------------------
// ����{�^���̕`�揈��
//-----------------------------------------------------------------------------
void UIScreen::DrawUICloseButton(Shader* shader)
{
	if (mCloseButton && TutorialNum == 3)
	{
		Texture* tex = mCloseButton->GetHighlighted() ? mCloseButtonOn : mCloseButtonOff;
		DrawTexture(shader, tex, mCloseButton->GetPosition());
	}
}


//-----------------------------------------------------------------------------
//   �}�E�X�̓��͏���
//-----------------------------------------------------------------------------
void UIScreen::ProcessInput(const uint8_t* keys)
{
	// Get mouse position and convert to (0,0) center coordinates
	Vector2 mousePos = GetMousePosition();

	// Highlight buttons for mButtons
	HighlightButtons(mButtons, mousePos);

	// Highlight item buttons and set item text for mItemButton
	HighlightItemButtons(mItemButton, mousePos);

	// Highlight tutorial buttons
	HighlightTutorialButton(mTutorialButtonRight, mousePos);
	HighlightTutorialButton(mTutorialButtonLeft, mousePos);
	HighlightCloseButton(mCloseButton, mousePos);
}


//-----------------------------------------------------------------------------
//   �}�E�X���W���擾���Ē��S���W�ɕϊ�����
//-----------------------------------------------------------------------------
Vector2 UIScreen::GetMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
	mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
	mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
	return mousePos;
}


//-----------------------------------------------------------------------------
//   �{�^���̃n�C���C�g����
//-----------------------------------------------------------------------------
void UIScreen::HighlightButtons(std::vector<Button*>& buttons, const Vector2& mousePos)
{
	for (auto b : buttons)
	{
		b->SetHighlighted(b->ContainsPoint(mousePos));
	}
}

//-----------------------------------------------------------------------------
//   �A�C�e���{�^���̃n�C���C�g�����ƃA�C�e���e�L�X�g�̐ݒ�
//-----------------------------------------------------------------------------
void UIScreen::HighlightItemButtons(std::vector<Button*>& itemButtons, const Vector2& mousePos)
{
	mArrowFlag = false;
	for (auto b : itemButtons)
	{
		bool isHighlighted = b->ContainsPoint(mousePos);
		b->SetHighlighted(isHighlighted);

		if (isHighlighted)
		{
			SetItemTextForItem(b);
		}
	}
}

//-----------------------------------------------------------------------------
//   �A�C�e���{�^���ɑΉ�����e�L�X�g��ݒ�
//-----------------------------------------------------------------------------
void UIScreen::SetItemTextForItem(Button* button)
{

	if (button->GetTexNum() == 0)
	{
		SetItemText("SwordText", Vector2(0.0f, 10.0f), 30);
		mArrowFlag = true;
	}
	else if (button->GetTexNum() == 1)
	{
		SetItemText("TorchText", Vector2(0.0f, 10.0f), 30);
		mArrowFlag = true;
	}
	else if (button->GetTexNum() == 2)
	{
		SetItemText("BowText", Vector2(0.0f, 10.0f), 30);
		mArrowFlag = true;
	}
	else if (button->GetTexNum() == 3)
	{
		SetItemText("BombText", Vector2(0.0f, 10.0f), 30);
		mArrowFlag = true;
	}
}

//-----------------------------------------------------------------------------
//   �`���[�g���A���{�^���̃n�C���C�g����
//-----------------------------------------------------------------------------
void UIScreen::HighlightTutorialButton(Button* button, const Vector2& mousePos)
{
	if (button)
	{
		button->SetHighlighted(button->ContainsPoint(mousePos));
	}
}

//-----------------------------------------------------------------------------
//   �N���[�Y�{�^���̃n�C���C�g����
//-----------------------------------------------------------------------------
void UIScreen::HighlightCloseButton(Button* button, const Vector2& mousePos)
{
	if (button)
	{
		button->SetHighlighted(button->ContainsPoint(mousePos));
	}
}


//-----------------------------------------------------------------------------
//   MainMenu�ł�mStartButton�̏���
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//    ���͏���
//-----------------------------------------------------------------------------
void UIScreen::HandleKeyPress(int key)
{
	if (key == SDL_BUTTON_LEFT)
	{
		// �{�^���z��̏���
		HandleButtonClick(mButtons);
		HandleButtonClick(mItemButton);
		HandleButtonClick(mStartButton);

		// �P��{�^���̏���
		if (mTutorialButtonRight && mTutorialButtonRight->GetHighlighted())
		{
			mTutorialButtonRight->OnClick();
		}
		if (mTutorialButtonLeft && mTutorialButtonLeft->GetHighlighted())
		{
			mTutorialButtonLeft->OnClick();
		}
		if (mCloseButton && mCloseButton->GetHighlighted())
		{
			mCloseButton->OnClick();
		}
	}
}

//-----------------------------------------------------------------------------
//    �{�^���̃N���b�N����
//-----------------------------------------------------------------------------
template <typename T>
void UIScreen::HandleButtonClick(const std::vector<T*>& buttons)
{
	for (auto b : buttons)
	{
		if (b->GetHighlighted())
		{
			b->OnClick();
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//     UI�����
//-----------------------------------------------------------------------------
void UIScreen::Close()
{
	mUIState = UIState::EClosing;
}


//-----------------------------------------------------------------------------
//     �`���[�g���A�������
//-----------------------------------------------------------------------------
void UIScreen::CloseTutorial()
{
	if (TutorialNum == 3) {
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		mUIState = UIState::EClosing;
	}
	
}


//-----------------------------------------------------------------------------
//    �^�C�g���̏�����
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//    text�̒ǉ�
//-----------------------------------------------------------------------------
void UIScreen::AddText(const std::string& text, Vector2 pos, int pointSize,
	const Vector3& color, int num
	 )
{
	
	Texture* tex = new Texture();
	tex = mFont->RenderText(text, color, pointSize,num);
	tex->SetPos(pos);
	mText.emplace_back(tex);
	
}


//-----------------------------------------------------------------------------
//    text�����������
//-----------------------------------------------------------------------------
void UIScreen::CloseText()
{
	for (auto b : mText) {
		delete b;
	}
	mText.clear();
}


//-----------------------------------------------------------------------------
//    Itemtext�̒ǉ�
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//    Button�̒ǉ�
//-----------------------------------------------------------------------------
void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()), 
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims,0);
	mButtons.emplace_back(b);
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
	
}


//-----------------------------------------------------------------------------
//    Start���j���[��Button�̒ǉ�
//-----------------------------------------------------------------------------
void UIScreen::StartButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(0.0f, -100.0f), dims,0);	
	mStartButton.emplace_back(b);	
}


//-----------------------------------------------------------------------------
//    �A�C�e�����j���[��Button�̒ǉ�
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//    Texture�̕`��
//-----------------------------------------------------------------------------
void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
	const Vector2& offset, float scale, bool flipY, int a)
{
	float yScale = static_cast<float>(texture->GetHeight()) * scale;


	//���̂݃T�C�Y��傫������
	if (a == 1) {
		yScale = static_cast<float>(texture->GetHeight()) * 1.0f;
	}
	//���]
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


//-----------------------------------------------------------------------------
//    �`���[�g���A���p�̃{�^���̕`��
//-----------------------------------------------------------------------------
void UIScreen::DrawButtonRight(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialRightButtonOn->GetWidth()),
		static_cast<float>(mTutorialRightButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(170.0f, -200.0f), dims, 0);
	mTutorialButtonRight = b;
}


//-----------------------------------------------------------------------------
//    �`���[�g���A���p�̃{�^���̕`��
//-----------------------------------------------------------------------------
void UIScreen::DrawButtonLeft(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialLeftButtonOn->GetWidth()),
		static_cast<float>(mTutorialLeftButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(-170.0f, -200.0f), dims, 0);
	mTutorialButtonLeft = b;
}


//-----------------------------------------------------------------------------
//    �`���[�g���A���p�̃{�^���̕`��
//-----------------------------------------------------------------------------
void UIScreen::DrawCloseButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mCloseButtonOn->GetWidth()),
		static_cast<float>(mCloseButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(320.0f, 250.0f), dims, 0);
	mCloseButton = b;
}


//-----------------------------------------------------------------------------
//   �}�E�X�̏�Ԃ��擾
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//  TutorialNum�����Z
//-----------------------------------------------------------------------------
void UIScreen::AddTutorialNum()
{ 
	if (TutorialNum < 4) {
		TutorialNum++;
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		
	}
	
}

//-----------------------------------------------------------------------------
//  TutorialNum�����Z
//-----------------------------------------------------------------------------
void UIScreen::RemoveTutorialNum()
{ 
	if (TutorialNum > 0) {
		TutorialNum--;
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
	}
}


///////////////////////////////////////////////////////////////////////////////
// Button class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//      �f�X�g���N�^�ł�.
//-----------------------------------------------------------------------------
Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}


//-----------------------------------------------------------------------------
//  Name Setter
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//  �{�^���̒��S����̏㉺���E�̒[�܂ł̋���
//-----------------------------------------------------------------------------
bool Button::ContainsPoint(const Vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y > (mPosition.y + mDimensions.y / 2.0f);
	return !no;
}


//-----------------------------------------------------------------------------
//  �N���b�N����
//-----------------------------------------------------------------------------
void Button::OnClick()
{
	if (mOnClick)
	{		
		mOnClick();
	}
}


