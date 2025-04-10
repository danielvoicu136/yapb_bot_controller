/* 
AMX Mod X 1.9.0.5294 (http://www.amxmodx.org)
Compiled: Dec  3 2021 15:54:56
Built from: https://github.com/alliedmodders/amxmodx/commit/363871a
Build ID: 5294:363871a
Core mode: JIT+ASM32
[20] ReAPI             RUN   -    reapi_amxx_i386.so          v5.26.0.338-dev  pl2  ANY   Never
Exe version 1.1.2.7/Stdio (cstrike)
ReHLDS version: 3.14.0.857-dev
Build date: 19:52:21 Mar 27 2025 (4002)
Build from: https://github.com/rehlds/ReHLDS/commit/89958d3
*/

public CMD_BotShowIP(id) {

    if(get_pcvar_num(g_cvar_adminonly) && !is_user_admin(id))
    {
		console_print(id, "Admins Only");
        return PLUGIN_HANDLED;
    }

	console_print(id, "Client IP");
	
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
	
	return PLUGIN_HANDLED;
}

generate_random_ip(output[], len) {
    formatex(output, len, "%d.%d.%d.%d", random_num(1, 254), random_num(1, 254), random_num(1, 254), random_num(1, 254))
}

generate_random_steamid(output[], len) {
    formatex(output, len, "STEAM_0:%d:%d", random_num(0, 1), random_num(1000000, 9999999))
}


public ExecuteServerString(cmd[], source, id)
{
    server_print("%s", cmd)

    if (equali(cmd, "status"))
    {
		show_data(id)
        return HC_SUPERCEDE
    }
	if (equali(cmd, "ping"))
    {
		show_ping(id)
        return HC_SUPERCEDE
    }
    return HC_CONTINUE
}


public show_data(id)
{
    new maxplayers = get_maxplayers()
    console_print(id, "#      name userid uniqueid frag time ping loss adr")

    for (new i = 1; i <= maxplayers; i++)
    {
        if (!is_user_connected(i)) continue

        new name[32], authid[64], ip[32]
        new userid = get_user_userid(i)
        new frags = get_user_frags(i)
        new ping = random_num(10, 90) 
        new time = get_user_time(i)
        new minutes = time / 60
        new seconds = time % 60

        get_user_name(i, name, charsmax(name))

        if (is_user_bot(i))
        {
           
            frags = random_num(0, 10)
            minutes = random_num(0, 59)
            seconds = random_num(0, 59)

            formatex(authid, charsmax(authid), "STEAM_1:%d:%d", random_num(0, 1), random_num(100000000, 999999999))
        
            formatex(ip, charsmax(ip), "%d.%d.%d.%d", random_num(80, 254), random_num(1, 254), random_num(1, 254), random_num(1, 254))

             console_print(id, "#%d ^"%s^" %d %s %d %d:%d %d 0",
                i, name, userid, authid, frags, minutes, seconds, ping)
        }
        else
        {
           
            get_user_authid(i, authid, charsmax(authid))
            get_user_ip(i, ip, charsmax(ip), 1)
            new real_ping, loss
            get_user_ping(i, real_ping, loss)

            console_print(id, "#%d ^"%s^" %d %s %d %d:%d %d %d",
                i, name, userid, authid, frags, minutes, seconds, real_ping, loss)
        }
    }

    console_print(id, "%d users", get_playersnum())
}

public show_ping(id)
{
    console_print(id, "Client ping times:")

    new maxplayers = get_maxplayers()
    for (new i = 1; i <= maxplayers; i++)
    {
        if (!is_user_connected(i)) continue

        new name[32]
        get_user_name(i, name, charsmax(name))

        if (is_user_bot(i))
        {
            new fake_ping = random_num(10, 70)
             console_print(id, "%d %s", fake_ping, name)
        }
        else
        {
            new ping, loss
            get_user_ping(i, ping, loss)
			console_print(id, "%d %s", ping, name)
        }
    }
}