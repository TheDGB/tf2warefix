// NEED TO MODIFY SO THAT IT IS A SHOOTING RANGE

//#define TALPA_TARGET "models/props/metal_box.mdl"
#define TALPA_TARGET "models/props/metal_box.mdl"
//#define TALPA_BASEPOS -6453
int g_iCubes[1500];
int g_iCurrent2;
int TalpaPrinted;
int Last = 0;

public void CatchCube_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, CatchCube_OnMapStart);
}	

public void CatchCube_OnMapStart() {
	AddFileToDownloadsTable("models/props/metal_box.dx90.vtx");
	AddFileToDownloadsTable("models/props/metal_box.mdl");
	AddFileToDownloadsTable("models/props/metal_box.phy");
	AddFileToDownloadsTable("models/props/metal_box.sw.vtx");
	AddFileToDownloadsTable("models/props/metal_box.vvd");
	AddFileToDownloadsTable("materials/models/props/metal_box.vmt");
	AddFileToDownloadsTable("materials/models/props/metal_box.vtf");
	AddFileToDownloadsTable("materials/models/props/metal_box_exponent.vtf");
	AddFileToDownloadsTable("materials/models/props/metal_box_lightwarp.vtf");
	AddFileToDownloadsTable("materials/models/props/metal_box_normal.vtf");
	AddFileToDownloadsTable("materials/models/props/metal_box_skin001.vmt");
	AddFileToDownloadsTable("materials/models/props/metal_box_skin001.vtf");
	PrecacheModel(TALPA_TARGET, true);
}

public void CatchCube_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, CatchCube_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, CatchCube_End);
	randomposspawn = GetRandomInt(0,1);	
	g_attack = true;
	g_StartTalpa = true;
}
public Action Timer_TalpaSpam(Handle timer) {
	if(g_StartTalpa == true){
	// Create a global variable visible only in the local scope (this function)
		while(TalpaPrinted == Last)
		{
			TalpaPrinted = GetRandomInt(0, 10);
		}
		Last = TalpaPrinted; 
 
		if (TalpaPrinted >= 3) {		
			TalpaPrinted = 0;
			return Plugin_Stop;
		}
		int iCube = CreateEntityByName("prop_physics");
		if(IsValidEntity(iCube)){
			DispatchKeyValue(iCube, "model", "models/props/metal_box.mdl");
			DispatchKeyValue(iCube, "disableshadows", "1");		
			DispatchKeyValue(iCube, "physicsmode", "1");			
			DispatchKeyValue(iCube, "modelscale", "1.0");
			DispatchKeyValue(iCube, "spawnflags", "256");		
			DispatchSpawn(iCube);
			//SetEntProp(iCube, Prop_Send, "m_usSolidFlags", 8);
			//SetEntProp(iCube, Prop_Send, "m_CollisionGroup", 2);
			switch (GetRandomInt(0, 10))
			{
				case 0:
				{
					float hw0Loc[3] = {3005.853760, 738.195862, -1070.968750};
					TeleportEntity(iCube, hw0Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 1:
				{
					float hw1Loc[3] = {1538.104126, 776.447205, -1070.968750};
					TeleportEntity(iCube, hw1Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 2:
				{
					float hw2Loc[3] = {1599.710571, -684.009338, -1070.968750};
					TeleportEntity(iCube, hw2Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 3:
				{
					float hw3Loc[3] = {3097.868408, -778.822449, -1070.968750};
					TeleportEntity(iCube, hw3Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 4:
				{
					float hw4Loc[3] = {2861.411377, -43.681313, -1070.968750};
					TeleportEntity(iCube, hw4Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 5:
				{
					float hw5Loc[3] = {2273.404541, 515.266113, -1070.968750};
					TeleportEntity(iCube, hw5Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 6:
				{
					float hw6Loc[3] = {1671.103394, 17.915150, -1070.968750};
					TeleportEntity(iCube, hw6Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 7:
				{
					float hw7Loc[3] = {2288.009277, -620.833069, -1070.968750};
					TeleportEntity(iCube, hw7Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 8:
				{
					float hw8Loc[3] = {2309.599121, 61.214115, -816.423279};
					TeleportEntity(iCube, hw8Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 9:
				{
					float hw9Loc[3] = {1906.734619, 358.714203, -895.415527};
					TeleportEntity(iCube, hw9Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 10:
				{
					float hw10Loc[3] = { 2759.260254, -452.046021, -888.494263};
					TeleportEntity(iCube, hw10Loc, NULL_VECTOR, NULL_VECTOR);
				}
		
			}
			g_iCubes[g_iCurrent2++] = iCube;		
			SDKHook(iCube, SDKHook_Touch, OnTouchCube);
			if(IsValidEntity(iCube)){
				CreateTimer(2.4, Timer_TalpaKill, iCube);
			}		
		}
		TalpaPrinted++;
		return Plugin_Continue;
	}
	return Plugin_Continue;
}
public Action Timer_TalpaKill(Handle timer, any iCube)
{
	if(IsValidEntity(iCube)){
		SDKUnhook(iCube, SDKHook_Touch, OnTouchCube);
		AcceptEntityInput(iCube, "Kill");		 
	}
	return Plugin_Stop;
}
public void CatchCube_End() {
	char strClassname[50];	
	for(int x=0; x<g_iCurrent2; x++)
	{
		if(IsValidEntity(g_iCubes[x]))
		{
			GetEdictClassname(g_iCubes[x], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics") == 0)
			{
				SDKUnhook(g_iCubes[x], SDKHook_Touch, OnTouchCube);
				AcceptEntityInput(g_iCubes[x], "Kill");
			}
		}
		g_iCubes[x] = 0;
	}
	g_iCurrent2 = 0;
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, CatchCube_Hurt);
	}
	g_StartTalpa = false;
	g_attack = false;
}


public Action CatchCube_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}

public void CatchCube_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int totalp = GetActivePlayers();
		int posap = 360 / totalp * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 2307.0 + (Cosine(DegToRad(float(posap)))*350.0);
		pos[1] = -52.0 - (Sine(DegToRad(float(posap)))*350.0);
		pos[2] = -1038.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(posap);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);	
		if (randomposspawn == 0) {	
			SetClientClass(client, "scout");
			RemoveClientWeapons(client);
			giveitem(client, 349);
		}
		if (randomposspawn == 1) {
			SetClientClass(client, "scout");
			RemoveClientWeapons(client);
			giveitem(client, 317);
		}
		g_Minipoints[client] = 0;		
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 20.0);
		CreateTimer(2.5, Timer_TalpaSpam, client, TIMER_REPEAT);
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, CatchCube_Hurt);	
		NoCollision(true);
		
	}
}

public Action OnTouchCube(int iCube, int client){
	char classname[64];
	GetEdictClassname(iCube, classname, sizeof(classname));
	if (StrEqual(classname, "prop_physics") && status == 2) {
		if(client <0 || client > MAXPLAYERS)return Plugin_Changed;
		if (!g_Complete[client]){	
			PlaySound(client);
			g_Minipoints[client] += 1;
			SDKUnhook(iCube, SDKHook_Touch, OnTouchCube);
			AcceptEntityInput(iCube, "Kill" );	
			if (g_Minipoints[client] >= 3) SetStateClient(client, true, true);
		}
	}
	return Plugin_Continue;
}

public void PlaySound(int client){
	if (IsValidClient(client)){
		EmitSoundToClient(client, SOUND_MINISCORE, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
	}			 
}