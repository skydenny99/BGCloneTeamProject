// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayTags.h"

namespace BaseGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_PlaneLook, "InputTag.PlaneLook");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack, "InputTag.Attack");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Walk, "InputTag.Walk");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Reload, "InputTag.Reload");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_FireModeSelect, "InputTag.FireModeSelect");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "InputTag.Interact");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ZoomIn, "InputTag.ZoomIn");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Leaning, "InputTag.Leaning");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Inventory, "InputTag.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ExitPlane, "InputTag.ExitPlane");

	
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Stand, "InputTag.ActionMode.Stand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Crouch, "InputTag.ActionMode.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Prone, "InputTag.ActionMode.Prone");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Rifle, "InputTag.ActionMode.Rifle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Unarmed, "InputTag.ActionMode.Unarmed");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ActionMode_Melee, "InputTag.ActionMode.Melee");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_Primary,"InputTag.Slot.Primary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_Secondary, "InputTag.Slot.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_SideArm, "InputTag.Slot.SideArm");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_Melee, "InputTag.Slot.Melee");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_Throwable, "InputTag.Slot.Throwable");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slot_Punch, "InputTag.Slot.Punch");

	
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot, "Item.Slot");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Armor, "Item.Slot.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Backpack, "Item.Slot.Backpack");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Helmet, "Item.Slot.Helmet");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Primary, "Item.Slot.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Secondary, "Item.Slot.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_SideArm, "Item.Slot.SideArm");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Melee, "Item.Slot.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Throwable, "Item.Slot.Throwable");
	UE_DEFINE_GAMEPLAY_TAG(Item_Slot_Punch, "Item.Slot.Punch");
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon, "Item.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Primary, "Item.Weapon.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Secondary, "Item.Weapon.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_SideArm, "Item.Weapon.SideArm");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Melee, "Item.Weapon.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Throwable, "Item.Weapon.Throwable");
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Punch, "Item.Weapon.Punch");
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Gear, "Item.Gear");
	UE_DEFINE_GAMEPLAY_TAG(Item_Gear_Armor, "Item.Gear.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Item_Gear_Backpack, "Item.Gear.Backpack");
	UE_DEFINE_GAMEPLAY_TAG(Item_Gear_Helmet, "Item.Gear.Helmet");
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts, "Item.Parts");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Magazine, "Item.Parts.Magazine");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Sight, "Item.Parts.Sight");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Sight_Rifle, "Item.Parts.Sight.Rifle");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Grip, "Item.Parts.Grip");
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Muzzle, "Item.Parts.Muzzle");
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Muzzle_Suppressor_AR, "Item.Parts.Muzzle.Suppressor.AR");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Muzzle_Suppressor_GROZA, "Item.Parts.Muzzle.Suppressor.GROZA");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Muzzle_Suppressor_SR, "Item.Parts.Muzzle.Suppressor.SR");
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Muzzle_Suppressor_SMG, "Item.Parts.Muzzle.Suppressor.SMG");


	
	UE_DEFINE_GAMEPLAY_TAG(Item_Parts_Stock, "Item.Parts.Stock");
	
	UE_DEFINE_GAMEPLAY_TAG(Weapon_FireBlock, "Weapon.FireBlock");

	
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_FireMode_Single, "Weapon.Gun.FireMode.Semi");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_FireMode_Burst, "Weapon.Gun.FireMode.Burst");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_FireMode_Auto, "Weapon.Gun.FireMode.Auto");

	UE_DEFINE_GAMEPLAY_TAG(State_Debuff_WeaponSpeedPenalty, "State.Debuff.WeaponSpeedPenalty");
	UE_DEFINE_GAMEPLAY_TAG(State_Debuff_BluezoneDamage, "State.Debuff.BluezoneDamage");
	UE_DEFINE_GAMEPLAY_TAG(State_Buff_ImmuneBluezone, "State.Buff.ImmuneBluezone");

	
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_ReloadDone, "GameplayEvent.ReloadDone");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_FireModeSelectDone, "GameplayEvent.FireModeSelectDone");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Player_HitReact, "GameplayEvent.Player.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Player_Dead, "GameplayEvent.Player.Dead");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_WeaponAttach, "GameplayEvent.WeaponAttach");

	
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Data_GunDamage, "Shared.Data.GunDamage");



	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Throttle, "InputTag.Vehicle.Throttle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Break, "InputTag.Vehicle.Break");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_HandBreak, "InputTag.Vehicle.HandBreak");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_ToggleCamera, "InputTag.Vehicle.ToggleCamera");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Steering, "InputTag.Vehicle.Steering");



	
}

