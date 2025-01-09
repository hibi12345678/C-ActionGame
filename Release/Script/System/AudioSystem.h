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
#include <unordered_map>
#include <string>
#include "Maths.h"
#include "SoundEvent.h"



///////////////////////////////////////////////////////////////////////////////
//namespace FMOD
///////////////////////////////////////////////////////////////////////////////
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};


///////////////////////////////////////////////////////////////////////////////
//AudioSystem class
///////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//�R���X�g���N�^
	explicit AudioSystem(class Game* game);
	//�f�X�g���N�^
	~AudioSystem();
	
	//Update
	void Update(float deltaTime);

	//����������
	bool Initialize();
	void LoadBank(const std::string& name);

	//�I������
	void Shutdown();
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	//Getter,Setter
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
	void SetListener(const Matrix4& viewMatrix, const Vector3& currentPos, float deltaTime, const Vector3& listenerPosition);



	SoundEvent PlayEvent(const std::string& name);

protected:
	//=========================================================================
	// protected variables.
	//=========================================================================
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	static unsigned int sNextID;
	class Game* mGame;
	// Map of loaded banks
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks; 
	// Map of event name to EventDescription
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// Map of event id to EventInstance
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	// FMOD studio system
	FMOD::Studio::System* mSystem;
	// FMOD Low-level system (in case needed)
	FMOD::System* mLowLevelSystem;
	// �O��t���[���̈ʒu
	Vector3 mPreviousPos = Vector3(0.0f, 0.0f, 0.0f); 
};