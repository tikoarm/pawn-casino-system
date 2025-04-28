public OnGameModeInit()
{
    for(new i = 0; i < MAX_TABLES; i++) 
    {
        Iter_Clear(CasinoTablePlayers[i]);
        CasinoTable[i][ctStartTime] = 0;
        CasinoTable[i][ctTimerType] = 0;
    }

    add_table_to_casino(50000, 50, 133, 12, 34, 1824, 2161.836670, 1020.428650, 1591.385742, 0.000000, -0.000022, -0.000106);
    add_table_to_casino(200000, 50, 133, 12, 34, 1824, 2168.836670, 1020.428650, 1591.385742, 0.000000, -0.000022, -0.000106);
    add_table_to_casino(500000, 50, 133, 12, 34, 1824, 2161.836670, 1026.868408, 1591.385742, 0.000000, -0.000014, 179.999908);
	add_table_to_casino(1000000, 50, 133, 12, 34, 1824, 2168.836670, 1026.868408, 1591.385742, 0.000000, -0.000014, 179.999908);
    
    //CreateCasinoTD();
    #if defined casino_OnGameModeInit
		casino_OnGameModeInit();
	#endif
    return 1;
}

stock ClearCasinoTableVarsForPlayer(playerid)
{
    set_casino_table_id(playerid, -1);
    set_casino_table_points1(playerid, 0);
    set_casino_table_points2(playerid, 0);
    set_casino_table_place(playerid, -1);
    return 1;
}

stock CasinoTableGetWinners(tableid)
{
    new total_players = Iter_Count(CasinoTablePlayers[tableid]);
    new stavka = CasinoTable[tableid][ctStavka];
    new komission = CasinoTable[tableid][ctKomission] * total_players;

    new first_player = Iter_First(CasinoTablePlayers[tableid]);
    new max_points = get_casino_table_points1(first_player) + get_casino_table_points2(first_player);
    foreach(new playerid: CasinoTablePlayers[tableid])
    {
        if(get_casino_table_points1(playerid) + get_casino_table_points2(playerid) > max_points) max_points = get_casino_table_points1(playerid) + get_casino_table_points2(playerid);
    }
    new Iterator:CasinoWinners<MAX_PLAYERS>;
    Iter_Clear(CasinoWinners);
    foreach(new playerid: CasinoTablePlayers[tableid])
    {
        if(get_casino_table_points1(playerid) + get_casino_table_points2(playerid) >= max_points)
        {
            Iter_Add(CasinoWinners, playerid);
            SetTimerEx("WinAnim", 500, 0, "d", playerid);
            //SetCasinoTableLineColor(get_casino_table_place(playerid), -63948289, tableid);
			SetCasinoTableLineColor(get_casino_table_place(playerid), 0xf7e994ff, tableid);
        } else SetTimerEx("LoseAnim", 500, 0, "d", playerid);
    }

    new total_winners = Iter_Count(CasinoWinners);
    new temp_priz = stavka * total_players;
    temp_priz = temp_priz / total_winners;
    foreach(new playerid:CasinoWinners)
    {
        new priz = temp_priz - komission;
        GiveMoney(playerid, priz);
        format(stringer, sizeof(stringer), "%s выиграл %s в кости", FakeName(playerid, 0), FormatNumber(priz));
        ProxDetector(10.0, playerid, stringer, 0xC2A2DAAA,0xC2A2DAAA,0xC2A2DAAA,0xC2A2DAAA,0xC2A2DAAA);
    }

    Iter_Clear(CasinoWinners);

    BizzAddCash(CasinoTable[tableid][ctCasinoIDX], komission);
    return 1;
}

stock CasinoTableSecondTimer()
{
    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(CasinoTable[i][ctTimerType] == 1)
        {
            CasinoTable[i][ctStartTime]--;
            foreach(new playerid: CasinoTablePlayers[i])
            {
                format(stringer, 32, CTABLE_TDTEXT_START, CasinoTable[i][ctStartTime]);
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
            }
            if(CasinoTable[i][ctStartTime] <= 0)
            {
                CasinoTable[i][ctStartTime] = 5;
                CasinoTable[i][ctTimerType] = 2;
            }
        }
        else if(CasinoTable[i][ctTimerType] == 2)
        {
            CasinoTable[i][ctStartTime] = 5;
            CasinoTable[i][ctTimerType] = 3;
            foreach(new playerid: CasinoTablePlayers[i])
            {
                format(stringer, 32, CTABLE_TDTEXT_END, CasinoTable[i][ctStartTime]);
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);

                set_casino_table_points1(playerid, random(6) + 1);
                set_casino_table_points2(playerid, random(6) + 1);
            }
            foreach(new forplayerid: CasinoTablePlayers[i])
            {
                foreach(new about_playerid: CasinoTablePlayers[i])
                {
                    SetCasinoTablePoints(forplayerid, about_playerid, get_casino_table_points1(about_playerid), get_casino_table_points2(about_playerid));
                }
            }
            CasinoTableGetWinners(i);
        }
        else if(CasinoTable[i][ctTimerType] == 3)
        {
            CasinoTable[i][ctStartTime]--;
            foreach(new playerid: CasinoTablePlayers[i])
            {
                format(stringer, 32, CTABLE_TDTEXT_END, CasinoTable[i][ctStartTime]);
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
            }

            if(CasinoTable[i][ctStartTime] <= 0)
            {
                CasinoTable[i][ctStartTime] = 0;
                CasinoTable[i][ctTimerType] = 0;

                foreach(new playerid: CasinoTablePlayers[i])
                {
                    ClearCasinoTableVarsForPlayer(playerid);
                    HideCasinoTD(playerid);
                }

                Iter_Clear(CasinoTablePlayers[i]);
                CasinoTable[i][ctStartTime] = 0;
                CasinoTable[i][ctTimerType] = 0;
            }
        }
    }
    return 1;
}
stock CasinoAddPlayerToClosestTable(playerid)
{
    new tableid = GetClosestCasinoTable(playerid);
    if(tableid == -1) return 0;
    if(PlayerInfo[playerid][pMoney] < CasinoTable[tableid][ctStavka]) return error(playerid, "Недостаточно средств для игры за этим столом.");
    if(CasinoTable[tableid][ctTimerType] >= 2) return error(playerid, "Вы не можете присоединиться к игре на данном этапе, подождите окончания игры!");
    if(get_casino_table_id(playerid) != -1) return FSCM(playerid, ""cError"Вы уже зарегистрированы за столом! Ваше место: №%i", get_casino_table_id(playerid) + 1);
    if(Iter_Count(CasinoTablePlayers[tableid]) >= MAX_PLAYERS_ON_TABLE) return FSCM(playerid, ""cError"Вы не можете принять участие в игре на данном столе. Максимум человек в одной игре: %i!", MAX_PLAYERS_ON_TABLE);
    ShowCasinoTD(playerid);

    if(CasinoTable[tableid][ctTimerType] == 1)
    {
        format(stringer, 32, CTABLE_TDTEXT_START, CasinoTable[tableid][ctStartTime]);
        PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
    }
    else if(CasinoTable[tableid][ctTimerType] == 2) PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], CTABLE_TDTEXT_WAITWINNER);
    else if(CasinoTable[tableid][ctTimerType] == 3)
    {
        format(stringer, 32, CTABLE_TDTEXT_END, CasinoTable[tableid][ctStartTime]);
        PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
    }
    set_casino_table_id(playerid, tableid);

    SortCasinoTable(tableid);
    
    set_casino_table_points1(playerid, 0);
    set_casino_table_points2(playerid, 0);
    FSCM(playerid, ""cHelp"Вы успешно поставили ставку: %s! Вы заняли %i место", FormatNumber(CasinoTable[tableid][ctStavka]), get_casino_table_place(playerid) + 1);

    UpdateCasinoTableNames(tableid);
    GiveMoney(playerid, -CasinoTable[tableid][ctStavka]);

    if(Iter_Count(CasinoTablePlayers[tableid]) >= 2 && CasinoTable[tableid][ctTimerType] == 0)
    {
        CasinoTable[tableid][ctStartTime] = 15;
        CasinoTable[tableid][ctTimerType] = 1;
    }
    return 1;
}
stock SortCasinoTable(tableid)
{
    Iter_Clear(CasinoTablePlayers[tableid]);
    new g = 0;

    for(new i = 0; i <= GetPlayerPoolSize(); i++)
    {
        if(get_casino_table_id(i) != tableid) continue;

        Iter_Add(CasinoTablePlayers[tableid], i);
        set_casino_table_place(i, g);
        g++;
    }

    UpdateCasinoTableNames(tableid);
    return 1;
}

stock KickPlayerFromCasinoTable(tableid, playerid)
{
    if(!Iter_Contains(CasinoTablePlayers[tableid], playerid)) return 0;

    new place = get_casino_table_place(playerid);

    GiveMoney(playerid, CasinoTable[tableid][ctStavka]);
    foreach(new playa: CasinoTablePlayers[tableid])
    {
        PlayerTextDrawHide(playa, CasinoPTD[playa][place]);
        PlayerTextDrawColor(playa, CasinoPTD[playa][place], -1);
        PlayerTextDrawShow(playa, CasinoPTD[playa][place]);
        PlayerTextDrawSetString(playa, CasinoPTD[playa][place], "-");
        SetCasinoTablePoints(playa, 0, 0, 0, place);
    }
    
    Iter_Remove(CasinoTablePlayers[tableid], playerid);
    HideCasinoTD(playerid);
    ClearCasinoTableVarsForPlayer(playerid);
    if(Iter_Count(CasinoTablePlayers[tableid]) <= 1)
    {
        CasinoTable[tableid][ctStartTime] = 0;
        CasinoTable[tableid][ctTimerType] = 0;
        foreach(new playa: CasinoTablePlayers[tableid])
        {
            PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], CTABLE_TDTEXT_WAITSTART);
        }
    }

    SortCasinoTable(tableid);
    return 1;
}
stock UpdateCasinoTableNames(tableid)
{
    foreach(new playerid: CasinoTablePlayers[tableid])
    {
        for(new i = 0; i < MAX_PLAYERS_ON_TABLE; i++)
        {
            PlayerTextDrawSetString(playerid, CasinoPTD[playerid][i], "-");
            SetCasinoTablePoints(playerid, 0, 0, 0, i);
            SetCasinoTableLineColor(i, -1, tableid);

            if(CasinoTable[tableid][ctTimerType] == 0)
            {
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], CTABLE_TDTEXT_WAITSTART);
            }
            else if(CasinoTable[tableid][ctTimerType] == 1)
            {
                format(stringer, sizeof(stringer), CTABLE_TDTEXT_START, CasinoTable[tableid][ctStartTime]);
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
            }
            else if(CasinoTable[tableid][ctTimerType] == 2) PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], CTABLE_TDTEXT_WAITWINNER);
            else if(CasinoTable[tableid][ctTimerType] == 3)
            {
                format(stringer, sizeof(stringer), CTABLE_TDTEXT_END, CasinoTable[tableid][ctStartTime]);
                PlayerTextDrawSetString(playerid, CasinoPTD[playerid][30], stringer);
            }
        }
    }
    foreach(new forplayer: CasinoTablePlayers[tableid])
    {
        foreach(new line_player: CasinoTablePlayers[tableid])
        {
            PlayerTextDrawSetString(forplayer, CasinoPTD[forplayer][get_casino_table_place(line_player)], Name(line_player));
            if(get_casino_table_points1(line_player) != 0) SetCasinoTablePoints(forplayer, line_player, get_casino_table_points1(line_player), get_casino_table_points2(line_player));
        }
    }
    return 1;
}
stock SetCasinoTablePoints(forplayerid, playerid, points1, points2, place = -1)
{
    new first, second;
    if(place == -1) GetCasinoTableIds(get_casino_table_place(playerid), first, second);
    else GetCasinoTableIds(place, first, second);

    PlayerTextDrawHide(forplayerid, CasinoPTD[forplayerid][first]);
    PlayerTextDrawHide(forplayerid, CasinoPTD[forplayerid][second]);
    
    if(points1 == 0 && points2 == 0) return 0;
    new Float:x, Float:y, Float:z, Float:zoom;

    GetCasinoTablePoint(points1, x, y, z, zoom);
    PlayerTextDrawSetPreviewRot(forplayerid, CasinoPTD[forplayerid][first], x, y, z, zoom);

    GetCasinoTablePoint(points2, x, y, z, zoom);
    PlayerTextDrawSetPreviewRot(forplayerid, CasinoPTD[forplayerid][second], x, y, z, zoom);

    PlayerTextDrawShow(forplayerid, CasinoPTD[forplayerid][first]);
    PlayerTextDrawShow(forplayerid, CasinoPTD[forplayerid][second]);
    return 1;
}

#include ../modules/systems/casino/static_functions
#include ../modules/systems/casino/textdraws
#include ../modules/systems/casino/hooks