// Airraid module

#define Team_Spec 1
#define Team_Red 2
#define Team_Blu 3

#define CTFPROJECTILE_ROCKET "models/weapons/w_models/w_rocket.mdl"
#define TF_PROJECTILE_ROCKET "models/weapons/w_models/w_rocket.mdl"

public void Airraid_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Airraid_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Airraid_End);	
	//AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Airraid_CheckDeadPlayers);
	g_AirRaidDeflect = true;
	//SetStateAll(true);
	g_attack = true;
	CreateTimer(0.5, iRocketsFired);	 
}

public void Airraid_OnMapStart() {
	PrecacheModel(CTFPROJECTILE_ROCKET, true);
	PrecacheModel(TF_PROJECTILE_ROCKET, true);	
}

public Action iRocketsFired(Handle hndl, any data) {
	float pos[3];
	float ang[3];
	ang[0] = 90.0;
	float rand;
	for (int i=-2; i<=2; i++) {
		for (int j=-2; j<=2; j++) {
			rand = GetRandomFloat(-150.0, 100.0);
			pos[2] = rand;
			pos[0] = i*244.0;
			pos[1] = j*244.0;
			RocketsFired(pos, ang, 350.0, 300.0, Team_Spec, true);
		}
	}  
	
	return Plugin_Continue;
}


int RocketsFired(float vPosition[3], float vAngles[3] = NULL_VECTOR, float flSpeed = 600.0, float flDamage = 450.0, int iTeam = Team_Spec, bool bCritical = false){

	char strClassname[32] = "";
	char strEntname[32] = "";

	strClassname = "CTFProjectile_Rocket";
	strEntname = "tf_projectile_rocket";
	
	int iRocket = CreateEntityByName(strEntname);
	if(!IsValidEntity(iRocket))
		return -0;
	
	float vVelocity[3];
	float vBuffer[3];
	
	GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
	
	vVelocity[0] = vBuffer[0]*flSpeed;
	vVelocity[1] = vBuffer[1]*flSpeed;
	vVelocity[2] = vBuffer[2]*flSpeed;
	
	TeleportEntity(iRocket, vPosition, vAngles, vVelocity);
	
	SetEntData(iRocket, FindSendPropInfo(strClassname, "m_nSkin"), (iTeam-2), 1, true);
	SetEntData(iRocket, FindSendPropInfo(strClassname, "m_bCritical"), bCritical, true);
	SetEntDataFloat(iRocket, FindSendPropInfo(strClassname, "m_iDeflected") + 4, flDamage, true);
	
	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "TeamNum", -1, -1, 0);

	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "SetTeam", -1, -1, 0); 
	
	DispatchSpawn(iRocket);
	return iRocket;
}

public void Airraid_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "pyro");
		RemoveClientWeapons(client);
		giveitem(client, 21);
		SetClientSlot(client, 0);
		TpEnabled(client);
		SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);
		g_attack = true;
	}
}

public void Airraid_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		TpDisabled(i);
		g_AirRaidDeflect = false;
		g_attack = false;
	}
}
public Action SoundHookBlast(int clients[64], int &numClients, char sound[PLATFORM_MAX_PATH], int &client, int &channel, float &volume, int &level, int &pitch, int &flags)
{
    if (StrContains(sound, "flame_thrower_airblast_rocket_redirect", false) != -1)
    {
		if(g_AirRaidDeflect){
			int clientOw = GetEntPropEnt(client, Prop_Data, "m_hOwnerEntity");
			if(clientOw <0 || clientOw > MAXPLAYERS)return Plugin_Changed;
			SetStateClient(clientOw, true, true);
		}
	}
    return Plugin_Continue;
}  