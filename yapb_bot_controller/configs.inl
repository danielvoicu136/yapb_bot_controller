// Configs and Options 

#define PLUGIN_REMINDER 45.0					// How many seconds to the next chat info message   

#define PLUGIN_TAG "!g[MAP]"  					// Tag for chat message 

#define PLUGIN_FORCE 120.0						// Force set bot number time to time 

#define MAX_CONTROL 99   						// How many times players can use replace    

#define ANNOUNCE_IN_CHAT	0					// Print a message with replace history 0 OFF - 1 ON 

#define ANNOUNCE_IN_HUD "| quick support and admin request on daeva.ro |^n| join chat and voice on discord.daeva.ro |"  


// FIX and compatibility layer 

#define FIX_ADMIN_FREE_LOOK 1					// Do the server use our yapb admin free look (find it in extra plugins folder)  0 NO - 1 YES  

// Team Slots and Team Bots Number   			// Configure based on the team slots , the number of bots to be added (maps have different team slots) 

new teamSpawnsBots[16][2] = {
    { 1 , 1 },
    { 2 , 1 },
    { 3 , 1 },
    { 4 , 2 },
    { 5 , 2 },
    { 6 , 3 },
    { 7 , 3 },
    { 8 , 4 },
    { 9 , 5 },
    { 10 , 5 },
    { 11 , 6 },
    { 12 , 6 },
    { 13 , 6 },
    { 14 , 7 },
    { 15 , 7 },
    { 16 , 7 }
};

