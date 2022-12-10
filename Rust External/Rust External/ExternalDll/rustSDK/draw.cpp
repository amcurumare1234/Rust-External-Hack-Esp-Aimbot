#include "draw.h"


void Skeleton(BasePlayer* BasePlayer)
{
	BoneList Bones[15] = {
		/*LF*/l_foot, l_knee, l_hip,
		/*RF*/r_foot, r_knee, r_hip,
		/*BD*/spine1, neck, head,
		/*LH*/l_upperarm, l_forearm, l_hand,
		/*RH*/r_upperarm, r_forearm, r_hand
	};
	Vector2 BonesPos[15];

	//get bones screen pos
	for (int j = 0; j < 15; j++) {
		if (!myLocalPlayer.WorldToScreen(BasePlayer->GetBonePosition(Bones[j]), &BonesPos[j]))
			return;
	}

	//draw main lines
	for (int j = 0; j < 15; j += 3) {
		GuiEngine::Esp::Line(Vector2{ BonesPos[j].x, BonesPos[j].y }, Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, D2D1::ColorF::Azure);
		GuiEngine::Esp::Line(Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, Vector2{ BonesPos[j + 2].x, BonesPos[j + 2].y }, D2D1::ColorF::Azure);
	}

	//draw add lines
	GuiEngine::Esp::Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Azure);
	GuiEngine::Esp::Line(Vector2{ BonesPos[5].x, BonesPos[5].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Azure);
	GuiEngine::Esp::Line(Vector2{ BonesPos[9].x, BonesPos[9].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Azure);
	GuiEngine::Esp::Line(Vector2{ BonesPos[12].x, BonesPos[12].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Azure);
}

bool DrawEsp::InFov(Vector2& screenPos)
{
	return Math::Calc2D_Dist(Vector2(Vars::Config::ScreenWidth / 2, Vars::Config::ScreenHigh / 2), screenPos) <= Vars::Aim::fov;
}



void DrawEsp::player_esp(BasePlayer player, LocalPlayer myPlayer, std::wstring name,bool isNPC)
{
	Vector2 Vec2topBoxPos;

	Vector3 Vec3topBoxPos = player.GetBonePosition(head); Vec3topBoxPos.y += 0.35f;

	if (myLocalPlayer.WorldToScreen(Vec3topBoxPos, &Vec2topBoxPos))
	{

		static short dist;
		dist = Math::Calc3D_Dist(Vec3topBoxPos, myLocalPlayer.GetBonePosition(head));

		if (player.HasFlags(Sleeping))//если спит
		{
			if (Vars::Esp::playerSleepers)GuiEngine::Esp::CenterString(Vec2topBoxPos, (name + L" [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF(D2D1::ColorF::Orange));
			
			return;
		}

		GuiEngine::Esp::CenterString({ Vec2topBoxPos.x, Vec2topBoxPos.y - 18 }, (name + std::wstring(L" [")+ std::to_wstring(dist) + std::wstring(L"m") + std::wstring(L"]")).c_str()   );
		//GuiEngine::Esp::CenterString({ Vec2topBoxPos.x, Vec2topBoxPos.y - 10 }, (std::to_wstring(dist) + std::wstring(L"m")).c_str());
		if (isNPC)return;

		Vector2 Vec2playerFoot;
		myLocalPlayer.WorldToScreen(player.GetBonePosition(l_foot), &Vec2playerFoot);
		static short boxHeight;
		static short boxWidth;
		boxHeight = Vec2playerFoot.y - Vec2topBoxPos.y;
		if (boxHeight <= 5)boxHeight =5;
		boxWidth = boxHeight / 2;

		//Box
		if (Vars::Esp::drawBox)
		{
			GuiEngine::Esp::rect((Vec2topBoxPos.x) - (boxWidth + 2) / 2, (Vec2topBoxPos.y - 1), boxWidth + 2, boxHeight + 2, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
			if (player.IsVisible())GuiEngine::Esp::rect(Vec2topBoxPos.x - boxWidth / 2, Vec2topBoxPos.y, boxWidth, boxHeight, { 0,255,0,1 });
			else GuiEngine::Esp::rect(Vec2topBoxPos.x - boxWidth / 2, Vec2topBoxPos.y, boxWidth, boxHeight, { 255,0,0,1 });

		}
	
		if (Vars::Esp::drawHealthBar)
		{
			static float healthThick = 2;
			static float health_to_box_dist = 2;

			GuiEngine::Esp::Line(
				{
				(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
				Vec2topBoxPos.y + boxHeight + 1
				},
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					Vec2topBoxPos.y - 1
				},
				D2D1::ColorF(D2D1::ColorF::Black, 0.5f), healthThick + 2);

			static float health; health = player.GetHealth();
			static float myGreen; myGreen = health / 100.0f;
			static float myRed; myRed = 1.0f - myGreen;
			GuiEngine::Esp::Line(
				{
				(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
				Vec2topBoxPos.y + boxHeight
				},
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					(Vec2topBoxPos.y + boxHeight) - boxHeight * ((health / 100.0f) <= 1.0f ? (health / 100.0f) : (1.0f))
				},
				{ myRed,myGreen,0,1 }, healthThick);
		}

		
		
		if(Vars::Esp::drawActiveWeapons)GuiEngine::Esp::CenterString({ Vec2topBoxPos.x, Vec2topBoxPos.y + boxHeight + 6 }, player.get_active_weapon_name().c_str());

		if (Vars::Esp::drawSkeleton)Skeleton(&player);
	}

}

/*
	//Health Bar
		if (Vars::Esp::drawHealthBar)
		{
			static float healthThick = 2;
			static float health_to_box_dist = 2;

			static float minWidth = 20;

			GuiEngine::Esp::Line(
				{
				(Vec2topBoxPos.x - (boxWidth <= minWidth ? minWidth : boxWidth) / 2),
				Vec2topBoxPos.y - 5
				},
				{
					(Vec2topBoxPos.x + (boxWidth <= minWidth ? minWidth : boxWidth) / 2),
					Vec2topBoxPos.y - 5
				},
				D2D1::ColorF(D2D1::ColorF::Black, 0.5f), healthThick + 3);

			static float health; health = player.GetHealth();
			static float myGreen; myGreen = health / 100.0f;
			static float myRed; myRed = 1.0f - myGreen;
			GuiEngine::Esp::Line(
				{
				(Vec2topBoxPos.x - (boxWidth <= minWidth ? minWidth : boxWidth) / 2+1),
				Vec2topBoxPos.y - 5 
				},
				{ 
				(Vec2topBoxPos.x - (boxWidth <= minWidth ? minWidth : boxWidth) / 2 + 1) +    (    ((boxWidth <= minWidth ? minWidth : boxWidth) * ((health / 100.0f) <= 1.0f ? (health / 100.0f) : (1.0f)))   )
				,
				Vec2topBoxPos.y - 5 
				},
				{ myRed,myGreen,0,1 }, healthThick);
		}
*/