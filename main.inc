#define MAX_TABLES 5
#define MAX_PLAYERS_ON_TABLE 10

#define CTABLE_TDTEXT_START             "HAНAЗO НEPE3: %i CEK" // default: "start: %i sec"
#define CTABLE_TDTEXT_END               "KOHEЙ НEPE3: %i CEK" // default: "end: %i sec"
#define CTABLE_TDTEXT_WAITWINNER        "OДЕГAHЕE МOАEГЕПEЗХ" // default: "waiting for winners"
#define CTABLE_TDTEXT_WAITSTART         "OДЕГAHЕE ЕВPOKOЛ" // default: "waiting for start"

static total_tables;
static casino_table_points1[MAX_PLAYERS];
static casino_table_points2[MAX_PLAYERS];
static casino_table_id[MAX_PLAYERS];
static casino_table_place[MAX_PLAYERS];

static casino_td_shown[MAX_PLAYERS];

enum TableInfoEnum
{
    ctStavka, //—тавка на столе
    Text3D:StavkaText,
    ctStartTime,
    ctTimerType,
    ctTableBank,
    ctKomission, //—колько берет с каждого человека
    ctVirtualWorld, //виртуальный мир стола
    ctInterior, //интерьер стола
    ctCasinoIDX, //Ќомер казино, к которому прив€зан стол, дл€ выдачи денег (игровой номер, не из Ѕƒ)
    ctCasinoObjectID,
    Float:ctX,
    Float:ctY,
    Float:ctZ,
    ctArea
};
new CasinoTable[MAX_TABLES][TableInfoEnum];
new Iterator:CasinoTablePlayers[MAX_TABLES]<MAX_PLAYERS>;

stock get_total_tables() return total_tables;
stock add_casino_table() { total_tables++; return total_tables; }

stock get_casino_table_points1(playerid) return casino_table_points1[playerid];
stock set_casino_table_points1(playerid, points) { casino_table_points1[playerid] = points; return 1; }

stock get_casino_table_points2(playerid) return casino_table_points2[playerid];
stock set_casino_table_points2(playerid, points) { casino_table_points2[playerid] = points; return 1; }

stock get_casino_table_id(playerid) return casino_table_id[playerid];
stock set_casino_table_id(playerid, value) { casino_table_id[playerid] = value; return 1; }

stock get_casino_table_place(playerid) return casino_table_place[playerid];
stock set_casino_table_place(playerid, value) { casino_table_place[playerid] = value; return 1; }

stock get_casino_td_shown(playerid) return casino_td_shown[playerid];
stock set_casino_td_shown(playerid, value) { casino_td_shown[playerid] = value; return 1; }

new PlayerText:CasinoPTD[MAX_PLAYERS][31];
new Text:CasinoTD[16];

#include ../modules/systems/casino/functions