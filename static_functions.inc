//add_table_to_casino(Сумма ставки на столе, Сумма комиссии казино, виртуальный мир, интерьер, номер казино который получает комиссию (игровой), ид объекта игрального стола, X,Y,Z,RX,RY,RZ координаты стола)
stock add_table_to_casino(stavka, komission, vw, int, casino_idx, table_objectid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    new amount = get_total_tables(), ct_string[64];
    if(amount + 1 >= MAX_TABLES) return printf("[!] Критическая необходимость увеличить MAX_TABLES (modules -> casino -> main.inc) [!]");
    new current_amount = add_casino_table();

    CasinoTable[amount][ctStavka] = stavka;
    CasinoTable[amount][ctKomission] = komission;
    CasinoTable[amount][ctVirtualWorld] = vw;
    CasinoTable[amount][ctInterior] = int;
    CasinoTable[amount][ctCasinoIDX] = casino_idx;

    CasinoTable[amount][ctX] = x;
    CasinoTable[amount][ctY] = y;
    CasinoTable[amount][ctZ] = z;
    CasinoTable[amount][ctArea] = CreateDynamicSphere(x, y, z, 3.0, vw, int);
    
    CasinoTable[amount][ctCasinoObjectID] = CreateDynamicObject(table_objectid, x, y, z, rx, ry, rz, vw, int);
    format(ct_string, sizeof(ct_string), "{FFFF00}Ставка: %s", FormatNumber(CasinoTable[amount][ctStavka]));
    CasinoTable[amount][StavkaText] = CreateDynamic3DTextLabel(ct_string, -1, x, y, z+0.7, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, vw, int);

    printf("Успешно создан стол для казино №%i (%f,%f,%f) VW: %i Int: %i. Всего: %i", amount, x, y, z, vw, int, current_amount);
    return 1;
}
public OnPlayerConnect(playerid)
{
	CreateCasinoPTD(playerid);
    ClearCasinoTableVarsForPlayer(playerid);

	#if defined casino_OnPlayerConnect
		casino_OnPlayerConnect(playerid);
	#endif
	return 1;
}

stock SetCasinoTableLineColor(line, colorid, tableid)
{
    foreach(new playerid: CasinoTablePlayers[tableid])
    {
        PlayerTextDrawHide(playerid, CasinoPTD[playerid][line]);
        PlayerTextDrawColor(playerid, CasinoPTD[playerid][line], colorid);
        PlayerTextDrawShow(playerid, CasinoPTD[playerid][line]);
    }
    return 1;
}
stock Casino_LeaveDynamicArea(playerid, areaid)
{
    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(CasinoTable[i][ctArea] == areaid)
        { 
            HideButton(playerid);
            break;
        }
    }

    if(get_casino_td_shown(playerid) == 0) return 0;

    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(CasinoTable[i][ctArea] == areaid)
        { 
            HideButton(playerid);
            KickPlayerFromCasinoTable(i, playerid);
            break;
        }
    }   
    return 1;
}
stock Casino_EnterDynamicArea(playerid, areaid)
{
    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(CasinoTable[i][ctArea] == areaid)
        { 
            ShowButtonAlt(playerid);
            break;
        }
    }   
    return 1;
}

stock ShowCasinoTD(playerid)
{
    if(get_casino_td_shown(playerid) == 1) return 0;
    set_casino_td_shown(playerid, 1);
    
    for(new i = 0; i < 31; i++) PlayerTextDrawShow(playerid, CasinoPTD[playerid][i]);
    for(new i = 0; i < 16; i++) TextDrawShowForPlayer(playerid, CasinoTD[i]);

    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(IsPlayerInDynamicArea(playerid, CasinoTable[i][ctArea]))
        { 
            HideButton(playerid);
            break;
        }
    }
    return 1;
}
stock HideCasinoTD(playerid)
{
    if(get_casino_td_shown(playerid) == 0) return 0;

    for(new i = 0; i < MAX_TABLES; i++)
    {
        if(IsPlayerInDynamicArea(playerid, CasinoTable[i][ctArea]))
        { 
            ShowButtonAlt(playerid);
            break;
        }
    }

    set_casino_td_shown(playerid, 0);

    for(new i = 0; i < 31; i++) PlayerTextDrawHide(playerid, CasinoPTD[playerid][i]);
    for(new i = 0; i < 16; i++) TextDrawHideForPlayer(playerid, CasinoTD[i]);
    return 1;
}
stock GetClosestCasinoTable(playerid)
{
    new idx = -1;
    for(new i = 0; i < get_total_tables(); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.5, CasinoTable[i][ctX], CasinoTable[i][ctY], CasinoTable[i][ctZ])) 
        {
            if(GetPlayerVirtualWorld(playerid) != CasinoTable[i][ctVirtualWorld]) continue;
            if(GetPlayerInterior(playerid) != CasinoTable[i][ctInterior]) continue;
            idx = i;
            break;
        }
    }
    return idx;
}
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys == KEY_WALK)
    {
        CasinoAddPlayerToClosestTable(playerid);
    }

    #if defined cas_OnPlayerKeyStateChange
		cas_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#endif
    return 1;
}

forward WinAnim(playerid);
public WinAnim(playerid)
{
    ApplyAnimation(playerid, "OTB", "wtchrace_win", 4.0, 0, 0, 0, 0, 0);
    return 1;
}
forward LoseAnim(playerid);
public LoseAnim(playerid)
{
    ApplyAnimation(playerid, "CASINO", "CARDS_LOSE", 4.0, 0, 0, 0, 0, 0);
    return 1;
}
stock FormatNumber(number, prefix[] = "$")
{
	new value[15],
		length;

	format(value, sizeof(value), "%d", (number < 0) ? (-number) : (number));

	if ((length = strlen(value)) > 3)
	{
		for (new i = length, l = 0; --i >= 0; l ++) 
		{
		    if ((l > 0) && (l % 3 == 0)) strins(value, ",", i + 1);
		}
	}
	if (prefix[0] != 0)
	    strins(value, prefix, 0);

	if (number < 0)
		strins(value, "-", 0);

	return value;
}
stock GetCasinoTablePoint(point, &Float:x, &Float:y, &Float:z, &Float:zoom)
{
    switch(point)
    {
        case 1: x = 270.000000, y = 0.000000, z = 0.000000, zoom = 1.000000;
        case 2: x = 270.000000, y = 90.000000, z = 0.000000, zoom = 1.000000;
        case 3: x = 180.000000, y = 360.000000, z = 0.000000, zoom = 1.000000;
        case 4: x = 270.000000, y = 270.000000, z = 270.000000, zoom = 1.000000;
        case 5: x = 180.000000, y = 360.000000, z = 270.000000, zoom = 1.000000;
        case 6: x = 270.000000, y = 180.000000, z = 0.000000, zoom = 1.000000;
    }
    return 1;
}
stock GetCasinoTableIds(place, &first, &second)
{
    first = place + 10;
    second = place + 20;
    return 1;
}