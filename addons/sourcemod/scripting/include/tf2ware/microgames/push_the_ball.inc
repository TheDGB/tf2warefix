bool g_bPushTheBallActive = false;
bool g_bBeamControlPushBall = false;

int RedPoints;
int BluPoints;
int numBallsSpawned = 0;
int numBallsSpawnedBlu = 0;

Handle hBalls = INVALID_HANDLE;

public int PushBall_Init()
{
	AddToForward(g_OnMapStart, INVALID_HANDLE, PushBall_OnMapStart);
	
	return 1;
}	

public int PushBall_OnMapStart()
{
    if (hBalls != INVALID_HANDLE)
    {
        CloseHandle(hBalls);
        hBalls = INVALID_HANDLE;
    }
    hBalls = CreateArray();
    PrecacheModel("materials/sprites/laser.vmt", true);
    return 1;
}
public int PushBall_OnMinigame()
{
    if (hBalls != INVALID_HANDLE)
    {
        CloseHandle(hBalls);
        hBalls = INVALID_HANDLE;
    }
    timeleft = 19;
    hBalls = CreateArray();

    AddToForward(g_justEntered, INVALID_HANDLE, PushBall_justEntered);
    AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, PushBall_OnGameFrame);
    AddToForward(g_OnTimerMinigame, INVALID_HANDLE, PushBall_TimerMinigame);
    AddToForward(g_OnEndMinigame, INVALID_HANDLE, PushBall_End);

    g_bPushTheBallActive = false;
    g_bBeamControlPushBall = true;

    if(g_bBeamControlPushBall)
    {
        CreateTimer(0.1, CreateBeamBalls);
        CreateTimer(4.0, RemoveOv);
        CreateTimer(0.1, SpawningBallsBlu, _, TIMER_REPEAT);
        CreateTimer(0.1, SpawningBallsRed, _, TIMER_REPEAT);
    }
    return 1;
}
public Action CreateBeamBalls(Handle timer, any data)
{
    float BeamLocation2[3] = {1402.148682, 0.0, -1060.968140};
    float BeamEndLocation2[3] = {3222.372803, 0.0, -1060.968628};
    StartLaser(BeamLocation2, BeamEndLocation2);
    g_bBeamControlPushBall = false;
    return Plugin_Handled;
}

 public Action SpawningBallsRed(Handle timer, any i)
 { 
	if (numBallsSpawned >= 10)
	{		
		numBallsSpawned = 0;
		return Plugin_Stop;
	}
	int iBall3 = CreateEntityByName("prop_physics_multiplayer");
	if(IsValidEntity(iBall3))
	{
		numBallsSpawned++;
		DispatchKeyValue(iBall3, "model", "models/player/items/scout/soccer_ball.mdl");
		DispatchKeyValue(iBall3,"skin", "0");	
		DispatchSpawn(iBall3);
		ActivateEntity(iBall3);
		//SetEntProp(iBall3, Prop_Data, "m_nSolidType", 6); 
		float dynamicposball2[3];
		float randomball;
		randomball = GetRandomFloat(1300.0,3100.0);
		dynamicposball2[0] = randomball;
		dynamicposball2[1] = 569.607971;
		dynamicposball2[2] = -1070.968750;
		TeleportEntity(iBall3, dynamicposball2, NULL_VECTOR, NULL_VECTOR);
		PushArrayCell(hBalls, iBall3);
	}
	return Plugin_Handled;
}
 public Action SpawningBallsBlu(Handle timer, any i)
 {
 
	if (numBallsSpawnedBlu >= 10)
	{		
		numBallsSpawnedBlu = 0;
		return Plugin_Stop;
	}
	int iBall3 = CreateEntityByName("prop_physics_multiplayer");
	if(IsValidEntity(iBall3))
	{
		numBallsSpawnedBlu++;
		DispatchKeyValue(iBall3, "model", "models/player/items/scout/soccer_ball.mdl");
		DispatchKeyValue(iBall3,"skin", "1");	
		DispatchSpawn(iBall3);
		ActivateEntity(iBall3);
		//SetEntProp(iBall3, Prop_Data, "m_nSolidType", 6); 
		float dynamicposball3[3];
		float randomball2;
		randomball2 = GetRandomFloat(1400.0,3000.0);
		dynamicposball3[0] = randomball2;
		dynamicposball3[1] = -569.607971;
		dynamicposball3[2] = -1070.968750;
		TeleportEntity(iBall3, dynamicposball3, NULL_VECTOR, NULL_VECTOR);
		PushArrayCell(hBalls, iBall3);
	}
	return Plugin_Handled;
}
public int PushBall_OnGameFrame()
{
    if(g_bPushTheBallActive)
    {
        for (int i = 1; i <= MaxClients; i++) 
        {
            if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) 
            {
                if (GetClientTeam(i) == 3)
                {
                    float posplayer[3];
                    GetClientAbsOrigin(i, posplayer);
                    if (posplayer[1] > 0.0)
                    {
                        float TeleplayerBackblu[3] = {2301.939697, -793.525208, -1047.804199};
                        TeleportEntity(i, TeleplayerBackblu, NULL_VECTOR, NULL_VECTOR);
                    }
                }
                if (GetClientTeam(i) == 2)
                {
                    float posplayer[3];
                    GetClientAbsOrigin(i, posplayer);
                    if (posplayer[1] < 0.0)
                    {
                        float TeleplayerBackred[3] = {2328.001953, 599.087891, -1036.577393};
                        TeleportEntity(i, TeleplayerBackred, NULL_VECTOR, NULL_VECTOR);
                    }
                }
            }
        }
    }
    return 1;
}
public Action RemOv(Handle timer)
{
    for(int i = 1; i <= MaxClients; i++) {
        if(IsValidClient(i)) {
            SetOverlay(i, "");
        }
    }
    return Plugin_Handled;
}

public int PushBall_TimerMinigame(int timer) 
{
    if (timer == 2)
    {
        BluPoints = 0;
        RedPoints = 0;
        for(int io = 0; io <= GetArraySize(hBalls)-1; io++)
        {
            int iBall = GetArrayCell(hBalls, io);
            if (IsValidEntity(iBall)) 
            {
                float positionball[3];
                GetEntityAbsOrigin(iBall, positionball);
                if (positionball[1] > 0.0)
                { 
                    RedPoints++;
                }
                if (positionball[1] < 0.0)
                { 
                    BluPoints++;
                }
                AcceptEntityInput(iBall, "Kill");
            }
        }
        for (int i = 1; i <= MaxClients; i++) 
        {
            if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) 
            {
                if (GetClientTeam(i) == 3)
                {
                    if(BluPoints < RedPoints)
                    {
                        SetStateClient(i, true, true);
                    }
                    if(BluPoints == 0)
                    {
                        SetStateClient(i, true, true);                
                    }
                }
                if (GetClientTeam(i) == 2)
                {
                    if(BluPoints > RedPoints)
                    {
                        SetStateClient(i, true, true);                
                    }
                    if(RedPoints == 0)
                    {
                        SetStateClient(i, true, true);                
                    }
                }
                if(BluPoints == RedPoints)
                {
                    SetStateClient(i, false, true);
                }
            }
        }
        PrintToChatAll("Blue Points \x04 %d", BluPoints);
        PrintToChatAll("Red Points \x04 %d", RedPoints);
    }
    return 1;
}

public void PushBall_justEntered(int client)
{
	if (IsValidClient(client))
	{
		RemoveClientWeapons(client);
		SetClientClass(client, "scout");
		giveitem(client, 1123);
		if (GetClientTeam(client) == 2)
		{
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
			
			int columnas = client;
			int rowas = 0;
			
			float pos[3];
			pos[0] = 1429.885132 + float(columnas*63);
			pos[1] = 729.170898 - float(rowas*63);
			pos[2] = -1070.968628;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = -90.0;
			ang[2] = 0.0;
			TeleportEntity(client, pos, ang, vel);
		}
		if (GetClientTeam(client) == 3)
		{
			float vel3[3];
			vel3[0] = 0.0;
			vel3[1] = 0.0;
			vel3[2] = 0.0;
			
			int columnas3 = client;
			int rowas3 = 0;
			
			float pos3[3];
			pos3[0] = 1424.334351 + float(columnas3*63);
			pos3[1] = -726.581055 - float(rowas3*63);
			pos3[2] = -1070.968750;

			float ang3[3];
			ang3[0] = 0.0;
			ang3[1] = 90.0;
			ang3[2] = 0.0;
			TeleportEntity(client, pos3, ang3, vel3);
		}
		SDKHook(client, SDKHook_OnTakeDamage, OnDamageBallsGame);
		g_attack = true;
		g_bPushTheBallActive = true;
	}
}
public Action OnDamageBallsGame(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3]){
	if(IsValidClient(victim)){
		TF2_AddCondition(victim, view_as<TFCond>(14), 0.001);
	}
	return Plugin_Continue;
} 
public void PushBall_End()
{
	for(int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i)) {
			SDKUnhook(i, SDKHook_OnTakeDamage, OnDamageBallsGame);
		}
	}
	char strClassname[50];	
	for(int z=0; z<GetArraySize(hBalls); z++)
	{
		int iBallr = GetArrayCell(hBalls, z);
		if(IsValidEntity(iBallr))
		{
			GetEdictClassname(iBallr, strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_multiplayer") != -1)
			{
				AcceptEntityInput(iBallr, "Kill");				   
			}
		}
	}
	g_bPushTheBallActive = false;
	ClearArray(hBalls);
	BluPoints = 0;
	RedPoints = 0;
}