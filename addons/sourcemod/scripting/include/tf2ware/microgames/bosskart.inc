//heave race kart

int g_HeavyStart_Timer = 0;
public void BossKart_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BossKart_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BossKart_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, BossKart_OnGameFrame);
	g_HeavyStart_Timer = 6;
	CreateTimer(1.0, BossKart_Timer);
	NoCollision(true);
}
public void BossKart_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) { 
			SDKUnhook(i, SDKHook_OnTakeDamage, BossKart_HurtClient);
			RemoveKart(i);
		}
	}
}
public Action BossKart_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void BossKart_justEntered(int client) {
	g_BossTargetMiniGame = true;
	if (IsValidClient(client)) {
		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnke = client;
		int rowke = 0;
		while (columnke > 5) 
		columnke = columnke - 5;
		rowke = rowke + 1;
			
		float pos[3];
		pos[0] = 2227.0 - float(columnke*55);
		pos[1] = -8461.0 - float(rowke*55);
		pos[2] = 2923.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
			
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, BossKart_HurtClient);
		SetClientClass(client, "medic");
		RemoveClientWeapons(client);
		giveitem(client, 939);
		SetEntityMoveType(client, MOVETYPE_NONE);
		CreateTimer(2.0, KartTime, client);
	}
}
public void BossKart_OnGameFrame() {
	for(int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[0] < -10662.0) SetStateClient(i, true, true);
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
		if (g_HeavyStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_HeavyStart_Timer = 0;
		}
	}
}

public Action BossKart_Timer(Handle hTimer) {
	g_HeavyStart_Timer -= 1;
	if (g_HeavyStart_Timer > 0) {
		char sound[512];
		char StartOv[512];
		Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_HeavyStart_Timer);
		Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_HeavyStart_Timer +1);
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, StartOv);
			}
		}
		if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
		else EmitSoundToAll(sound);
		if (g_HeavyStart_Timer < 5) CPrintToChatAll("{red}%d{default}!", g_HeavyStart_Timer);
		CreateTimer(1.0, BossKart_Timer);
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