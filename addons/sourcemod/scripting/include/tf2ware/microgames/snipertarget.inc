int var_sniper_target = -1;

// NEED TO MODIFY SO THAT IT IS A SHOOTING RANGE

#define SNIPER_TARGET "models/props_training/target_heavy.mdl"
#define SNIPER_BASEPOS 1

int g_Target_Pos = 0;
int g_Target_Random = 0;
bool g_Target_Ready = false;

public void SniperTarget_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, SniperTarget_OnMapStart);
}	

public void SniperTarget_OnMapStart() {
	PrecacheModel(SNIPER_TARGET, true);	
}

public void SniperTarget_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, SniperTarget_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SniperTarget_End);
	AddToForward(g_OnTimerMinigame, INVALID_HANDLE, SniperTarget_TimerMinigame);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, SniperTarget_OnGameFrame);

	randommini = GetRandomInt(0,1);
	
	g_attack = true;
	g_Target_Random = GetRandomInt(0,12);
	g_Target_Pos = 0;
	g_Target_Ready = false;
}

public void SniperTarget_TimerMinigame(int timer) {
	if (timer == 6) {
		var_sniper_target = CreateEntityByName("prop_physics");
		
		if (IsValidEdict(var_sniper_target)) {						
			char model[128];
			Format(model, sizeof(model), SNIPER_TARGET);
			
			DispatchKeyValue(var_sniper_target, "model", model);
			DispatchSpawn(var_sniper_target);
			SetEntityMoveType(var_sniper_target, MOVETYPE_NONE);
			
			SDKHook(var_sniper_target, SDKHook_OnTakeDamage, SniperTarget_Hurt2);
			
			g_Target_Ready = true;
			
			Sniper_SetPos();
		}
	}
	if (timer == 2) {
		g_Target_Ready = false;
	}
}

void Sniper_SetPos() {
	float pos[3];
	float ang[3];
	ang[0] = 0.0;
	ang[1] = 270.0;
	ang[2] = 0.0;
	
	pos[2] = float(SNIPER_BASEPOS + g_Target_Pos);
	
	if (g_Target_Random == 0) {
		pos[0] = 1639.0;
		pos[1] = -4023.0;
	}
	if (g_Target_Random == 1) {
		pos[0] = 2071.0;
		pos[1] = -3895.0;
	}
	if (g_Target_Random == 2) {
		pos[0] = 2982.0;
		pos[1] = -3655.0;
	}
	if (g_Target_Random == 3) {
		pos[0] = 1886.0;
		pos[1] = -3735.0;
	}
	if (g_Target_Random == 4) {
		pos[0] = 1753.0;
		pos[1] = -3735.0;
	}
	if (g_Target_Random == 5) {
		pos[0] = 1559.0;
		pos[1] = -3734.0;
	}
	if (g_Target_Random == 6) {
		pos[0] = 2339.0;
		pos[1] = -3895.0;
	}
	if (g_Target_Random == 7) {
		pos[0] = 2644.0;
		pos[1] = -4023.0;
	}
	if (g_Target_Random == 8) {
		pos[0] = 2805.0;
		pos[1] = -4023.0;
	}
	if (g_Target_Random == 9) {
		pos[0] = 2952.0;
		pos[1] = -4023.0;
	}
	if (g_Target_Random == 10) {
		pos[0] = 2229.0;
		pos[1] = -3895.0;
	}
	if (g_Target_Random == 11) {
		pos[0] = 1570.0;
		pos[1] = -4017.0;
	}
	if (g_Target_Random == 12) {
		pos[0] = 1874.0;
		pos[1] = -3735.0;
	}
	if (IsValidEdict(var_sniper_target)) TeleportEntity(var_sniper_target, pos, ang, NULL_VECTOR);
}

public void SniperTarget_OnGameFrame() {
	if (g_Target_Ready && g_Target_Pos < 100) {
		g_Target_Pos += 6;
		Sniper_SetPos();
	}
	if (g_Target_Ready == false && g_Target_Pos > 0) {
		g_Target_Pos -= 6;
		Sniper_SetPos();
	}
}

public void SniperTarget_End() {
	if (var_sniper_target >= 0 && IsValidEntity(var_sniper_target)) {
		SDKUnhook(var_sniper_target, SDKHook_OnTakeDamage, SniperTarget_Hurt2);
		AcceptEntityInput(var_sniper_target, "Kill" );
		var_sniper_target = -1;
	}
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, SniperTarget_Hurt);
	}
	g_attack = false;
	g_Target_Ready = false;
}

public Action SniperTarget_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}

public Action SniperTarget_Hurt2(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	if (IsValidClient(attacker)) SetStateClient(attacker, true, true);
	return Plugin_Changed;
}

public void SniperTarget_justEntered(int client) {
	if (IsValidClient(client)) {
		if (randommini == 0) {		
			SetClientClass(client, "sniper");
			RemoveClientWeapons(client);
			giveitem(client, 664);
		}
		if (randommini == 1) {
			SetClientClass(client, "spy");
			RemoveClientWeapons(client);
			giveitem(client, 24);
		}
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnu = client;
		int rowu = 0;
		
		float pos[3];
		pos[0] = 1443.321 + float(columnu*63);
		pos[1] = -5256.0 - float(rowu*63);
		pos[2] = 166.0;
		
		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);		
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, SniperTarget_Hurt);
	}
}