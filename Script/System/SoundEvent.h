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
#include <string>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//class
///////////////////////////////////////////////////////////////////////////////
class SoundEvent
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	SoundEvent();

	//Getter,Setter
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	void Set3DAttributes(const Matrix4& worldTrans);

	//座標
	bool Is3D() const;

	bool IsValid();
	void Restart();
	void Stop(bool allowFadeOut = true);

protected:
	// Make this constructor protected and AudioSystem a friend
	// so that only AudioSystem can access this constructor.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	//=========================================================================
	// private variables.
	//=========================================================================
	unsigned int mID;
	class AudioSystem* mSystem;
};