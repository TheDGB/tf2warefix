// Push KArt
int g_KPStart_Timer = 0;
public void KartPush_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, KartPush_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, KartPush_End);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, KartPush_CheckDeadPlayers);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, KartPush_OnGameFrame);
	
	SetStateAll(true);
	g_attack = false;
	ServerCommand("sm_cvar tf_halloween_kart_fast_turn_speed  200");
	g_KPStart_Timer = 6;
	CreateTimer(1.0, KartPush_Timer);
}

public void KartPush_justEntered(int client) {
	if (IsValidClient(client)) {	
		SetClientClass(client, "heavy");
		RemoveClientWeapons(client);
		giveitem(client, 939);
	
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetActivePlayers();
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 7660.337891 + (Cosine(DegToRad(float(posa)))*260.0);
		pos[1] = -3355.126709 - (Sine(DegToRad(float(posa)))*260.0);
		pos[2] = -436.968689;
		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetEntityMoveType(client, MOVETYPE_NONE);
		CreateTimer(2.0, KartTime, client);
	}
}
public void KartPush_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void KartPush_End() {
	ServerCommand("sm_cvar tf_halloween_kart_fast_turn_speed  100");
}
public void KartPush_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (g_KPStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_KPStart_Timer = 0;
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
}
	
public Action KartPush_Timer(Handle hTimer) {
	g_KPStart_Timer -= 1;
			
	if (g_KPStart_Timer > 0) {
		char sound[512];
		char StartOv[512];
		Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_KPStart_Timer);
		Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_KPStart_Timer +1);
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, StartOv);
			}
		}
		if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
		else EmitSoundToAll(sound);
		if (g_KPStart_Timer < 5) CPrintToChatAll("{red}%d{default}!", g_KPStart_Timer);
		CreateTimer(1.0, KartPush_Timer);
	}
	else {
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, "start_race");
				CreateTimer(1.0, RemoveOv, i);
			}
		}
		return Plugin_Continue;
	}
	return Plugin_Handled;
}