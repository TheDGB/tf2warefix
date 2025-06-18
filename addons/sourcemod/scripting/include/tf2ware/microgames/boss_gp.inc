//kart race
int g_GpStart_Timer = 0;
bool ClientFinish = false;
bool ClientLapEnd[MAXPLAYERS+1];
int iCont = -1;
public int GpRace_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Grap_Cow_OnMapStart);
	return 0;
}	
public int GpRace_OnMapStart() {
	PrecacheModel("models/props_urban/urban_shipping_container01.mdl", true);
	PrecacheModel("models/props_gameplay/security_fence512.mdl", true);
	return 0;
}
public int GpRace_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, GpRace_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, GpRace_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, GpRace_OnGameFrame);
	g_GpStart_Timer = 6;
	CreateTimer(1.0, GpRace_Timer);
	CreateTimer(1.0, SpawnContainer);
	ClientFinish = false;
	return 0;
}

public void GpRace_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) { 
			SDKUnhook(i, SDKHook_OnTakeDamage, GpRace_HurtClient);
			RemoveKart(i);
			SetEntPropFloat(i, Prop_Send, "m_flModelScale", 1.0);
		}
	}
	if(iCont != 1){
		if(IsValidEntity(iCont)){
			AcceptEntityInput(iCont, "Kill");
			iCont = -1;
		}
	}
}
public Action GpRace_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void GpRace_justEntered(int client) {
	g_BossTargetMiniGame = true;
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
			
		int columnmiu = client;
		int rowmiu = 0;
		while (columnmiu > 2) {
		columnmiu = columnmiu - 2;
		rowmiu = rowmiu + 1;
		}
		
		float pos[3];
		pos[0] = -1074.0 + float(columnmiu*90);
		pos[1] = 3867.0 - float(rowmiu*90);
		pos[2] = -1524.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 180.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
							
			
			// No damage
		SDKHook(client, SDKHook_OnTakeDamage, GpRace_HurtClient);
		SetClientClass(client, "pyro");
		RemoveClientWeapons(client);
		giveitem(client, 939);
		ClientLapEnd[client] = true;
		SetEntityMoveType(client, MOVETYPE_NONE);
		NoCollision(true);
		CreateTimer(2.0, KartTime, client);
		SetEntPropFloat(client, Prop_Send, "m_flModelScale", 0.5);
	}
}
public int GpRace_OnGameFrame() {
	for(int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[0] > -2550.0 && pos[1] < 4080.0 && pos[2] < -1220.0){
				if(ClientLapEnd[i]){
					ClientLapEnd[i] = false;
					SetEntPropFloat(i, Prop_Send, "m_flKartNextAvailableBoost", 5.0);
					SetOverlay(i, "");
				}
			}
			if (pos[0] < -1441.0 && pos[1] > 4080.0 && pos[2] < -900.0){
				if(ClientLapEnd[i]){
					SetEntPropFloat(i, Prop_Send, "m_flKartNextAvailableBoost", 0.1);
					float hwLoc[3] = {-1236.715942, 5239.314941, -650.468689};
					float hwVel[3] = {0.0, 0.0, 0.0};
					TeleportEntity(i, hwLoc, NULL_VECTOR, hwVel);
				}
			}
			//EmitSoundToClient(i, SOUND_MINISCORE, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			if (pos[2] > -850.0){
				if(!ClientLapEnd[i]){
					EmitSoundToClient(i, SOUND_MINISCORE, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
					g_Minipoints[i] += 1;
					PrintToChat(i, "Laps : %i", g_Minipoints[i]);
					SetEntPropFloat(i, Prop_Send, "m_flKartNextAvailableBoost", 0.1);
					PrintToChat(i, "Turbo Recharged !");
					ClientLapEnd[i] = true;
					SetOverlay(i, "turbo_jump");
				}
			}
			if (g_Minipoints[i] >= 5) {
				SetStateClient(i, true, true);
				if(!ClientFinish){
					CPrintToChatAllEx(i, "{teamcolor}%N{default} made it to the goal first!", i);
					g_GpStart_Timer = 10;
					CreateTimer(0.0, GpRace_Timer);
					ClientFinish = true;
				} 
			} 
		}        
	}
	int forcedPct = -1;
	if(forcedPct >= 0) {
		for(int i = 1; i <= MaxClients; i++) {
			if(IsClientInGame(i)) {
				SetEntProp(i, Prop_Send, "m_iKartHealth", forcedPct);
			}
		}
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (g_GpStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_GpStart_Timer = 0;
		}
	}
	
	return 0;
}
public Action GpRace_Timer(Handle hTimer) {
	if (status == 2) {
		g_GpStart_Timer -= 1;
		if (g_GpStart_Timer > 0) {
			char sound[512];
			char StartOv[512];
			Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_GpStart_Timer);
			Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_GpStart_Timer +1);
			for(int i = 1; i <= MaxClients; i++) {
				if(IsValidClient(i)) {
					SetOverlay(i, StartOv);
				}
			}
			if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
			else EmitSoundToAll(sound);
			if (g_GpStart_Timer < 5)CPrintToChatAll("{red}%d{default}!", g_GpStart_Timer);
			CreateTimer(1.0, GpRace_Timer);
		}
		else {
			if(ClientFinish){
				if (microgametimer != INVALID_HANDLE) TriggerTimer(microgametimer);
			}
		}
	}
	return Plugin_Handled;
}
public Action SpawnContainer(Handle timer){

	int i_GpCont = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont))
	{
		DispatchKeyValue(i_GpCont, "model", "models/props_urban/urban_shipping_container01.mdl");
		SetEntProp(i_GpCont, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont);
		ActivateEntity(i_GpCont);
		float containerpos[3];
		containerpos[0] = -4042.0;//randomPet;
		containerpos[1] = 4093.0;
		containerpos[2] = -1600.0;
		TeleportEntity(i_GpCont, containerpos, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont; 
	}
	int i_GpCont2 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont2))
	{
		DispatchKeyValue(i_GpCont2, "model", "models/props_urban/urban_shipping_container01.mdl");
		SetEntProp(i_GpCont2, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont2);
		ActivateEntity(i_GpCont2);
		float containerpos2[3];
		containerpos2[0] = -3535.0;//randomPet;
		containerpos2[1] = 3914.0;
		containerpos2[2] = -1600.0;
		TeleportEntity(i_GpCont2, containerpos2, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont2; 
	}
/* 	int i_GpCont3 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont3))
	{
		DispatchKeyValue(i_GpCont3, "model", "models/props_gameplay/security_fence512.mdl");
		SetEntProp(i_GpCont3, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont3);
		ActivateEntity(i_GpCont3);
		float containerpos3[3];
		containerpos3[0] = -2276.0;//randomPet;
		containerpos3[1] = 4441.0;
		containerpos3[2] = -1370.0;
		TeleportEntity(i_GpCont3, containerpos3, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont3; 
	}
	int i_GpCont4 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont4))
	{
		DispatchKeyValue(i_GpCont4, "model", "models/props_gameplay/security_fence512.mdl");
		SetEntProp(i_GpCont4, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont4);
		ActivateEntity(i_GpCont4);
		float containerpos4[3];
		containerpos4[0] = -2739.0;//randomPet;
		containerpos4[1] = 4411.0;
		containerpos4[2] = -1370.0;
		TeleportEntity(i_GpCont4, containerpos4, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont4; 
	}
	int i_GpCont5 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont4))
	{
		DispatchKeyValue(i_GpCont5, "model", "models/props_gameplay/security_fence512.mdl");
		SetEntProp(i_GpCont5, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont5);
		ActivateEntity(i_GpCont5);
		float containerpos5[3];
		containerpos5[0] = -1751.0;//randomPet;
		containerpos5[1] = 4411.0;
		containerpos5[2] = -883.0;
		TeleportEntity(i_GpCont5, containerpos5, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont5; 
	}
	int i_GpCont6 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont4))
	{
		DispatchKeyValue(i_GpCont6, "model", "models/props_gameplay/security_fence512.mdl");
		SetEntProp(i_GpCont6, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont6);
		ActivateEntity(i_GpCont6);
		float containerpos6[3];
		containerpos6[0] = -1374.0;//randomPet;
		containerpos6[1] = 4441.0;
		containerpos6[2] = -691.0;
		TeleportEntity(i_GpCont6, containerpos6, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont6; 
	}
	int i_GpCont7 = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(i_GpCont4))
	{
		DispatchKeyValue(i_GpCont7, "model", "models/props_gameplay/security_fence512.mdl");
		SetEntProp(i_GpCont7, Prop_Send, "m_nSolidType", 6);
		DispatchSpawn(i_GpCont7);
		ActivateEntity(i_GpCont7);
		float containerpos7[3];
		containerpos7[0] = -2374.0;//randomPet;
		containerpos7[1] = 4441.0;
		containerpos7[2] = -1165.0;
		TeleportEntity(i_GpCont6, containerpos7, NULL_VECTOR, NULL_VECTOR);
		iCont = i_GpCont7; 
	} */
	
	return Plugin_Handled;
}