#pragma once

#include "vars.h"

namespace Vars
{
	namespace Config
	{
		DWORD64 GameObjectManager = NULL;
		DWORD64 BaseNetworkable = NULL;

		int ScreenWidth = 1920;
		int ScreenHigh = 1080;

		std::atomic<short> espFPS = 0;
		std::atomic<short> menuFPS = 0;

		bool LocalPlayerIsValid = false;
		bool panic = false;
		bool MenuActive = false;
	}

	namespace  Aim
	{
		bool aim = false;
		bool randomBone = false;
	    DWORD64 addr_BasePlayer_on_Aimming=NULL;
		float fov = 10.0f;
		bool drawFov = false;
		float smooth = 0.2f;

		float recoil = 1.0f;
		bool NoSpread = false;
		bool NoSway = false;

		bool ModifyBullet = false;
		float fat = 0.1f;
	}

	namespace  Esp
	{
		//player
		bool playerEsp = false;
		bool drawBox = false;
		bool drawHealthBar = false;
		bool drawSkeleton = false;
		bool drawActiveWeapons = false;
		

		bool playerCorpse = false;
		bool playerSleepers = false;
		bool npcEsp = false;


		//loot
		 bool stone = false;
		 bool sulphur = false;
		 bool metal = false;
		 bool hemp = false;
		 bool dropItems = false;
		 bool airdrop = false;

		//etc
		bool landAirTurret = false;
		bool autoTurret = false;
	    bool guntrap = false;
		bool flameturret = false;

		bool cupboard = false;
		bool cupboardPlayer = false;
	    bool largeBox = false;
	    bool smallBox = false;
		bool stash = false;
		bool sleepingbag = false;

		bool minicopter = false;
		bool cow = false;
		bool boat = false;
		bool bigBoat = false;
		
	}

	namespace  Misc
	{
		bool walkWatter = false;
		bool spider = false;
		bool HigthJump = false;
		bool speedhack = false;
	    float speedBonus= 0.f;
		bool WatterBoost = false;
		bool WallWalk = false;

		bool ThirdPersonMode = false;
		bool AdminMode = false;
		bool superMelee = false;
		bool superEoka = false;
		bool alwaysDay = false;
	    float alwaysDay_float = 10.0f;

		bool crosshair = false;

	}



}