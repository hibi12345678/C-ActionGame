#pragma once
#include "Actor.h"

class TreeActor : public Actor
{
public:
	TreeActor(class Game* game);
	~TreeActor();


	TypeID GetType() const override { return TPlaneActor; }

private:

};
