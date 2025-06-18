//Long jump cars
int g_LRStart_Timer = 0;

public void LongJumpCar_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, LongJumpCar_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, LongJumpCar_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, LongJumpCar_OnGameFrame);
	g_LRStart_Timer = 6;
	NoCollision(true);
	CreateTimer(1.0, LongJumpCar_Timer);
}


public void LongJumpCar_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) { 
			SDKUnhook(i, SDKHook_OnTakeDamage, LongJumpCar_HurtClient);
			RemoveKart(i);
		}
	}
}
public Action LongJumpCar_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void LongJumpCar_justEntered(int client) {
	g_BossTargetMiniGame = true;
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		int columnasd = client;
		int rowasd = 0;
		while (columnasd > 15) {
		columnasd = columnasd - 15;
		rowasd = rowasd + 1;
		}
		
		float pos[3];
		pos[0] = -6669.0 + float(columnasd*63);
		pos[1] = -4998.0 - float(rowasd*63);
		pos[2] = -1304.0;
		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, LongJumpCar_HurtClient);
		//RemoveClientWeapons(client);	
		SetClientClass(client, "scout");
		RemoveClientWeapons(client);
		giveitem(client, 939);
		SetEntityMoveType(client, MOVETYPE_NONE);
		//GetClientAbsAngles(client, pos);
		//RequestFrame(KartTime2, client);
		CreateTimer(2.0, KartTime, client);
		//Kart(client);
		//TF2_RespawnPlayer(client);		
	}
}

public void LongJumpCar_OnGameFrame() {
	for(int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] > -1307.0) SetStateClient(i, true, true);
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
		if (g_LRStart_Timer == 0 && IsClientInGame(i)) {
			SetEntityMoveType(i, MOVETYPE_WALK);
			g_LRStart_Timer = 0;
		}
	}
}

public Action LongJumpCar_Timer(Handle hTimer) {
	g_LRStart_Timer -= 1;
	if (g_LRStart_Timer > 0) {
		char sound[512];
		char StartOv[512];
		Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_LRStart_Timer);
		Format(StartOv, sizeof(StartOv), "tf2ware_minigame_44_%d", g_LRStart_Timer +1);
		for(int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i)) {
				SetOverlay(i, StartOv);
			}
		}
		if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
		else EmitSoundToAll(sound);
		if (g_LRStart_Timer < 5) CPrintToChatAll("{red}%d{default}!", g_LRStart_Timer);
		CreateTimer(1.0, LongJumpCar_Timer);
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