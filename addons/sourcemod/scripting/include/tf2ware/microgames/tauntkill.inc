public void TauntKill_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, TauntKill_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, TauntKill_End);
	randomtauntselection = GetRandomInt(0,6);
	tauntvalueold = TauntDisable.BoolValue;
	if(TauntDisable.BoolValue){
		SetConVarBool(TauntDisable,false);
	}
	NoCollision(false);
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
	g_attack = true;
}

public void TauntKill_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int total = GetActivePlayers();
		int posa = 360 / total * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 2312.0 + (Cosine(DegToRad(float(posa)))*500.0);
		pos[1] = -21.0 - (Sine(DegToRad(float(posa)))*500.0);
		pos[2] = -1028.0;
		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posa);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		if (randomtauntselection == 0) {		
			SetClientClass(client, "sniper");
			RemoveClientWeapons(client);
			SetClientSlot(client, 0);
			giveitem(client, 1005);
		}
		if (randomtauntselection == 1) {
			SetClientClass(client, "heavy");
			RemoveClientWeapons(client);
			SetClientSlot(client, 2);
			giveitem(client, 5);
		}
		if (randomtauntselection == 2) {
			SetClientClass(client, "spy");
			RemoveClientWeapons(client);
			SetClientSlot(client, 2);
			giveitem(client, 665);
		}
		if (randomtauntselection == 3) {
			SetClientClass(client, "pyro");
			RemoveClientWeapons(client);
			SetClientSlot(client, 1);
			giveitem(client, 12);
		}
		if (randomtauntselection == 4) {
			SetClientClass(client, "medic");
			RemoveClientWeapons(client);
			SetClientSlot(client, 2);
			giveitem(client, 37);
		}
		if (randomtauntselection == 5) {
			SetClientClass(client, "engineer");
			RemoveClientWeapons(client);
			SetClientSlot(client, 0);
			giveitem(client, 1004);
		}
		if (randomtauntselection == 6) {
			SetClientClass(client, "demoman");
			RemoveClientWeapons(client);
			SetClientSlot(client, 2);
			giveitem(client, 132);
		}
		SDKHook(client, SDKHook_OnTakeDamage, TauntKill_OnTakeDamageClient);
	}
}

public Action TauntKill_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3], int damagecustom) {
	if(IsValidClient(victim)){
		if((damagecustom == TF_CUSTOM_TAUNT_GRAND_SLAM) || (damagecustom == TF_CUSTOM_PICKAXE) || (damagecustom == TF_CUSTOM_TAUNT_HADOUKEN) || (damagecustom == TF_CUSTOM_TAUNT_ARMAGEDDON) || (damagecustom == TF_CUSTOM_FLARE_PELLET) || (damagecustom == TF_CUSTOM_TAUNT_BARBARIAN_SWING) || (damagecustom == TF_CUSTOM_TAUNT_HIGH_NOON) || (damagecustom == TF_CUSTOM_TAUNT_ENGINEER_SMASH) || (damagecustom == TF_CUSTOM_TAUNT_ENGINEER_ARM) || (damagecustom == TF_CUSTOM_TAUNT_UBERSLICE) || (damagecustom == TF_CUSTOM_TAUNT_ARROW_STAB) || (damagecustom == TF_CUSTOM_TAUNT_FENCING))
		{
			SetStateClient(attacker, true, true);
		}
		else
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public void TauntKill_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)){
			SDKUnhook(i, SDKHook_OnTakeDamage, TauntKill_OnTakeDamageClient);
		}
		if(tauntvalueold == 1){
			SetConVarBool(TauntDisable,true);
		}
		tauntvalueold = -1;
		g_attack = false;
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
}