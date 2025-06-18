
Handle hPetBreak = INVALID_HANDLE;
float currentscale = 1.0;

bool g_Canhit[MAXPLAYERS+1];
public void BreakPassTime_Init()
{
    AddToForward(g_OnMapStart, INVALID_HANDLE, BreakPassTime_OnMapStart);
}    	

public void BreakPassTime_OnMapStart()
{
    if (hPetBreak != INVALID_HANDLE)
    {
        CloseHandle(hPetBreak);
        hPetBreak = INVALID_HANDLE;
    }
}
public void BreakPassTime_OnMinigame()
{
    hPetBreak = CreateArray();
    
    AddToForward(g_justEntered, INVALID_HANDLE, BreakPassTime_justEntered);
    AddToForward(g_OnEndMinigame, INVALID_HANDLE, BreakPassTime_End);
    for (int i = 1; i <= MaxClients; i++) 
    {
        if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) 
        {
            if (GetClientTeam(i) == 2)
            {
                CreateTimer(0.3, PballRed, i);
            }
            if (GetClientTeam(i) == 3)
            {
                CreateTimer(0.3, PballBlue, i);
            }
        }
    }
}
public void BreakPassTime_justEntered(int client)
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
			NoCollision(true);
			g_Canhit[client] = true;
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
			NoCollision(true);
			g_Canhit[client] = true;
		}
		g_attack = true;
	}
}
public Action PballRed(Handle timer, any client){
	if (IsValidClient(client))
	{
		float npos2[3];
		float vAngles[3];
		float bpos2[3];
		GetClientAbsOrigin(client, npos2);
		GetClientEyeAngles(client, vAngles);
		MoveForward(npos2, vAngles, bpos2, 350.0);
		int i_PT = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(i_PT))
		{
			DispatchKeyValue(i_PT, "model", "models/passtime/ball/passtime_ball.mdl");
			DispatchKeyValue(i_PT,"skin", "0");	
			DispatchSpawn(i_PT);
			ActivateEntity(i_PT);
			//SetEntProp(i_PT, Prop_Data, "m_nSolidType", 6);
			SetEntPropFloat(i_PT, Prop_Send, "m_flModelScale", 1.0);
			currentscale = GetEntPropFloat(i_PT, Prop_Send, "m_flModelScale");
			bpos2[2] = -1070.968750;
/* 			float dynamicpospet2[3];
			dynamicpospet2[0] = bpos;
			dynamicpospet2[1] = 377.607971;
			dynamicpospet2[2] = -1070.968750; */
			TeleportEntity(i_PT, bpos2, NULL_VECTOR, NULL_VECTOR);
			SetEntityMoveType(i_PT, MOVETYPE_NONE);
			int Rent = EntIndexToEntRef(i_PT);
			SDKHook(Rent, SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
			PushArrayCell(hPetBreak, i_PT);
		}
	}
	
	return Plugin_Stop;
}
public Action PballBlue(Handle timer, any client){
	if (IsValidClient(client))
	{
		float npos2[3];
		float vAngles[3];
		float bpos2[3];
		GetClientAbsOrigin(client, npos2);
		GetClientEyeAngles(client, vAngles);
		MoveForward(npos2, vAngles, bpos2, 350.0);
		int i_PT = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(i_PT))
		{
			DispatchKeyValue(i_PT, "model", "models/passtime/ball/passtime_ball.mdl");
			DispatchKeyValue(i_PT,"skin", "1");	
			DispatchSpawn(i_PT);
			ActivateEntity(i_PT);
			//SetEntProp(i_PT, Prop_Data, "m_nSolidType", 6); 
			SetEntPropFloat(i_PT, Prop_Send, "m_flModelScale", 1.0);
			currentscale = GetEntPropFloat(i_PT, Prop_Send, "m_flModelScale");
			bpos2[2] = -1070.968750;
/* 			float dynamicpospet3[3];
			float randomPet2;
			randomPet2 = bpos2;//GetRandomFloat(1400.0,3000.0);
			dynamicpospet3[0] =  randomPet2;
			dynamicpospet3[1] = -377.607971;
			dynamicpospet3[2] = -1070.968750; */
			TeleportEntity(i_PT, bpos2, NULL_VECTOR, NULL_VECTOR);
			SetEntityMoveType(i_PT, MOVETYPE_NONE);
			int Rent = EntIndexToEntRef(i_PT);
			SDKHook(Rent , SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
			PushArrayCell(hPetBreak, i_PT);
		}
	}
	
	return Plugin_Stop;
}
/* public Action PballRed(Handle timer, any client){
	if (IsValidClient(client))
	{
		float npos[3];
		GetClientAbsOrigin(client, npos);
		float bpos = npos[0];
		int i_PT = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(i_PT))
		{
			DispatchKeyValue(i_PT, "model", "models/passtime/ball/passtime_ball.mdl");
			DispatchKeyValue(i_PT,"skin", "0");	
			DispatchSpawn(i_PT);
			ActivateEntity(i_PT);
			//SetEntProp(i_PT, Prop_Data, "m_nSolidType", 6);
			SetEntPropFloat(i_PT, Prop_Send, "m_flModelScale", 1.0);
			currentscale = GetEntPropFloat(i_PT, Prop_Send, "m_flModelScale");
			float dynamicpospet2[3];
			dynamicpospet2[0] = bpos;
			dynamicpospet2[1] = 377.607971;
			dynamicpospet2[2] = -1070.968750;
			TeleportEntity(i_PT, dynamicpospet2, NULL_VECTOR, NULL_VECTOR);
			SetEntityMoveType(i_PT, MOVETYPE_NONE);
			int Rent = EntIndexToEntRef(i_PT);
			SDKHook(Rent, SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
			PushArrayCell(hPetBreak, i_PT);
		}
	}
}
public Action PballBlue(Handle timer, any client){
	if (IsValidClient(client))
	{
		float npos2[3];
		GetClientAbsOrigin(client, npos2);
		float bpos2 = npos2[0];
		int i_PT = CreateEntityByName("prop_physics_multiplayer");
		if(IsValidEntity(i_PT))
		{
			DispatchKeyValue(i_PT, "model", "models/passtime/ball/passtime_ball.mdl");
			DispatchKeyValue(i_PT,"skin", "1");	
			DispatchSpawn(i_PT);
			ActivateEntity(i_PT);
			//SetEntProp(i_PT, Prop_Data, "m_nSolidType", 6); 
			SetEntPropFloat(i_PT, Prop_Send, "m_flModelScale", 1.0);
			currentscale = GetEntPropFloat(i_PT, Prop_Send, "m_flModelScale");
			float dynamicpospet3[3];
			float randomPet2;
			randomPet2 = bpos2;//GetRandomFloat(1400.0,3000.0);
			dynamicpospet3[0] =  randomPet2;
			dynamicpospet3[1] = -377.607971;
			dynamicpospet3[2] = -1070.968750;
			TeleportEntity(i_PT, dynamicpospet3, NULL_VECTOR, NULL_VECTOR);
			SetEntityMoveType(i_PT, MOVETYPE_NONE);
			int Rent = EntIndexToEntRef(i_PT);
			SDKHook(Rent , SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
			PushArrayCell(hPetBreak, i_PT);
		}
	}
} */
public void BreakPassTime_End()
{
	for (int i = 1; i <= MaxClients; i++) 
	{
		if (IsValidClient(i)) 
		{
			g_Canhit[i] = false;
		}
	}
	for(int x = 0;x <= GetArraySize(hPetBreak)-1; x++)
	{
		int i_PT = GetArrayCell(hPetBreak, x);
		if (IsValidEntity(i_PT)) 
		{	
			AcceptEntityInput(i_PT, "Kill");
			SDKHook(i_PT, SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
		}
	}
	CloseHandle(hPetBreak);
}
public Action OnDamageBreakPassTimesGame(int entity, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3]){
	if(IsValidEntity(entity) && IsValidClient(attacker)){
		if (g_Canhit[attacker]){
			currentscale = GetEntPropFloat(entity, Prop_Send, "m_flModelScale");
			float newscale = currentscale +0.3;
			SetEntPropFloat(entity, Prop_Send, "m_flModelScale", newscale) ;
			if(currentscale > 2.5){
				SDKUnhook(entity, SDKHook_OnTakeDamage, OnDamageBreakPassTimesGame);
				AcceptEntityInput(entity, "Kill");
				SetStateClient(attacker, true, true);
				g_Canhit[attacker] = false;
			}
		}
	}
	return Plugin_Continue;
}