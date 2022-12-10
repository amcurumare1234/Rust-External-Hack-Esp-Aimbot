#include "aim.h"

bool InFov(class BasePlayer& BasePlayer_on_Aimming, enum BoneList bone)
{

	Vector2 ScreenPos;
	if (!myLocalPlayer.WorldToScreen(BasePlayer_on_Aimming.GetBonePosition(head), &ScreenPos)) return false;

	return Math::Calc2D_Dist(Vector2(Vars::Config::ScreenWidth / 2, Vars::Config::ScreenHigh / 2), ScreenPos) <= Vars::Aim::fov;
}


void Normalize(Vector2& angle)
{
	if (angle.x > 89)angle.x = 89;
	if (angle.x < -89)angle.x = -89;
	while (angle.y > 180)angle.y -= 360;
	while (angle.y < -180)angle.y += 360;
}


float GetBulletSpeed()
{
	switch (myLocalPlayer.myActiveWeapon.GetID())
	{
	case 1545779598: //ak47
		return 375.f;
	case 2482412119: //lr300
		return 375.f;
	case 3390104151: //semi-rifle
		return 375.f;
	case 28201841: //m39
		return 375.f* 1.16f;
	case 2225388408: //m249
		return 375.f * 1.4f;

	case 1588298435: //bolt
		return 375.f * 1.8f;
	case 3516600001: //l96	
		return 375.f * 3.2f;

	case 1318558775: //mp5a4
		return 300.f * 0.8f;
	case 1796682209: //smg
		return 300.f * 0.8f;
	case 2536594571: //thompson
		return 300.f;
	case 3442404277: //m92
		return 300.f;
	case 818877484: //semi-pistol
		return 300.f;
	case 1373971859: //python
		return 300.f;
	case 649912614: //revolver
		return 300.f;

	default:
		return 0.f;

		/*
	case 1801741824: //bolt
		return 656.25f;
	case 1798229440: //l96
		return 1125.f;
	case 1802481984: //m39
		return 469.f;
	case 1798228096: //revolver
		return 300.f;
	case 818877484: //p250
		return 300.f;
	case 1802077840: //waterpipe, 250-green ammo, 100-other
		return 100.f;
	case 1798227968: //double barrel, 250-green ammo, 100-other
		return 100.f;
		*/


	}
}


//unuse
double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity) {
	double pitch = (atan2(height, DepthPlayerTarget));
	double BulletVelocityXY = velocity * cos(pitch);
	double Time = DepthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time);
	return TotalVerticalDrop * 10;
}

Vector3 Prediction(const Vector3& my_Pos, BasePlayer& BasePlayer_on_Aimming, BoneList Bone)
{


	Vector3 BonePos = BasePlayer_on_Aimming.GetBonePosition(Bone);

	float Dist = Math::Calc3D_Dist(my_Pos, BonePos);

	float BulletSpeed= GetBulletSpeed();
	if (BulletSpeed <= 0)return BonePos;

	float BulletTime = Dist / BulletSpeed;
	Vector3 vel = BasePlayer_on_Aimming.GetVelocity();
	Vector3 Predict = vel * BulletTime;
	BonePos += Predict;


	float DegAngle = myLocalPlayer.GetBA().x;
	float coef = 1;
	if (DegAngle >= 10 && Dist <=100)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.1f;
	}
	else if(DegAngle>=0)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.9f;
	}
	BonePos.y += (6.4f * BulletTime * BulletTime) * coef;//4.94f


	//std::cout << "coef: "<< coef << std::endl;
	//std::cout << "DegAngle: " << DegAngle << std::endl;
	//BonePos.y -= BonePos.y*1.f / Dist;

	return  BonePos;
	
}

void GoToTarget(BasePlayer &BasePlayer_on_Aimming, BoneList bone)
{


	Vector3 Local = myLocalPlayer.GetBonePosition(head);
	Vector3 PlayerPos = Prediction(Local, BasePlayer_on_Aimming, bone);
	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - myLocalPlayer.GetBA();
	//std::cout << "Local:" << Local.x << " " << Local.y <<" "<< Local.z<< std::endl;


	Normalize(Offset);

	//std::cout << "Offset:" << Offset.x << " " << Offset.y << std::endl;
	Offset.x *= 1.0f - (Vars::Aim::smooth* 0.3+0.4);
    Offset.y *= 1.0f - (Vars::Aim::smooth *0.3+0.4);

	Vector2 AngleToAim = myLocalPlayer.GetBA() + Offset;
	myLocalPlayer.SetBA(AngleToAim);
}

void Aim(DWORD64& BasePlayer_on_Aimming)
{
	static BasePlayer Player;
	Player.set_addr(BasePlayer_on_Aimming);


	if (BasePlayer_on_Aimming && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{

		if (Player.IsDead())BasePlayer_on_Aimming = NULL;
		else
		{

			static int boneArr[6] = { head ,spine1 ,r_upperarm ,l_breast ,r_hand };
			static BoneList bone;
			static DWORD64 isBasePlayerChange = NULL;
			if (isBasePlayerChange != Player.get_addr())
			{
				if (Vars::Aim::randomBone)bone = BoneList(boneArr[int(rand() % 6)]);
				else bone = head;

				isBasePlayerChange = Player.get_addr();
			}

			static int start = 0;
			static int summ = 0;

			summ += clock() - start;
			start = clock();

			if (summ >= 20)
			{
				GoToTarget(Player, bone);
				summ = 0;
			}



			
			//увелививаем smooth-ухудшаем контроль отдачи
			static int RCSstart = 0;
			static int RCSsumm = 0;

			RCSsumm += clock() - RCSstart;
			RCSstart = clock();

			
			if (RCSsumm >= (0.2 + Vars::Aim::smooth * 0.3)*10   )
			{
				myLocalPlayer.SetRA({ 0.f,0.f });
				RCSsumm = 0;
			}
		



		}


	}
	else BasePlayer_on_Aimming=NULL;
}