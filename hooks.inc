//
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit casino_OnGameModeInit
#if defined casino_OnGameModeInit
	forward casino_OnGameModeInit();
#endif
//
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect casino_OnPlayerConnect
#if defined casino_OnPlayerConnect
	forward casino_OnPlayerConnect(playerid);
#endif
//
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange cas_OnPlayerKeyStateChange
#if defined cas_OnPlayerKeyStateChange
	forward cas_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif