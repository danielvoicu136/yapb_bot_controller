

public TASK_Bot_Stuck(task_id)
{
   
    new id = task_id - TASK_BOT_STUCK_OFFSET;

    if (!is_user_alive(id) || !is_user_bot(id))
        return;

    if (same_origin(id))
    {
        if (has_alive_teammates(id))
        {
         
		if (get_pcvar_num(g_cvar_afk_bots_slay)) { 
			user_kill_and_transfer(id);
		}
			
            /*
                new name[33];
                get_user_name(id, name, 32);
                client_print(0, print_chat, "%s (bot) was slain for stuck.", name);
            */
        }
    }
    else
    {
        pev(id, pev_origin, player_origin[id]);
    }

    remove_task(task_id);
    set_task(BOT_STUCK_TIME, "TASK_Bot_Stuck", task_id);
}

public same_origin(id)
{
	new Float:origin[3];
	pev(id, pev_origin, origin);

	if ((floatabs(origin[0] - player_origin[id][0]) >= BOT_STUCK_RADIUS) || 
		(floatabs(origin[1] - player_origin[id][1]) >= BOT_STUCK_RADIUS)) {
		return 0;
	}
	return 1;
}

public has_alive_teammates(id)
{
    new team = get_user_team(id);

    for (new i = 1; i <= get_maxplayers(); i++)
    {
        if (i != id && is_user_alive(i) && get_user_team(i) == team)
            return 1;
    }
    return 0;
}
