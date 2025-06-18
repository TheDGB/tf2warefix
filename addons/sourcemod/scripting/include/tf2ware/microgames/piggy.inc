// piggyback module

#define PLAYER		  "player"
int g_piggy[MAXPLAYERS+1];
static const char TF2Weapons[][]={"tf_weapon_fists", "tf_weapon_shovel", "tf_weapon_bat", "tf_weapon_fireaxe", "tf_weapon_bonesaw", "tf_weapon_bottle", "tf_weapon_sword", "tf_weapon_club", "tf_weapon_wrench"};
public int PiggyBack_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, PiggyBack_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, PiggyBack_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, PiggyBack_OnGameFrame);
	int AllPlayers = GetClientCount(true);
	int target = GetRandomInt(1, AllPlayers);
	while ((!IsValidClient(target)) || GetClientTeam(target) < 2 || g_Spawned[target] == false || !IsPlayerAlive(target)) {
		target = GetRandomInt(1, AllPlayers);
	}
	g_Mission[target] = 1;
	g_result = 0;
	
	return Plugin_Handled;
}

public int PiggyBack_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i) && g_Mission[i] == 1) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] > 1600.0){ 
				SetStateClient(i, true, true);
				for (int ii = 1; ii <= MaxClients; ii++) {
					if (IsValidClient(ii) && IsPlayerAlive(ii) && IsClientParticipating(ii) && g_Mission[ii] == 0 && (g_piggy[ii] <= -1)) {
						FakeClientCommand(ii, "explode");
					}
				}
			}
		}
		if (IsValidClient(i) && (g_Mission[i] == 1) && (g_result >= 3)){ 
			SetStateClient(i, false);
			FakeClientCommand(i, "explode");
			SDKUnhook(i, SDKHook_PreThink, OnPreThinkPiggy);
			SDKUnhook(i, SDKHook_OnTakeDamage, PiggyBack_Hurt);
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			for (int ii = 1; ii <= MaxClients; ii++) {
				if (IsValidClient(ii) && IsPlayerAlive(ii) && IsClientParticipating(ii) && g_Mission[ii] == 0 && g_piggy[ii]) {
					RemovePiggy(ii);				
				}
			}
		}
	}
	
	return Plugin_Continue;
}

public void PiggyBack_justEntered(int client) {
	if ((IsValidClient(client)) && (g_Mission[client] == 0)) {
		g_attack = true;
		g_piggy[client] = -1;
		SetClientClass(client, "pyro");
		RemoveClientWeapons(client);
		SetClientSlot(client, 2);
		giveitem(client, 214);
		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnyt = client;
		int rowyt = 0;
		while (columnyt > 12) {
			columnyt = columnyt - 12;
			rowyt = rowyt + 1;
		}
	 
		float pos[3];
		pos[0] = -4649.0 + float(columnyt*55);
		pos[1] = -5373.0 - float(rowyt*55);
		pos[2] = 388.0;
		
		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SDKHook(client, SDKHook_PreThink, OnPreThinkPiggy);
		SDKHook(client, SDKHook_OnTakeDamage, PiggyBack_Hurt);
		//NoCollision(true);				
		CreateTimer(0.5, GiveBuffTarget, client);
	}
	if ((IsValidClient(client)) && (g_Mission[client] == 1)){
		g_attack = true;
		SetClientClass(client, "heavy");
		RemoveClientWeapons(client);
		SetClientSlot(client, 2);
		g_piggy[client] = -1;
		giveitem(client, 5);
		float velT[3];
		velT[0] = 0.0;
		velT[1] = 0.0;
		velT[2] = 0.0;
		float posT[3]; 
		posT[0] = -3829.521973;
		posT[1] = -2421.670166;
		posT[2] = -395.943115;
		
		float angT[3];
		angT[0] = 0.0;
		angT[1] = 90.0;
		angT[2] = 0.0;
		TeleportEntity(client, posT, angT, velT);
		SDKHook(client, SDKHook_OnTakeDamage, PiggyBack_Hurt);
		SDKHook(client, SDKHook_PreThink, SDKHooks_SpeedClient);
		//NoCollision(true);
		CreateTimer(0.5, GiveBuffTarget, client);		
	}
}
public Action GiveBuffTarget( Handle timer, any client)
{
	if (IsValidClient(client)){
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 27.7);
	}
	
	return Plugin_Continue;
}
public void PiggyBack_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SDKUnhook(i, SDKHook_PreThink, OnPreThinkPiggy);
			SDKUnhook(i, SDKHook_OnTakeDamage, PiggyBack_Hurt);
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			if(g_piggy[i]){
				RemovePiggy(i);
			}
		}
	}
}
public int Piggy(int entity, int other) {
	//Classnames of entities
	char otherName[64];
	char classname[64];

	GetEdictClassname(entity, classname, sizeof(classname));
	GetEdictClassname(other, otherName, sizeof(otherName));
	
	if (strcmp(classname, PLAYER) == 0 && strcmp(otherName, PLAYER) == 0 && entity != other && (TF2_GetPlayerClass(other) == TFClass_Heavy && TF2_GetPlayerClass(entity) == TFClass_Pyro) && IsPlayerAlive(entity) && IsPlayerAlive(other) && (g_piggy[entity] <= -1) && (g_piggy[other] <= -1) && (g_piggy[other] != entity)) {
		if ((TF2_GetPlayerClass(other) != TFClass_Spy)) {
			float PlayerVec[3];
			float PlayerVec2[3];
			float vecClientEyeAng[3];
			float vecClientVel[3];
			vecClientVel[0] = 0.0;
			vecClientVel[1] = 0.0;
			vecClientVel[2] = 0.0;
			GetClientAbsOrigin(entity, PlayerVec2);
			GetClientAbsOrigin(other, PlayerVec);
			GetClientEyeAngles(other, vecClientEyeAng); // Get the angle the player is looking
			float distance;
			distance = GetVectorDistance(PlayerVec2, PlayerVec, true);
			
			if(distance <= 20000.0) {
				
				//if (IsPlayerAlive(other)) CPrintToChatEx(other, entity, "{teamcolor}%N{default} is on your back", entity);
				//if (IsPlayerAlive(other)) CPrintToChatEx(entity, other, "You are piggybacking {teamcolor}%N{default}", other);
				SetStateClient(entity, true, true);
				g_result += 1;
				SetEntData(entity, g_offsCollisionGroup, 2, 4, true);
				
				PlayerVec[2] -= 60;
				TeleportEntity(entity, PlayerVec, vecClientEyeAng, vecClientVel);
				
				char tName[32];
				GetEntPropString(other, Prop_Data, "m_iName", tName, sizeof(tName));
				DispatchKeyValue(entity, "parentname", tName);
				
				SetVariantString("!activator");
				AcceptEntityInput(entity, "SetParent", other, other, 0);
				SetVariantString("flag");
				AcceptEntityInput(entity, "SetParentAttachment", other, other, 0);
				SetEntPropFloat(entity, Prop_Send, "m_flModelScale", 0.2);
				
				g_piggy[entity] = other;
			}
		}
	}
	
	return 0;
}
public int OnPreThinkPiggy(int client) {
	int iButtons = GetClientButtons(client);
		
	if ((iButtons & IN_ATTACK2)) {
		char Weapon[128];
		int go = false;
			
		GetClientWeapon(client, Weapon, sizeof(Weapon));
		for (int i = 0; i < sizeof(TF2Weapons); i++) {
				if (StrEqual(Weapon,TF2Weapons[i],false)) go = true;
		}
		if (go) TraceTarget(client);
		//if (!(go)) PrintToChatAll("Weapon: %s", Weapon);
	}
	
	if (g_piggy[client] > -1) {
		iButtons &= ~IN_ATTACK;
		iButtons &= ~IN_ATTACK2;
		SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
	}
	
	return 0;
}
public int RemovePiggy(int entity) {
	//Classnames of entities
	char classname[64];

	GetEdictClassname(entity, classname, sizeof(classname));
	
	if (strcmp(classname, PLAYER) == 0 && (g_piggy[entity] > -1)) {
	
		int other = g_piggy[entity];
	
		//if (IsPlayerAlive(other)) CPrintToChatEx(other, entity, "{teamcolor}%N{default} jumped off your back", entity);

		AcceptEntityInput(entity, "SetParent", -1, -1, 0);
		SetEntityMoveType(entity, MOVETYPE_WALK);
		
		g_piggy[entity] = -1;
		
		if (IsValidClient(entity) && IsPlayerAlive(entity)) {
			float PlayerVec[3];
			float vecClientEyeAng[3];
			float vecClientVel[3];
			vecClientVel[0] = 0.0;
			vecClientVel[1] = 0.0;
			vecClientVel[2] = 0.0;
			GetClientAbsOrigin(other, PlayerVec);
			GetClientEyeAngles(other, vecClientEyeAng); // Get the angle the player is looking
			TeleportEntity(entity, PlayerVec, NULL_VECTOR, vecClientVel);
			SetEntPropFloat(entity, Prop_Send, "m_flModelScale", 1.0);
			SetEntData(entity, g_offsCollisionGroup, 5, 4, true);
		}
	}
	
	return 0;
}

public int TraceTarget(int client) {
	char classname[64];
	float vecClientEyePos[3];
	float vecClientEyeAng[3];
	GetClientEyePosition(client, vecClientEyePos); // Get the position of the player's eyes
	GetClientEyeAngles(client, vecClientEyeAng); // Get the angle the player is looking

	//Check for colliding entities
	TR_TraceRayFilter(vecClientEyePos, vecClientEyeAng, MASK_PLAYERSOLID, RayType_Infinite, TraceRayDontHitSelf, client);

	if (TR_DidHit(INVALID_HANDLE)) {
		int TRIndex = TR_GetEntityIndex(INVALID_HANDLE);
		GetEdictClassname(TRIndex, classname, sizeof(classname));
		if (strcmp(classname, PLAYER) == 0) Piggy(client, TRIndex);
	}
	
	return 0;
}

public bool TraceRayDontHitSelf(int entity, int mask, any data) {
	if(entity == data)	{ // Check if the TraceRay hit the itself.
		return false;	// Don't let the entity be hit
	}
	return true;	 // It didn't hit itself
}
public Action PiggyBack_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}