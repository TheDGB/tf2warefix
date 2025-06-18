//Portal minigame by TonyBaretta
#define PORTAL_MDL "models/props_manor/doorframe_01_door_01.mdl"
#define PORTAL2_MDL "models/props_gameplay/sign_wood_cap001.mdl"

int g_Portal1;
int g_Portal2;
int g_Portal3;
int g_BackPortal;
int g_BackPortal2;
int g_BackPortal3;

public void Portals_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Portals_OnMapStart);
}	
public void Portals_OnMapStart() {
	PrecacheModel(PORTAL_MDL, true);
	PrecacheModel(PORTAL2_MDL, true);	
}
public void Portals_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Portals_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Portals_End);	
	
	g_attack = false;
	g_PortalStart = true;
	CreateTimer(0.5, CreatePortals);
		
}

public Action CreatePortals(Handle timer){
	if(g_PortalStart == true){
		g_BackPortal = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_BackPortal, PORTAL2_MDL);
		DispatchKeyValue(g_BackPortal, "modelscale", "0.5");
		DispatchKeyValue(g_BackPortal, "skin", "0");
		DispatchSpawn(g_BackPortal);
		float port24Loc[3];
		port24Loc[0] = 512.968750;
		port24Loc[1] = 313.954407;
		port24Loc[2] = -630.968681;
		float ang24[3];
		ang24[0] = 0.0;
		ang24[1] = 180.0;
		ang24[2] = 0.0;
		TeleportEntity(g_BackPortal, port24Loc, ang24, NULL_VECTOR);
		SetEntityMoveType(g_BackPortal, MOVETYPE_NONE);

		g_BackPortal2 = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_BackPortal2, PORTAL2_MDL);
		DispatchKeyValue(g_BackPortal2, "modelscale", "0.5");
		DispatchKeyValue(g_BackPortal2, "skin", "1");
		DispatchSpawn(g_BackPortal2);
		float port25Loc[3];
		port25Loc[0] = 512.968750;
		port25Loc[1] = 0.954407;
		port25Loc[2] = -630.968681;
		float ang25[3];
		ang25[0] = 0.0;
		ang25[1] = 180.0;
		ang25[2] = 0.0;
		TeleportEntity(g_BackPortal2, port25Loc, ang25, NULL_VECTOR);
		SetEntityMoveType(g_BackPortal2, MOVETYPE_NONE);
		
		g_BackPortal3 = CreateEntityByName("prop_dynamic");
		SetEntityModel(g_BackPortal3, PORTAL2_MDL);
		DispatchKeyValue(g_BackPortal3, "modelscale", "0.5");
		DispatchKeyValue(g_BackPortal3, "skin", "2");
		DispatchSpawn(g_BackPortal3);
		float port26Loc[3];
		port26Loc[0] = 512.968750;
		port26Loc[1] = -324.954407;
		port26Loc[2] = -630.968681;
		float ang26[3];
		ang26[0] = 0.0;
		ang26[1] = 180.0;
		ang26[2] = 0.0;
		TeleportEntity(g_BackPortal3, port26Loc, ang26, NULL_VECTOR);
		SetEntityMoveType(g_BackPortal3, MOVETYPE_NONE);
		

		g_Portal1 = CreateEntityByName("prop_physics_override");
		SetEntityModel(g_Portal1, PORTAL_MDL);
		DispatchSpawn(g_Portal1);
		float portLoc[3] = {512.968750, 290.954407, -844.968689};
		SetEntProp(g_Portal1, Prop_Send, "m_nSolidType", 6);
		TeleportEntity(g_Portal1, portLoc, NULL_VECTOR, NULL_VECTOR);
		SetEntityMoveType(g_Portal1, MOVETYPE_NONE);
		SDKHook(g_Portal1, SDKHook_StartTouch, OnTouchPortal);
		
		g_Portal2 = CreateEntityByName("prop_physics_override");
		SetEntityModel(g_Portal2, PORTAL_MDL);
		DispatchSpawn(g_Portal2);
		float port2Loc[3] = {512.968750, -25.0, -844.968689};
		TeleportEntity(g_Portal2, port2Loc, NULL_VECTOR, NULL_VECTOR);
		SetEntProp(g_Portal2, Prop_Send, "m_nSolidType", 6);
		SetEntityMoveType(g_Portal2, MOVETYPE_NONE);
		SDKHook(g_Portal2, SDKHook_StartTouch, OnTouchPortal);

		g_Portal3 = CreateEntityByName("prop_physics_override");
		SetEntityModel(g_Portal3, PORTAL_MDL);
		DispatchSpawn(g_Portal3);
		float port3Loc[3] = {512.968750, -349.313385, -844.968689};
		SetEntProp(g_Portal3, Prop_Send, "m_nSolidType", 6);
		TeleportEntity(g_Portal3, port3Loc, NULL_VECTOR, NULL_VECTOR);
		SetEntityMoveType(g_Portal3, MOVETYPE_NONE);
		SDKHook(g_Portal3, SDKHook_StartTouch, OnTouchPortal);
	}
	
	return Plugin_Continue;
}

public int OnTouchPortal(int entity, int client)
{
	if(client <0 || client > MAXPLAYERS)return false;
	if(!IsClientParticipating(client))return false;
	if(IsValidEntity(entity) && IsClientParticipating(client)){
		switch (GetRandomInt(0, 1))
		{
			case 0:
			{
				float Final[3] = {387.300262, 2545.674316, 507.735199};
				TeleportEntity(client, Final, NULL_VECTOR, NULL_VECTOR);
				SetStateClient(client, true, true);
			}
			case 1:
			{
				float Final2[3] = {146.950500, 1156.312988, -69.223061};
				TeleportEntity(client, Final2, NULL_VECTOR, NULL_VECTOR);
				SetStateClient(client, false, true);
				//FakeClientCommand(client, "explode");
			}
		}
	}
	return true;
}
public void Portals_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "pyro");
		DisableClientWeapons(client);
		ClientCommand(client, "thirdperson");
		NoCollision(true);
	}
}

public void Portals_End() {
	for (int i = 1; i <= MaxClients; i++) {
	//SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
	if (IsValidClient(i))ClientCommand(i, "wait; firstperson");
	}
	if (IsValidEntity(g_Portal1)){
		SDKUnhook(g_Portal1, SDKHook_StartTouch, OnTouchPortal);
		AcceptEntityInput(g_Portal1, "Kill");
	}
	if (IsValidEntity(g_Portal2)){
		SDKUnhook(g_Portal2, SDKHook_StartTouch, OnTouchPortal);
		AcceptEntityInput(g_Portal2, "Kill");
	}
	if (IsValidEntity(g_Portal3)){
		SDKUnhook(g_Portal3, SDKHook_StartTouch, OnTouchPortal);
		AcceptEntityInput(g_Portal3, "Kill");
	}
	if (IsValidEntity(g_BackPortal)){
		AcceptEntityInput(g_BackPortal, "Kill");
	}
	if (IsValidEntity(g_BackPortal2)){
		AcceptEntityInput(g_BackPortal2, "Kill");
	}
	if (IsValidEntity(g_BackPortal3)){
		AcceptEntityInput(g_BackPortal3, "Kill");
	}
	g_PortalStart = false;
}