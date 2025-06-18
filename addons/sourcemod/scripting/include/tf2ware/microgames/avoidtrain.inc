//Avoid The Train Tnx to FlaminSarge for tf2 Train Rain Script

bool bSounds;
float HEIGHT[3] = { 1950.0, 0.0, 0.0 };
float HEIGHT2[3] = { 1953.0, 0.0, 0.0 };

public void AvoidTrain_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, AvoidTrain_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, AvoidTrain_End);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, AvoidTrain_CheckDeadPlayers);
	HookEntityOutput("func_tracktrain", "OnUser3", OnTrainReset);
	SetStateAll(true);
	if (g_Gamemode == GAMEMODE_WIPEOUT) {
			for (int i = 1; i <= MaxClients; i++) {
				if (IsValidClient(i) && IsClientParticipating(i)) {
					KillAllThings();
					SpawnTrainsAboveEverything(i);
				}
			}
		}
	if (g_Gamemode == GAMEMODE_NORMAL) {
		int client;
		int client2 = client;
		KillAllThings();
		client = GetRandomPlayer();
		SpawnTrainsAboveEverything(client);
		client2 = GetRandomPlayer();
		SpawnTrainsAboveEverything(client2);
	}
}
public void AvoidTrain_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "scout");
		DisableClientWeapons(client);
	}
}
public void AvoidTrain_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}
stock void SpawnTrainsAboveEverything(int client)
{
	float pos[3];
	char name[32];
	int alive = 0;
	if (GetClientCount(true) < 1) return;
	int trainref = CreateTrainTrack();
	int train = EntRefToEntIndex(trainref);
	if (!IsValidEntity(train)) return;
	SetVariantString("TrainRainPath1");
	AcceptEntityInput(train, "TeleportToPathTrack");
	AcceptEntityInput(train, "StartForward");
	float hurtmin[3] = { -200.0, -80.0, -305.65 };
	float hurtmax[3] = { 380.7, 80.0, 195.65 };
	float clipmin[3] = { 0.0, -70.0, -205.65 };
	float clipmax[3] = { 296.7, 70.0, 305.65 };

	alive++;
	GetClientAbsOrigin(client, pos);
		//pos[2] += 1000.0;
	pos[0] += 53.4;

	int ent = CreateEntityByName("prop_dynamic");
	DispatchKeyValue(ent, "parentname", "TrainRain");
	SetVariantString("TrainRain");
	AcceptEntityInput(ent, "SetParent");
	FormatEx(name, sizeof(name), "TrainRainProp%d", client);
	DispatchKeyValue(ent, "targetname", name);
	DispatchKeyValue(ent, "solid", "0");
	DispatchKeyValue(ent, "angles", "0 180 0");
	SetEntityModel(ent, "models/props_vehicles/train_enginecar.mdl");
	DispatchSpawn(ent);
	float andtrain[3] = {0.0, 180.0, 0.0};
	TeleportEntity(ent, pos, andtrain, NULL_VECTOR);
	ActivateEntity(ent);
	PushStackCell(hEntStack, EntIndexToEntRef(ent));	

	int clip = CreateEntityByName("func_brush");
	DispatchKeyValue(clip, "parentname", "TrainRain");
	SetVariantString("TrainRain");
	AcceptEntityInput(clip, "SetParent");
	FormatEx(name, sizeof(name), "TrainRainClip%d", client);
	DispatchKeyValue(clip, "targetname", name);
	DispatchSpawn(clip);
	ActivateEntity(clip);
	PushStackCell(hEntStack, EntIndexToEntRef(clip));
	TeleportEntity(clip, pos, NULL_VECTOR, NULL_VECTOR);
	SetEntityModel(clip, "models/player/demo.mdl");
	SetEntPropVector(clip, Prop_Send, "m_vecMins", clipmin);
	SetEntPropVector(clip, Prop_Send, "m_vecMaxs", clipmax);
	SetEntProp(clip, Prop_Send, "m_nSolidType", 2);
	int enteffects = GetEntProp(clip, Prop_Send, "m_fEffects");
	enteffects |= 32;
	SetEntProp(clip, Prop_Send, "m_fEffects", enteffects);

	int hurt = CreateEntityByName("trigger_hurt");
	DispatchKeyValue(hurt, "damage", "5000");
	DispatchKeyValue(hurt, "damagetype", "16");
	DispatchKeyValue(hurt, "parentname", "TrainRain");
	SetVariantString("TrainRain");
	AcceptEntityInput(hurt, "SetParent");
	DispatchKeyValue(hurt, "spawnflags", "1097");
	FormatEx(name, sizeof(name), "TrainRainHurt%d", client);
	DispatchKeyValue(hurt, "targetname", name);
	DispatchSpawn(hurt);
	ActivateEntity(hurt);
	PushStackCell(hEntStack, EntIndexToEntRef(hurt));
	TeleportEntity(hurt, pos, NULL_VECTOR, NULL_VECTOR);
	SetEntityModel(hurt, "models/player/demo.mdl");
	SetEntPropVector(hurt, Prop_Send, "m_vecMins", hurtmin);
	SetEntPropVector(hurt, Prop_Send, "m_vecMaxs", hurtmax);
	SetEntProp(hurt, Prop_Send, "m_nSolidType", 2);
	enteffects = GetEntProp(hurt, Prop_Send, "m_fEffects");
	enteffects |= 32;
	SetEntProp(hurt, Prop_Send, "m_fEffects", enteffects);

	if (alive > 0 && bSounds && GetRandomInt(0, 3))
	{
		EmitSoundToAll(TRAIN_RAIN, SOUND_FROM_PLAYER);	//doubling these is bad but whatever
		if (!GetRandomInt(0, 2)) EmitSoundToAll(TRAIN_RAIN, SOUND_FROM_PLAYER);
	}
}
public void OnTrainHurt(const char[] output, int caller, int activator, float delay)
{
	char name[32];
	if (!bSounds) return;
	GetEntPropString(caller, Prop_Data, "m_iName", name, sizeof(name));
	if (GetRandomInt(0, 5)) return;
	if (StrContains(name, "TrainRainHurt") == -1) return;
	EmitSoundToAll(TRAIN_HIT, activator);	//doubling these is baaaaad but whatever
	EmitSoundToAll(TRAIN_HIT, activator);
}
public void OnTrainReset(const char[] output, int caller, int activator, float delay)
{
	char name[32];
	GetEntPropString(caller, Prop_Data, "m_iName", name, sizeof(name));
	if (!StrEqual(name, "TrainRain")) return;
	KillAllThings();
}
stock void KillAllThings()	//train)
{
	int ref;
	if (hEntStack == INVALID_HANDLE) return;
	while (!IsStackEmpty(hEntStack))
	{
		PopStackCell(hEntStack, ref);
		int ent = EntRefToEntIndex(ref);
		if (IsValidEntity(ent)) AcceptEntityInput(ent, "Kill");
	}
	return;

}
stock int CreateTrainTrack(bool forceReset = false, bool kill = false)
{
	static int trainref = INVALID_ENT_REFERENCE;
	static int trackrefs[3] = { INVALID_ENT_REFERENCE, ... };
	int train = EntRefToEntIndex(trainref);
	int track1 = EntRefToEntIndex(trackrefs[0]);
	int track2 = EntRefToEntIndex(trackrefs[1]);
	int track4 = EntRefToEntIndex(trackrefs[2]);
	if (forceReset || kill)
	{
		if (IsValidEntity(train)) AcceptEntityInput(train, "KillHierarchy");
		if (IsValidEntity(track1)) AcceptEntityInput(track1, "Kill");
		if (IsValidEntity(track2)) AcceptEntityInput(track2, "Kill");
		if (IsValidEntity(track4)) AcceptEntityInput(track4, "Kill");
		trainref = INVALID_ENT_REFERENCE;
		trackrefs[0] = INVALID_ENT_REFERENCE;
		trackrefs[1] = INVALID_ENT_REFERENCE;
		trackrefs[2] = INVALID_ENT_REFERENCE;
	}
	if (kill) return INVALID_ENT_REFERENCE;
	if (!IsValidEntity(track4))
	{
		track4 = CreateEntityByName("path_track");
		DispatchKeyValue(track4, "target", "TrainRainPath1");
	}
	if (!IsValidEntity(track2))
	{
		track2 = CreateEntityByName("path_track");
		DispatchKeyValue(track2, "targetname", "TrainRainPath2");
	}
	if (!IsValidEntity(track1))
	{
		track1 = CreateEntityByName("path_track");
		DispatchKeyValue(track1, "targetname", "TrainRainPath1");
	}
	DispatchKeyValue(track4, "spawnflags", "16");
	DispatchKeyValue(track4, "speed", "1500");
	DispatchKeyValue(track4, "targetname", "TrainRainPath4");
	TeleportEntity(track4, HEIGHT2, NULL_VECTOR, NULL_VECTOR);
	DispatchSpawn(track4);
	ActivateEntity(track4);
	trackrefs[2] = EntIndexToEntRef(track4);

	DispatchKeyValue(track2, "speed", "1500");
	DispatchKeyValue(track2, "target", "TrainRainPath4");
	float andtb[3] ={ -1000.0, 0.0, 0.0 };
	TeleportEntity(track2, andtb, NULL_VECTOR, NULL_VECTOR);
	SetVariantString("OnPass TrainRain:Stop::0.0:-1");
	AcceptEntityInput(track2, "AddOutput");
	SetVariantString("OnPass TrainRain:FireUser3::0.0:-1");
	AcceptEntityInput(track2, "AddOutput");
	DispatchSpawn(track2);
	ActivateEntity(track2);	//OnPass, trigger TrainRainStop, which does Stop on TrainRain
	trackrefs[1] = EntIndexToEntRef(track2);

	DispatchKeyValue(track1, "spawnflags", "16");
	DispatchKeyValue(track1, "speed", "1500");
	DispatchKeyValue(track1, "target", "TrainRainPath2");
	TeleportEntity(track1, HEIGHT, NULL_VECTOR, NULL_VECTOR);
	DispatchSpawn(track1);
	ActivateEntity(track1);
	trackrefs[0] = EntIndexToEntRef(track1);
		/*new track3 = CreateEntityByName("path_track");
		DispatchKeyValue(track3, "speed", "1500");
		DispatchKeyValue(track3, "target", "TrainRainPath2");
		DispatchKeyValue(track3, "targetname", "TrainRainPath3");
		TeleportEntity(track3, Float:{ 1000.0, 0.0, 0.0 }, NULL_VECTOR, NULL_VECTOR);
		DispatchSpawn(track3);
		ActivateEntity(track3);	//used to enable/disable cameras, unneeded*/

	if (!IsValidEntity(train))
	{
		train = CreateEntityByName("func_tracktrain");
		DispatchKeyValue(train, "spawnflags", "528");
		DispatchKeyValue(train, "solid", "0");
		DispatchKeyValue(train, "origin", "0 0 1000");
		DispatchKeyValue(train, "speed", "1100");
		DispatchKeyValue(train, "startspeed", "1100");
		DispatchKeyValue(train, "target", "TrainRainPath1");
		DispatchKeyValue(train, "targetname", "TrainRain");
		TeleportEntity(train, HEIGHT, NULL_VECTOR, NULL_VECTOR);
		DispatchSpawn(train);
		ActivateEntity(train);
		SetVariantString("TrainRainPath1");
		AcceptEntityInput(train, "TeleportToPathTrack");
		AcceptEntityInput(train, "Stop");
		trainref = EntIndexToEntRef(train);
	}
	return trainref;
}
stock bool IsTSL(bool forceRecalc = false)
{
	bool tsl = false;
	bool found = false;
	char map[32];
	if (forceRecalc)
	{
		found = false;
	}
	if (!found)
	{
		GetCurrentMap(map, sizeof(map));
		tsl = StrContains(map, "trainsawlaser", false) != -1;
		found = true;
	}
	return tsl;
}
public void AvoidTrain_End() {
	CreateTrainTrack(_, true);
	//KillAllThings();
}