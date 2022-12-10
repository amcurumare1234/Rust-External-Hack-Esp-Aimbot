#pragma once

#define oGameObjectManager  0x17C1F18
#define oBaseNetworkable 51386112 
// Base networkable
//      "Name": "Class$BaseNetworkable",
//      "Signature": "BaseNetworkable_c*"


#define oItemContainer 0x28 //public ItemContainer containerBelt;
#define oListList 0x38 //	public List<Item> contents;

//public class PlayerModel : ListComponent<PlayerModel>, IOnParentDestroying
#define oNewVelocity 0x22C // private Vector3 newVelocity;

//public class BasePlayer : BaseCombatEntity
#define oModel 0x128 //public Model model;
#define oLifeState 0x21C// public BaseCombatEntity.LifeState lifestate; 
#define oHealth 0x224// private float _health;
#define oSmoothVelocity 0x22C// private Vector3 newVelocity
#define oVisible 0x278// internal bool visible;
#define oPlayerInput 0x4E0// public PlayerInput input;
#define oPlayerModel 0x4C0// public PlayerModel playerModel;
#define oBaseMovement 0x4E8// public BaseMovement movement;
#define oclActiveItem 0x5D0 // private uint clActiveItem;
#define oDisplayName 0x6E0// protected string _displayName
#define oPlayerInventory 0x690 // public PlayerInventory inventory;
#define oclothingWaterSpeedBonus 0x758 // public float clothingWaterSpeedBonus;
#define oFrozen 0x4C8 // public bool Frozen;
#define oMovement 0x4E8// public BaseMovement movement;
#define oPlayerFlags 0x680// public BasePlayer.PlayerFlags playerFlags;
#define oClothingAccuracyBonus 0x75C// public float clothingAccuracyBonus;

		//*** PlayerInput ***//
#define obodyAngles 0x3C // private Vector3 bodyAngles;
#define oRecoilAngles 0x64 // public Vector3 recoilAngles;


		//*** Player Model ***//


//public class Model : MonoBehaviour, IPrefabPreProcess // TypeDefIndex: 9383
#define oboneTransforms 0x48 // public Transform[] boneTransforms;



//public class PlayerWalkMovement : BaseMovement
#define oGroundAngle 0xC4// private float groundAngle;
#define oGroundAngleNew 0xC8// private float groundAngleNew;
#define oGravityMultiplier 0x84// public float gravityMultiplier;
#define oFlying 0x14C// private bool flying;
#define oGrounded 0x140//private bool grounded;

#define oGravityMultiplierSwimming 0x88 // public float gravityMultiplierSwimming;


//public class Item
#define oHeldEntity 0x98// private EntityRef heldEntity; 

//хуина конская
//public class BaseMelee : AttackEntity // TypeDefIndex: 8850
#define oAttackRadius 0x294 // public float attackRadius;
#define oisAutomatic 0x298//public bool isAutomatic;
#define omaxDistance 0x290 //public float maxDistance;
#define oBlockSprintOnAttack 0x299 //public bool blockSprintOnAttack;
#define oonlyThrowAsProjectile 0x27A //public bool onlyThrowAsProjectile;
#define othrowReady 0x2D0 //private bool throwReady;



//public class BaseProjectile : AttackEntity
#define oRecoil 0x2D8 // public RecoilProperties recoil;
#define oAutomatic 0x288 // public bool automatic;
#define oStancePenalty 0x31C //private float stancePenalty;
#define oAimconePenalty 0x320 //private float aimconePenalty;
#define oAimCone 0x2E8 //public float aimCone;
#define oHipAimCone 0x2EC //public float hipAimCone;
#define oAimconePenaltyPerShot 0x2F0 //public float aimconePenaltyPerShot;


//public class RecoilProperties : ScriptableObject
#define oRecoilYawMin 0x18 // public float recoilYawMin;
#define oRecoilYawMax 0x1C // public float recoilYawMax;
#define oRecoilPitchMin 0x20 // public float recoilPitchMin;
#define oRecoilPitchMax 0x24  // public float recoilPitchMax;
#define oADSScale 0x30 // public float ADSScale; 
#define oMovementPenalty 0x34 // public float movementPenalty;