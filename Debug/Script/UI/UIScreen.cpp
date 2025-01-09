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
//      コンストラクタです.
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
//      デストラクタです.
//-----------------------------------------------------------------------------
UIScreen::~UIScreen()
{
	// mTitleの解放
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	// mItemTextの解放
	if (mItemText)
	{
		mItemText->Unload();
		delete mItemText;
	}

	// 各ボタンやテキストの解放
	for (auto b : mButtons) { delete b; }
	for (auto b : mStartButton) { delete b; }
	for (auto b : mText) { delete b; }
	for (auto b : mItemButton) { delete b; }

	// チュートリアルボタンの解放
	delete mTutorialButtonRight;
	delete mTutorialButtonLeft;
	delete mCloseButton;

	// コンテナのクリア
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
//   描画処理
//-----------------------------------------------------------------------------
void UIScreen::Draw(Shader* shader)
{
	// 背景の描画
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}

	// タイトルの描画
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}

	// ボタンの描画
	DrawUIButtons(shader, mButtons, mButtonOn, mButtonOff);
	DrawUIButtons(shader, mStartButton, mButtonOn, mButtonOff);
	DrawUIButtons(shader, mItemButton, mItemButtonOn, mItemButtonOff);

	// テキストの描画
	for (auto tex : mText)
	{
		DrawTexture(shader, tex, tex->GetPos());
	}

	// アイテムボタンの描画
	DrawUIItemButtons(shader);
	
	// アイテムテキストの描画
	if (mItemText)
	{
		if (mArrowFlag) {
			DrawTexture(shader, mArrow, Vector2(0.0f, -70.0f), 0.5f);
		}
		DrawTexture(shader, mItemText, mItemText->GetPos());
	}

	// チュートリアル画面の描画
	if (Game::GameState::ETutorial == mGame->GetState())
	{
		DrawUITutorial(shader);
	}

	// チュートリアルボタンの描画
	DrawUITutorialButtons(shader);

	// 閉じるボタンの描画
	DrawUICloseButton(shader);

	// アイテム画面の描画
	if (Game::GameState::EItem == mGame->GetState())
	{
		Texture* mEquipped = mFont->RenderText("Equipped", Color::White, 30);
		DrawTexture(shader, mEquipped, Vector2(-250.0f, -80.0f));

	}
}


//-----------------------------------------------------------------------------
// ボタンの描画処理
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
// アイテムボタンの描画処理
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
// チュートリアル画面の描画処理
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
// チュートリアルボタンの描画処理
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
// 閉じるボタンの描画処理
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
//   マウスの入力処理
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
//   マウス座標を取得して中心座標に変換する
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
//   ボタンのハイライト処理
//-----------------------------------------------------------------------------
void UIScreen::HighlightButtons(std::vector<Button*>& buttons, const Vector2& mousePos)
{
	for (auto b : buttons)
	{
		b->SetHighlighted(b->ContainsPoint(mousePos));
	}
}

//-----------------------------------------------------------------------------
//   アイテムボタンのハイライト処理とアイテムテキストの設定
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
//   アイテムボタンに対応するテキストを設定
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
//   チュートリアルボタンのハイライト処理
//-----------------------------------------------------------------------------
void UIScreen::HighlightTutorialButton(Button* button, const Vector2& mousePos)
{
	if (button)
	{
		button->SetHighlighted(button->ContainsPoint(mousePos));
	}
}

//-----------------------------------------------------------------------------
//   クローズボタンのハイライト処理
//-----------------------------------------------------------------------------
void UIScreen::HighlightCloseButton(Button* button, const Vector2& mousePos)
{
	if (button)
	{
		button->SetHighlighted(button->ContainsPoint(mousePos));
	}
}


//-----------------------------------------------------------------------------
//   MainMenuでのmStartButtonの処理
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
//    入力処理
//-----------------------------------------------------------------------------
void UIScreen::HandleKeyPress(int key)
{
	if (key == SDL_BUTTON_LEFT)
	{
		// ボタン配列の処理
		HandleButtonClick(mButtons);
		HandleButtonClick(mItemButton);
		HandleButtonClick(mStartButton);

		// 単一ボタンの処理
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
//    ボタンのクリック処理
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
//     UIを閉じる
//-----------------------------------------------------------------------------
void UIScreen::Close()
{
	mUIState = UIState::EClosing;
}


//-----------------------------------------------------------------------------
//     チュートリアルを閉じる
//-----------------------------------------------------------------------------
void UIScreen::CloseTutorial()
{
	if (TutorialNum == 3) {
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		mUIState = UIState::EClosing;
	}
	
}


//-----------------------------------------------------------------------------
//    タイトルの初期化
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
//    textの追加
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
//    textメモリを解放
//-----------------------------------------------------------------------------
void UIScreen::CloseText()
{
	for (auto b : mText) {
		delete b;
	}
	mText.clear();
}


//-----------------------------------------------------------------------------
//    Itemtextの追加
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
//    Buttonの追加
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
//    StartメニューのButtonの追加
//-----------------------------------------------------------------------------
void UIScreen::StartButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(0.0f, -100.0f), dims,0);	
	mStartButton.emplace_back(b);	
}


//-----------------------------------------------------------------------------
//    アイテムメニューのButtonの追加
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
//    Textureの描画
//-----------------------------------------------------------------------------
void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
	const Vector2& offset, float scale, bool flipY, int a)
{
	float yScale = static_cast<float>(texture->GetHeight()) * scale;


	//横のみサイズを大きくする
	if (a == 1) {
		yScale = static_cast<float>(texture->GetHeight()) * 1.0f;
	}
	//反転
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
//    チュートリアル用のボタンの描画
//-----------------------------------------------------------------------------
void UIScreen::DrawButtonRight(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialRightButtonOn->GetWidth()),
		static_cast<float>(mTutorialRightButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(170.0f, -200.0f), dims, 0);
	mTutorialButtonRight = b;
}


//-----------------------------------------------------------------------------
//    チュートリアル用のボタンの描画
//-----------------------------------------------------------------------------
void UIScreen::DrawButtonLeft(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mTutorialLeftButtonOn->GetWidth()),
		static_cast<float>(mTutorialLeftButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(-170.0f, -200.0f), dims, 0);
	mTutorialButtonLeft = b;
}


//-----------------------------------------------------------------------------
//    チュートリアル用のボタンの描画
//-----------------------------------------------------------------------------
void UIScreen::DrawCloseButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mCloseButtonOn->GetWidth()),
		static_cast<float>(mCloseButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(320.0f, 250.0f), dims, 0);
	mCloseButton = b;
}


//-----------------------------------------------------------------------------
//   マウスの状態を取得
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
//  TutorialNumを加算
//-----------------------------------------------------------------------------
void UIScreen::AddTutorialNum()
{ 
	if (TutorialNum < 4) {
		TutorialNum++;
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Tutorial");
		
	}
	
}

//-----------------------------------------------------------------------------
//  TutorialNumを減算
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
//      コンストラクタです.
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
//      デストラクタです.
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
//  ボタンの中心からの上下左右の端までの距離
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
//  クリック処理
//-----------------------------------------------------------------------------
void Button::OnClick()
{
	if (mOnClick)
	{		
		mOnClick();
	}
}


