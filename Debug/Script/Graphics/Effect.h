//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Effekseer.h"
#include "EffekseerRendererGL.h"


///////////////////////////////////////////////////////////////////////////////
// Effcet class
///////////////////////////////////////////////////////////////////////////////
class Effect {
public:
	Effect();
	~Effect();
	void DrawEffect();

private:

	Effekseer::ManagerRef manager;
	EffekseerRendererGL::RendererRef renderer;
    Effekseer::EffectRef effect;
	Effekseer::Handle efkHandle;
};