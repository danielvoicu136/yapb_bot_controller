public CMD_BotShowIP(id) {

    if(get_pcvar_num(g_cvar_adminonly) && !is_user_admin(id))
    {
		console_print(id, "|======= Admins Only AMX_SHOWIP ============|");
        return PLUGIN_HANDLED;
    }

	console_print(id, "|=====================================|");
	
	new i, players[32], numPlayers;
	
	get_players(players, numPlayers);

	for (i = 0; i < numPlayers; i++) {
		new playerName[32];
		new playerIP[32];
		new playerSteamID[32];

		get_user_name(players[i], playerName, sizeof(playerName) - 1)

		if (is_user_bot(players[i])) {
			generate_random_ip(playerIP, sizeof(playerIP))
			generate_random_steamid(playerSteamID, sizeof(playerSteamID))
		} else {
			get_user_ip(players[i], playerIP, sizeof(playerIP) - 1, 1)
			get_user_authid(players[i], playerSteamID, sizeof(playerSteamID) - 1)
		}

		console_print(id, "%s - %s - %s", playerName, playerIP, playerSteamID)
	}

	console_print(id, "|=====================================|");
	
	return PLUGIN_HANDLED;
}

generate_random_ip(output[], len) {
    formatex(output, len, "%d.%d.%d.%d", random_num(1, 254), random_num(1, 254), random_num(1, 254), random_num(1, 254))
}

generate_random_steamid(output[], len) {
    formatex(output, len, "STEAM_0:%d:%d", random_num(0, 1), random_num(1000000, 9999999))
}