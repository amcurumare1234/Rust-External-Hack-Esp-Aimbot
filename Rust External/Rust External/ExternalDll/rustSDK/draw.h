#pragma once
#include "GuiEngine.h"
#include "rustSDK.h"
#include <iostream>
#include <string>

namespace DrawEsp
{
	bool InFov(Vector2& screenPos);
	void player_esp(BasePlayer player, LocalPlayer myPlayer, std::wstring name, bool isNPC=false);
}
