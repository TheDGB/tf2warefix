// Monocolus module by Tony

#if !defined Team_Spec
    #define Team_Spec 1
#endif
#if !defined Team_Red
    #define Team_Red 2
#endif
#if !defined Team_Blu
    #define Team_Blu 3
#endif

#define MONOCULUS "eyeball_boss"
int mono1;
int mono2;
public void Monocolus_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Monocolus_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Monocolus_End);	
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Monocolus_CheckDeadPlayers);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Monocolus_OnGameFrame);
		
	SetStateAll(true);
	g_attack = true;
	ServerCommand("sm_cvar tf_flamethrower_burstammo 0");
	{
	int monster = CreateEntityByName(MONOCULUS);
	if (IsValidEntity(monster)) {
			DispatchSpawn(monster);
			SetEntProp(monster, Prop_Data, "m_iTeamNum", 5);
			SetEntPropFloat(monster, Prop_Send, "m_flModelScale", 1.0);		
			float monLoc[3] = {-2.459815, -9.593908, 269.047699};
			TeleportEntity(monster, monLoc, NULL_VECTOR, NULL_VECTOR);
			mono2 = monster;
		}
	}
	CreateTimer(3.0, Spawn_Monster);
}
public void Monocolus_OnGameFrame() {
	float Apos[3];
	float Bpos[3];
	if(IsValidEntity(mono1)){
		GetEntPropVector(mono1, Prop_Data, "m_vecOrigin", Apos);
		if(IsValidEntity(mono2)){
			GetEntPropVector(mono2, Prop_Data, "m_vecOrigin", Bpos);
		}
		float Dist = GetVectorDistance(Apos, Bpos);
		float TeleDist;
		TeleDist = 80.0;
		if(Dist <= TeleDist)
		{
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
			float Fpos[3];
			Fpos[0] = 238.0;
			Fpos[1] = 108.0;
			Fpos[2] = -261.0;
			float ang[3];
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;
					
			TeleportEntity(mono1, Fpos, ang, vel);
		}
	}
}
public void Monocolus_OnMapStart()
{
	PrecacheModel("models/props_halloween/halloween_demoeye.mdl", true);
	PrecacheModel("models/props_halloween/eyeball_projectile.mdl", true);
		
	for(int i = 1; i <= 3; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_eyeball/eyeball_laugh0%d.mp3", i);
		PrecacheSound(iString, true);
	}
	for(int i = 1; i <= 3; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_eyeball/eyeball_mad0%d.mp3", i);
		PrecacheSound(iString, true);
	}
	for(int i = 1; i <= 13; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_eyeball/eyeball0%d.mp3", i);
		else Format(iString, sizeof(iString), "vo/halloween_eyeball/eyeball%d.mp3", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	PrecacheSound("vo/halloween_eyeball/eyeball_biglaugh01.mp3", true);
	PrecacheSound("vo/halloween_eyeball/eyeball_boss_pain01.mp3", true);
	PrecacheSound("vo/halloween_eyeball/eyeball_teleport01.mp3", true);
	PrecacheSound("ui/halloween_boss_summon_rumble.mp3", true);
	PrecacheSound("ui/halloween_boss_chosen_it.mp3", true);
	PrecacheSound("ui/halloween_boss_defeated_fx.mp3", true);
	PrecacheSound("ui/halloween_boss_defeated.mp3", true);
	PrecacheSound("ui/halloween_boss_player_becomes_it.mp3", true);
	PrecacheSound("ui/halloween_boss_summoned_fx.mp3", true);
	PrecacheSound("ui/halloween_boss_summoned.mp3", true);
	PrecacheSound("ui/halloween_boss_tagged_other_it.mp3", true);
	PrecacheSound("ui/halloween_boss_escape.mp3", true);
	PrecacheSound("ui/halloween_boss_escape_sixty.mp3", true);
	PrecacheSound("ui/halloween_boss_escape_ten.mp3", true);
	PrecacheSound("ui/halloween_boss_tagged_other_it.mp3", true);
}

public void Monocolus_justEntered(int client) {
	if (IsValidClient(client)) {
		SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);

		SetClientClass(client, "pyro");
		RemoveClientWeapons(client);
		SetClientSlot(client, 0);
		giveitem(client, 21);
		TpEnabled(client);
		SetEntityHealth(client, 500);
		g_attack = true;		
	}
}
public void Monocolus_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void Monocolus_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)){
			SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
			TpDisabled(i);
		}
		g_attack = false;			
	}
	int monster = -1;
	while((monster = FindEntityByClassname(monster, MONOCULUS)) != -1) {
		if(IsValidEntity(monster)) {
			Handle g_Event = CreateEvent("eyeball_boss_killed", true);
			FireEvent(g_Event);
			AcceptEntityInput(monster, "Kill");
		}
	}
	ServerCommand("sm_cvar tf_flamethrower_burstammo 25");
}
public Action Spawn_Monster(Handle hndl, any data){
	int monster = CreateEntityByName(MONOCULUS);
	if(IsValidEntity(monster)){
		DispatchSpawn(monster);
		SetEntProp(monster, Prop_Data, "m_iTeamNum", 5);
		SetEntPropFloat(monster, Prop_Send, "m_flModelScale", 1.0);
		float mon3Loc[3] = {-75.153320, 188.454568, 559.468079};
		TeleportEntity(monster, mon3Loc, NULL_VECTOR, NULL_VECTOR);
		mono1 = monster;
	}
	
	return Plugin_Continue;
}
