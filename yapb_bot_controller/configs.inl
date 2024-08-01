// Configs and Options 

#define PLUGIN_REMINDER 45.0					// How many seconds to next chat info message   

#define PLUGIN_TAG "!g[MAP]"  					// Tag for chat message 

#define PLUGIN_FORCE 120.0						// Force set bot number time to time 

#define MAX_CONTROL 99   						// How many times players can control others  

#define ANNOUNCE_IN_CHAT	0					// Should we print a message with controls history 

// When we look to the real player we can show add messages (so we can know this is the real player)

#define ANNOUNCE_IN_HUD "| quick support and free admin on daeva.ro |^n| admins can change maps and ban players |"  


// Bots fake countries

new const countries[][] = {
    "Albania", "Andorra", "Armenia", "Austria", "Azerbaijan", "Belarus", "Belgium", "Bosnia and Herzegovina",
    "Bulgaria", "Croatia", "Cyprus", "Czech Republic", "Denmark", "Estonia", "Finland", "France", "Georgia",
    "Germany", "Greece", "Hungary", "Iceland", "Ireland", "Italy", "Kosovo", "Latvia", "Liechtenstein",
    "Lithuania", "Luxembourg", "Malta", "Moldova", "Monaco", "Montenegro", "Netherlands", "North Macedonia",
    "Norway", "Poland", "Portugal", "Romania", "Russia", "San Marino", "Serbia", "Slovakia", "Slovenia",
    "Spain", "Sweden", "Switzerland", "Ukraine", "United Kingdom", "Vatican City"
};

// Team Spawns - Team Bots 
// Exemple : 10 Team Spawns / 5 Bots ... 16 Team Spawns / 12 Bots 
new teamSpawnsBots[16][2] = {
    { 1 , 0 },
    { 2 , 1 },
    { 3 , 1 },
    { 4 , 2 },
    { 5 , 2 },
    { 6 , 3 },
    { 7 , 3 },
    { 8 , 4 },
    { 9 , 4 },
    { 10 , 5 },
    { 11 , 5 },
    { 12 , 7 },
    { 13 , 8 },
    { 14 , 10 },
    { 15 , 11 },
    { 16 , 12 }
};

