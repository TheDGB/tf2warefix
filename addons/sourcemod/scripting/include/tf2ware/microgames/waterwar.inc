// Push KArt

public void WaterWar_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, WaterWar_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, WaterWar_End);
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
	g_attack = true;
}

public void WaterWar_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetActivePlayers();
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 17.577761 + (Cosine(DegToRad(float(posa)))*400.0);
		pos[1] = 30.966433 - (Sine(DegToRad(float(posa)))*400.0);
		pos[2] = 174.708862;
		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "pyro");			
		RemoveClientWeapons(client);
		giveitem(client, 813);
		SetClientSlot(client, 2);
		SetEntityMoveType(client, MOVETYPE_NONE);
		SDKHook(client, SDKHook_OnTakeDamage, WaterWar_OnTakeDamageClient);
		CreateTimer(0.5, WaterTime, client);
	}
}

public Action WaterWar_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		SetStateClient(attacker, true, true);
		damage = 950.0;
	}
	return Plugin_Changed;
}

public void WaterWar_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)){
			TF2_RemoveCondition(i, TFCond_Jarated);
			SDKUnhook(i, SDKHook_OnTakeDamage, WaterWar_OnTakeDamageClient);	
			g_attack = false;
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
}