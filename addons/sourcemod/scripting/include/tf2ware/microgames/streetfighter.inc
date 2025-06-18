
public int StreetFighter_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, StreetFighter_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, StreetFighter_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, StreetFighter_End);
	//SetStateAll(true);
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
	NoCollision(true);
	
	return 0;
}
public void StreetFighter_justEntered(int client) {

	if (IsValidClient(client)) {
		RemoveClientWeapons(client);
		SetClientClass(client, "pyro");
		RemoveClientWeapons(client);
		giveitem(client, 12);
		SetClientSlot(client, 1);	
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
		g_attack = false;
		NoCollision(true);
		SDKHook(client, SDKHook_OnTakeDamage, StreetFighter_OnTakeDamageClient);
	}
}
public void StreetFighter_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, StreetFighter_OnTakeDamageClient);
	}
}
public int StreetFighter_OnGameFrame()
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
	
	return 0;
}
public Action AddCond(Handle timer, any client){
	int indextaunt=GetEntProp(client, Prop_Send, "m_iTauntItemDefIndex");
	switch(indextaunt)
	{
		case 167, 438, 463, 1106, 1107, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121:
		{
			return Plugin_Handled;			
		}
	}
	TF2_AddCondition(client, TFCond_Kritzkrieged, 1.3);
	TF2_AddCondition(client, TFCond_Buffed, 1.3);
	
	return Plugin_Handled;
}
public Action FireBall(Handle timer, any client){
	if (!IsValidClient(client)) return Plugin_Handled;
	if (!IsPlayerAlive(client)) return Plugin_Handled;
	if (TF2_GetPlayerClass(client) != TFClass_Pyro) return Plugin_Handled;
	if (!TF2_IsPlayerInCondition(client, TFCond_Taunting)) return Plugin_Handled;
	float vPosition[3];
	float vAngles[3];
	vAngles[2] += 25.0;
	int iTeam = GetClientTeam(client);
	GetClientEyePosition(client, vPosition);
	GetClientEyeAngles(client, vAngles);
	int weapon=GetEntPropEnt(client, Prop_Send, "m_hActiveWeapon");
	int index=GetEntProp(weapon, Prop_Send, "m_iItemDefinitionIndex");
	if(weapon && IsValidEdict(weapon))
	{
		switch(index)
		{
			case 12, 1141, 199, 39, 351, 415, 595, 1081, 1153:
			{
				int indextaunt=GetEntProp(client, Prop_Send, "m_iTauntItemDefIndex");
				switch(indextaunt)
				{
					case 167, 438, 463, 1106, 1107, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121:
					{
						return Plugin_Handled;			
					}
				}
				CreateFireBall(client, vPosition, vAngles, 650.0, 800.0, iTeam, true);
				TF2_AddCondition(client, TFCond_Ubercharged, 0.1);
				EmitSoundToAll("gamesound/tf2ware/kenryu.mp3", client);
				return Plugin_Changed;
			}
		}
	
	}		
	return Plugin_Handled;
}
int CreateFireBall(int client, float vPosition[3], float vAngles[3], float flSpeed = 650.0, float flDamage = 800.0, int iTeam, bool bCritical = false){

	char strClassname[32] = "CTFProjectile_Rocket";
	//int iRocket = CreateEntityByName("tf_projectile_energy_ball");
	int iRocket = CreateEntityByName("tf_projectile_spellfireball");
	if(!IsValidEntity(iRocket))
		return -0;
		
	float vVelocity[3];
	float vBuffer[3];
	
	GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
	
	vVelocity[0] = vBuffer[0]*flSpeed;
	vVelocity[1] = vBuffer[1]*flSpeed;
	vVelocity[2] = vBuffer[2]*flSpeed;
	
	TeleportEntity(iRocket, vPosition, vAngles, vVelocity);
	
	SetEntData(iRocket, FindSendPropInfo("CTFProjectile_Rocket", "m_iTeamNum"), GetClientTeam(client), true);
	SetEntData(iRocket, FindSendPropInfo(strClassname, "m_bCritical"), bCritical, true);
	SetEntPropEnt(iRocket, Prop_Send, "m_hOwnerEntity", client);
	SetEntDataFloat(iRocket, FindSendPropInfo(strClassname, "m_iDeflected") + 4, flDamage, true);
	
	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "TeamNum", -1, -1, 0);

	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "SetTeam", -1, -1, 0); 
	DispatchKeyValue(iRocket, "modelscale", "3.0");
	DispatchSpawn(iRocket);
	return iRocket;
}
public Action StreetFighter_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		if(attacker == victim) return Plugin_Handled;
		if(GetClientTeam(attacker) == GetClientTeam(victim))return Plugin_Handled;
		SetStateClient(attacker, true, true);
		damage = 800.0;
	}
	return Plugin_Changed;
}