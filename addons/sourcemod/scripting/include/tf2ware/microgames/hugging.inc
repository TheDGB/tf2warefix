// Hugging module
#define HEAVY_MODEL "models/player/geavy.mdl"
char var_heavy_love[][] = {"gamesound/tf2ware/heavy_ilu.mp3", "vo/heavy_specialcompleted08.mp3", "vo/heavy_award04.mp3"};

public void Hugging_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Hugging_OnMapStart);
}

public void Hugging_OnMapStart() {
	char input[512];
	for (int i = 1; i <= sizeof(var_heavy_love); i++) {
		Format(input, sizeof(input), "sound/%s", var_heavy_love[i-1]);
		AddFileToDownloadsTable(input);		
		AddFileToDownloadsTable("models/player/geavy.dx80.vtx"); 
		AddFileToDownloadsTable("models/player/geavy.dx90.vtx"); 
		AddFileToDownloadsTable("models/player/geavy.mdl"); 
		AddFileToDownloadsTable("models/player/geavy.phy");
		AddFileToDownloadsTable("models/player/geavy.sw.vtx"); 
		AddFileToDownloadsTable("models/player/geavy.vvd"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/eyeball_l.vmt"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/eyeball_r.vmt"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/gayweapon_blue.vmt"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/gayweapon_hands.vmt"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/gayweapon_hands.vtf"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/gayweapon_red.vmt"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/gayweapon_red.vtf"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/geavy_head.vtf"); 
		AddFileToDownloadsTable("materials/models/player/hvyweapon/geavy_head_blue.vmt");
		AddFileToDownloadsTable("materials/models/player/hvyweapon/geavy_head_red.vmt");
		precacheSound(var_heavy_love[i-1]);
		PrecacheModel(HEAVY_MODEL, true);		
	}
}

public void Hugging_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Hugging_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Hugging_End);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Hugging_PlayerDeath);
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
	SetConVarInt(FindConVar("mp_autoteambalance"), 0);
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsValidTeam(i)){
			OldTeam[i] = GetClientTeam(i);
		}
	}
	int AllPlayers = GetClientCount(true);
	int target = 0;
	int attempts = 0;
	int heavies = GetActivePlayers() / 4;
	if (heavies <= 1) heavies = 2;
	if (heavies >= 4) heavies = 3;
	int heavyclients[MAXPLAYERS+1];
	int scoutclients[MAXPLAYERS+1];
	for (int i = 1; i <= heavies; i++) {
		attempts = 0;
		target = GetRandomInt(1, AllPlayers);
		while (((!IsValidClient(target)) || IsClientParticipating(target) == false || g_Spawned[target] == false || IsFakeClient(target) || g_Mission[target] > 0) && attempts <= 50) {
			target = GetRandomInt(1, AllPlayers);
			attempts += 1;
		}
		if (attempts <= 50) {
			g_Mission[target] = 1;
		}
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			if (g_Mission[i] == 0) {
				SetStateClient(i, true);
				scoutclients[i-1] = i;
			}
			else {
				SetStateClient(i, false);
				heavyclients[i-1] = i;
			}
		}
	}
	
	AddRandVocalization("heaviestaunt", heavyclients, 4.0, 8.0);
	AddRandVocalization("scoutdisgust", scoutclients, 1.5, 5.0);
	
	NoCollision(false);
	
}
public Action Hugging_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (status == 2) {
		if (damage > 0) damage = 1.0;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

public void Hugging_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			SDKUnhook(i, SDKHook_Touch, Hugging_OnPlayerTouch);
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			ChangeClientTeamAlive(i, OldTeam[i]);
		}
	}
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
	SetConVarInt(FindConVar("mp_autoteambalance"), 1);
}

public void Hugging_justEntered(int client) {
	if (IsValidClient(client)) {
	
		float pos[3];
		float ang[3];
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		int column = client;
		int row = 0;
		while (column > 10) {
			column = column - 10;
			row = row + 1;
		}
		if (g_Mission[client] == 0) {
			if (GetClientTeam(client) == 2) ChangeClientTeamAlive(client, 3);
			SetClientClass(client, "scout");
			DisableClientWeapons(client);       
			pos[0] = 6140.0 - float(column*80);
			pos[1] = 35.0 + float(row*80);
			pos[2] = -1600.0;
			ang[0] = 0.0;
			ang[1] = -90.0;
			ang[2] = 0.0;
		}//m_iTotalScore
		if (g_Mission[client] == 1) {
			if (GetClientTeam(client) == 3) ChangeClientTeamAlive(client, 2);
			SetClientClass(client, "heavy");
			Createheavyskin(client);
			RemoveClientWeapons(client);
			giveitem(client, 5);
			pos[0] = 6008.0 - float(column*80);
			pos[1] = -1175.0 - float(row*80);
			pos[2] = -1600.0;
			ang[0] = 0.0;
			ang[1] = 90.0;
			ang[2] = 0.0;
			SDKHook(client, SDKHook_PreThink, SDKHooks_SpeedClient);
		}
		TeleportEntity(client, pos, ang, vel);
		SDKHook(client, SDKHook_Touch, Hugging_OnPlayerTouch);
	}
}

public void Hugging_OnPlayerTouch(int entity, int other) {
	if (status != 2) return;
	if (!StrEqual(minigame, "Hugging")) return;
	char classname[64];
	char classname2[64];
	GetEdictClassname(entity, classname, sizeof(classname));
	GetEdictClassname(other, classname2, sizeof(classname2));
	if (StrEqual(classname, "player") && StrEqual(classname2, "player") && IsValidClient(entity) && IsValidClient(other) && status == 2 && IsPlayerAlive(entity) && IsPlayerAlive(other) && (g_Mission[other] > 0 || g_Mission[entity] > 0)) {
		if (g_Mission[entity] > 0 && g_Mission[other] > 0) return;
		int attacker = entity;
		int victim = other;
		if (g_Mission[other] > 0) {
			attacker = other;
			victim = entity;
		}
		ForcePlayerSuicide(victim);
		EmitSoundToAll(SOUND_HEAVY_KISS, attacker);
		EmitSoundToClient(victim, SOUND_HEAVY_KISS);
	}
}

public void Hugging_PlayerDeath(int client) {
	SetStateClient(client, false, true);
	
	CreateTimer(0.1, Hugging_Check);
}

public Action Hugging_Check(Handle timer) {
	if (status != 2) return Plugin_Continue;
	if (!StrEqual(minigame, "Hugging")) return Plugin_Continue;

	bool complete = true;
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (g_Mission[i] == 0) && IsClientParticipating(i) && IsPlayerAlive(i)) {
			complete = false;
		}
	}
	if (complete) {
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && (g_Mission[i] == 1) && IsClientParticipating(i)) {
				SetStateClient(i, true, true);
			}
		}
	}
	return Plugin_Continue;
}

void Createheavyskin(int client) {   
	//Add a new model
	char strModel[PLATFORM_MAX_PATH];
	Format(strModel, sizeof(strModel), "");
	if (TF2_GetPlayerClass(client) == TFClass_Heavy) Format(strModel, sizeof(strModel), "models/player/geavy.mdl");
	
	if (IsClientInGame(client) && IsPlayerAlive(client)) {
		SetVariantString(strModel);
		AcceptEntityInput(client, "SetCustomModel");
		if (!StrEqual(strModel, "")) {
			SetEntProp(client, Prop_Send, "m_bUseClassAnimations",1);
			SetEntProp(client, Prop_Send, "m_nBody", 0);
		}
	}
}

/* CleanString()
**
** Cleans the given string from any illegal character.
** -------------------------------------------------------------------------- */
stock void CleanString(char[] strBuffer)
{
	// Cleanup any illegal characters
	new Length = strlen(strBuffer);
	for (new iPos=0; iPos<Length; iPos++)
	{
		switch(strBuffer[iPos])
		{
			case '\r': strBuffer[iPos] = ' ';
			case '\n': strBuffer[iPos] = ' ';
			case '\t': strBuffer[iPos] = ' ';
		}
	}
	
	// Trim string
	TrimString(strBuffer);
}
/* FMax()
**
** Returns the maximum of the two values given.
** -------------------------------------------------------------------------- */
stock float FMax(float a, float b)
{
	return (a > b)? a:b;
}
	/* FMin()
**
* Returns the minimum of the two values given.
** -------------------------------------------------------------------------- */
stock float FMin(float a, float b)
{
	return (a < b)? a:b;
}

/* GetURandomIntRange()
**
** 
** -------------------------------------------------------------------------- */
stock int GetURandomIntRange(int iMin, int iMax)
{
    return iMin + (GetURandomInt() % (iMax - iMin + 1));
}
/* GetURandomFloatRange()
**
** 
** -------------------------------------------------------------------------- */
stock float GetURandomFloatRange(float fMin, float fMax)
{
    return fMin + (GetURandomFloat() * (fMax - fMin));
}

// EOF 