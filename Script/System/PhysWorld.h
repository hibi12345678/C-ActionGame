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
#include <vector>
#include <functional>
#include "Collision.h"
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
// PhysWorld class
///////////////////////////////////////////////////////////////////////////////
class PhysWorld
{
public:
	//CollisioinInfo Struct
	struct CollisionInfo
	{
		// Point of collision
		Vector3 mPoint;
		// Normal at collision
		Vector3 mNormal;
		// Component collided with
		class BoxComponent* mBox;
		// Owning actor of component
		class Actor* mActor;
	};

	//=========================================================================
    // public methods.
    //=========================================================================
	//コンストラクタ
	PhysWorld(class Game* game);

	//Getter,Setter
	const std::vector<class BoxComponent*>& PhysWorld::GetBoxComponents() const { return mBoxes; }

    //Add,Remove
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);
	void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
};
