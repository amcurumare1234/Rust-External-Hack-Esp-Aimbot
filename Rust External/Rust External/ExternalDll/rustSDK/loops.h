#pragma once

#include "driverRW.h"
#include "tools.h"
#include "GuiEngine.h"
#include "offsets.h"
#include "myMath.h"
#include "vars.h"
#include "RustSDK.h"
#include "draw.h"
#include "aim.h"

struct objInf {
	DWORD64 objAddr;
	byte prefabNumber;
};

enum ObjList : byte
{
	player=1,
	corpse,
	backpack,
	npc,
	stone,
	sulfur,
	metal,
	hemp,
	world,
	supply,
	sam_site_turret,
	autoturret,
	guntrap,
	flameturret,
	cupboard,
	box_wooden_large,
	woodbox_deployed,
	small_stash,
	sleepingbag,
	minicopter,
	scraptransporthelicopter,
	rowboat,
	rhib
};
void BaseNetworkable_loop(DWORD64 BaseNetworkable);
void esp_drawner();
void GameObjectManager_loop(DWORD64 BaseUnityPlayer);
void misc();