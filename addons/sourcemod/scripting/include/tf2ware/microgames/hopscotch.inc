// Hopscotch module

int g_Hopscotch_arena = 0;
int g_Hopscotch_weapon = false;

public void Hopscotch_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Hopscotch_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Hopscotch_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Hopscotch_End);
	
	g_Hopscotch_arena = GetRandomInt(0,1);
	g_Hopscotch_weapon = false;
	
	if (g_Hopscotch_arena == 1) {
		g_attack = true;
		CreateTimer(4.9, Hopscotch_Weapon);
	}
}

public Action Hopscotch_Weapon(Handle hTimer) {
	if (status == 2 && g_Hopscotch_arena == 1) {
		g_Hopscotch_weapon = true;
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsClientParticipating(i)) {
				RemoveClientWeapons(i);
				g_attack = true;
				giveitem(i, 18);			
			}
		}
	}
	
	return Plugin_Stop;
}

public void Hopscotch_justEntered(int client) {
	if (IsValidClient(client)) {		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnju = client;
		int rowju = 0;
		while (columnju > 10) {
			columnju = columnju - 10;
			rowju = rowju + 1;
		}
		
		float pos[3];
		
		if (g_Hopscotch_arena == 1) {
			pos[0] = -369.0	 + float(columnju*55);
			pos[1] = -3436.0 - float(rowju*55);
			pos[2] = -1180.0;
		}
		else {
		pos[0] = -1414.0  - float(columnju*55);
		pos[1] = -4148.0 - float(rowju*55);
		pos[2] = 1260.0;
		}

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		if (g_Hopscotch_arena == 1) SetClientClass(client, "soldier");
		else SetClientClass(client, "heavy");
		DisableClientWeapons(client);
		if (g_Hopscotch_weapon && g_Hopscotch_arena == 1) {
			RemoveClientWeapons(client);
			giveitem(client, 18);
		}
		SDKHook(client, SDKHook_OnTakeDamage, Hopscotch_OnTakeDamageClient);
		NoCollision(true);
	}
}

public void Hopscotch_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (g_Hopscotch_arena == 0 && pos[1] > -61.0  && g_Complete[i] == false ) SetStateClient(i, true, true);
			if (g_Hopscotch_arena == 1 && pos[2] > 1419.0 && pos[1] > -2762.0 && g_Complete[i] == false ) SetStateClient(i, true, true);
			if (g_Hopscotch_arena == 1) SetEntityHealth(i, 200);
		}
	}
}

public Action Hopscotch_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker) && IsValidClient(victim) && victim != attacker && status == 2) {
		damage = 0.0;
	}
	return Plugin_Changed;
}

public void Hopscotch_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, Hopscotch_OnTakeDamageClient);
		g_attack = false;
	}
}