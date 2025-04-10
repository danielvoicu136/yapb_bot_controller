native freelook_over_local(id);

ControlReplacer(iPlayer)
{
	static iTarget;
	iTarget = entity_get_int(iPlayer, EV_INT_iuser2)
	
	if(is_user_bot(iTarget) || g_bPlayerInactive[iTarget])
	{
			
		if(get_user_team(iPlayer) != get_user_team(iTarget))
			return PLUGIN_HANDLED
			
		if (get_pcvar_num(g_cvar_fix_freelook)) { 
			freelook_over_local(iPlayer);
		}
	
		static Float:fPlane[3], Float:fOrigin[3], Float:fVelocity[3]
		entity_get_vector(iTarget, EV_VEC_angles, fPlane)
		entity_get_vector(iTarget, EV_VEC_origin, fOrigin)
		entity_get_vector(iTarget, EV_VEC_velocity, fVelocity)
		
		ExecuteHamB(Ham_CS_RoundRespawn, iPlayer)
		
		
		attach_view(iPlayer, iTarget)
		
		entity_set_vector(iPlayer, EV_VEC_origin, fOrigin)
		entity_set_vector(iPlayer, EV_VEC_angles, fPlane)
		entity_set_vector(iPlayer, EV_VEC_velocity, fVelocity)
		entity_set_vector(iTarget, EV_VEC_origin, Float:{9999.0, 9999.0, 9999.0})
		
		checkPlayerInvalidOrigin(iPlayer);
		
		strip_user_weapons(iPlayer);
		give_item(iPlayer, "weapon_knife");
		static szWeaponName[20];
		
		for(new iWeapon = CSW_P228, iAmmoType, iAmmo, iMagazine;iWeapon <= CSW_P90; iWeapon++)
		{
			if(INVALID_WEAPONS & (1 << iWeapon))
				continue
		
			if(user_has_weapon(iTarget, iWeapon))
			{
				get_weaponname(iWeapon, szWeaponName, charsmax(szWeaponName))
				
				new iWeaponEntity = find_ent_by_owner(-1, szWeaponName, iTarget)
				if(iWeaponEntity > 0)
				{
					iAmmoType = m_rgAmmo_player_Slot0 + get_pdata_int(iWeaponEntity, m_iPrimaryAmmoType, XTRA_OFS_WEAPON)
					iAmmo = get_pdata_int(iWeaponEntity, m_iClip, XTRA_OFS_WEAPON)
					iMagazine = get_pdata_int(iTarget, iAmmoType, XTRA_OFS_PLAYER)
					
					give_item(iPlayer, szWeaponName)
					set_pdata_int(iPlayer, iAmmoType, iMagazine, XTRA_OFS_PLAYER)
					set_pdata_int(iWeaponEntity, m_iClip, iAmmo, XTRA_OFS_WEAPON)
				}
			}
		}
			
		if(fm_user_has_shield(iTarget))
		{
			give_item(iPlayer, "weapon_shield")
		}
			
		if(cs_get_user_defuse(iTarget))
		{
			cs_set_user_defuse(iPlayer, 1)
		}
		
		if(user_has_weapon(iTarget, CSW_C4))
		{
			fm_transfer_user_gun(iTarget, iPlayer, CSW_C4)
		}
			
		static iArmorType;iArmorType = get_pdata_int(iTarget, OFFSET_ARMOR_TYPE)
		cs_set_user_armor(iPlayer, get_user_armor(iTarget), CsArmorType:iArmorType)
		set_user_health(iPlayer, get_user_health(iTarget))
			
		attach_view(iPlayer, iPlayer)
		
		user_kill_and_transfer(iTarget);
		
		g_iPlayerControl[iPlayer]++
		 
		if(get_pcvar_num(g_cvar_chat_replace))
		{
			static szName[MAX_PLAYERS], szTargetName[MAX_PLAYERS]
			get_user_name(iPlayer, szName, charsmax(szName))
			get_user_name(iTarget, szTargetName, charsmax(szTargetName))
			ColorChat(0, "%s!n Spectator player!g %s !nreplaced!g %s",PLUGIN_TAG, szName, szTargetName)
		}
		
	}

	return PLUGIN_CONTINUE
}

checkPlayerInvalidOrigin(playerid)
{
	new Float:fOrigin[3], Float:fMins[3], Float:fVec[3]
	pev(playerid, pev_origin, fOrigin)
	
	new hull = (pev(playerid, pev_flags) & FL_DUCKING) ? HULL_HEAD : HULL_HUMAN
	if(is_hull_vacant(fOrigin, hull)) 
	{
		engfunc(EngFunc_SetOrigin, playerid, fOrigin)
		return
	}
	else
	{
		pev(playerid, pev_mins, fMins)
		fVec[2] = fOrigin[2]
		
		for(new i; i < sizeof g_fSizes; i++)
		{
			fVec[0] = fOrigin[0] - fMins[0] * g_fSizes[i][0]
			fVec[1] = fOrigin[1] - fMins[1] * g_fSizes[i][1]
			fVec[2] = fOrigin[2] - fMins[2] * g_fSizes[i][2]
			if(is_hull_vacant(fVec, hull))
			{
				engfunc(EngFunc_SetOrigin, playerid, fVec)
				set_pev(playerid, pev_velocity, Float:{0.0, 0.0, 0.0})
				break
			}
		}
	}
}

is_hull_vacant(const Float:origin[3], hull)
{
	new tr = 0
	engfunc(EngFunc_TraceHull, origin, origin, 0, hull, 0, tr)
	if(!get_tr2(tr, TR_StartSolid) && !get_tr2(tr, TR_AllSolid) && get_tr2(tr, TR_InOpen))
		return true
	
	return false
}