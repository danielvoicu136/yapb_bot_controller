
#define TASKID_CHECKCAMPING		858

#define MAX_PLAYERS				32

#define fm_user_has_shield(%1)			(get_pdata_int(%1, OFFSET_SHIELD) & HAS_SHIELD)
#define OFFSET_SHIELD				510
#define HAS_SHIELD				(1<<24)

#define m_rgAmmo_player_Slot0			376
#define OFFSET_TEAM				114
#define OFFSET_ARMOR_TYPE    			112
#define m_iClip					51
#define m_iPrimaryAmmoType			49
#define XTRA_OFS_PLAYER  			5
#define XTRA_OFS_WEAPON				4
 
#define OBS_IN_EYE 				4
#define OBS_CHASE_FREE          2 

#define TERRORISTS				1
#define CTS					2

#define MAX_GLOCK18_BPAMMO			40
#define MAX_USP_BPAMMO				24

new const Float:g_fSizes[][3] =
{ 
	{0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0},
	{0.0, 0.0, 2.0}, {0.0, 0.0, -2.0}, {0.0, 2.0, 0.0}, {0.0, -2.0, 0.0}, {2.0, 0.0, 0.0}, {-2.0, 0.0, 0.0}, {-2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}, {2.0, -2.0, 2.0}, {2.0, 2.0, -2.0}, {-2.0, -2.0, 2.0}, {2.0, -2.0, -2.0}, {-2.0, 2.0, -2.0}, {-2.0, -2.0, -2.0},
	{0.0, 0.0, 3.0}, {0.0, 0.0, -3.0}, {0.0, 3.0, 0.0}, {0.0, -3.0, 0.0}, {3.0, 0.0, 0.0}, {-3.0, 0.0, 0.0}, {-3.0, 3.0, 3.0}, {3.0, 3.0, 3.0}, {3.0, -3.0, 3.0}, {3.0, 3.0, -3.0}, {-3.0, -3.0, 3.0}, {3.0, -3.0, -3.0}, {-3.0, 3.0, -3.0}, {-3.0, -3.0, -3.0},
	{0.0, 0.0, 4.0}, {0.0, 0.0, -4.0}, {0.0, 4.0, 0.0}, {0.0, -4.0, 0.0}, {4.0, 0.0, 0.0}, {-4.0, 0.0, 0.0}, {-4.0, 4.0, 4.0}, {4.0, 4.0, 4.0}, {4.0, -4.0, 4.0}, {4.0, 4.0, -4.0}, {-4.0, -4.0, 4.0}, {4.0, -4.0, -4.0}, {-4.0, 4.0, -4.0}, {-4.0, -4.0, -4.0},
	{0.0, 0.0, 5.0}, {0.0, 0.0, -5.0}, {0.0, 5.0, 0.0}, {0.0, -5.0, 0.0}, {5.0, 0.0, 0.0}, {-5.0, 0.0, 0.0}, {-5.0, 5.0, 5.0}, {5.0, 5.0, 5.0}, {5.0, -5.0, 5.0}, {5.0, 5.0, -5.0}, {-5.0, -5.0, 5.0}, {5.0, -5.0, -5.0}, {-5.0, 5.0, -5.0}, {-5.0, -5.0, -5.0}
}

const INVALID_WEAPONS =				(1 << CSW_KNIFE)|(1 << CSW_C4)|(1 << CSW_HEGRENADE)|(1 << CSW_FLASHBANG)|(1 << CSW_SMOKEGRENADE)

new g_SyncObj;

new bool:g_bRoundEnded

new g_iPlayerControl[MAX_PLAYERS + 1]
new bool:g_bPlayerInactive[MAX_PLAYERS + 1]

new bool:ChangeLevel = false, StandardDeviation[33], Meter[33], CheckCampingTime[33]
new CoordsBody[33][4][3], CoordsEyes[33][4][3]


new g_CurrentMap[32];
new g_SpawnPoints[2];
new g_EstimateTeamSpawns;
new g_EstimateBotsNumber;
new g_TotalBotsNumber;

#define TASKID_FIXIT 256

new g_freezeTime
new Float:g_flSpawned[ 33 ]


new Float:player_origin[33][3]; 


#define TASK_BOT_STUCK_OFFSET 569569 

new const PLUGIN_IDENTIFIER[] = "yapb_bot_controller";

new const BOMB_TARGET_CLASSNAME[] = "func_bomb_target";


new g_cvar_force_bots;
new g_cvar_chat_replace;
new g_cvar_max_control;
new g_cvar_fix_freelook;