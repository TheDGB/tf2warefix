
#define Treasure_Hunt_MDL "models/player/items/pyro/treasure_hat_oct.mdl"
#define Secret_Diary_MDL "models/player/items/all_class/hwn_spellbook_diary.mdl"
int g_Treasure_Hunt = -1;
int select_zone;
// new g_Treasure_Hunt2;
// new g_Treasure_Hunt3;
int g_Treasure_HuntStart = false;
public int Treasure_Hunt_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Treasure_Hunt_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Treasure_Hunt_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Treasure_Hunt_End);
	//SetStateAll(false);
	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			SetStateClient(i, false, true);
		}
	}
	select_zone = GetRandomInt(0,1);
	g_Treasure_HuntStart = true;
	if(select_zone == 0){
		CreateTimer(3.5, CreateTreasure_Hunt);
	}
	if(select_zone == 1){
		CreateTimer(3.5, CreateSecretDiary_Hunt);
	}
	g_attack = true; 

    return 0;	
}
public int Treasure_Hunt_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Treasure_Hunt_OnMapStart);
	
    return 0;
}	
public int Treasure_Hunt_OnMapStart() {
    PrecacheModel(Treasure_Hunt_MDL, true);
    PrecacheModel(Secret_Diary_MDL, true);
    return 0;
}
public int Treasure_Hunt_justEntered(int client) {
	if (IsValidClient(client)) {
		g_Mission[client] = select_zone;
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		if(g_Mission[client] == 0){
			int columncow = client;
			int rowcow = 0;
			while (columncow > 2) {
			columncow = columncow - 2;
			rowcow = rowcow + 1;
			}
			float pos[3];
			pos[0] = 745.0 + float(columncow*90);
			pos[1] = 2979.0 - float(rowcow*90);
			pos[2] = 316.0;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;

			TeleportEntity(client, pos, ang, vel);
			RemoveClientWeapons(client);
			SetClientClass(client, "demoman");
			giveitem(client, 307);
			ClientCommand(client,"slot6");
			ForceGrapplingHook(client);
			NoCollision(true);
			g_attack = true;
			SDKHook(client, SDKHook_OnTakeDamage, Hunt_Hurt);
			SetStateClient(client, false, true);
		}
		if(g_Mission[client] == 1){
			int columncow3 = client;
			int rowcow3 = 0;
			while (columncow3 > 5) {
			columncow3 = columncow3 - 5;
			rowcow3 = rowcow3 + 1;
			}
			float pos[3];
			pos[0] = 8115.306152 + float(columncow3*90);

			pos[1] = -6307.487305 - float(rowcow3*90);
			pos[2] = 545.031311;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = -180.0;
			ang[2] = 0.0;

			TeleportEntity(client, pos, ang, vel);
			RemoveClientWeapons(client);
			SetClientClass(client, "scout");
			giveitem(client, 317);
			ClientCommand(client,"slot6");
			ForceGrapplingHook(client);
			NoCollision(true);
			g_attack = true;
			SDKHook(client, SDKHook_OnTakeDamage, Hunt_Hurt);
			SetStateClient(client, false, true);
		}
	}
	
	return 0;
}
public void Treasure_Hunt_End() {
	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			//TpDisabled(i);
			SDKUnhook(i, SDKHook_OnTakeDamage, Hunt_Hurt);
			RemoveClientWeapons(i);
			int index = -1;
			while ((index = FindEntityByClassname2(index, "tf_projectile_grapplinghook")) != -1)
			if(i == GetEntPropEnt(index, Prop_Send , "m_hOwnerEntity"))
			{
				RemoveEdict(index);
				//PrintToServer("Removing %i's lightning orb %i", Client, index);
			}
		}
	}
	if (IsValidEntity(g_Treasure_Hunt)) {
		AcceptEntityInput(g_Treasure_Hunt, "Kill");
		g_Treasure_Hunt = -1;
	}
	g_attack = false;
	g_Treasure_HuntStart = false;
}
public Action CreateTreasure_Hunt(Handle timer){
	if(g_Treasure_HuntStart){
		PrintToChatAll("\x04 the Buried Treasure is spawned!");
		g_Treasure_Hunt = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(g_Treasure_Hunt)){
			SetEntityModel(g_Treasure_Hunt, Treasure_Hunt_MDL);
			DispatchKeyValue(g_Treasure_Hunt, "modelscale", "2.0");
			SetEntProp(g_Treasure_Hunt, Prop_Send, "m_nSolidType", 2);
			DispatchSpawn(g_Treasure_Hunt);
			float port26Loc[3];
			switch (GetRandomInt(1, 8))
			{
				case 1:
				{
					port26Loc[0] = 2466.510010;
					port26Loc[1] = 2473.412598;
					port26Loc[2] = 50.605011;
				}
				case 2:
				{
					port26Loc[0] = 3272.742676;
					port26Loc[1] = 2742.690674;
					port26Loc[2] = -77.493637;
				}
				case 3:
				{
					port26Loc[0] = 762.238831;
					port26Loc[1] = 1590.975586;
					port26Loc[2] = 250.181641;
				}
				case 4:
				{
					port26Loc[0] = 2608.916504;
					port26Loc[1] = 3513.425781;
					port26Loc[2] = 80.730911;
				}
				case 5:
				{
					port26Loc[0] = 3564.905762;
					port26Loc[1] = 2045.317749;
					port26Loc[2] = -93.319275;
				}
				case 6:
				{
					port26Loc[0] = 3843.322021;
					port26Loc[1] = 3341.357178;
					port26Loc[2] = -81.641701;
				}
				case 7:
				{
					port26Loc[0] = 2480.684814;
					port26Loc[1] = 2272.269287;
					port26Loc[2] = -10.24545;
				}
				case 8:
				{
					port26Loc[0] = 2756.415771;
					port26Loc[1] = 2443.780029;
					port26Loc[2] = -10.85679;
				}
			}
			TeleportEntity(g_Treasure_Hunt, port26Loc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(g_Treasure_Hunt, "unusual_bubbles", true, 22.0);
		}
	}
		
	return Plugin_Handled;
}
public Action CreateSecretDiary_Hunt(Handle timer){
	if(g_Treasure_HuntStart){
		PrintToChatAll("\x04 the Secret Diary is spawned!");
		g_Treasure_Hunt = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(g_Treasure_Hunt)){
			SetEntityModel(g_Treasure_Hunt, Secret_Diary_MDL);
			DispatchKeyValue(g_Treasure_Hunt, "modelscale", "2.0");
			SetEntProp(g_Treasure_Hunt, Prop_Send, "m_nSolidType", 2);
			DispatchSpawn(g_Treasure_Hunt);
			float portdiaryLoc[3];
			switch (GetRandomInt(1, 6))
			{
				case 1:
				{
					portdiaryLoc[0] = 5472.914551; // 5472.914551 -6555.668457 689.036621
					portdiaryLoc[1] = -6555.668457;
					portdiaryLoc[2] = 689.036621;
				}
				case 2:
				{
					portdiaryLoc[0] = 5692.677246; //5692.677246 -5790.875977 304.727112
					portdiaryLoc[1] = -5790.875977;
					portdiaryLoc[2] = 304.727112;
				}
				case 3:
				{
					portdiaryLoc[0] = 6551.962402; //6551.962402 -7233.310059 388.774323
					portdiaryLoc[1] = -7233.310059;
					portdiaryLoc[2] = 388.774323;
				}
				case 4:
				{
					portdiaryLoc[0] = 5776.306152; //5776.306152 -7224.204590 373.370850
					portdiaryLoc[1] = -7224.204590;
					portdiaryLoc[2] = 373.370850;
				}
				case 5:
				{
					portdiaryLoc[0] = 7867.724121; //7867.724121 -5770.405762 273.031311
					portdiaryLoc[1] = -5770.405762;
					portdiaryLoc[2] = 273.031311;
				}
				case 6:
				{
					portdiaryLoc[0] = 4588.176758; //4588.176758 -6083.404785 273.031311
					portdiaryLoc[1] = -6083.404785;
					portdiaryLoc[2] = 273.031311;
				}
			}
			TeleportEntity(g_Treasure_Hunt, portdiaryLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(g_Treasure_Hunt, "superrare_beams1", true, 22.0);
		}
	}
	
    return Plugin_Handled;	
}
public Action Hunt_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	
	if ((IsValidClient(attacker)) || (IsValidClient(victim)))
	damage = 0.0;
	return Plugin_Changed;
}
public int Treasure_Hunt_OnGameFrame() {
	float Apos[3];
	float Bpos[3];
	if(IsValidEntity(g_Treasure_Hunt)){
		GetEntPropVector(g_Treasure_Hunt, Prop_Data, "m_vecOrigin", Bpos);
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			GetEntPropVector(i, Prop_Data, "m_vecOrigin", Apos);
			float Dist = GetVectorDistance(Apos, Bpos);
			//PrintToChatAll(" polipo e : %f %f", Apos, Bpos);
			if(i <0 || i > MAXPLAYERS)continue;
			float TeleDist;
			if(select_zone == 0){
				TeleDist = 60.0;
			}
			if(select_zone == 1){
				TeleDist = 80.0;
			}
			if(Dist <= TeleDist)
			{
				float vel[3];
				vel[0] = 0.0;
				vel[1] = 0.0;
				vel[2] = 0.0;
				float Fpos[3];
				Fpos[0] = 564.668701;
				Fpos[1] = 2459.562744;
				Fpos[2] = 316.0;
				float ang[3];
				ang[0] = 0.0;
				ang[1] = 0.0;
				ang[2] = 0.0;
				
				TeleportEntity(i, Fpos, ang, vel);
				CreateTimer(0.1, GiveWinMinigame, i);
			}
		}
	}
	
	return 0;
}
public Action GiveWinMinigame(Handle timer, any iClient){
	if (IsValidClient(iClient) && IsClientParticipating(iClient)) {
		SetStateClient(iClient, true, true);
	}
	
	return Plugin_Handled;
}