
#define GrapCow_MDL "models/props_2fort/cow001_reference.mdl"

int g_GrapCow;
int g_GrapCow2;
int g_GrapCow3;
int g_GrapCowStart = false;
public void Grap_Cow_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Grap_Cow_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Grap_Cow_End);	
	g_GrapCowStart = true;
	CreateTimer(0.1, CreateGrapCow);
	g_attack = true;  
}
public void Grap_Cow_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Grap_Cow_OnMapStart);
}	
public void Grap_Cow_OnMapStart() {
	PrecacheModel(GrapCow_MDL, true);	
}
public void Grap_Cow_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		int columncow = client;
		int rowcow = 0;
		while (columncow > 2) {
		columncow = columncow - 2;
		rowcow = rowcow + 1;
		}
		
		float pos[3];
		pos[0] = 2837.0 + float(columncow*90);
		pos[1] = 257.0 - float(rowcow*90);
		pos[2] = -1052.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = -180.0;
		ang[2] = 0.0;

		TeleportEntity(client, pos, ang, vel);
		RemoveClientWeapons(client);
		SetClientClass(client, "demoman");
		giveitem(client, 307);
		ClientCommand(client,"slot6");
		ForceGrapplingHook(client);
		//TpEnabled(client);
		NoCollision(true);
		g_attack = true;
		SDKHook(client, SDKHook_OnTakeDamage, Cow_Hurt);		
	}
}
public void Grap_Cow_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			//TpDisabled(i);
			SDKUnhook(i, SDKHook_OnTakeDamage, Cow_Hurt);
			RemoveClientWeapons(i);
		}
	}
	SDKUnhook(g_GrapCow, SDKHook_OnTakeDamage, Cow_Hurt2);
	SDKUnhook(g_GrapCow2, SDKHook_OnTakeDamage, Cow_Hurt2);
	SDKUnhook(g_GrapCow3, SDKHook_OnTakeDamage, Cow_Hurt2);
	if (IsValidEntity(g_GrapCow)) {
		AcceptEntityInput(g_GrapCow, "Kill");
	}
	if (IsValidEntity(g_GrapCow2)) {
		AcceptEntityInput(g_GrapCow2, "Kill");
	}
	if (IsValidEntity(g_GrapCow3)) {
		AcceptEntityInput(g_GrapCow3, "Kill");
	}
	g_attack = false;
	g_GrapCowStart = false;
}
public Action CreateGrapCow(Handle timer){
	if(g_GrapCowStart){
		
		g_GrapCow = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_GrapCow, GrapCow_MDL);
		DispatchKeyValue(g_GrapCow, "modelscale", "1.0");
		DispatchKeyValue(g_GrapCow, "spawnflags", "1097");
		SetEntProp(g_GrapCow, Prop_Send, "m_nSolidType", 2);
		//DispatchKeyValue(g_GrapCow, "collisions", "Use Bounding Box");
		SetEntityMoveType(g_GrapCow, MOVETYPE_VPHYSICS);
		DispatchSpawn(g_GrapCow);
		float port26Loc[3];
		port26Loc[0] = 1353.001343;
		port26Loc[1] = 0.024048;
		port26Loc[2] = -551.177124;
		TeleportEntity(g_GrapCow, port26Loc, NULL_VECTOR, NULL_VECTOR);		  
		SDKHook(g_GrapCow, SDKHook_OnTakeDamage, Cow_Hurt2);
		//SetEntityMoveType(g_GrapCow, MOVETYPE_NONE);
		
		g_GrapCow2 = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_GrapCow2, GrapCow_MDL);
		DispatchKeyValue(g_GrapCow2, "modelscale", "1.0");
		DispatchKeyValue(g_GrapCow2, "spawnflags", "1097");
		SetEntProp(g_GrapCow2, Prop_Send, "m_nSolidType", 2);
		//DispatchKeyValue(g_GrapCow2, "collisions", "Use Bounding Box");
		SetEntityMoveType(g_GrapCow2, MOVETYPE_VPHYSICS);
		DispatchSpawn(g_GrapCow2);
		float port27Loc[3];
		port27Loc[0] = 1353.001343;
		port27Loc[1] = -500.024048;
		port27Loc[2] = -551.177124;
		TeleportEntity(g_GrapCow2, port27Loc, NULL_VECTOR, NULL_VECTOR);		  
		SDKHook(g_GrapCow2, SDKHook_OnTakeDamage, Cow_Hurt2);
		//SetEntityMoveType(g_GrapCow2, MOVETYPE_NONE);
		
		g_GrapCow3 = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_GrapCow3, GrapCow_MDL);
		DispatchKeyValue(g_GrapCow3, "modelscale", "1.0");
		DispatchKeyValue(g_GrapCow3, "spawnflags", "1097");
		SetEntProp(g_GrapCow3, Prop_Send, "m_nSolidType", 2);
		//DispatchKeyValue(g_GrapCow3, "collisions", "Use Bounding Box");
		SetEntityMoveType(g_GrapCow3, MOVETYPE_VPHYSICS);
		DispatchSpawn(g_GrapCow3);
		float port28Loc[3];
		port28Loc[0] = 1353.001343;
		port28Loc[1] = 500.024048;
		port28Loc[2] = -551.177124;
		TeleportEntity(g_GrapCow3, port28Loc, NULL_VECTOR, NULL_VECTOR);		  
		SDKHook(g_GrapCow3, SDKHook_OnTakeDamage, Cow_Hurt2);
		//SetEntityMoveType(g_GrapCow3, MOVETYPE_NONE);
	}
	
	return Plugin_Handled;
}
public Action Cow_Hurt2(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	
	if (IsValidClient(attacker)) SetStateClient(attacker, true, true);
	damage = 0.0;
	return Plugin_Changed;
}
public Action Cow_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype){
	
	if ((IsValidClient(attacker)) || (IsValidClient(victim)))
	damage = 0.0;
	return Plugin_Changed;
}