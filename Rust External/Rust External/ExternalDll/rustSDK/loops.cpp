#include "loops.h"
#include <future>
using namespace std;


//обновляет адресса обьектов (как правила они не меняются в процессе игры)
void GameObjectManager_loop(DWORD64 ObjMgr)
{
	/*
	for (DWORD64 BaseObject = read(ObjMgr + 0x18, DWORD64);; BaseObject = read(BaseObject + 0x8, DWORD64))
	{
		
		DWORD64 GameObject = read(BaseObject + 0x10, DWORD64);
		WORD Tag = read(GameObject + 0x54, WORD);

		DWORD64 ObjectClass = read(GameObject + 0x30, DWORD64);
		DWORD64	Entity = read(ObjectClass + 0x18, DWORD64);

		if (Tag == 5)
		{
			//std::cout << 5 << std::endl;
			myLocalPlayer.ObjectClassCamera = ObjectClass;
			myLocalPlayer.BaseEntityCamera = Entity;
			break;
		}

	}
	*/
	for (DWORD64 BaseObject = read(ObjMgr + 0x8, DWORD64); BaseObject && BaseObject != read(ObjMgr, DWORD64); BaseObject = read(BaseObject + 0x8, DWORD64))
	{
		DWORD64 GameObject = read(BaseObject + 0x10, DWORD64);
		WORD Tag = read(GameObject + 0x54, WORD);

		DWORD64 ObjectClass = read(GameObject + 0x30, DWORD64);
		DWORD64	Entity = read(ObjectClass + 0x18, DWORD64);


		if (Tag == 6)
		{
			//std::cout <<6 << std::endl;
			//cout << "entity" << endl;
		}

		//camera
		else if (Tag == 5)
		{
		//	std::cout << 5 << std::endl;
			 
			//std::cout << GameObject << std::endl;
		    //std::cout << ObjectClass << std::endl;
			//std::cout << Entity << std::endl;

			myLocalPlayer.ObjectClassCamera = ObjectClass;
			myLocalPlayer.BaseEntityCamera = Entity;
		}


		//sky
		else if (Tag == 20011)
		{
			//std::cout << 20011 << std::endl;

			DWORD64 Dome = read(Entity + 0x28, DWORD64);
			myLocalPlayer.TodCycle = read(Dome + 0x38, DWORD64);

		}


	}
}


static bool localPlayerIsExist = false;
static vector<objInf> draw_entities = {};
static std::mutex drawnerMtx;
void BaseNetworkable_loop(DWORD64 BaseNetworkable)
{

	DWORD64 EntityRealm = read(BaseNetworkable + 0xB8, DWORD64);
	DWORD64 ClientEntities = read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = read(ClientEntities_list + 0x28, DWORD64);

	if (!ClientEntities_values) return;
	int EntityCount = read(ClientEntities_values + 0x10, int);

	DWORD64 EntityBuffer = read(ClientEntities_values + 0x18, DWORD64);

	localPlayerIsExist = false;
	vector<objInf> BN_draw_entities = {};
	for (int i = 0; i < EntityCount; i++)
	{
		DWORD64 Entity = read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);//BaseEntity
		//std::cout<<"Entity:" << Entity << std::endl;
		if (Entity <= 100000) continue;

		DWORD64 Object = read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;
		
		DWORD64 ObjectClass = read(Object + 0x30, DWORD64); //CGameObject
		if (ObjectClass <= 100000) continue;

		if (read(ObjectClass + 0x54, int16_t)==6)
		{
			std::cout << "Object:" << Object << std::endl;
		}

		DWORD64 addr_name = read(ObjectClass + 0x60, DWORD64);

		static char BNname[110] = { '\0' }; //имя текущего обьекта 
		kernelHandler.read_memory(addr_name, (uint64_t)BNname, sizeof(BNname));
		BNname[109] = { '\0' };


	

		objInf currentObj;
		if (strstr(BNname, "LocalPlayer"))
		{
		
			localPlayerIsExist = true;
			myLocalPlayer.set_addr(read(Object + 0x28, DWORD64));
				//std::cout <<std::hex<< myLocalPlayer.get_addr() << std::endl;
			/*
			DWORD64 Movement = read(myLocalPlayer.get_addr() + oBaseMovement, DWORD64);
			std::cout << "Movement  : " << Movement << std::endl;
			//vel.y += 5.0f;
			//vel.x += 5.0f;
			//vel.z += 5.0f;
			//write(Movement + 0x110, vel, Vector3);
			Vector3 vel = read(Movement + 0xD4, Vector3);
			std::cout << "previousVelocity  : " << vel.x <<" "<< vel.y<<" "<< vel.z << std::endl;

			DWORD64 TimeSince = read(Movement + 0x128, DWORD64);
			std::cout << "Movement + 0x128  : " << Movement + 0x128 << std::endl;
			float Time = read(TimeSince, float);
			std::cout << "Time  : " << Time << std::endl;

			*/

			//std::cout << std::hex <<"Object:"<< Object << std::endl;
		//	std::cout <<std::hex<< myLocalPlayer.get_addr() << std::endl;
			/*
			DWORD64	Transform = read(myLocalPlayer.ObjectClassCamera + 0x8, DWORD64);
			DWORD64 Vec = read(Transform + 0x38, DWORD64);
			Vector3 pos = read(Vec + 0x90, Vector3);
			pos.y += 40;
			write(Vec + 0x90, pos);
			*/
			//std::cout << std::hex << Transform << std::endl;
			//std::cout << pos.y << std::endl;
			//Vars::Config::MenuActive = myLocalPlayer.IsMenu();

			//Vector2 BA = myLocalPlayer.GetBA();
			//Vector2 RA = myLocalPlayer.GetRA();
			//std::cout << "BA:"<< BA.x<<" "<< BA.y << std::endl;

			//std::cout << "RA:" << RA.x << " " << RA.y << std::endl;



			//std::cout <<"Active ID:" <<myLocalPlayer.myActiveWeapon.GetID()<< std::endl;


			//myLocalPlayer.myActiveWeapon.fatbullet();
			//Vector3 pos = myLocalPlayer.GetBonePosition(head);
			//std::cout << "my pos    : " << pos.x <<" "<< pos.y<<" "<< pos.z << std::endl;
		}
		
	/*
		currentObj.objAddr = Object;
		BN_draw_entities.push_back(currentObj);
		continue;
		if (1);
	*/
		//entity
		else if (Vars::Esp::playerEsp && strstr(BNname, "player.prefab") && !strstr(BNname, "prop") && !strstr(BNname, "corpse"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::player;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::playerCorpse && strstr(BNname, "player_corpse.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::corpse;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::playerCorpse && strstr(BNname, "item_drop_backpack.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::backpack;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::npcEsp && (strstr(BNname, "scientist") && !strstr(BNname, "corpse")))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::npc;
			BN_draw_entities.push_back(currentObj);
		}

		//loot
		else if (Vars::Esp::stone && strstr(BNname, "stone-ore.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::stone;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::sulphur && strstr(BNname, "sulfur-ore.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::sulfur;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::metal && strstr(BNname, "metal-ore.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::metal;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::hemp && strstr(BNname, "hemp-collectable.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::hemp;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::dropItems && strstr(BNname, "(world)"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::world;
			BN_draw_entities.push_back(currentObj);
		}

		else if (Vars::Esp::airdrop && strstr(BNname, "supply_drop.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::supply;
			BN_draw_entities.push_back(currentObj);
		}

		//etc
		else if (Vars::Esp::landAirTurret && strstr(BNname, "sam_site_turret_deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::sam_site_turret;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::autoTurret && strstr(BNname, "autoturret_deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::autoturret;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::guntrap && strstr(BNname, "guntrap.deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::guntrap;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::flameturret && strstr(BNname, "flameturret.deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::flameturret;
			BN_draw_entities.push_back(currentObj);
		}

		else if (Vars::Esp::cupboard && strstr(BNname, "cupboard.tool.deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::cupboard;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::largeBox && strstr(BNname, "box.wooden.large.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::box_wooden_large;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::smallBox && strstr(BNname, "woodbox_deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::woodbox_deployed;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::stash && strstr(BNname, "small_stash_deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::small_stash;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::sleepingbag && strstr(BNname, "sleepingbag_leather_deployed.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::sleepingbag;
			BN_draw_entities.push_back(currentObj);
		}

		else if (Vars::Esp::minicopter && strstr(BNname, "minicopter.entity.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::minicopter;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::cow && strstr(BNname, "scraptransporthelicopter.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::scraptransporthelicopter;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::boat && strstr(BNname, "rowboat.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::rowboat;
			BN_draw_entities.push_back(currentObj);
		}
		else if (Vars::Esp::bigBoat && strstr(BNname, "rhib.prefab"))
		{
			currentObj.objAddr = Object;
			currentObj.prefabNumber = ObjList::rhib;
			BN_draw_entities.push_back(currentObj);
		}
	}

	drawnerMtx.lock();
	draw_entities.clear();
	draw_entities = BN_draw_entities;
	drawnerMtx.unlock();


	Vars::Config::LocalPlayerIsValid = (localPlayerIsExist == true ? true : false);
	if (localPlayerIsExist == true)
	{
		Vars::Config::LocalPlayerIsValid = true;
	}
	else
	{
		Vars::Config::LocalPlayerIsValid = false;
		myLocalPlayer.set_addr(NULL);
		myLocalPlayer.BaseEntityCamera = NULL;
		myLocalPlayer.TodCycle = NULL;
	}
}


void esp_drawner()
{

	if (Vars::Config::LocalPlayerIsValid && myLocalPlayer.update_view_matrix())
	{
		GuiEngine::Esp::String({ 10, 55 }, (L"Entity draw count:" + to_wstring(draw_entities.size())).c_str());

		drawnerMtx.lock();

		for (int i = 0; i < draw_entities.size(); i++)
		{
			DWORD64 ObjectClass = read(draw_entities[i].objAddr + 0x30, DWORD64);
			if (ObjectClass <= 100000) continue;


			
		/*
			int16_t tag = read(ObjectClass + 0x54, int16_t);
			DWORD64 addr_name = read(ObjectClass + 0x60, DWORD64);

			static char ESPname[110]; //имя текущего обьекта
			kernelHandler.read_memory(addr_name, (uint64_t)ESPname, sizeof(ESPname));
			ESPname[109] = { '\0' };

			DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64); //Tag 449
			if (!gameObject)continue;
			DWORD64 Trans = read(gameObject + 0x8, DWORD64);
			if (!Trans)continue;
			DWORD64 Vec = read(Trans + 0x38, DWORD64);
			Vector3 pos = read(Vec + 0x90, Vector3);

			Vector2 Pos1;

			if (myLocalPlayer.WorldToScreen(pos, &Pos1))
			{
				//Pos1.y -= i*10;
				int wchars_num = MultiByteToWideChar(CP_UTF8, 0, ESPname, -1, NULL, 0);
				wchar_t* wstr = new wchar_t[wchars_num];
				MultiByteToWideChar(CP_UTF8, 0, ESPname, -1, wstr, wchars_num);
				GuiEngine::Esp::String(Pos1, (std::to_wstring(tag) +L":"+wstr).c_str(), D2D1::ColorF::Red);
				delete[] wstr;
				
			}
		
			
			continue;
	*/

	//entity
			if (Vars::Esp::playerEsp && (draw_entities[i].prefabNumber == (byte)ObjList::player))
			{
				static BasePlayer Player;
				Player.set_addr(read(draw_entities[i].objAddr + 0x28, DWORD64));
				if (!read(Player.get_addr() + oPlayerModel, DWORD64)) continue;
				if (Player.IsDead())continue;

				if (Vars::Aim::aim
					&& !Vars::Aim::addr_BasePlayer_on_Aimming
					&& Player.IsVisible()
					&& !Player.HasFlags(Sleeping)
					&& InFov(Player, head))
				{
					Vars::Aim::addr_BasePlayer_on_Aimming = Player.get_addr();
				}

				/*
				if (Player.GetName() == L"_DIKTOR_")
				{
					Vector3 pos = Player.GetBonePosition(head);
					std::cout << "Player pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
				}
				 */


				DrawEsp::player_esp(Player, myLocalPlayer, Player.GetName());
			}
			else if (Vars::Esp::playerCorpse && (draw_entities[i].prefabNumber == (byte)ObjList::corpse || draw_entities[i].prefabNumber == (byte)ObjList::backpack))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64); //Tag 449
				if (!gameObject)continue;
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				if (!Trans)continue;
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos;
				if (myLocalPlayer.WorldToScreen(pos, &Pos))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos, (L"Corpse [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
					//GuiEngine::Esp::CenterString(Pos,L"Corpse", D2D1::ColorF::Red);
				}


			}
			//&& (strstr(ESPname, "scientistnpc.prefab") || strstr(ESPname, "scientist_astar_full_any.prefab") || strstr(ESPname, "scientist_turret_any.prefab") || strstr(ESPname, "heavyscientist.prefab"))
			else if (Vars::Esp::npcEsp && (draw_entities[i].prefabNumber == (byte)ObjList::npc))
			{
				static BasePlayer player;
				player.set_addr(read(draw_entities[i].objAddr + 0x28, DWORD64)); //берем BasePlayer игрока
				if (!read(player.get_addr() + 0x4A8, DWORD64)) continue;//проверяем тектуру

				DrawEsp::player_esp(player, myLocalPlayer, L"NPC", true);
			}

			//loot 
			else if (Vars::Esp::stone && (draw_entities[i].prefabNumber == (byte)ObjList::stone))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"stone [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Brown);

				}
			}
			else if (Vars::Esp::sulphur && (draw_entities[i].prefabNumber == (byte)ObjList::sulfur))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"sulfur [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Gold);

				}
			}
			else if (Vars::Esp::metal && (draw_entities[i].prefabNumber == (byte)ObjList::metal))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"metal [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Orange);

				}
			}
			else if (Vars::Esp::hemp && (draw_entities[i].prefabNumber == (byte)ObjList::hemp))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"hemp [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Green);

				}
			}
			else if (Vars::Esp::dropItems && (draw_entities[i].prefabNumber == (byte)ObjList::world))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				//std::cout << gameObject << std::endl;
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);
				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					std::wstring name;
					DWORD64 Entity = read(draw_entities[i].objAddr + 0x28, DWORD64);
					DWORD64 Item = read(Entity + 0x150, DWORD64);
					DWORD64 ItemDefinition = read(Item + 0x20, DWORD64);
					DWORD64 TranslatePhrase = read(ItemDefinition + 0x28, DWORD64);
					DWORD64 addr_name = read(TranslatePhrase + 0x18, DWORD64);
					_UncStr Str = read(addr_name, _UncStr);
					for (int i = 0; i < Str.len && i < 20; i++)
					{
						name += Str.str[i];
					}

					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (name + L" [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Violet);

				}
			}
			else if (Vars::Esp::airdrop && (draw_entities[i].prefabNumber == (byte)ObjList::supply))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"supply drop [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Purple);
				}
			}

			//etc
			else if (Vars::Esp::landAirTurret && (draw_entities[i].prefabNumber == (byte)ObjList::sam_site_turret))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"land-air turret [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}
			else if (Vars::Esp::autoTurret && (draw_entities[i].prefabNumber == (byte)ObjList::autoturret))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"autoturret [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);

				}
			}
			else if (Vars::Esp::guntrap && (draw_entities[i].prefabNumber == (byte)ObjList::guntrap))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"guntrap [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}
			else if (Vars::Esp::flameturret && (draw_entities[i].prefabNumber == (byte)ObjList::flameturret))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"flameturret [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}

			else if (Vars::Esp::cupboard && (draw_entities[i].prefabNumber == (byte)ObjList::cupboard))
			{
				//std::cout << std::hex << read(ObjectClass + 0x28, DWORD64)<<std::endl ;
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					if (Vars::Esp::cupboardPlayer && DrawEsp::InFov(Pos1))
					{
						DWORD64 Entity = read(draw_entities[i].objAddr + 0x28, DWORD64);
						std::cout << Entity << std::endl;
						DWORD64 Autorized = read(Entity + 0x580, DWORD64);
						DWORD countPlayer = read(Autorized + 0x18, DWORD);
						if (countPlayer > 0)
						{
							DWORD64 items = read(Autorized + 0x10, DWORD64);

							float h = 10.0f, w = 10.0f;
							countPlayer > 0 ? h = countPlayer * 15 : h;
							//считаем самую длинную строку
							for (int i = 0; i < countPlayer; i++)
							{
								DWORD64 item = read(items + 0x20 + (i * 0x8), DWORD64);
								DWORD64 string_struct = read(item + 0x18, DWORD64);
								_UncStr structure = read(string_struct, _UncStr);
								if (structure.len * 10 > w)w = structure.len * 10;
							}

							GuiEngine::Esp::fill_rect(Pos1.x - w / 2, Pos1.y - h, w, h, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
							for (int i = 0; i < countPlayer; i++)
							{
								DWORD64 item = read(items + 0x20 + (i * 0x8), DWORD64);
								DWORD64 string_struct = read(item + 0x18, DWORD64);
								_UncStr structure = read(string_struct, _UncStr);
								std::wstring name = {};
								for (int i = 0; i < structure.len && i < 20; i++)
								{
									name += structure.str[i];
								}

								GuiEngine::Esp::CenterString({ Pos1.x,Pos1.y - (15 + i * 15) + 5 }, name.c_str());
							}
						}
						else
						{
							GuiEngine::Esp::fill_rect(Pos1.x - 20, Pos1.y - 8, 45, 18, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
							GuiEngine::Esp::CenterString(Pos1, L"Empty", D2D1::ColorF::White);
						}

					}
					else
					{
						static short dist;
						dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
						GuiEngine::Esp::CenterString(Pos1, (L"cupboard [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
					}
				}
			}
			else if (Vars::Esp::largeBox && (draw_entities[i].prefabNumber == (byte)ObjList::box_wooden_large))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64); //Tag 449
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"large box [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::smallBox && (draw_entities[i].prefabNumber == (byte)ObjList::woodbox_deployed))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"small box [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::stash && (draw_entities[i].prefabNumber == (byte)ObjList::small_stash))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"small stash [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::sleepingbag && (draw_entities[i].prefabNumber == (byte)ObjList::sleepingbag))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"sleeping bag [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}

			else if (Vars::Esp::minicopter && (draw_entities[i].prefabNumber == (byte)ObjList::minicopter))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"minicopter [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::cow && (draw_entities[i].prefabNumber == (byte)ObjList::scraptransporthelicopter))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"begemot [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::boat && (draw_entities[i].prefabNumber == (byte)ObjList::rowboat))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"boat [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::boat && (draw_entities[i].prefabNumber == (byte)ObjList::rhib))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"big boat [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}

		}
		drawnerMtx.unlock();

		if (Vars::Misc::crosshair)GuiEngine::Esp::Crosshair(Vars::Config::ScreenWidth, Vars::Config::ScreenHigh, { 255,255,255,0.7f }, 1.5f);
		if (Vars::Aim::drawFov)GuiEngine::Esp::Сircle({ (float)Vars::Config::ScreenWidth / 2,(float)Vars::Config::ScreenHigh / 2 }, { 255,255,255,0.7f }, Vars::Aim::fov, 1.f);
	}
}



/*
int wchars_num = MultiByteToWideChar(CP_UTF8, 0, ESPname, -1, NULL, 0);
wchar_t* wstr = new wchar_t[wchars_num];
MultiByteToWideChar(CP_UTF8, 0, ESPname, -1, wstr, wchars_num);
delete[] wstr;
*/
void misc()
{
	if (Vars::Config::LocalPlayerIsValid)
	{

		if (Vars::Misc::spider)myLocalPlayer.spider();
		if (Vars::Misc::alwaysDay)myLocalPlayer.alwaysDay(Vars::Misc::alwaysDay_float);
		if (Vars::Misc::speedhack)myLocalPlayer.speedhack();




		//amdin
		static bool trigger_AdminMode = false;
		if (Vars::Misc::AdminMode)
		{

			myLocalPlayer.set_player_flags(4, 0);
			trigger_AdminMode = true;
		}
		else if (trigger_AdminMode == true)
		{
			myLocalPlayer.set_player_flags(0, 4);
			trigger_AdminMode = false;
		}
		//___________

		static bool trigger_ThirdPersonMode = false;
		if (Vars::Misc::ThirdPersonMode)
		{
			myLocalPlayer.set_player_flags(1024, 0);
			trigger_ThirdPersonMode = true;
		}
		else if (trigger_ThirdPersonMode == true)
		{
			myLocalPlayer.set_player_flags(0, 1024);
			trigger_ThirdPersonMode = false;
		}

		static bool trigger_WatterBoost = false;
		if (Vars::Misc::WatterBoost)
		{
			myLocalPlayer.water_boost(2.0f);
			trigger_WatterBoost = true;
		}
		else if (trigger_WatterBoost == true)
		{
			myLocalPlayer.water_boost(0.0f);
			trigger_WatterBoost = false;
		}

		static bool trigger_WalkWatter = false;
		if (Vars::Misc::walkWatter)
		{
			myLocalPlayer.WalkWater(true);
			trigger_WalkWatter = true;
		}
		else if (trigger_WalkWatter == true)
		{
			myLocalPlayer.WalkWater(false);
			trigger_WalkWatter = false;
		}

		static bool trigger_HigthWalk = false;
		if (Vars::Misc::WallWalk)
		{

			myLocalPlayer.wall_walk(true);
			trigger_HigthWalk = true;
		}
		else if (trigger_HigthWalk == true)
		{
			myLocalPlayer.wall_walk(false);
			trigger_HigthWalk = false;
		}


		static bool trigger_HigthJump = false;
		if (Vars::Misc::HigthJump)
		{
			myLocalPlayer.SetGravity(1.4f);
			trigger_HigthJump = true;
		}
		else if (trigger_HigthJump == true)
		{
			myLocalPlayer.SetGravity(2.5f);
			trigger_HigthJump = false;
		}

		static bool trigger_NoSway = false;
		if (Vars::Aim::NoSway)
		{
			myLocalPlayer.NoSway(0.99f);
			trigger_NoSway = true;
			//std::cout << "NoSway on" << std::endl;
		}
		else if (trigger_NoSway == true)
		{
			//std::cout << "NoSway off" << std::endl;
			myLocalPlayer.NoSway(0.0f);
			trigger_NoSway = false;
		}

	}
}


