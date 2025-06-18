// Airblast module
int RandMis;
public void Airblast_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Airblast_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Airblast_End);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Airblast_CheckDeadPlayers);
	
	SetStateAll(true);
	g_attack = true;
	RandMis = GetRandomInt(0,1);
}

public void Airblast_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetActivePlayers();
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 38.007339  + (Cosine(DegToRad(float(posa)))*220.0);
		pos[1] = -1795.701782 - (Sine(DegToRad(float(posa)))*220.0);
		pos[2] = 459.432190;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		switch (RandMis) {
			case 0: {
				SetClientClass(client, "pyro");
				RemoveClientWeapons(client);
				giveitem(client, 21);
				SetClientSlot(client, 0);
				TpEnabled(client);
				SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);
			}
			case 1: {
				SetClientClass(client, "scout");
				RemoveClientWeapons(client);
				giveitem(client, 45);
				SetClientSlot(client, 0);
				SetEntityHealth(client, 800);
				//TpEnabled(client);
				//SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);
				//SDKHook(client, SDKHook_OnTakeDamage, AirBlast_HurtClient);
			}
		}
		NoCollision(true);
	}
}

public void Airblast_RemoveLeftClick(int client) {
	int iButtons = GetClientButtons(client);
	if ((status == 2) && GetConVarBool(ww_enable) && g_enabled) {
		if ((iButtons & IN_ATTACK)) {
			iButtons &= ~IN_ATTACK;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
		}
	}
}

public void Airblast_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}
public Action AirBlast_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void Airblast_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		SDKUnhook(i, SDKHook_OnTakeDamage, AirBlast_HurtClient);
		if (IsValidClient(i)){
			TpDisabled(i);
			RemoveClientWeapons(i);
		}
	}
	g_attack = false;
}