// Hit Enemy module

public void HitEnemy_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, HitEnemy_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, HitEnemy_End);
	randommini = GetRandomInt(0,5);
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
	g_attack = true;
}

public void HitEnemy_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, HitEnemy_OnTakeDamageClient);
	}
	g_attack = false;
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
}

public Action HitEnemy_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		SetStateClient(attacker, true, true);
		damage = 650.0;
	}
	return Plugin_Changed;
}


public void HitEnemy_justEntered(int client) {
	if (IsValidClient(client)) {		
		if (randommini == 0) {
			SetClientClass(client, "spy");
			RemoveClientWeapons(client);
			giveitem(client, 161);
		}
		if (randommini == 1) {
			RemoveClientWeapons(client);
			SetClientClass(client, "demoman");
			giveitem(client, 19);
		}
		if (randommini == 2) {
			RemoveClientWeapons(client);
			SetClientClass(client, "medic");
			giveitem(client, 36);
		}
		if (randommini == 3) {
			RemoveClientWeapons(client);
			SetClientClass(client, "scout");
			giveitem(client, 669);
		}
		if (randommini == 4) {
			RemoveClientWeapons(client);
			SetClientClass(client, "sniper");
			giveitem(client, 14);
		}
		if (randommini == 5) {
			RemoveClientWeapons(client);
			SetClientClass(client, "engineer");
			giveitem(client, 294);
		}
		SDKHook(client, SDKHook_OnTakeDamage, HitEnemy_OnTakeDamageClient);
	}
}