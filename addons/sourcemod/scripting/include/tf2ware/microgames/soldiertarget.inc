int var_soldier_target = -1;

public void SoldierTarget_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, SoldierTarget_OnMapStart);
}	

public void SoldierTarget_OnMapStart() {
	PrecacheModel("models/props_farm/wooden_barrel.mdl", true);
	PrecacheModel("models/props_forest/milk_crate.mdl", true);
}	

public void SoldierTarget_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, SoldierTarget_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SoldierTarget_End);
	AddToForward(g_OnTimerMinigame, INVALID_HANDLE, SoldierTarget_TimerMinigame);
	g_SoldierTargGameStart = true;
	g_attack = true;
}

public void SoldierTarget_TimerMinigame(int timer) {
	if(g_SoldierTargGameStart == true){
		if (timer == 6) {
			var_soldier_target = CreateEntityByName("prop_physics");
			//var_soldier_target = CreateEntityByName("prop_dynamic");
		
			if (IsValidEdict(var_soldier_target)) {
				char model[128];
				Format(model, sizeof(model), "models/props_farm/wooden_barrel.mdl");

				DispatchKeyValue(var_soldier_target, "model", model);
				DispatchSpawn(var_soldier_target);

				SDKHook(var_soldier_target, SDKHook_OnTakeDamage, SoldierTarget_Hurt2);
			
				Soldier_SetPos();
			}
		}
	}
}

void Soldier_SetPos() {
	float pos[3];
	float vel[3];
	
	pos[0] = 3072.0;
	pos[1] = -3588.0;
	pos[2] = 150.0;
	
	vel[0] = -8000.0;
	vel[1] = 0.0;
	vel[2] = 12000.0;

	if (IsValidEdict(var_soldier_target)){
		TeleportEntity(var_soldier_target, pos, NULL_VECTOR, NULL_VECTOR);		  
		TeleportEntity(var_soldier_target, pos, NULL_VECTOR, vel);
	}
}

public Action Target_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
	if (!StrEqual(minigame, "SoldierTarget")) return Plugin_Continue;
	if (IsValidClient(attacker)) SetStateClient(attacker, true, true);
	damage = 0.0;	 
	return Plugin_Changed;
}

public void SoldierTarget_End() {
	if (var_soldier_target >= 0 && IsValidEntity(var_soldier_target)) {
		SDKUnhook(var_soldier_target, SDKHook_OnTakeDamage, SoldierTarget_Hurt2);
		AcceptEntityInput(var_soldier_target, "Kill" );
		var_soldier_target = -1;
	}
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, SoldierTarget_Hurt);
		g_attack = false;
	}
	g_SoldierTargGameStart = false;
	
}

public void SoldierTarget_justEntered(int client) {
	if (IsValidClient(client)) {	
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnsol = client;
		int rowsol = 0;
		
		float pos[3];
		pos[0] = 1443.321 + float(columnsol*63);
		pos[1] = -5256.0 - float(rowsol*63);
		pos[2] = 166.0;
		
		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "soldier");
		RemoveClientWeapons(client);
		SetClientSlot(client, 1);
		//SetEntProp(client, Prop_Send, "m_bDrawViewmodel", 1);
		giveitem(client, 10);
		SDKHook(client, SDKHook_OnTakeDamage, SoldierTarget_Hurt);
	}		
}
public Action SoldierTarget_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)){
	damage = 0.0;
	}
	return Plugin_Changed;
}


public Action SoldierTarget_Hurt2(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	
	if (IsValidClient(attacker)) SetStateClient(attacker, true, true);
	damage = 0.0;
	return Plugin_Changed;
}

public Action SoldierTarget_GiveHealth(Handle hTimer, any iClient) {
	if (IsValidClient(iClient) && IsPlayerAlive(iClient)) {
		int iMaxHealth = GetClientMaxHealth(iClient);
		SetEntityHealth(iClient, iMaxHealth);  
	}
	return Plugin_Stop;
}