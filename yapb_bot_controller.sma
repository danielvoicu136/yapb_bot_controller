#include <amxmodx>
#include <cstrike>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>
#include <fun>
#include <unixtime>

#include "yapb_bot_controller/constants.inl"
#include "yapb_bot_controller/license.inl"
#include "yapb_bot_controller/transfer.inl"

#include "yapb_bot_controller/configs.inl"
#include "yapb_bot_controller/replace.inl"
#include "yapb_bot_controller/inactivity.inl"
#include "yapb_bot_controller/map.inl"
#include "yapb_bot_controller/fakedata.inl"
#include "yapb_bot_controller/stuck.inl"

#define PLUGIN_NAME		"YAPB Bot Controller"
#define PLUGIN_VERSION	"1.2"
#define PLUGIN_AUTHOR	"Daniel" 


public plugin_init()
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	
	register_cvar("yapb_bot_controller", PLUGIN_IDENTIFIER);
	
	g_cvar_force_bots = register_cvar("yapb_bot_force", "1");			
	
	g_cvar_max_control = register_cvar("yapb_max_control", "99");		
	
	g_cvar_chat_replace = register_cvar("yapb_info_replace", "0");		 
	
	g_cvar_fix_freelook = register_cvar("yapb_use_adminfreelook", "1");		


	g_SyncObj = CreateHudSyncObj();
		
	register_forward(FM_CmdStart, "CmdStart", ._post=true)
	
	register_logevent("LOGEVENT_RoundEnd", 2, "1=Round_End");
	register_event("HLTV", "EVENT_RoundStart", "a", "1=0", "2=0");
	register_event("TextMsg", "EVENT_RoundRestart", "a", "2&#Game_C", "2&#Game_w");
	
	RegisterHam( Ham_Spawn,  "player", "FWD_PlayerSpawn", 1 )
	RegisterHam( Ham_Killed, "player", "FWD_PlayerKilled" )
	g_freezeTime = get_cvar_pointer("mp_freezetime")
	
	register_clcmd("say /map", "CMD_MapCheck");
	register_clcmd("say_team /map", "CMD_MapCheck");
	register_clcmd("say map", "CMD_MapCheck");
	register_clcmd("say_team map", "CMD_MapCheck");

	register_concmd("amx_showip", "CMD_BotShowIP", ADMIN_USER, "Show player name, IP, and SteamID");
	register_concmd("amx_who", "CMD_BotShowIP", ADMIN_USER, "Show player name, IP, and SteamID");

	set_task(PLUGIN_SET_BOT, "TASK_SetBotsNumber"); 
	set_task(PLUGIN_FORCE_BOT, "TASK_ForceBot", 0, "", 0, "b"); 
	
	set_task(PLUGIN_FORCE_BOMB, "TASK_BombTransfer", 0, "", 0, "b");

	TASK_LICENSE_ALWAYS();
   
}

public client_putinserver(id)
{
	g_iPlayerControl[id] = 0;
	
}

public EVENT_RoundRestart()
{
	static iPlayers[MAX_PLAYERS]
	new iNum
	get_players(iPlayers, iNum)
	for(new i, iPlayer;i < iNum;i++)
	{
		g_iPlayerControl[iPlayer] = 0;
		g_bPlayerInactive[iPlayer] = false;
		Meter[iPlayer] = 0;
		CheckCampingTime[iPlayer] = 0;
	}
}
	
public EVENT_RoundStart()
{
	g_bRoundEnded = false
	
	static iPlayers[MAX_PLAYERS]
	new iNum
	get_players(iPlayers, iNum)
	for(new i, iPlayer;i < iNum;i++)
	{
		iPlayer = iPlayers[i]
		if(g_iPlayerControl[iPlayer] > 0)
		{
			g_iPlayerControl[iPlayer] = 0;
			g_bPlayerInactive[iPlayer] = false;
			Meter[iPlayer] = 0;
			CheckCampingTime[iPlayer] = 0;
		}
	}
	set_task(1.0, "TASK_CheckCamping", TASKID_CHECKCAMPING, _, _, "b");
}


public CmdStart(iPlayer, userCmdHandle, randomSeed) 
{
	if(is_user_alive(iPlayer) || !(TERRORISTS <= get_user_team(iPlayer) <= CTS))
		return FMRES_IGNORED

	if(g_bRoundEnded)
		return FMRES_IGNORED
			
	static iButton;
	iButton = get_uc(userCmdHandle, UC_Buttons)
	
	if(((iButton & IN_USE) || (iButton & IN_RELOAD)) && ((pev(iPlayer, pev_iuser1) == OBS_IN_EYE) || (pev(iPlayer, pev_iuser1) == OBS_CHASE_FREE)))
	{
		if(g_iPlayerControl[iPlayer] >= get_pcvar_num(g_cvar_max_control)) { 
			client_print(iPlayer, print_chat, "Replace blocked becouse of %d players limit !",get_pcvar_num(g_cvar_max_control));
			return FMRES_IGNORED
		}
		
		ControlReplacer(iPlayer)
		
		set_uc(userCmdHandle, UC_Buttons, (iButton & ~IN_USE) & ~IN_USE)
		set_uc(userCmdHandle, UC_Buttons, (iButton & ~IN_RELOAD) & ~IN_RELOAD)
		return FMRES_IGNORED
	}
	return FMRES_IGNORED
}


public server_changelevel() {
	ChangeLevel = true
}

public LOGEVENT_RoundEnd() {
	g_bRoundEnded = true;
	remove_task(TASKID_CHECKCAMPING);
}

public plugin_precache()
{
	get_mapname(g_CurrentMap, 31);
	register_forward(FM_Spawn, "FWD_Spawn_Map", 0);
}

public FWD_Spawn_Map(ent)
{
	if( !pev_valid(ent) )
	{
		return;
	}
	
	new class[32];
	pev(ent, pev_classname, class, 31);
	
	if( equal(class, "info_player_start", 0) )
	{
		g_SpawnPoints[0]++;
	}
	else if( equal(class, "info_player_deathmatch", 0) )
	{
		g_SpawnPoints[1]++;
	}
}


ColorChat(const id, const input[], any:...) 
{
	new iNum = 1, iPlayers[MAX_PLAYERS]
	
	static szMsg[192]
	vformat(szMsg, charsmax(szMsg), input, 3)
	
	replace_all(szMsg, charsmax(szMsg), "!g", "^4" )
	replace_all(szMsg, charsmax(szMsg), "!n", "^1" )
	replace_all(szMsg, charsmax(szMsg), "!t", "^3" )
   
	if(id) 	iPlayers[0] = id
	else 	get_players(iPlayers, iNum, "ch" )
		
	for(new i, iPlayer; i < iNum;i++)
	{
		iPlayer = iPlayers[i]
		
		message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("SayText"), _, iPlayer)  
		write_byte(iPlayer)
		write_string(szMsg)
		message_end()
	}
}

