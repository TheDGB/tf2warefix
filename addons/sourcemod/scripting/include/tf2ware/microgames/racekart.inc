//kart race
int g_KrStart_Timer = 0;

public void KartRace_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, KartRace_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, KartRace_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, KartRace_OnGameFrame);
	g_KrStart_Timer = 6;
	CreateTimer(1.0, KartRace_Timer);
}

public void KartRace_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) { 
			SDKUnhook(i, SDKHook_OnTakeDamage, KartRace_HurtClient);
			RemoveKart(i);
		}
	}
}
public Action KartRace_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void KartRace_justEntered(int client) {
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
		SDKHook(client, SDKHook_OnTakeDamage, KartRace_HurtClient);
		//SetRandomClass(client);
		SetClientClass(client, "demoman");
		RemoveClientWeapons(client);
		giveitem(client, 939);
		SetEntityMoveType(client, MOVETYPE_NONE);
		//NoCollision(true);
		CreateTimer(2.0, KartTime, client);
	}
}
public void KartRace_OnGameFrame() {
	for(int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[2] > -850.0) SetStateClient(i, true, true);
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
		if (g_KrStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_KrStart_Timer = 0;
		}
	}
}
public Action KartRace_Timer(Handle hTimer) {
	g_KrStart_Timer -= 1;
	if (g_KrStart_Timer > 0) {
		char sound[512];
 		char StartOv[512];
		Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_KrStart_Timer);
		Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_KrStart_Timer +1);
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, StartOv);
			}
		}
		if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
		else EmitSoundToAll(sound);
		if (g_KrStart_Timer < 5)CPrintToChatAll("{red}%d{default}!", g_KrStart_Timer);
		CreateTimer(1.0, KartRace_Timer);
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