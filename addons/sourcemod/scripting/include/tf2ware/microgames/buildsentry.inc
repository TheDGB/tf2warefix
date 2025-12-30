int build_action;

public void BuildSentry_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BuildSentry_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BuildSentry_End);
	
	build_action = GetRandomInt(0, 3);
	
	HookEvent("player_builtobject", OnBuildObject, EventHookMode_Post);
	
	g_attack = true;
}

public Action OnBuildObject(Event hEvent, const char[] strEventName, bool bDontBroadcast) {
	int obj = GetEventInt(hEvent, "index");
	int client = GetEntPropEnt(obj, Prop_Send, "m_hBuilder");
	
	if(!IsValidClient(client) || !IsPlayerAlive(client) || !IsValidEntity(obj)) {
		return Plugin_Continue;
	}
	
	if(g_Complete[client]) {
		return Plugin_Continue;
	}
	
	char classname[32];
	GetEdictClassname(obj, classname, sizeof(classname));

	if(StrEqual(classname, "obj_sentrygun") && build_action == 0) {
		SetStateClient(client, true, true);
		return Plugin_Continue;
	}
	
	if(StrEqual(classname, "obj_dispenser") && build_action == 1) {
		SetStateClient(client, true, true);
		return Plugin_Continue;
	}
	
	if(StrEqual(classname, "obj_teleporter") && build_action == 2) {
		if(TF2_GetObjectMode(obj) == TFObjectMode_Entrance) {
			SetStateClient(client, true, true);
		}
		return Plugin_Continue;
	}
	
	if(StrEqual(classname, "obj_teleporter") && build_action == 3) {
		if(TF2_GetObjectMode(obj) == TFObjectMode_Exit) {
			SetStateClient(client, true, true);
		}
		return Plugin_Continue;
	}
	
	if(StrEqual(classname, "obj_sentrygun") || StrEqual(classname, "obj_dispenser") || StrEqual(classname, "obj_teleporter")) {
		SetStateClient(client, false, true);
	}
	
	return Plugin_Continue;
}

public void BuildSentry_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i)) {
			SDKUnhook(i, SDKHook_OnTakeDamage, BuildSentry_OnTakeDamageClient);
		}
	}
	
	DestroyBuildings();
	
	char weapon[128];
	int maxent = GetMaxEntities();
	for (int i = MaxClients; i < maxent; i++) {
		if (IsValidEntity(i)) {
			GetEdictClassname(i, weapon, sizeof(weapon));
			if (StrContains(weapon, "tf_weapon_") != -1) {
				AcceptEntityInput(i, "Kill");
			}
		}
	}
	
	g_attack = false;
	UnhookEvent("player_builtobject", OnBuildObject);
}

public Action BuildSentry_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		damage = 0.0;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

public void BuildSentry_justEntered(int client) {
	if (!IsValidClient(client)) {
		return;
	}
	
	g_Mission[client] = build_action;
	
	RemoveClientWeapons(client);
	
	SetClientClass(client, "engineer");
	
	float vel[3];
	vel[0] = 0.0;
	vel[1] = 0.0;
	vel[2] = 0.0;
	
	int totalp = GetActivePlayers();
	int posap = 360 / totalp * (g_Id[client] - 1);
	
	float pos[3];
	pos[0] = 2359.0 + (Cosine(DegToRad(float(posap))) * 370.0);
	pos[1] = -66.0 - (Sine(DegToRad(float(posap))) * 370.0);
	pos[2] = -1028.0;
	
	float ang[3];
	ang[0] = 0.0;
	ang[1] = float(180 - posap);
	ang[2] = 0.0;
	
	TeleportEntity(client, pos, ang, vel);
	
	SDKHook(client, SDKHook_OnTakeDamage, BuildSentry_OnTakeDamageClient);
	
	int flags = GetEntityFlags(client) | FL_NOTARGET;
	SetEntityFlags(client, flags);
	
	SetEntProp(client, Prop_Data, "m_iAmmo", 150, 4, 3);

	NoCollision(true);

	CreateTimer(0.3, GiveEngineerWeapons, client);
}

public Action GiveEngineerWeapons(Handle timer, any client) {
	if (!IsValidClient(client) || !IsPlayerAlive(client)) {
		return Plugin_Stop;
	}
	
	TF2Items_GiveWeapon(client, 7);
	
	TF2Items_GiveWeapon(client, 28);

	TF2Items_GiveWeapon(client, 25);

	SetEntProp(client, Prop_Data, "m_iAmmo", 150, 4, 3);
	
	return Plugin_Stop;
}
