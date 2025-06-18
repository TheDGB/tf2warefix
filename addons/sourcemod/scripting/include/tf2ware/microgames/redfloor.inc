// RedFloor module

public void RedFloor_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, RedFloor_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, RedFloor_End);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, RedFloor_PlayerDeath);
	
	g_attack = true;
	SetStateAll(true);
	
	CreateTimer(5.0, Minigame16_Timer);
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
}

public Action RedFloor_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker) && IsValidClient(victim)) {
		float ang[3];
		float vel[3];
		GetClientEyeAngles(attacker, ang);
		GetEntPropVector(victim, Prop_Data, "m_vecVelocity", vel);
		vel[0] -= 300.0 * Cosine(DegToRad(ang[1])) * -1.0 * damage*0.01;
		vel[1] -= 300.0 * Sine(DegToRad(ang[1])) * -1.0 * damage*0.01;
		vel[2] += 450.0;
		TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, vel);
		TF2_AddCondition(victim, view_as<TFCond>(14), 0.001);
		damage = 0.0;
	}
	return Plugin_Changed;
}

public void RedFloor_justEntered(int client) {
	if (IsValidClient(client)) {		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetClientCount(true);
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 2275.0   + (Cosine(DegToRad(float(posa)))*220.0);
		pos[1] = -2275.0  - (Sine(DegToRad(float(posa)))*220.0);
		pos[2] = 150.49;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "spy");
		RemoveClientWeapons(client);
		giveitem(client, 4);
		SDKHook(client, SDKHook_OnTakeDamage, RedFloor_OnTakeDamageClient);
		NoCollision(true);
	}
}

public void RedFloor_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SDKUnhook(i, SDKHook_OnTakeDamage, RedFloor_OnTakeDamageClient);
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
}

public Action Minigame16_Timer(Handle hTimer) {
	if (!StrEqual(minigame, "RedFloor")) return Plugin_Stop;
	
	if (status == 2) {
		int activeplayers = 0;
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && g_Complete[i] && IsClientParticipating(i)) {
				activeplayers += 1;
			}
		}
			
		if (activeplayers > 2) {
			CreateTimer(5.0, Minigame16_Timer);
		}
		else {
			TriggerTimer(microgametimer);
		}
	}
	return Plugin_Stop;
}

public void RedFloor_PlayerDeath(int client) {
	SetStateClient(client, false, true);
}