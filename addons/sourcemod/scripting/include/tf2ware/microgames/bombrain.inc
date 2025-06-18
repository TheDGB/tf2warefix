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

#define SND_SHOOT			"weapons/grenade_launcher_shoot.mp3"
#define SND_WARN			"misc/doomsday_lift_warning.mp3"
#define MDL_BOMBLET			"models/weapons/w_models/w_stickybomb.mdl"

Handle g_bombspam;
float g_flCvarSpawnDelay = 0.5;
float g_flCvarBombDamage = 50.0;

public void BombRain_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BombRain_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BombRain_End);	
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, BombRain_CheckDeadPlayers);
		
	SetStateAll(true);
	g_attack = true;
	ServerCommand("sm_cvar tf_flamethrower_burstammo 0");
	g_BombRainEnabled = true;
	if(g_BombRainEnabled == true){
	CreateTimer(1.2, iPipeFired);
	}	
}
public void BombRain_OnMapStart() {    
	PrecacheSound(SND_SHOOT, true);
	PrecacheSound(SND_WARN, true);
	PrecacheModel(MDL_BOMBLET, true);	
}

public Action iPipeFired(Handle hndl, any data) {
	float pos[3]; float ang[3];
	ang[0] = 35.0;
	float rand;
	for (int i=-2; i<=2; i++) {
		for (int j=-2; j<=2; j++) {
			rand = GetRandomFloat(-500.0, 400.0);
			pos[2] = rand;
			pos[0] = i*244.0;
			pos[1] = j*244.0;
			PipeFired(pos, ang, 60.0);
		}
	} 

    return Plugin_Continue;	
}

int PipeFired(float vPosition[3], float vAngles[3] = NULL_VECTOR, float flSpeed = 600.0){
	
	int iPipe = CreateEntityByName("tf_projectile_pipe");
	if(!IsValidEntity(iPipe))
		return -0;
	
	float vVelocity[3];
	float vBuffer[3];
		
	GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
	
	vVelocity[0] = vBuffer[0]*flSpeed;
	vVelocity[1] = vBuffer[1]*flSpeed;
	vVelocity[2] = vBuffer[2]*flSpeed;
	
	TeleportEntity(iPipe, vPosition, vAngles, vVelocity);
	//SetEntPropEnt(iPipe, Prop_Data, "m_hThrower",client);
	SetEntPropFloat(iPipe, Prop_Send, "m_flDamage", 150.0); 
	SetEntProp(iPipe, Prop_Send, "m_iTeamNum", GetRandomInt(1,2));
	SetEntPropFloat(iPipe, Prop_Send, "m_flModelScale", 1.5); 
		
	DispatchSpawn(iPipe);
	g_bombspam = CreateTimer(0.2, Timer_SetProps, EntIndexToEntRef(iPipe), TIMER_REPEAT);
	return iPipe;
}

public void BombRain_justEntered(int client) {
	if (IsValidClient(client)) {
		SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);
		SetClientClass(client, "pyro");
		TpEnabled(client);
		RemoveClientWeapons(client);
		giveitem(client, 21);
		SetClientSlot(client, 0);		
		SetEntityHealth(client, 250);		
	}
	g_attack = true;
}

public void BombRain_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void BombRain_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		if (IsValidClient(i)){
			TpDisabled(i);	
			g_attack = false;
		}
	}
	ServerCommand("sm_cvar tf_flamethrower_burstammo 25");
	ClearTimer2(g_bombspam);
	g_BombRainEnabled = false;	
}
stock void ClearTimer2(Handle &timer)
{
	if(timer != INVALID_HANDLE)
	{
		KillTimer(timer);
		timer = INVALID_HANDLE;
	}
}

public Action Timer_SetProps(Handle timer, any entity)
{
	int ent = EntRefToEntIndex(entity);
	char sClass[32];
	
	if (ent > 0 && ent > MaxClients && IsValidEntity(ent) && GetEntityClassname(ent, sClass, sizeof(sClass)))
	{
		if(StrEqual(sClass, "tf_projectile_pipe") && GetEntProp(ent, Prop_Send, "m_bTouched") && GetEntProp(ent, Prop_Send, "m_iType") != 2)
		{
			int client = GetEntPropEnt(ent, Prop_Send, "m_hThrower");
			int clientteam = Team_Spec;			//Throwers team

			if(!CheckCommandAccess(client, "sm_minibombs_access", 0))
				return Plugin_Handled;
			
			float pos[3];
			GetEntPropVector(ent, Prop_Send, "m_vecOrigin", pos);
				
			float g_angles[3]; float g_angles2[3];		//Rotate sticky a bit when it shoots a bomblet
			GetEntPropVector(ent, Prop_Send, "m_angRotation", g_angles);

			g_angles2[0] = (g_angles[0] += GetRandomFloat(5.0,45.0));
			g_angles2[1] = (g_angles[1] += GetRandomFloat(5.0,45.0));
			g_angles2[2] = (g_angles[2] += GetRandomFloat(5.0,45.0));
				
			float ang[3];
			ang[0] = GetRandomFloat(-90.0, 90.0); 			//Left, Right
			ang[1] = GetRandomFloat(-90.0, 90.0); 			//Forward, Back
			ang[2] = GetRandomFloat(240.0, 340.0); 			//Up, Down
				
			int pitch = 150;
			EmitAmbientSound(SND_SHOOT, pos, ent, _, _, _, pitch);
				
			int ent2 = CreateEntityByName("tf_projectile_pipe");
				
			if(ent2 != -1)
			{					
				SetEntPropEnt(ent2, Prop_Data, "m_hThrower", client);
				SetEntProp(ent2, Prop_Send, "m_iTeamNum", clientteam);
				SetEntProp(ent2, Prop_Send, "m_bCritical", true);
				SetEntPropFloat(ent2, Prop_Send, "m_flModelScale", 0.9);
				SetEntPropFloat(ent2, Prop_Send, "m_flDamage", g_flCvarBombDamage);
					
				DispatchSpawn(ent2);
					
				SetEntityModel(ent2, MDL_BOMBLET);
					
				TeleportEntity(ent2, pos, NULL_VECTOR, ang);							//Teleport bomblet to momma stickybomb
				TeleportEntity(ent, NULL_VECTOR, g_angles2, NULL_VECTOR);				//Rotate	
					
				CreateTimer(g_flCvarSpawnDelay, Timer_SetProps, EntRefToEntIndex(ent));		//The cycle continues..
			}
		}
	}
	return Plugin_Handled;
}
