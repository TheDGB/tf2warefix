//Dodgeball Rocket
int ntarget = -1;
int iDb_Ball = -1;
int TeamSelection;
int OldTeamTarget;
int roundstart;
int iHudSpeed;
float InitialSpeed = 600.0;
float BallSpeed;
float RocketSpawnpos_Blue[3];
float RocketSpawnpos_Red[3];
bool rocketon = false;
static const float g_fSpin[] = {0.0, 0.0, 40.0};
int RedTrail[] = {255, 0, 0, 255};
int BluTrail[] = {0, 255, 255, 255};
int Trail;
bool Dodgestart = false;
public void Dodgeball_OnMinigame() {
    AddToForward(g_justEntered, INVALID_HANDLE, Dodgeball_justEntered);
    AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Dodgeball_OnGameFrame);
    AddToForward(g_OnEndMinigame, INVALID_HANDLE, Dodgeball_End);    
    AddToForward(g_PlayerDeath, INVALID_HANDLE, Dodgeball_PlayerDeath);
    SetStateAll(true);
    g_attack = true;
    ServerCommand("sm_cvar tf_flamethrower_burstammo 0");
    roundstart = true;
    Dodgestart = true;
    PrepareEnt();
}
void PrepareEnt() {
	int index = -1;
	while ((index = FindEntityByClassname(index, "info_target")) != -1)
	{
		if(IsValidEntity(index)){
			char entName[50];
			GetEntPropString(index, Prop_Data, "m_iName", entName, sizeof(entName));
			if(strcmp(entName, "rocket_spawn_blue") == 0)
			{
				GetEntPropVector(index, Prop_Data, "m_vecAbsOrigin", RocketSpawnpos_Blue);
				//PrintToChatAll("Blu");
				//CreateParticle(index, "env_fire_small_smoke");
			}
			if(strcmp(entName, "rocket_spawn_red") == 0)
			{
				GetEntPropVector(index, Prop_Data, "m_vecAbsOrigin", RocketSpawnpos_Red);
				//PrintToChatAll("Red");
				//CreateParticle(index, "env_fire_small_smoke");
			}
		}
	}
	CreateTimer(5.0, iBallFiredTeamSelection);
}
public void Dodgeball_OnGameFrame() {
	if (!IsValidEntity(iDb_Ball) && rocketon) {
		rocketon = false;
		CreateTimer(1.0, iBallFiredTeamSelection);
		return;
	}
	if (IsValidEntity(iDb_Ball)) {
		OnCheckPos(iDb_Ball);
	}
	for (int i = 1; i <= MaxClients; i++) 
	{
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) 
		{
			if (GetClientTeam(i) == 3)
			{
				float posplayer[3];
				GetClientAbsOrigin(i, posplayer);
				if (posplayer[0] < 2450.0)
				{
					float TeleplayerBackblu[3] = {3145.098145, -16.586035, -1047.804199};
					TeleportEntity(i, TeleplayerBackblu, NULL_VECTOR, NULL_VECTOR);
				}
			}
			if (GetClientTeam(i) == 2)
			{
				float posplayer[3];
				GetClientAbsOrigin(i, posplayer);
				if (posplayer[0] > 2450.0)
				{
					float TeleplayerBackred[3] = {1519.578491, -19.790518, -1036.577393};
					TeleportEntity(i, TeleplayerBackred, NULL_VECTOR, NULL_VECTOR);
				}
			}
		}
	}
}
public void Dodgeball_justEntered(int client) {
	if (IsValidClient(client))
	{
		if (GetClientTeam(client) == 3)
		{
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
			
			int columnas = client;
			int rowas = 0;
			
			float pos[3];
			pos[0] = 1589.0 + float(columnas*63);
			pos[1] = 755.0 - float(rowas*63);
			pos[2] = -1067.968628;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = -180.0;
			ang[2] = 0.0;
			TeleportEntity(client, pos, ang, vel);
		}
		if (GetClientTeam(client) == 2)
		{
			float vel3[3];
			vel3[0] = 0.0;
			vel3[1] = -180.0;
			vel3[2] = 0.0;
			
			int columnas3 = client;
			int rowas3 = 0;
			
			float pos3[3];
			pos3[0] = 3159.0 + float(columnas3*63);
			pos3[1] = -702.0 - float(rowas3*63);
			pos3[2] = -1067.968628;

			float ang3[3];
			ang3[0] = 0.0;
			ang3[1] = 0.0;
			ang3[2] = 0.0;
			TeleportEntity(client, pos3, ang3, vel3);
		}
		SetClientClass(client, "pyro");
		TF2_RemoveWeaponSlot(client, 1);
		TF2_RemoveWeaponSlot(client, 2);
		ForceFlamePyro(client);
		SetClientSlot(client, 0);
		SDKHook(client, SDKHook_OnTakeDamage, HurtClient);
		SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);
		//TpEnabled(client);
		g_attack = true;
		
	}
}
public Action iBallFiredTeamSelection(Handle hndl) {
    if(TeamSelection <= 1) {
        TeamSelection = GetRandomInt(2,3);
        OldTeamTarget = TeamSelection;
    }
    iBallFired();
    return Plugin_Handled;
}
public Action iBallFired() {
	if(roundstart){
		float Pos[3];
		
		float ang[3];
		ang[0] = -90.0;
		if (IsValidEntity(iDb_Ball))
		{
			SDKUnhook(iDb_Ball, SDKHook_Touch, OnTouchDodge);
			SDKUnhook(iDb_Ball, SDKHook_OnTakeDamage, Dodgeball_Hurt);
			AcceptEntityInput(iDb_Ball, "Kill");
		}
		RocketsFiredBall(Pos, ang, 600.0, 300.0, Team_Spec, true);
		rocketon = true;
	}
    return Plugin_Handled;
}
int RocketsFiredBall(float vPosition[3], float vAngles[3] = NULL_VECTOR, float flSpeed = 600.0, float flDamage = 450.0, int iTeam = Team_Spec, bool bCritical = false) {
    float Pos[3];
    float ang[3];
    ang[0] = -90.0;
    
    // Remove unused strClassname declaration
    char strEntname[32] = "tf_projectile_rocket";
    
    iDb_Ball = CreateEntityByName(strEntname);
    if(!IsValidEntity(iDb_Ball))
        return -0;
	
	iDb_Ball = CreateEntityByName(strEntname);
	if(!IsValidEntity(iDb_Ball))
		return -0;
	
	float vVelocity[3];
	float vBuffer[3];
	
	GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
	
	vVelocity[0] = vBuffer[0]*flSpeed;
	vVelocity[1] = vBuffer[1]*flSpeed;
	vVelocity[2] = vBuffer[2]*flSpeed;
	
	TeleportEntity(iDb_Ball, vPosition, vAngles, vVelocity);
	
    int skinOffset = FindSendPropInfo("CTFProjectile_Rocket", "m_nSkin");
    int critOffset = FindSendPropInfo("CTFProjectile_Rocket", "m_bCritical");
    int deflectedOffset = FindSendPropInfo("CTFProjectile_Rocket", "m_iDeflected");
    
    SetEntData(iDb_Ball, skinOffset, (iTeam-2), 1, true);
    SetEntData(iDb_Ball, critOffset, bCritical, true);
    SetEntDataFloat(iDb_Ball, deflectedOffset + 4, flDamage, true);
	
	SetVariantInt(iTeam);
	AcceptEntityInput(iDb_Ball, "TeamNum", -1, -1, 0);

	SetVariantInt(iTeam);
	AcceptEntityInput(iDb_Ball, "SetTeam", -1, -1, 0); 
	
	DispatchSpawn(iDb_Ball);
	SetEntPropEnt(iDb_Ball, Prop_Send, "m_hOwnerEntity", 0);
	if(OldTeamTarget == 2 || TeamSelection == 2){
		Pos = RocketSpawnpos_Red;
		vVelocity[0] = -InitialSpeed;
	}
	if(OldTeamTarget == 3 || TeamSelection == 3){
		Pos = RocketSpawnpos_Blue;
		vVelocity[0] = InitialSpeed;
	}
	BallSpeed = InitialSpeed;
	TeleportEntity(iDb_Ball, Pos, ang, vVelocity);
	SetEntityMoveType(iDb_Ball, MOVETYPE_FLY);
	DispatchKeyValue(iDb_Ball, "solid", "6");
	SetEntPropVector(iDb_Ball, Prop_Data, "m_vecAngVelocity", g_fSpin);
	SDKHook(iDb_Ball, SDKHook_Touch, OnTouchDodge);
	SDKHook(iDb_Ball, SDKHook_TraceAttack, Dodgeball_Hurt);
	if(OldTeamTarget == 2){
		TeamSelection = 3;
	}
	else
	if(OldTeamTarget == 3){
		TeamSelection = 2;
	}
	if(TeamSelection == 2){			
		//SetEntityRenderColor(iDb_Ball, 0, 255, 255, 255);
		TE_SetupBeamFollow(iDb_Ball, Trail, 0, 2.0, 1.0, 1.0, 1, BluTrail);
		TE_SendToAll();
	}
	if(TeamSelection == 3){
		//SetEntityRenderColor(iDb_Ball, 255, 0, 0, 255);
		TE_SetupBeamFollow(iDb_Ball, Trail, 0, 2.0, 1.0, 1.0, 1, RedTrail);
		TE_SendToAll();
	}
	if(IsValidClient(ntarget) && IsValidEntity(iDb_Ball)){
		ntarget = GetRandomPlayerTeam(TeamSelection);
	}
	OldTeamTarget = TeamSelection;
	return iDb_Ball;
}
public void OnCheckPos(int entity) {
	float ProjLocation[3];
	float ProjVector[3];
	float ProjSpeed;
	float ProjAngle[3];
	float TargetLocation[3];
	float AimVector[3];
	if(GetPlayerCountTeam(2) <= 0 || GetPlayerCountTeam(3) <= 0){
		CreateTimer(2.0, ForceEnd);
		return;
	}
	if((IsValidClient(ntarget) && GetClientTeam(ntarget)<= 1) || !IsValidClient(ntarget)){
		ntarget =  GetRandomPlayerTeamDB(GetRandomInt(2,3));
	}
	if(IsValidClient(ntarget) && !IsPlayerAlive(ntarget) && GetClientTeam(ntarget)>= 2){
		ntarget =  GetRandomPlayerTeamDB(GetClientTeam(ntarget));
	}
	if(IsValidClient(ntarget))
	{
		GetEntPropVector(entity, Prop_Data, "m_vecAbsOrigin", ProjLocation);
		GetClientAbsOrigin(ntarget, TargetLocation);
		TargetLocation[2] += 40.0;
		MakeVectorFromPoints(ProjLocation, TargetLocation , AimVector);
		GetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", ProjVector);					
		ProjSpeed = GetVectorLength(ProjVector);					
		AddVectors(ProjVector, AimVector, ProjVector);	
		NormalizeVector(ProjVector, ProjVector);
		GetEntPropVector(entity, Prop_Data, "m_angRotation", ProjAngle);
		GetVectorAngles(ProjVector, ProjAngle);
		SetEntPropVector(entity, Prop_Data, "m_angRotation", ProjAngle);					
		ScaleVector(ProjVector, BallSpeed);
			
		SetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", ProjVector);
		TeleportEntity(entity, NULL_VECTOR, ProjAngle, ProjVector);
		
		float Distance2 = GetVectorDistance(ProjLocation, TargetLocation);
		
		float HitVec[3];
		if (Distance2 > 100.0)
		{		
			iHudSpeed = RoundToZero(BallSpeed);
		}
		if(iHudSpeed > 1500){
			if (Distance2 > 200.0 && Distance2 < 1500.0){
				AddVectors(AimVector, ProjVector, HitVec);
				HitVec = AimVector;
				NormalizeVector(HitVec, HitVec);
				ScaleVector(HitVec, ProjSpeed);
				float FinalAng[3];
				GetVectorAngles(HitVec, FinalAng);
				TeleportEntity(entity, NULL_VECTOR, FinalAng, HitVec);
			}
		}
		if (Distance2 < 501.0){
			AddVectors(AimVector, ProjVector, HitVec);
			HitVec = AimVector;
			NormalizeVector(HitVec, HitVec);
			ScaleVector(HitVec, ProjSpeed);
			float FinalAng[3];
			GetVectorAngles(HitVec, FinalAng);
			TeleportEntity(entity, NULL_VECTOR, FinalAng, HitVec);
		}
		if (Distance2 < 10.0)
		{
			if (IsValidClient(ntarget) && IsValidEntity(entity))
			{
				GetClientAbsOrigin(ntarget, TargetLocation);
				int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
				float Damage = 500.0;
				if(owner <= 0){
					SDKHooks_TakeDamage(ntarget, 0, 0, Damage);
				}
				if(IsValidClient(owner)){
					SDKHooks_TakeDamage(ntarget, owner, owner, Damage);
				}
				ClientExplode(ntarget, TargetLocation);
				RequestFrame(Remove_Ragdoll, ntarget);
				if (IsValidEntity(entity))
				{
					SDKUnhook(entity, SDKHook_Touch, OnTouchDodge);
					SDKUnhook(entity, SDKHook_OnTakeDamage, Dodgeball_Hurt);
					AcceptEntityInput(entity, "Kill");
					iDb_Ball = -1;
					//CreateTimer(0.4, iBallFiredTeamSelection);
				}
			}
		}
	}
}
public Action SoundHookBlastDB(int clients[64], int &numClients, char sound[PLATFORM_MAX_PATH], int &client, int &channel, float &volume, int &level, int &pitch, int &flags)
{
	if(Dodgestart){
		if (StrContains(sound, "flame_thrower_airblast_rocket_redirect", false) != -1)
		{
			int entity = iDb_Ball;
			int clientOw = GetEntPropEnt(client, Prop_Data, "m_hOwnerEntity");
			if(clientOw <0 || clientOw > MAXPLAYERS)return Plugin_Changed;
			ntarget = clientOw;
			float vel[3];
			if(BallSpeed <= 3500){
				BallSpeed += 100.0;
			}
			if (IsValidEntity(entity))
			{
				GetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", vel);
				float currentspeed = SquareRoot(Pow(vel[0],2.0)+Pow(vel[1],2.0));
				float x = currentspeed / (BallSpeed*2.0);
				vel[0] /= -x;
				vel[1] /= -x;
				vel[2] = BallSpeed*2.5;

				TeleportEntity(entity, NULL_VECTOR, NULL_VECTOR, vel);
			}
			if(IsValidClient(ntarget)){
				//SetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", vel);
				SetEntProp(entity, Prop_Data, "m_iTeamNum", GetClientTeam(ntarget));
				//SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", ntarget);
				if(GetClientTeam(ntarget) == 2){
					ntarget =  GetRandomPlayerTeam(3);
					OldTeamTarget = 3;
				}
				else
				if(GetClientTeam(ntarget) == 3){
					ntarget =  GetRandomPlayerTeam(2);
					OldTeamTarget = 2;
				}
				if(OldTeamTarget == 2){
					TE_SetupBeamFollow(entity, Trail, 0, 2.0, 1.0, 1.0, 1, BluTrail);
					TE_SendToAll();
				}
				if(OldTeamTarget == 3){
					TE_SetupBeamFollow(entity, Trail, 0, 2.0, 1.0, 1.0, 1, RedTrail);
					TE_SendToAll();
				}
				//PrintToChatAll("pippo");
			}
		}
	}
	return Plugin_Changed;
} 
public Action Dodgeball_Hurt(int entity, int &attacker, int &inflictor, float &damage, int &damagetype) {
	entity = iDb_Ball;
	if (IsValidClient(attacker) && IsValidEntity(entity) && attacker == ntarget) {
		if(((GetEntityFlags(entity) & FL_ONGROUND)))return Plugin_Continue;
		
		float vel[3];
		damage = 100.0;
		if(BallSpeed <= 3500){
			BallSpeed += 100.0;
		}
		GetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", vel);
		float currentspeed = SquareRoot(Pow(vel[0],2.0)+Pow(vel[1],2.0));
		float x = currentspeed / (BallSpeed*2.0);
		vel[0] /= -x;
		vel[1] /= -x;
		vel[2] = BallSpeed*2.5;

		TeleportEntity(entity, NULL_VECTOR, NULL_VECTOR, vel);
		//SetEntPropVector(entity, Prop_Data, "m_vecAbsVelocity", vel);
		SetEntProp(entity, Prop_Data, "m_iTeamNum", GetClientTeam(attacker));
		SetEntPropFloat(entity, Prop_Data, "m_flDamage", 99.0);
		SetEntPropFloat(entity, Prop_Data, "m_DmgRadius", 350.0);
		SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", attacker);
		if(GetClientTeam(ntarget) == 2){
			ntarget =  GetRandomPlayerTeam(3);
			SetEntityRenderColor(iDb_Ball, 255, 0, 0, 255);
			OldTeamTarget = 3;
		}
		else
		if(GetClientTeam(ntarget) == 3){
			ntarget =  GetRandomPlayerTeam(2);
			SetEntityRenderColor(iDb_Ball, 0, 255, 255, 255);
			OldTeamTarget = 2;
		}
		if(OldTeamTarget == 2){
			TE_SetupBeamFollow(entity, Trail, 0, 2.0, 1.0, 1.0, 1, BluTrail);
			TE_SendToAll();
		}
		if(OldTeamTarget == 3){
			TE_SetupBeamFollow(entity, Trail, 0, 2.0, 1.0, 1.0, 1, RedTrail);
			TE_SendToAll();
		}
	}
	return Plugin_Changed;
}
public Action HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if(victim <0 || victim > MAXPLAYERS)return Plugin_Changed;
	else
	if(damage <=200){
		damage = 0.0;
	}
	return Plugin_Changed;
}
public Action ForceEnd(Handle timer)
{
	if (microgametimer != INVALID_HANDLE) TriggerTimer(microgametimer);
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, HurtClient);
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		TpDisabled(i);
		g_attack = false;
	}
	if(IsValidEntity(iDb_Ball)){
		SDKUnhook(iDb_Ball, SDKHook_Touch, OnTouchDodge);
		SDKUnhook(iDb_Ball, SDKHook_OnTakeDamage, Dodgeball_Hurt);
		AcceptEntityInput(iDb_Ball, "Kill");
		iDb_Ball =-1;
	}
	roundstart = false;
	g_attack = false;
	Dodgestart = true;
	
	return Plugin_Stop;
}
public void Dodgeball_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, HurtClient);
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		TpDisabled(i);
		g_attack = false;
	}
	if(IsValidEntity(iDb_Ball)){
		SDKUnhook(iDb_Ball, SDKHook_Touch, OnTouchDodge);
		SDKUnhook(iDb_Ball, SDKHook_OnTakeDamage, Dodgeball_Hurt);
		AcceptEntityInput(iDb_Ball, "Kill");
		iDb_Ball =-1;
	}
	ServerCommand("sm_cvar tf_flamethrower_burstammo 25");
	roundstart = false;
	g_attack = false;
}
public Action OnTouchDodge(int entity, int other)
{
	entity = iDb_Ball;
	if(((GetEntityFlags(entity) & FL_ONGROUND)))return Plugin_Continue;
	if(other && other <= MaxClients){
		if (IsValidClient(other) && GetClientTeam(other) == TeamSelection && other == ntarget) {
			float TargetLocation[3];
			GetClientAbsOrigin(other, TargetLocation);
			ClientExplode(other, TargetLocation);
			int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
			float Damage = 500.0;
			if(owner <= 0){
				SDKHooks_TakeDamage(ntarget, 0, 0, Damage);
			}
			if(IsValidClient(owner)){
				SDKHooks_TakeDamage(ntarget, owner, owner, Damage);
			}
			RequestFrame(Remove_Ragdoll, other);
			if (IsValidEntity(entity)) {
				SDKUnhook(entity, SDKHook_Touch, OnTouchDodge);
				SDKUnhook(entity, SDKHook_OnTakeDamage, Dodgeball_Hurt);
				AcceptEntityInput(entity, "Kill");
				iDb_Ball = -1;
				//CreateTimer(0.4, iBallFiredTeamSelection);
			}
		}
	}
	return Plugin_Continue;
}
int GetRandomPlayerTeamDB(int team)
{
	int clients[MAXPLAYERS+1]; int clientCount;
	for (int i = 1; i <= MaxClients; i++)
		if (IsValidClient(i) && IsPlayerAlive(i) && GetClientTeam(i) == team && IsClientParticipating(i))
		clients[clientCount++] = i;
	return (clientCount == 0) ? -1 : clients[GetRandomInt(0, clientCount-1)];
}
public void Dodgeball_PlayerDeath(int client) {
	SetStateClient(client, false, true);
	//CreateTimer(0.4, iBallFiredTeamSelection);
}