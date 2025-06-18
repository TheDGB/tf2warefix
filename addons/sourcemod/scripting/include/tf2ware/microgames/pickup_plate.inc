
#define Plate_mdl "models/items/plate.mdl"
#define Platexmas_mdl "models/items/plate_sandwich_xmas.mdl"
#define Platerobo_mdl "models/items/plate_robo_sandwich.mdl"
#define Platesteak_mdl "models/workshop/weapons/c_models/c_buffalo_steak/plate_buffalo_steak.mdl"
#define Platebanana_mdl "models/items/banana/plate_banana.mdl"
#define Platecioccolate_mdl "models/workshop/weapons/c_models/c_chocolate/plate_chocolate.mdl"
#define Platefishcake_mdl "models/workshop/weapons/c_models/c_fishcake/plate_fishcake.mdl"

int Ent_Plate = -1;
int Ent_PlateX = -1;
int Ent_PlateR = -1;
int Ent_PlateS = -1;
int Ent_PlateB = -1;
int Ent_PlateC = -1;
int Ent_PlateF = -1;

int Ent_PlateStart = false;
public void Pick_up_plate_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Pick_up_plate_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Pick_up_plate_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Pick_up_plate_End);
	//SetStateAll(false);
	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			g_Mission[i] = GetRandomInt(0,6);
			SetStateClient(i, false, true);
		}
	}
	Ent_PlateStart = true;
	CreateTimer(1.0, CreatePlate);
	CreateTimer(1.0, CreatePlateXmas);
	CreateTimer(1.0, CreatePlateRobo);
	CreateTimer(1.0, CreatePlateSteak);
	CreateTimer(1.0, CreatePlateBanana);
	CreateTimer(1.0, CreatePlateCioccolate);
	CreateTimer(1.0, CreatePlateFish);
}
public void Pick_up_plate_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Pick_up_plate_OnMapStart);
}	
public void Pick_up_plate_OnMapStart() {
	PrecacheModel(Plate_mdl, true);
	PrecacheModel(Platexmas_mdl, true);
	PrecacheModel(Platerobo_mdl, true);
	PrecacheModel(Platesteak_mdl, true);
	PrecacheModel(Platebanana_mdl, true);
	PrecacheModel(Platecioccolate_mdl, true);
	PrecacheModel(Platefishcake_mdl, true);
}
public void Pick_up_plate_justEntered(int client) {
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
		pos[0] = 745.0 + float(columncow*90);
		pos[1] = 2979.0 - float(rowcow*90);
		pos[2] = 316.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = -180.0;
		ang[2] = 0.0;

		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "heavy");
		RemoveClientWeapons(client);
		NoCollision(true);
		g_attack = true;
		giveitem(client, 239);
		SDKHook(client, SDKHook_OnTakeDamage, Plate_Hurt);
		SetStateClient(client, false, true);
		SetEntityHealth(client, 150);
	}
}
public void Pick_up_plate_End() {
	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			//TpDisabled(i);
			SDKUnhook(i, SDKHook_OnTakeDamage, Plate_Hurt);
		}
	}
	if (IsValidEntity(Ent_Plate)) {
		SDKUnhook(Ent_Plate, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_Plate, "Kill");
		Ent_Plate = -1;
	}
	if (IsValidEntity(Ent_PlateX)) {
		SDKUnhook(Ent_PlateX, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateX, "Kill");
		Ent_PlateX = -1;
	}
	if (IsValidEntity(Ent_PlateR)) {
		SDKUnhook(Ent_PlateR, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateR, "Kill");
		Ent_PlateR = -1;
	}
	if (IsValidEntity(Ent_PlateS)) {
		SDKUnhook(Ent_PlateS, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateS, "Kill");
		Ent_PlateS = -1;
	}
	if (IsValidEntity(Ent_PlateB)) {
		SDKUnhook(Ent_PlateB, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateB, "Kill");
		Ent_PlateB = -1;
	}
	if (IsValidEntity(Ent_PlateC)) {
		SDKUnhook(Ent_PlateC, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateC, "Kill");
		Ent_PlateC = -1;
	}
	if (IsValidEntity(Ent_PlateF)) {
		SDKUnhook(Ent_PlateF, SDKHook_StartTouch, OnTouchPlate);
		AcceptEntityInput(Ent_PlateF, "Kill");
		Ent_PlateF = -1;
	}
	g_attack = false;
	Ent_PlateStart = false;
}
public Action CreatePlate(Handle timer){
	if(Ent_PlateStart){
		Ent_Plate = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_Plate)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 3000.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_Plate, Plate_mdl);
			DispatchKeyValue(Ent_Plate, "modelscale", "2.0");
			DispatchKeyValue(Ent_Plate, "targetname", "sandvich"); 
			SetEntProp(Ent_Plate, Prop_Send, "m_nSolidType", 2);
			SetEntityMoveType(Ent_Plate, MOVETYPE_NONE);
			DispatchSpawn(Ent_Plate);
			TeleportEntity(Ent_Plate, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_Plate, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_Plate, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;	
}
public Action CreatePlateXmas(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateX = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateX)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 2800.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateX, Platexmas_mdl);
			DispatchKeyValue(Ent_PlateX, "modelscale", "2.0");
			SetEntProp(Ent_PlateX, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateX, "targetname", "sandvichx");
			SetEntityMoveType(Ent_PlateX, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateX);
			TeleportEntity(Ent_PlateX, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateX, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateX, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;	
}
public Action CreatePlateRobo(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateR = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateR)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 2600.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateR, Platerobo_mdl);
			DispatchKeyValue(Ent_PlateR, "modelscale", "2.0");
			SetEntProp(Ent_PlateR, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateR, "targetname", "sandvichr");
			SetEntityMoveType(Ent_PlateR, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateR);
			TeleportEntity(Ent_PlateR, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateR, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateR, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;		
}
public Action CreatePlateSteak(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateS = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateS)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 2400.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateS, Platesteak_mdl);
			DispatchKeyValue(Ent_PlateS, "modelscale", "2.0");
			SetEntProp(Ent_PlateS, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateS, "targetname", "steak_plate");
			SetEntityMoveType(Ent_PlateS, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateS);
			TeleportEntity(Ent_PlateS, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateS, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateS, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;		
}
public Action CreatePlateBanana(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateB = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateB)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 2200.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateB, Platebanana_mdl);
			DispatchKeyValue(Ent_PlateB, "modelscale", "2.0");
			SetEntProp(Ent_PlateB, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateB, "targetname", "banana_plate");
			SetEntityMoveType(Ent_PlateB, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateB);
			TeleportEntity(Ent_PlateB, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateB, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateB, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;		
}
public Action CreatePlateCioccolate(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateC = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateC)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 2000.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateC, Platecioccolate_mdl);
			DispatchKeyValue(Ent_PlateC, "modelscale", "2.0");
			SetEntProp(Ent_PlateC, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateC, "targetname", "cioc");
			SetEntityMoveType(Ent_PlateC, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateC);
			TeleportEntity(Ent_PlateC, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateC, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateC, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;		
}
public Action CreatePlateFish(Handle timer){
	if(Ent_PlateStart){
		Ent_PlateF = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(Ent_PlateF)){
			float plateLoc[3];
			plateLoc[0] = 248.0; // setpos 248.0 2769.633789 260.0;setang 70.026016 1.954027 0.000000
			plateLoc[1] = 1800.0;
			plateLoc[2] = 300.0;
			SetEntityModel(Ent_PlateF, Platefishcake_mdl);
			DispatchKeyValue(Ent_PlateF, "modelscale", "2.0");
			SetEntProp(Ent_PlateF, Prop_Send, "m_nSolidType", 2);
			DispatchKeyValue(Ent_PlateF, "targetname", "fishcake");
			SetEntityMoveType(Ent_PlateF, MOVETYPE_NONE);
			DispatchSpawn(Ent_PlateF);
			TeleportEntity(Ent_PlateF, plateLoc, NULL_VECTOR, NULL_VECTOR);
			CreateParticle(Ent_PlateF, "h2013_corpse_flame", true, 10.0);
			SDKHook(Ent_PlateF, SDKHook_StartTouch, OnTouchPlate);
		}
	}	

    return Plugin_Stop;		
}
public void OnTouchPlate(int entity, int client)
{
	float vel[3];
	vel[0] = 0.0;
	vel[1] = 0.0;
	vel[2] = 0.0;
	float Finalpos[3];
	Finalpos[0] = 1066.0;
	Finalpos[1] = 2517.0;
	Finalpos[2] = 260.0;
	float ang[3];
	ang[0] = 0.0;
	ang[1] = 180.0;
	ang[2] = 0.0;
	if(client <0 || client > MAXPLAYERS)return;
	if(!IsClientParticipating(client))return;
	if(IsValidEntity(entity) && IsClientParticipating(client)){
		switch(g_Mission[client]){
			case 0:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "sandvich") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 42);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 1:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "sandvichx") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 1002);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 2:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "sandvichr") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 863);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 3:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "banana_plate") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 1190);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 4:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "steak_plate") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 311);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 5:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "cioc") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 159);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
			case 6:
			{
				char strName[50];
				GetEntPropString(entity, Prop_Data, "m_iName", strName, sizeof(strName));

				if(strcmp(strName, "fishcake") == 0)
				{
					//SetStateClient(client, true, true);
					TeleportEntity(client, Finalpos, ang, vel);
					giveitem(client, 433);
					return;
				}
				else
				TF2_StunPlayer(client, 0.5, _, TF_STUNFLAGS_GHOSTSCARE);
			}
		}
	}
}
public void Pick_up_plate_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			if ((TF2_IsPlayerInCondition(i, TFCond_Taunting)) && (sandwich[i])) {
				SetStateClient(i, true, true);
				sandwich[i] = false;
			}
		}
	}
}
public Action Plate_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	
	if ((IsValidClient(attacker)) || (IsValidClient(victim)))
	damage = 0.0;
	return Plugin_Changed;
}