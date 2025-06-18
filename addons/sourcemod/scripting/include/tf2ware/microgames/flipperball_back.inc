// flipperball module

int g_iBallBacks[1500];
int g_iClientBallsBack[MAXPLAYERS+1][1500];
int g_iCurrentBackBall;
int g_ClientCurrentBallBack[MAXPLAYERS+1];
int numPrintedBallFlipperBack = 0;


public int FlipperBack_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, FlipperBack_OnMapStart);
	
	return 0;
}	

public int FlipperBack_OnMapStart() {
	PrecacheModel("models/player/items/scout/soccer_ball.mdl", true);
	
	return 0;
}

public int FlipperBack_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, FlipperBack_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, FlipperBack_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, FlipperBack_OnGameFrame);
	g_BallBackReady = true;
	if(g_BallBackReady == true){
		CreateTimer(1.0, Timer_BallBackSpam, _, TIMER_REPEAT);
	}
	
	return 0;
}
public int FlipperBack_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] > -4376.0)SetEntProp(i, Prop_Data, "m_takedamage", 2, 1);
			if (pos[1] > 1600.0) SetStateClient(i, true, true);
		}
	}
	for(int b=0; b<g_iCurrentBackBall; b++) {
		char strClassname[50];
		if (IsValidEntity(g_iBallBacks[b])) {
			GetEdictClassname(g_iBallBacks[b], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_multiplayer") == 0){
				float pos[3];
				GetEntityAbsOrigin(g_iBallBacks[b], pos);
				if (pos[1] > 1600.0){
					SDKUnhook(g_iBallBacks[b], SDKHook_Touch, OnTouchBallBack);
					AcceptEntityInput(g_iBallBacks[b], "Kill");
				}
			}	
		}
	}
	
	return 0;
}

public void FlipperBack_justEntered(int client) {
	g_BallBackReady = true;
	if (IsValidClient(client)) {
		SetClientClass(client, "pyro");
		DisableClientWeapons(client);
		
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
		NoCollision(true);
		SDKHook(client, SDKHook_OnTakeDamage, FlipperBack_Hurt);
		int iBallBack = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(iBallBack)){	
			SDKHook(iBallBack, SDKHook_Touch, OnTouchBallBack);
			g_iBallBacks[g_iCurrentBackBall++] = iBallBack;
			g_iClientBallsBack[client][g_ClientCurrentBallBack[client]++] = iBallBack;
		}
		SetEntProp(client, Prop_Data, "m_takedamage", 0, 1);
		CreateTimer(0.5, GiveBuff, client);
		//CreateTimer(2.0, Backmortal, client);
	}
}
public Action Backmortal( Handle timer, any client)
{
	if (IsValidClient(client)){
		SetEntProp(client, Prop_Data, "m_takedamage", 2, 1);
	}
	
	return Plugin_Continue;
}
public Action GiveBuff( Handle timer, any client)
{
	if (IsValidClient(client)){
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 27.7);
	}
	
	return Plugin_Continue;
}
public void FlipperBack_End() {
	char strClassname[50];	
	for(int i=0; i<g_iCurrentBackBall; i++)
	{
		if(IsValidEntity(g_iBallBacks[i]))
		{
			GetEdictClassname(g_iBallBacks[i], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_multiplayer") == 0)
			{
				AcceptEntityInput(g_iBallBacks[i], "Kill");				   
			}
		}
		g_iBallBacks[i] = 0;
		g_BallBackReady = false;
	}
	g_iCurrentBackBall = 0;
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, FlipperBack_Hurt);
	}
	g_BallBackReady = false;
}
public Action OnTouchBallBack(int iBallBack, int other)
{
	if(other && other <= MaxClients){
		FakeClientCommand(other, "explode");
	}
	
	return Plugin_Continue;
} 
public Action Timer_BallBackSpam(Handle timer)
{

	if (numPrintedBallFlipperBack >= 50) {		
		numPrintedBallFlipperBack = 0;
		return Plugin_Stop;
	}
	if(g_BallBackReady == true){
		char Skin[3];
		Format(Skin, 3, "%d", GetRandomInt(0,1));
		int iBallBack = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(iBallBack)){
			DispatchKeyValue(iBallBack, "model", "models/player/items/scout/soccer_ball.mdl");
			DispatchKeyValue(iBallBack, "disableshadows", "1");
			DispatchKeyValue(iBallBack, "physicsmode", "1");
			DispatchKeyValue(iBallBack,"skin", Skin);				
			DispatchKeyValue(iBallBack, "modelscale", "2.0");
			DispatchKeyValue(iBallBack, "spawnflags", "256");
			SDKHook(iBallBack, SDKHook_Touch, OnTouchBallBack);	
			DispatchSpawn(iBallBack);
			SetEntProp(iBallBack, Prop_Send, "m_usSolidFlags", 8);
			SetEntProp(iBallBack, Prop_Send, "m_CollisionGroup", 2);
			int target;
			target = GetRandomPlayer();
			float dynamicpos[3];
			if(IsValidClient(target)){
				GetClientAbsOrigin(target, dynamicpos);
			}
			dynamicpos[1] = -5124.7;
			dynamicpos[2] = 685.2;
			TeleportEntity(iBallBack, dynamicpos, NULL_VECTOR, NULL_VECTOR);
			g_iBallBacks[g_iCurrentBackBall++] = iBallBack;		
		}
		numPrintedBallFlipperBack++;
		return Plugin_Continue;
	}
	return Plugin_Continue;
}
public Action FlipperBack_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}