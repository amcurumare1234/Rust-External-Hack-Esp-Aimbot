#include "RustSDK.h"
#include <iostream>
LocalPlayer myLocalPlayer;




#pragma region BasePlayer

void BasePlayer::set_addr(DWORD64 ptr)
{
	if (this_ptr == ptr)return;
	this_ptr = ptr;
};

DWORD64 BasePlayer::get_addr()
{
	return this_ptr;
}

bool BasePlayer::HasFlags(int Flg)
{
	if (!this_ptr)return 0;
	return (read(this_ptr + oPlayerFlags, int) & Flg);
}

bool BasePlayer::IsDead()
{
	if (!this_ptr)return 0;
	return read(this_ptr + oLifeState, bool);;
}


bool BasePlayer::IsVisible()
{
	if (!this_ptr)return 0;
	DWORD64 PlayerModel = read(this_ptr + oPlayerModel, DWORD64);
	return read(PlayerModel + oVisible, bool);

}

float BasePlayer::GetHealth() {
	if (!this_ptr)return 0.f;

	return read(this_ptr + oHealth, float);
}

std::wstring BasePlayer::GetName() {
	if (!this_ptr)return L"Null str";
	_UncStr structure = read(read(this_ptr + oDisplayName, DWORD64), _UncStr);
	std::wstring name = {};

	for (int i = 0; i < structure.len && i < 20; i++)
	{
		name += structure.str[i];
	}

	return  name;
}

Vector3 BasePlayer::GetVelocity() {
	if (!this_ptr)return { 0.f,0.f, 0.f };
	DWORD64 PlayerModel = read(this_ptr + oPlayerModel, DWORD64);
	//	kernelHandler.read_memory(src, (uint64_t)&ret, sizeof(type));
	return read(PlayerModel + oNewVelocity, Vector3);
}

DWORD64  BasePlayer::GetTransform(int bone)
{
	if (!this_ptr)return 0;
	DWORD64 player_model = read(this_ptr + oModel, DWORD64);// public Model model;
	DWORD64 array_boneTransforms = read(player_model + oboneTransforms, DWORD64);//public Transform[] boneTransforms; public class Model : MonoBehaviour, IPrefabPreProcess // TypeDefIndex: 9383
	//std::cout << std::hex << "array_boneTransforms: " << array_boneTransforms << std::endl;
	DWORD64 BoneValue = read((array_boneTransforms + (0x20 + (bone * 0x8))), DWORD64);

	return read(BoneValue + 0x10, DWORD64);
}




struct TransformData
{
	uint64_t pTransformArray;
	uint64_t pTransformIndices;
};
struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};


Vector3 BasePlayer::GetPosition(DWORD64 pTransform)
{
	if (!pTransform || !this_ptr) return Vector3{ 0.f, 0.f, 0.f };

	static struct Matrix34
	{
		BYTE vec0[16];
		BYTE vec1[16];
		BYTE vec2[16];
	};

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	unsigned int Index = read(pTransform + 0x40, unsigned int);

	if (Index >= 1000 || Index <= 0)
	{
		//std::cout << "invalid index: " << Index << std::endl;
		return Vector3{ 0.f, 0.f, 0.f };
	}


	DWORD64 pTransformData = read(pTransform + 0x38, DWORD64);


	DWORD64 transformData[2];

	kernelHandler.read_memory(pTransformData + 0x18, (DWORD64)transformData, 16);

	size_t sizeMatriciesBuf = 48 * Index + 48;
	size_t sizeIndicesBuf = 4 * Index + 4;


	Matrix34* pMatriciesBuf = new Matrix34[sizeMatriciesBuf];
	int* pIndicesBuf = new int[sizeIndicesBuf];


	kernelHandler.read_memory(transformData[0], (ULONG_PTR)pMatriciesBuf, sizeMatriciesBuf);
	kernelHandler.read_memory(transformData[1], (ULONG_PTR)pIndicesBuf, sizeIndicesBuf);


	__m128 result = *(__m128*)((ULONG_PTR)pMatriciesBuf + 0x30 * Index);
	int transformIndex = *(int*)((ULONG_PTR)pIndicesBuf + 0x4 * Index);

	byte pSafe = 0;
	while (transformIndex >= 0 && pSafe++ < 200)
	{
		Matrix34 matrix34 = *(Matrix34*)((ULONG_PTR)pMatriciesBuf + 0x30 * transformIndex);

		__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));
		__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));
		__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));
		__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));
		__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));
		__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));
		__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

		result = _mm_add_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))), tmp7)), *(__m128*)(&matrix34.vec0));

		transformIndex = *(int*)((ULONG_PTR)pIndicesBuf + 0x4 * transformIndex);
	}
	delete[]pMatriciesBuf;
	delete[]pIndicesBuf;
	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}


Vector3 BasePlayer::GetBonePosition(BoneList BoneID)
{
	if (!this_ptr)return { 0.f,0.f, 0.f };
	return GetPosition(GetTransform(BoneID));
}

std::wstring BasePlayer::get_active_weapon_name()
{
	if (!this_ptr)return L"Null str";

	int ActweUID = read(this_ptr + oclActiveItem, int);//id оружия
	if (ActweUID == NULL)
	{
		return L"empty";
	}


	DWORD64 Inventory = read(this_ptr + oPlayerInventory, DWORD64);
	DWORD64 Belt = read(Inventory + oItemContainer, DWORD64);
	DWORD64 ItemList = read(Belt + oListList, DWORD64);  // public List<Item> itemList;
	DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
	for (int i = 0; i < 6; i++)
	{
		DWORD64 weapon = read(Items + 0x20 + (i * 0x8), DWORD64);//public class Item
		int UID = read(weapon + 0x28, int);

		if (UID == ActweUID)
		{
			DWORD64 ItemDefinition = read(weapon + 0x20, DWORD64);
			DWORD64 TranslatePhrase = read(ItemDefinition + 0x28, DWORD64);
			DWORD64 addr_name = read(TranslatePhrase + 0x18, DWORD64);
			_UncStr Str = read(addr_name, _UncStr);
			if (!Str.len) return L"dick";

			std::wstring name = {};
			for (int i = 0; i < Str.len && i < 20; i++)
			{
				name += Str.str[i];
			}
			return name;

			//std::cout << "weapon " << weapon << std::endl;
		}

	}
	return L"dick";
}

#pragma endregion







#pragma region LocalPlayer

//Желательно обновить перед отрисовкой
bool LocalPlayer::update_view_matrix()
{
	if (BaseEntityCamera != NULL)
	{
		pViewMatrix = read(BaseEntityCamera + 0x2E4, Matrix4x4);
		return true;
		//  std::cout <<std::hex <<"game: "<<BaseEntityCamera + 0x2E4 << std::endl;
		//  std::cout << std::hex <<"cheat: "<< &pViewMatrix << std::endl;
	}
	else return false;
}

bool LocalPlayer::WorldToScreen(Vector3 world, Vector2* screen)
{

	Matrix4x4* matrix = &pViewMatrix;


	Vector3 TransVec = Vector3(matrix->_14, matrix->_24, matrix->_34);
	Vector3 RightVec = Vector3(matrix->_11, matrix->_21, matrix->_31);
	Vector3 UpVec = Vector3(matrix->_12, matrix->_22, matrix->_32);

	const float w = TransVec.dot(world) + matrix->_44;

	if (w < 0.5f)return false; //0.098f

	const float x = RightVec.dot(world) + matrix->_41;
	const float y = UpVec.dot(world) + matrix->_42;


	*screen =
	{
		((float)Vars::Config::ScreenWidth * 0.5f) * (1.f + x / w),
		((float)Vars::Config::ScreenHigh * 0.5f + 1) * (1.f - y / w)
	};
	return true;
}

void LocalPlayer::spider()
{
	if (!this_ptr)return;
	DWORD64 Movement = read(this_ptr + oBaseMovement, DWORD64);
	if (!Movement)return;

	write(Movement + oGroundAngle, 0.f, float);
	write(Movement + oGroundAngleNew, 0.f, float);

};

void LocalPlayer::alwaysDay(float time)
{
	if (!this_ptr)return;
	if (TodCycle != NULL)
	{
		write(TodCycle + 0x10, time, float);
	}

};

void  LocalPlayer::WalkWater(bool enable)
{
	if (!this_ptr)return;
	DWORD64 Movement = read(this_ptr + oMovement, DWORD64);
	if (!Movement )return;

	static bool x130 = read(Movement + oGrounded, bool);
	static float xB8 = read(Movement + oGroundAngleNew, float);
	static float x74 = read(Movement + oGravityMultiplier, float);
	static float x78 = read(Movement + oGravityMultiplierSwimming, float);

	DWORD64 model = read(this_ptr + oPlayerModel, DWORD64);
	int onLoader = read(model + 0x2C, int);
	//std::cout << onLoader << std::endl;


	if (myLocalPlayer.GetBonePosition(l_foot).y <= 1.0f && enable)
		//if (GetAsyncKeyState(0x10) && enable)
	{

		write(Movement + oGrounded, true, bool);
		write(Movement + oGroundAngleNew, 0.f, float);
		write(Movement + oGravityMultiplier, 0.f, float);
		write(Movement + oGravityMultiplierSwimming, 0.f, float);
	}
	else
	{

		write(Movement + oGrounded, x130, bool);
		write(Movement + oGroundAngleNew, xB8, float);
		write(Movement + oGravityMultiplier, x74, float);
		write(Movement + oGravityMultiplierSwimming, x78, float);
	}
}

void LocalPlayer::speedhack()
{
	if (!this_ptr)return;
	write(this_ptr + 0x6FC, 0.f - Vars::Misc::speedBonus, float);
}

void LocalPlayer::water_boost(float val)
{
	if (!this_ptr)return;
	write(this_ptr + oclothingWaterSpeedBonus, val, float);
};


void LocalPlayer::wall_walk(bool enable)
{
	if (!this_ptr)return;

	DWORD64 Movement = read(this_ptr + oBaseMovement, DWORD64);
	if (!Movement)return;
	/*
	static DWORD64 cache = NULL;
	if (read(Movement + 0xA8, DWORD64) || cache)
	{
		if (read(Movement + 0xA8, DWORD64))
		{
			cache = read(Movement + 0xA8, DWORD64);
		}

		write(Movement + 0xA8, cache, DWORD64);

		write(Movement + 0x134, true, bool);
		write(Movement + 0x135, true, bool);

		write(Movement + 0x13A, false, bool);
		write(Movement + 0x13B, false, bool);
	}

	return;
	*/

	if (enable)
	{
		write(Movement + 0x88, 50.f, float);
		write(Movement + 0x7C, 360.f, float);
		write(Movement + 0x80, 360.f, float);
		write(Movement + 0x84, 360.f, float);
	}
	else
	{
		write(Movement + 0x88, 0.5f, float);
		write(Movement + 0x7C, 50.f, float);
		write(Movement + 0x80, 60.f, float);
		write(Movement + 0x84, 90.f, float);
	}

};


bool  LocalPlayer::IsMenu() {
	if (!this) return false;
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input) return false;

	return !(read(Input + 0x94, bool));
}

void LocalPlayer::SetGravity(float val) {
	if (!this_ptr)return;

	DWORD64 Movement = read(this_ptr + oMovement, DWORD64);
	if (!Movement)return;

	//write(Movement + 0xB0, val, float);
	write(Movement + oGravityMultiplier, val, float);
}

void LocalPlayer::set_player_flags(int set, int remove)
{
	if (!this_ptr)return;
	int Flags = read(this_ptr + oPlayerFlags, int);

	Flags = (Flags | set) & ~remove;
	write(this_ptr + oPlayerFlags, Flags, int);
	//BITOUT(Flags);
}

void LocalPlayer::set_active_weapon()
{
	if (!this_ptr)return;

	int ActweUID = read(this_ptr + oclActiveItem, int);//id оружия
	if (ActweUID == NULL)
	{
		myActiveWeapon.set_addr(NULL);
		return;
	}


	DWORD64 Inventory = read(this_ptr + oPlayerInventory, DWORD64);
	if (!Inventory)return;

	DWORD64 Belt = read(Inventory + oItemContainer, DWORD64);
	DWORD64 ItemList = read(Belt + oListList, DWORD64);  // public List<Item> itemList;
	DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
	for (int i = 0; i < 6; i++)
	{
	
	
		DWORD64 weapon = read(Items + 0x20 + (i * 0x8), DWORD64); //public class Item
		int UID = read(weapon + 0x28, int);

	   /*
		if (weapon == myActiveWeapon.get_addr() && ActweUID == UID)
		{
			std::cout << "already equal " << std::endl;
			return;
		}
		*/
		if (ActweUID == UID)
		{
			if(weapon)myActiveWeapon.set_addr(weapon);
			else myActiveWeapon.set_addr(NULL);
			return;
			/*
			if (read(weapon + oHeldEntity, DWORD64) && read((read(weapon + oHeldEntity, DWORD64)) + oRecoil , DWORD64) && (float)read((read(weapon + oHeldEntity, DWORD64)) + 0x298, float) > 1)//проверяем,есть ли свойства оружия
			{
				myActiveWeapon.set_addr(weapon);
				return;
			}

			else
			{
				myActiveWeapon.set_addr(NULL);
				return;
			}
			*/
		}

	}
	myActiveWeapon.set_addr(NULL);
}

void LocalPlayer::NoSway(float var)
{
	if (!this_ptr)return;
	write(this_ptr + oClothingAccuracyBonus, var, float);
}



Vector2  LocalPlayer::GetBA() {
	if (!this_ptr)return {0.0f,0.0f};
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return { 0.0f,0.0f };

	return  read(Input + obodyAngles, Vector2);
}
Vector2  LocalPlayer::GetRA() {
	if (!this_ptr)return{ 0.0f,0.0f };
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return { 0.0f,0.0f };

	return read(Input + oRecoilAngles, Vector2);
}

void  LocalPlayer::SetBA(const Vector2& newAngle) {
	if (!this_ptr)return;
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return ;

	write(Input + obodyAngles, newAngle, Vector2);
}

void  LocalPlayer::SetRA(const Vector2& newAngle)
{
	if (!this_ptr)return;
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return ;

	write(Input + oRecoilAngles, newAngle, Vector2);

}

#pragma endregion






#pragma region Item

bool Item::set_addr(DWORD64 ptr)
{
	if (this_ptr == ptr)return false;
	this_ptr = ptr;
};

DWORD64 Item::get_addr()
{
	return this_ptr;
}

DWORD64 Item::GetID()
{
	if (!this_ptr)return 0;

	DWORD64 Info = read(this_ptr + 0x20, DWORD64);   ///prefab id
	if (!Info)return  NULL;

	return read(Info + 0x18, DWORD64);
}


float Old_RecoilYawMax;
float Old_RecoilYawMin;
float Old_RecoilPitchMax;
float Old_RecoilPitchMin;
float Old_ADSScale;
float Old_MovementPenalty;

DWORD64 oldRecoilPtr = NULL;

void Item::no_recoil(float factor)
{
	if (!this_ptr)return;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	DWORD64 ActiveRecoilPtr = read(Held + oRecoil, DWORD64);

	if (oldRecoilPtr != ActiveRecoilPtr)//если поменялось оружие
	{
		//std::cout << "Held__________________________________________________" << Held << std::endl;
	    //std::cout << "old " << std::hex << oldRecoilPtr << std::endl;
		//std::cout << "new " << std::hex << ActiveRecoilPtr << std::endl;

		if (oldRecoilPtr != NULL)
		{

			write(oldRecoilPtr + oRecoilYawMax, Old_RecoilYawMax, float);
			write(oldRecoilPtr + oRecoilYawMin, Old_RecoilYawMin, float);

			write(oldRecoilPtr + oRecoilPitchMax, Old_RecoilPitchMax, float);
			write(oldRecoilPtr + oRecoilPitchMin, Old_RecoilPitchMin, float);

			write(oldRecoilPtr + oADSScale, Old_ADSScale, float);
			write(oldRecoilPtr + oMovementPenalty, Old_MovementPenalty, float);
		}

		if (ActiveRecoilPtr != NULL)
		{

			Old_RecoilYawMax = read(ActiveRecoilPtr + oRecoilYawMax, float);
			Old_RecoilYawMin = read(ActiveRecoilPtr + oRecoilYawMin, float);
			Old_RecoilPitchMax = read(ActiveRecoilPtr + oRecoilPitchMax, float);
			Old_RecoilPitchMin = read(ActiveRecoilPtr + oRecoilPitchMin, float);
			Old_ADSScale = read(ActiveRecoilPtr + oADSScale, float);
			Old_MovementPenalty = read(ActiveRecoilPtr + oMovementPenalty, float);

			write(ActiveRecoilPtr + oRecoilYawMax, factor * Old_RecoilYawMax, float);
			write(ActiveRecoilPtr + oRecoilYawMin, factor * Old_RecoilYawMin, float);

			write(ActiveRecoilPtr + oRecoilPitchMax, factor * Old_RecoilPitchMax, float);
			write(ActiveRecoilPtr + oRecoilPitchMin, factor * Old_RecoilPitchMin, float);

			write(ActiveRecoilPtr + oADSScale, factor * Old_ADSScale, float);
			write(ActiveRecoilPtr + oMovementPenalty, factor * Old_MovementPenalty, float);


		}
		oldRecoilPtr = ActiveRecoilPtr;
	}
	else if (factor == 1.0f)//если отключили отдачу
	{
		write(ActiveRecoilPtr + oRecoilYawMax, Old_RecoilYawMax, float);
		write(ActiveRecoilPtr + oRecoilYawMin, Old_RecoilYawMin, float);

		write(ActiveRecoilPtr + oRecoilPitchMax, Old_RecoilPitchMax, float);
		write(ActiveRecoilPtr + oRecoilPitchMin, Old_RecoilPitchMin, float);

		write(ActiveRecoilPtr + oADSScale, Old_ADSScale, float);
		write(ActiveRecoilPtr + oMovementPenalty, Old_MovementPenalty, float);
	}
	else //смена отдачи активного оружия
	{
		write(ActiveRecoilPtr + oRecoilYawMax, factor * Old_RecoilYawMax, float);
		write(ActiveRecoilPtr + oRecoilYawMin, factor * Old_RecoilYawMin, float);

		write(ActiveRecoilPtr + oRecoilPitchMax, factor * Old_RecoilPitchMax, float);
		write(ActiveRecoilPtr + oRecoilPitchMin, factor * Old_RecoilPitchMin, float);

		write(ActiveRecoilPtr + oADSScale, factor * Old_ADSScale, float);
		write(ActiveRecoilPtr + oMovementPenalty, factor * Old_MovementPenalty, float);
	}
}

void Item::NoSpread()
{
	if (!this_ptr)return ;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	if (!Held)return;

	write(Held + oStancePenalty, 0.f, float);
	write(Held + oAimconePenalty, 0.f, float);
	write(Held + oAimCone, 0.f, float);
	write(Held + oHipAimCone, 0.f, float);
	write(Held + oAimconePenaltyPerShot, 0.f, float);
}

void Item::fatbullet()
{
	if (!this_ptr)return;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);//BaseProjectile
	if (!Held)return;
	//std::cout << std::hex << "Held: " << Held << std::endl;

	DWORD64 Projectile_list = read(Held + 0x338, DWORD64);

	//std::cout << std::hex << "Projectile_list: " << Projectile_list << std::endl;

	DWORD64 Projectile_items = read(Projectile_list + 0x10, DWORD64);
	//std::cout << std::hex << "Projectile_items: "<< Projectile_items << std::endl;

	for (int i = 0; i < 24; i++)
	{
		DWORD64 Projectile_item = read(Projectile_items + 0x20 + (i * 0x8), DWORD64);

		if (Projectile_item)
		{
			float thick = read(Projectile_item + 0x2C, float);
			if (thick > 0.f && thick < 4.0f) //public float thickness;
			{
				write(Projectile_item + 0x2C, Vars::Aim::fat, float);
				//float gravity = read(Projectile_item + 0x28, float);
				//write(Projectile_item + 0x28, 100.0f, float);

			}
			//std::cout << "Speed: " << read(Projectile_item + 0x2C, float) << std::endl;
		}

	

		/*
		if (Projectile_item)
		{
			DWORD64 ItemModProjectile = read(Projectile_item + 0xE8, DWORD64);

				if (ItemModProjectile)
				{
					if (read(ItemModProjectile + 0x34, float) > 0.0003f)
					{
						//std::cout << read(ItemModProjectile + 0x34, float) << std::endl;
					}

				}

				*/

	}
}


void  Item::super_melee()
{
	if (!this_ptr)return;

	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);

	if (!Held)return;

	write(Held + omaxDistance, 3.0f, float);
	write(Held + oAttackRadius, 1.0f, float);

	write(Held + oBlockSprintOnAttack, false, bool);
	write(Held + oonlyThrowAsProjectile, false, bool);
	write(Held + oisAutomatic, true, bool);
	write(Held + othrowReady, true, bool);

}


void  Item::super_eoka()
{
	if (!this_ptr)return;

	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	if (!Held)return;
	//std::cout << "Held:" << std::hex << Held << std::endl;
	write(Held + 0x360, 1.0f, float);
	write(Held + 0x370, true, bool);

}

#pragma endregion