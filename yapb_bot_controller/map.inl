public TaskSetBotsNumber()
{
	g_EstimateTeamSpawns = 0; 
	
	if(g_SpawnPoints[0] < g_SpawnPoints[1]) {
		g_EstimateTeamSpawns = g_SpawnPoints[0]; 
	} else {
		g_EstimateTeamSpawns = g_SpawnPoints[1];
	} 
	
	if(g_EstimateTeamSpawns > 16) {
		g_EstimateTeamSpawns = 16; 
	}
	
	if(g_EstimateTeamSpawns < 1) {
		server_cmd("amx_map de_dust2");
	}

	g_EstimateBotsNumber = teamSpawnsBots[g_EstimateTeamSpawns-1][1]; 
	
	g_TotalBotsNumber = g_EstimateBotsNumber * 2 
	server_cmd("amx_cvar yb_quota %d",g_TotalBotsNumber)
		
}

public TASK_ForceBot() 
{
   server_cmd("amx_cvar yb_quota %d",g_TotalBotsNumber);
   server_cmd("amx_cvar sv_unlag 1");
}

public CMD_MapCheck(id) { 
	if(is_user_connected(id)) 
	{ 
		new iTeamA = g_SpawnPoints[0];
		new iTeamB = g_SpawnPoints[1];
		new iTotal = iTeamA + iTeamB; 
		ColorChat(id, "%s!n Name:!g %s !nSpawns:!g %d !n( %d - %d - %d )",PLUGIN_TAG, g_CurrentMap, iTotal, iTeamA, iTeamB, g_TotalBotsNumber);
		server_cmd("amx_cvar yb_quota %d",g_TotalBotsNumber);
	}
}


public FWD_PlayerSpawn(id)
{
	if (is_user_alive(id))
		g_flSpawned[id] = get_gametime()
}

public FWD_PlayerKilled( id, iAttacker, iShouldGib )
{
	if((get_gametime() - g_flSpawned[id]) < 0.01)
	{
		new Float:timer = get_pcvar_num(g_freezeTime) + 2.0
		set_task(timer, "TASK_RespawnFix", id + TASKID_FIXIT)
	}
	return HAM_IGNORED
}

public TASK_RespawnFix(id)
{
	ExecuteHam(Ham_CS_RoundRespawn, id - TASKID_FIXIT)

	return PLUGIN_CONTINUE
}
