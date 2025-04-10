stock get_user_team_pev(id)
{
    return pev(id, pev_team);
}

public user_kill_and_transfer(id)
{
	if (!is_user_connected(id) || !is_user_alive(id)) 
	{
		return; 
	}

	user_silentkill(id);
	redirect_spectators(id);
}

public redirect_spectators(id)
{
    static szName[32];       
    static playersList[32];  
    new playersCount = 0;    
    new idTeam = get_user_team_pev(id); 

    for (new i = 1; i <= get_maxplayers(); i++) 
    {
        if (is_user_connected(i) && is_user_alive(i) && i != id && get_user_team_pev(i) == idTeam) 
        {
            playersList[playersCount++] = i;
        }
    }

    if (playersCount == 0) 
    {
        return; 
    }

    for (new player = 1; player <= get_maxplayers(); player++) 
    {
        if (is_user_connected(player) && !is_user_alive(player))  
        {
            if (pev(player, pev_iuser2) == id)  
            {
                new nextPlayer = playersList[random(playersCount)];  
                get_user_name(nextPlayer, szName, charsmax(szName));
                engclient_cmd(player, "follow", szName);  
            }
        }
    }
}


public TASK_BombTransfer()
{
    new bomb_carrier = find_bomb_carrier(); 

    for (new id = 1; id <= MAX_PLAYERS; id++) 
    {
        if (!is_user_connected(id) || !is_user_alive(id) || cs_get_user_team(id) != CS_TEAM_T)
            continue; 

        if (bomb_carrier != id && is_in_bombsite(id) && ( bomb_carrier > 0 )  && !is_in_bombsite(bomb_carrier))
        {
            transfer_bomb(bomb_carrier, id); 
            client_print(id, print_chat, "Plant the bomb because you are in the site !"); 
        }
    }
}


stock bool:is_in_bombsite(id)
{
    new Float:player_origin[3];
    pev(id, pev_origin, player_origin);

    new ent = -1;
    while ((ent = find_ent_in_sphere(ent, player_origin, 100.0)) > 0) 
    {
        if (entity_is_bombsite(ent))
            return true;
    }

    return false;
}

stock bool:entity_is_bombsite(ent)
{
	new classname[32]; 
	entity_get_string(ent, EV_SZ_classname, classname, charsmax(classname)); 
	return equali(classname, BOMB_TARGET_CLASSNAME);
}

public transfer_bomb(carrier, id)
{
    if (carrier > 0 && is_user_alive(carrier) && is_user_alive(id)) 
    {
		fm_transfer_user_gun(carrier, id, CSW_C4)
		client_print(id, print_chat, "Bomb successfully transferred to you !"); 
    }
}

stock find_bomb_carrier()
{
    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        if (is_user_alive(i) && cs_get_user_team(i) == CS_TEAM_T) 
        {
            if (user_has_weapon(i, CSW_C4)) 
            {
                return i; 
            }
        }
    }
    return -1; 
}
