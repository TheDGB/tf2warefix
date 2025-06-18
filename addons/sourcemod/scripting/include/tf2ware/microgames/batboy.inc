// batboy module

public void Batboy_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, BossTarget_OnMapStart);
}	

public void Batboy_OnMapStart() {
	PrecacheModel( "models/weapons/...", true);
}

public void Batboy_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Batboy_justEntered);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Batboy_CheckDeadPlayers);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Batboy_End);

	randommini = GetRandomInt(0,5);
	SetStateAll(true);
	g_attack = true;
	
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
}

public Action Batboy_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker) && IsValidClient(victim)) {
		SetStateClient(attacker, true, true);
		damage = 35.0;
	} 
	return Plugin_Changed;
}

public void Batboy_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetActivePlayers();
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 36.399994  + (Cosine(DegToRad(float(posa)))*220.0);
		pos[1] = -1792.500854 - (Sine(DegToRad(float(posa)))*220.0);
		pos[2] = 459.432190;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		NoCollision(true);		
		if (randommini == 0) {
			SetClientClass(client, "medic");
			RemoveClientWeapons(client);			
			giveitem(client, 8);		   
		}
		if (randommini == 1) {
			SetClientClass(client, "heavy");
			RemoveClientWeapons(client);
			giveitem(client, 5);
			TF2_SetHealth(client, 200);			
		}
		if (randommini == 2) {
			SetClientClass(client, "scout");
			RemoveClientWeapons(client);
			giveitem(client, 660);
		}		
		if (randommini == 3) {
			SetClientClass(client, "pyro");
			RemoveClientWeapons(client);
			giveitem(client, 1181);
			SetClientSlot(client, 2);
		}
		if (randommini == 4) {
			SetClientClass(client, "soldier");
			RemoveClientWeapons(client);
			giveitem(client, 6);
		}		
		if (randommini >= 5) {
			SetClientClass(client, "engineer");
			RemoveClientWeapons(client);
			giveitem(client, 7);						
		}
		SDKHook(client, SDKHook_OnTakeDamage, Batboy_OnTakeDamageClient);
	}
}

public void Batboy_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void Batboy_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SDKUnhook(i, SDKHook_OnTakeDamage, Batboy_OnTakeDamageClient);
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
	g_attack = false;
}	