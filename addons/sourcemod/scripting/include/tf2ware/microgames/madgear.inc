//Long jump cars
int g_MadGearStart_Timer = 0;

public void MadGear_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, MadGear_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, MadGear_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, MadGear_OnGameFrame);
	g_MadGearStart_Timer = 6;
	CreateTimer(1.0, MadGear_Timer);
}


public void MadGear_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) { 
			SDKUnhook(i, SDKHook_OnTakeDamage, MadGear_HurtClient);
			RemoveKart(i);
		}
	}
}
public Action MadGear_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)){
		damage = 0.0;
	}
	return Plugin_Changed;
}
public void MadGear_justEntered(int client) {
	g_BossTargetMiniGame = true;
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		if(GetClientTeam(client) == 3){
			int columnasd = client;
			int rowasd = 0;
			while (columnasd > 5) {
			columnasd = columnasd - 5;
			rowasd = rowasd + 3;
			}
			
			float posblu[3];
			posblu[0] = -7754.0 + float(columnasd*25);
			posblu[1] = -4860.0 - float(rowasd*25);
			posblu[2] = -852.0;
			float angblu[3];
			angblu[0] = 0.0;
			angblu[1] = 90.0;
			angblu[2] = 0.0;
			TeleportEntity(client, posblu, angblu, vel);
		}
		if(GetClientTeam(client) == 2){
			int columnasdr = client;
			int rowasdr = 0;
			while (columnasdr > 5) {
			columnasdr = columnasdr - 5;
			rowasdr = rowasdr + 3;
			}
			
			float posred[3];
			posred[0] = -8313.0 + float(columnasdr*25);
			posred[1] = -4860.0 - float(rowasdr*25);
			posred[2] = -852.0;
			float angred[3];
			angred[0] = 0.0;
			angred[1] = 90.0;
			angred[2] = 0.0;
			TeleportEntity(client, posred, angred, vel);
		}
		
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, MadGear_HurtClient);
		//RemoveClientWeapons(client);	
		SetClientClass(client, "soldier");
		RemoveClientWeapons(client);
		giveitem(client, 939);
		SetEntityMoveType(client, MOVETYPE_NONE);
		//GetClientAbsAngles(client, pos);
		CreateTimer(2.0, KartTime, client);
		//Kart(client);
		//TF2_RespawnPlayer(client);
		NoCollision(true);		
	}
}

public void MadGear_OnGameFrame() {
	for(int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] > 9866.0) SetStateClient(i, true, true);
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
		if (g_MadGearStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_MadGearStart_Timer = 0;
		}
	}
}

public Action MadGear_Timer(Handle hTimer) {
	g_MadGearStart_Timer -= 1;
	if (g_MadGearStart_Timer > 0) {
		char sound[512];
		char StartOv[512];
		Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_MadGearStart_Timer);
		Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_MadGearStart_Timer +1);
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, StartOv);
			}
		}
		if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
		else EmitSoundToAll(sound);
		if (g_MadGearStart_Timer < 5) CPrintToChatAll("{red}%d{default}!", g_MadGearStart_Timer);
		CreateTimer(1.0, MadGear_Timer);
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