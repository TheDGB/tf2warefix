// Airraid module
#if !defined Team_Spec
#define Team_Spec 1
#endif
#if !defined Team_Red
#define Team_Red 2
#endif
#if !defined Team_Blu
#define Team_Blu 3
#endif

#define PARATARGET_MDL "models/props_coldfront/waste_base.mdl"
int g_ParaTarget;

public void Paracadute_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Paracadute_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Paracadute_End);	
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Paracadute_CheckDeadPlayers);
	g_ParaTargetStart = true;
	CreateTimer(0.1, CreateParaTarget);
	g_attack = true;  
}
public void Paracadute_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Paracadute_OnMapStart);
}   
public void Paracadute_OnMapStart() {
	PrecacheModel(PARATARGET_MDL, true);	
}
public void Paracadute_justEntered(int client) {
	if (IsValidClient(client)) {
		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int totalp = GetActivePlayers();
		int posap = 360 / totalp * (g_Id[client]-1);
			
		float pos[3];
		pos[0] = 2316.0 + (Cosine(DegToRad(float(posap)))*370.0);
		pos[1] = -57.0 - (Sine(DegToRad(float(posap)))*370.0);
		pos[2] = 0.0;
			
		float ang[3];
		ang[0] = 90.0;
		ang[1] = float(180-posap);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "demoman");
		RemoveClientWeapons(client);
		giveitem(client, 1101);
		SetClientSlot(client, 0);
		giveitem(client, 939);
		TpEnabled(client);
		NoCollision(true);
		g_attack = true;			
	}
}
public void Paracadute_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void Paracadute_End() {
	for (int i = 1; i <= MaxClients; i++) {
		TpDisabled(i);
		g_attack = false;
	}
	g_ParaTargetStart = false;
	SDKUnhook(g_ParaTarget, SDKHook_StartTouch, OnTouchParaTarget);
	if(IsValidEntity(g_ParaTarget)){
		AcceptEntityInput(g_ParaTarget, "Kill");
	}
}
public Action CreateParaTarget(Handle timer){
	if(g_ParaTargetStart == true){
		g_ParaTarget = CreateEntityByName("prop_physics_override");
		SetEntityModel(g_ParaTarget, PARATARGET_MDL);
		DispatchKeyValue(g_ParaTarget, "modelscale", "1.0");
		DispatchKeyValue(g_ParaTarget, "skin", "0");
		DispatchKeyValue(g_ParaTarget, "physicsmode", "1");
		DispatchKeyValue(g_ParaTarget, "spawnflags", "256");
		//DispatchKeyValue(g_ParaTarget, "collisions", "Use Bounding Box");
		DispatchSpawn(g_ParaTarget);
		float port24Loc[3];
		port24Loc[0] = 2311.354980;
		port24Loc[1] = -23.636475;
		port24Loc[2] = -850.955811;

		
		float ang24[3];
		ang24[0] = 0.0;
		ang24[1] = 0.0;
		ang24[2] = 0.0;
		TeleportEntity(g_ParaTarget, port24Loc, ang24, NULL_VECTOR);
		SetEntityMoveType(g_ParaTarget, MOVETYPE_NONE);
		SDKHook(g_ParaTarget, SDKHook_StartTouch, OnTouchParaTarget);
	}
	
	return Plugin_Continue;
}

public int OnTouchParaTarget(int entity, int client)
{
	if(client <0 || client > MAXPLAYERS)return false;
	if(IsValidEntity(entity) && IsClientParticipating(client)){
		SetStateClient(client, true, true);
	}
	return true;
}