//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include "Maths.h"
#include "SoundEvent.h"


///////////////////////////////////////////////////////////////////////////////
//Button class
///////////////////////////////////////////////////////////////////////////////
class Button
{
public:
	//=========================================================================
	// public variables.
	//=========================================================================
	//コンストラクタ
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims,int texNumber);
	//デストラクタ
	~Button();

	// Getter,setter
	class Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	bool GetHighlighted() const { return mHighlighted; }
	const int  GetTexNum() { return mTexNum; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	void SetName(const std::string& name);

	bool ContainsPoint(const Vector2& pt) const;
	void OnClick();

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	bool mHighlighted; //ボタンの上にマウスが重なっているか
	int mTexNum; 
	Vector2 mPosition;
	Vector2 mDimensions; //間隔

	class Texture* mNameTex;
	class Font* mFont;
	std::function<void()> mOnClick;
	std::string mName;

};


///////////////////////////////////////////////////////////////////////////////
// UIScreen class
///////////////////////////////////////////////////////////////////////////////
class UIScreen
{
public:
	//Enum UIState
	enum UIState
	{
		EActive,
		EClosing
	};

	//Enum State
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EItem,
		EQuit
	};

	//=========================================================================
	// public variables.
	//=========================================================================
	State currentState;

    //=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	UIScreen(class Game* game);

	//デストラクタ
	virtual ~UIScreen();

    //Upadate
	virtual void Update(float deltaTime);

	//入力
	virtual void ProcessInput(const uint8_t* keys);
	virtual void StartInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	//終了処理
	void Close();
	void CloseText();
	void CloseTutorial();

	//Getter,Setter
	UIState GetState() const { return mUIState; }
	int GetTutorialNum()const { return TutorialNum; }
	void SetTitle(const std::string& text,
				  const Vector3& color = Color::White,
				  int pointSize = 60);
	void SetItemText(const std::string& text, Vector2 pos, int pointSize,
		const Vector3& color = Color::White
	);

	//Add,Remove
	void AddText(const std::string& text, Vector2 pos, int pointSize,
		const Vector3& color = Color::White, int num = 0
		);
	void AddButton(const std::string& name, std::function<void()> onClick);
	void StartButton(const std::string& name, std::function<void()> onClick);
	void ItemButton(const std::string& name, int texNumber ,std::function<void()> onClick);
	void DrawButtonRight(const std::string& name, std::function<void()> onClick);
	void DrawButtonLeft(const std::string& name, std::function<void()> onClick);
	void DrawCloseButton(const std::string& name, std::function<void()> onClick);
	void AddTutorialNum();
	void RemoveTutorialNum();

	virtual void Draw(class Shader* shader);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	int texNum;
	int TutorialNum; //チュートリアルの番号
	bool mArrowFlag; //武器が変わるかの判定
	Vector2 mTitlePos; //タイトル位置
	Vector2 mNextButtonPos; //次のボタンの位置
	Vector2 mNextItemButtonPos; //次のアイテムボタンの位置
	Vector2 mNextTextPos; //次の画像の位置
	Vector2 mBGPos; //背景位置

	UIState mUIState;
	SoundEvent mMusicEvent;
	class Game* mGame;
	class Font* mFont;
	class Texture* mItemText;
	class Texture* mTitle;
	class Texture* mBackground;
	class Texture* mButtonOn;
	class Texture* mButtonOff;
	class Texture* mItemButtonOn;
	class Texture* mItemButtonOff;
	class Texture* mTutorialButtonOn;
	class Texture* mTutorialButtonOff;
	class Texture* mSword;
	class Texture* mBow;
	class Texture* mBomb;
	class Texture* mTorch;
	class Texture* mArrow;
	class Texture* mTutorialRightButtonOn;
	class Texture* mTutorialRightButtonOff;
	class Texture* mTutorialLeftButtonOn;
	class Texture* mTutorialLeftButtonOff;
	class Texture* mCloseButtonOn;
	class Texture* mCloseButtonOff;
	class Texture* mTutorial;
	class Texture* mTutorialTex0;
	class Texture* mTutorialTex1_1;
	class Texture* mTutorialTex1_2;
	class Texture* mTutorialTex1_3;
	class Texture* mTutorialTex2;
	class Button* mTutorialButtonRight;
	class Button* mTutorialButtonLeft;
	class Button* mCloseButton;
	std::vector<Texture*> mText;
	std::vector<Button*> mButtons;
	std::vector<Button*> mStartButton;
	std::vector<Button*> mItemButton;
	std::vector<Button*> mTutorialButton;

	//=========================================================================
    // private methods.
    //=========================================================================
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f,
		bool flipY = false,
		int a = 0); //画像の描画
	void SetRelativeMouseMode(bool relative); //マウスの状態を代入
};
