// BBall module

public void BBall_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BBall_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BBall_End);

	g_attack = true;
}

public Action BBall_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (status == 2 && IsValidClient(victim)) {
		int iMaxHealth = GetClientMaxHealth(victim);
		SetEntityHealth(victim, iMaxHealth);
		
		if (damage >= float(iMaxHealth-5)) damage = float(iMaxHealth-10);
		if (damage > 150.0) damage = 150.0;
		CreateTimer(0.0, BBall_GiveHealth, victim);
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

public void BBall_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, BBall_OnTakeDamageClient);
		g_attack = false;
	}
}

public Action BBall_GiveHealth(Handle hTimer, any iClient) {
	if (IsValidClient(iClient) && IsPlayerAlive(iClient)) {
		int iMaxHealth = GetClientMaxHealth(iClient);
		SetEntityHealth(iClient, iMaxHealth);  
	}
	
	return Plugin_Stop;
}



public Action Hook_StartTouch_Pipe(int entity, int other) {
	char classname[64];
	GetEdictClassname(other, classname, sizeof(classname));
	
	if (StrEqual(classname, "func_button") && status == 2) {
		int owner = GetEntPropEnt(entity, Prop_Send, "m_hThrower");
		if (g_Complete[owner] == false) {
			EmitSoundToClient(owner, SOUND_MINISCORE, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			g_Minipoints[owner] += 1;
			if (g_Minipoints[owner] >= 7) SetStateClient(owner, true, true);
			AcceptEntityInput(entity, "Kill" );
		}
	}
	
	return Plugin_Continue;
}

public void BBall_justEntered(int client) {
	if (IsValidClient(client)) {	
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnq = client;
		int rowq = 0;
		while (columnq > 17) {
			columnq = columnq - 17;
			rowq = rowq + 1;
		}

		float pos[3];
		pos[0] = 5202.0	 + float(columnq*55);
		pos[1] = -3179.0 + float(rowq*55);
		pos[2] = -1131.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "demoman");
		RemoveClientWeapons(client);
		giveitem(client, 19);
		SDKHook(client, SDKHook_OnTakeDamage, BBall_OnTakeDamageClient);
	}
}