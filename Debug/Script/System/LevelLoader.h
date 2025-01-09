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
#include <rapidjson/document.h>
#include <functional>
#include <string>
#include <unordered_map>
#include "Maths.h"


using ActorFunc = std::function<class Actor*(class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<
	class Component*(class Actor*, const rapidjson::Value&)
>;

///////////////////////////////////////////////////////////////////////////////
//LevelLoder class
///////////////////////////////////////////////////////////////////////////////
class LevelLoader
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	static bool LoadLevel(class Game* game, const std::string& fileName);
	static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
	static void SaveLevel(class Game* game, const std::string& fileName);

protected:
    //=========================================================================
	// private methods.
	//=========================================================================
	// Helper to load global properties
	static void LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
	// Helper to load in actors
	static void LoadActors(class Game* game, const rapidjson::Value& inArray);
	// Helper to load in components
	static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);
	// Maps for data
	static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;
	// Helper to save global properties
	static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, 
		class Game* game, rapidjson::Value& inObject);
	// Helper to save actors
	static void SaveActors(rapidjson::Document::AllocatorType& alloc,
		class Game* game, rapidjson::Value& inArray);
	// Helper to save components
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc,
		const class Actor* actor, rapidjson::Value& inArray);
};


///////////////////////////////////////////////////////////////////////////////
//JsonHelper class
///////////////////////////////////////////////////////////////////////////////
class JsonHelper
{
public:
	// Helpers - Return true if successful, and also sets out parameter to parsed value
	// For each function, the first parameter is the containing JSON object, the second is the
	// name of the property in the containing object, and the third is the value you acquire.
	// Furthermore, if the property is not found, each function is guaranteed not to modify the
	// return value.
	static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
	static bool GetQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);

	// Setter functions
	static void AddInt(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, float value);
	static void AddString(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const std::string& value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, bool value);
	static void AddVector3(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const Vector3& value);
	static void AddQuaternion(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const Quaternion& value);
};
