// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include "SoundEvent.h"
class Button
{
public:
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims,int texNumber);
	~Button();

	// Set the name of the button
	void SetName(const std::string& name);
	
	// Getters/setters
	class Texture* GetNameTex() { return mNameTex; }
	const Vector2& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }
	const int  GetTexNum() { return mTexNum; }
	// Returns true if the point is within the button's bounds
	bool ContainsPoint(const Vector2& pt) const;
	// Called when button is clicked
	void OnClick();
private:
	std::function<void()> mOnClick;
	std::string mName;
	class Texture* mNameTex;
	class Font* mFont;
	Vector2 mPosition;
	Vector2 mDimensions;
	int mTexNum;
	bool mHighlighted;
	class AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;

	
};

class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// UIScreen subclasses can override these
	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void StartInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);
	// Tracks if the UI is active or closing
	enum UIState
	{
		EActive,
		EClosing
	};
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EItem,
		EQuit
	};
	State currentState;
	
	// Set state to closing

	void Close();
	// Get state of UI screen
	UIState GetState() const { return mUIState; }
	// Change the title text
	void SetTitle(const std::string& text,
				  const Vector3& color = Color::White,
				  int pointSize = 60);
	void AddText(const std::string& text,Vector2 pos, int pointSize,
		const Vector3& color = Color::White
		);
	void CloseText();
	void SetItemText(const std::string& text, Vector2 pos, int pointSize,
		const Vector3& color = Color::White
	);
	// Add a button to this screen
	void AddButton(const std::string& name, std::function<void()> onClick);
	// Add a button to this screen
	void StartButton(const std::string& name, std::function<void()> onClick);
	void ItemButton(const std::string& name, int texNumber ,std::function<void()> onClick);

protected:
	// Helper to draw a texture
	void DrawTexture(class Shader* shader, class Texture* texture,
					 const Vector2& offset = Vector2::Zero,
					 float scale = 1.0f,
					 bool flipY = false,
	                 int a = 0);
	// Sets the mouse mode to relative or not
	void SetRelativeMouseMode(bool relative);
	class Game* mGame;
	
	class Font* mFont;
	class Texture* mTitle;
	class Texture* mBackground;
	class Texture* mButtonOn;
	class Texture* mButtonOff;
	class Texture* mItemButtonOn;
	class Texture* mItemButtonOff;
	class Texture* mSword;
	class Texture* mBow;
	class Texture* mBomb;
	class Texture* mTorch;
	class Texture* mArrow;
	bool mArrowFlag;
	// Configure positions
	Vector2 mTitlePos;
	Vector2 mNextButtonPos;
	Vector2 mNextItemButtonPos;
	Vector2 mNextTextPos;
	Vector2 mBGPos;
	int texNum;
	// State
	UIState mUIState;
	
	std::vector<Texture*> mText;
	class Texture* mItemText;
	std::vector<Button*> mButtons;

	std::vector<Button*> mStartButton;

	std::vector<Button*> mItemButton;
};
