#define MeetChester_MDL "models/workshop/player/items/all_class/taunt_burstchester/taunt_burstchester_heavy.mdl"
bool g_StartMoveChester = false;
int iChesterEnt = -1;
int iChesterEntBig = -1;
bool MoveUp = false;
bool MoveUpBig = false;
int MoveUpFloat = 0;
int MoveUpFloatBig = 0;
Handle Spawntime = INVALID_HANDLE;

public int MeetChester_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, MeetChester_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, MeetChester_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, MeetChester_End);
	g_attack = false;
	g_StartMoveChester = true;
	MoveUpFloat = 0;
	MoveUpFloatBig = 0;
	MoveUpBig = false;
	SetStateAll(true);
	if(Spawntime != INVALID_HANDLE)
	{
		CloseHandle(Spawntime);
		Spawntime = INVALID_HANDLE;
	}
	CreateTimer(24.0, Timer_SpawnBigChester);
	CreateTimer(12.0, Timer_SpawnBigChester);
	CreateTimer(4.0, Timer_StartChester);
	PrecacheModel(MeetChester_MDL, true);
	
	return 0;
}
public void MeetChester_justEntered(int client) {
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
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 20.0);
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, CatchCube_Hurt);	
		NoCollision(true);		
	}
}
public Action Timer_StartChester(Handle timer) {
	int clientcount = GetPlayersAlive();
	if(clientcount >= 10)Spawntime = CreateTimer(1.5, Timer_CheckTarget, _, TIMER_REPEAT);
	else
	Spawntime = CreateTimer(2.0, Timer_CheckTarget, _, TIMER_REPEAT);
	
	return Plugin_Handled;
}
public Action Timer_CheckTarget(Handle timer) {
	if(g_StartMoveChester){
		float pos[3];
		int iClient;
		iClient = GetRandomPlayer();
		if (IsValidClient(iClient) && IsPlayerAlive(iClient) && IsClientParticipating(iClient)) {
			GetEntPropVector(iClient, Prop_Data, "m_vecOrigin", pos);
			pos[1] = pos[1] + GetRandomFloat(-50.0 , 50.0);
			pos[0] = pos[0] + GetRandomFloat(-50.0 , 50.0);
			pos[2] = -1200.0 + MoveUpFloat;
			float ang[3];
			ang[0] = -90.0;
			ang[1] = GetRandomFloat(-180.0 , 180.0);
			iChesterEnt = CreateEntityByName("prop_dynamic");
			if(IsValidEntity(iChesterEnt)){
				DispatchKeyValue(iChesterEnt, "modelscale", "5.0");
				DispatchKeyValue(iChesterEnt, "model", MeetChester_MDL);
				SetEntProp(iChesterEnt, Prop_Send, "m_nSolidType", 2);
				DispatchSpawn(iChesterEnt);
				//SetEntityRenderColor(iChesterEnt, 255, 0, 0, 255);
				TeleportEntity(iChesterEnt, pos, ang, NULL_VECTOR);
				int Rent = EntRefToEntIndex(iChesterEnt);
				CreateTimer(1.0, Timer_RemoveTarget, Rent );
				MoveUp = true;
			}
		}
	}
	
	return Plugin_Handled;
}
public Action Timer_SpawnBigChester(Handle timer) {
	if(g_StartMoveChester){
		float pos[3];
		pos[1] = 0.554688;
		pos[0] = 2308.089977;
		pos[2] = -1200.0 + MoveUpFloatBig;
		float ang[3];
		ang[0] = -90.0;
		iChesterEntBig = CreateEntityByName("prop_dynamic");
		if(IsValidEntity(iChesterEntBig)){
			DispatchKeyValue(iChesterEntBig, "modelscale", "8.0");
			DispatchKeyValue(iChesterEntBig, "model", MeetChester_MDL);
			SetEntProp(iChesterEntBig, Prop_Send, "m_nSolidType", 2);
			DispatchSpawn(iChesterEntBig);
			//SetEntityRenderColor(iChesterEntBig, 255, 0, 0, 255);
			TeleportEntity(iChesterEntBig, pos, ang, NULL_VECTOR);
			int Rent = EntRefToEntIndex(iChesterEntBig);
			CreateTimer(2.5, Timer_RemoveTarget, Rent );
			MoveUpBig = true;
		}
	}
	
	return Plugin_Handled;
}
int MeetChester_Pos(){
	float pos[3];
	if(IsValidEntity(iChesterEnt)){
		GetEntPropVector(iChesterEnt, Prop_Data, "m_vecOrigin", pos);
		pos[2] = -1200.0 + MoveUpFloat;
		TeleportEntity(iChesterEnt, pos, NULL_VECTOR, NULL_VECTOR);
	}
	
	return 0;
}
int ChesterBig_Pos(){
	float pos[3];
	if(IsValidEntity(iChesterEntBig)){
		GetEntPropVector(iChesterEntBig, Prop_Data, "m_vecOrigin", pos);
		pos[2] = -1200.0 + MoveUpFloatBig;
		TeleportEntity(iChesterEntBig, pos, NULL_VECTOR, NULL_VECTOR);
	}
	
	return 0;
}
public int MeetChester_OnGameFrame() {
	if (MoveUp && MoveUpFloat < 100) {
		MoveUpFloat += 3;
		MeetChester_Pos();
	}
	if (MoveUp == false && MoveUpFloat > 0) {
		MoveUpFloat -= 3;
		MeetChester_Pos();
	}
	if (MoveUpBig && MoveUpFloatBig < 170) {
		MoveUpFloatBig += 3;
		ChesterBig_Pos();
	}
	if (MoveUpBig == false && MoveUpFloatBig > 0) {
		MoveUpFloatBig -= 3;
		ChesterBig_Pos();
	}
	
	return 0;
}
public Action Timer_RemoveTarget(Handle timer, any iEntity) {
	if(g_StartMoveChester == true){
		if(IsValidEntity(iEntity)){
			AcceptEntityInput(iEntity, "Kill");
			MoveUp = false;
			MoveUpBig = false;
		}
	}
	
	return Plugin_Handled;
}
public Action OnTouchChester(int iEnt, int client){
	char classname[64];
	GetEdictClassname(iEnt, classname, sizeof(classname));
	if (StrEqual(classname, "prop_dynamic") && status == 2) {
		if(client <0 || client > MAXPLAYERS)return Plugin_Changed;
		FakeClientCommand(client,"explode");	
		SetStateClient(client, false, true);
	}
	return Plugin_Continue;
}
public void MeetChester_End() {
	for (int i = 1; i <= MaxClients; i++) {
		g_attack = false;
		g_StartMoveChester = false;
		SDKUnhook(i, SDKHook_OnTakeDamage, CatchCube_Hurt);
	}
	if(IsValidEntity(iChesterEnt)){
		AcceptEntityInput(iChesterEnt, "Kill");
	}
	if(Spawntime != INVALID_HANDLE)
	{
		CloseHandle(Spawntime);
		Spawntime = INVALID_HANDLE;
	} 
}