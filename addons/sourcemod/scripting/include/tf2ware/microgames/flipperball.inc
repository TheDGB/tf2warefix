// flipperball module

int g_iBalls[1500];
int g_iClientBalls[MAXPLAYERS+1][1500];
int g_iCurrent;
int g_ClientCurrent[MAXPLAYERS+1];
int numPrintedBallFlipper = 0;


public void Flipper_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Flipper_OnMapStart);
}	

public void Flipper_OnMapStart() {
	PrecacheModel("models/player/items/scout/soccer_ball.mdl", true);
}

public void Flipper_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Flipper_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Flipper_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Flipper_OnGameFrame);
	g_BallReady = true;
	if(g_BallReady == true){
		CreateTimer(0.5, Timer_BallSpam, _, TIMER_REPEAT);
	}
}

public void  Flipper_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] < -5303.0) SetStateClient(i, true, true);
		}
	}
	for(int b=0; b<g_iCurrent; b++) {
		char strClassname[50];
		if (IsValidEntity(g_iBalls[b])) {
			GetEdictClassname(g_iBalls[b], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_multiplayer") == 0){
				float pos[3];
				GetEntityAbsOrigin(g_iBalls[b], pos);
				if (pos[1] > 1600.0){
					SDKUnhook(g_iBalls[b], SDKHook_Touch, OnTouch);
					AcceptEntityInput(g_iBalls[b], "Kill");
				}
			}	
		}
	}
}

public void Flipper_justEntered(int client) {
	g_BallReady = true;
	if (IsValidClient(client)) {		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnyt = client;
		int rowyt = 0;
		while (columnyt > 9) {
			columnyt = columnyt - 9;
			rowyt = rowyt + 1;
		}
	 
		float pos[3];
		pos[0] = -4331.0 + float(columnyt*55);
		pos[1] = 2112.0 - float(rowyt*55);
		pos[2] = -1510.0;
		
		float ang[3];
		ang[0] = 0.0;
		ang[1] = -90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "scout");
		DisableClientWeapons(client);
		NoCollision(true);
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 27.0);
		SDKHook(client, SDKHook_OnTakeDamage, Flipper_Hurt);
		int iBall = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(iBall)){
			//DispatchKeyValue(iBall, "model", "models/player/items/scout/soccer_ball.mdl");
			//DispatchKeyValue(iBall, "disableshadows", "1");
			//DispatchKeyValue(iBall, "physicsmode", "1");
			//DispatchKeyValue(iBall, "modelscale", "2.5");
			//DispatchKeyValue(iBall, "spawnflags", "256");			
	
			SDKHook(iBall, SDKHook_Touch, OnTouch);	
			//DispatchSpawn(iBall);
			//SetEntProp(iBall, Prop_Send, "m_usSolidFlags", 8);
			//SetEntProp(iBall, Prop_Send, "m_CollisionGroup", 2);
			//new Float:hwLocation[3] = {-9913.791992, -1034.553711, 682.025940};
			//TeleportEntity(iBall, hwLocation, NULL_VECTOR, NULL_VECTOR);
			g_iBalls[g_iCurrent++] = iBall;
			g_iClientBalls[client][g_ClientCurrent[client]++] = iBall;
		}		
	}
}

public void Flipper_End() {
	char strClassname[50];	
	for(int i=0; i<g_iCurrent; i++)
	{
		if(IsValidEntity(g_iBalls[i]))
		{
			GetEdictClassname(g_iBalls[i], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_multiplayer") == 0)
			{
				AcceptEntityInput(g_iBalls[i], "Kill");				   
			}
		}
		g_iBalls[i] = 0;
		g_BallReady = false;
	}
	g_iCurrent = 0;
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, Flipper_Hurt);
	}
	g_BallReady = false;
}
public Action OnTouch(int iBall, int other)
{
	if(other && other <= MaxClients){
		FakeClientCommand(other, "explode");
	}
	
	return Plugin_Continue;
} 
public Action Timer_BallSpam(Handle timer)
{ 
	if (numPrintedBallFlipper >= 50) {		
		numPrintedBallFlipper = 0;
		return Plugin_Stop;
	}
	if(g_BallReady == true){
		char Skin[3];
		Format(Skin, 3, "%d", GetRandomInt(0,1));
		int iBall = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(iBall)){
			DispatchKeyValue(iBall, "model", "models/player/items/scout/soccer_ball.mdl");
			DispatchKeyValue(iBall, "disableshadows", "1");
			DispatchKeyValue(iBall, "physicsmode", "1");
			DispatchKeyValue(iBall,"skin", Skin);			
			DispatchKeyValue(iBall, "modelscale", "2.0");
			DispatchKeyValue(iBall, "spawnflags", "256");
			SDKHook(iBall, SDKHook_Touch, OnTouch);	
			DispatchSpawn(iBall);
			SetEntProp(iBall, Prop_Send, "m_usSolidFlags", 8);
			SetEntProp(iBall, Prop_Send, "m_CollisionGroup", 2);
			int target;
			target = GetRandomPlayer();
			float dynamicpos[3];
			if(IsValidClient(target)){
				GetClientAbsOrigin(target, dynamicpos);
			}
			dynamicpos[1] = -5124.7;
			dynamicpos[2] = 685.2;
			TeleportEntity(iBall, dynamicpos, NULL_VECTOR, NULL_VECTOR);
			g_iBalls[g_iCurrent++] = iBall;		
		}
		numPrintedBallFlipper++;
		return Plugin_Continue;
	}
	return Plugin_Continue;
}
public Action Flipper_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}