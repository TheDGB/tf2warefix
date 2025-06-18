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

#define MERASMUS "merasmus"
enum tf_cond 
{
 TFCond_Stunned = (1 << 15)
}
public void MeraStun_OnMapStart()
{
	PrecacheMerasmus();
}
// Precache Merasmus files
// Thanks to Chaosxk for a wonderful example of how to make precaching sounds better
void PrecacheMerasmus()
{
	PrecacheModel("models/bots/merasmus/merasmus.mdl", true);
	PrecacheModel("models/prop_lakeside_event/bomb_temp.mdl", true);
	PrecacheModel("models/prop_lakeside_event/bomb_temp_hat.mdl", true);
	
	for(int i = 1; i <= 17; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_appears0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_appears%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 11; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_attacks0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_attacks%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 54; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_headbomb0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_headbomb%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 33; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_held_up0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_held_up%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 2; i <= 4; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_island0%d.wav", i);
		PrecacheSound(iString, true);
	}
	
	for(int i = 1; i <= 3; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_bcon_skullhat0%d.wav", i);
		PrecacheSound(iString, true);
	}

	for(int i = 1; i <= 2; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_combat_idle0%d.wav", i);
		PrecacheSound(iString, true);
	}
	
	for(int i = 1; i <= 12; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_defeated0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_defeated%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 9; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_found0%d.wav", i);
		PrecacheSound(iString, true);
	}

	for(int i = 3; i <= 6; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_grenades0%d.wav", i);
		PrecacheSound(iString, true);
	}
	
	for(int i = 1; i <= 26; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_headbomb_hit0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_headbomb_hit%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 19; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_hide_heal10%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_hide_heal1%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 49; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_hide_idles0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_hide_idles%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 16; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_leaving0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_leaving%d.wav", i);
		if(FileExists(iString)) {
			PrecacheSound(iString, true);
		}
	}
	
	for(int i = 1; i <= 5; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_pain0%d.wav", i);
		PrecacheSound(iString, true);
	}
	
	for(int i = 4; i <= 8; i++) {
		char iString[PLATFORM_MAX_PATH];
		Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_ranged_attack0%d.wav", i);
		PrecacheSound(iString, true);
	}
	
	for(int i = 2; i <= 13; i++) {
		char iString[PLATFORM_MAX_PATH];
		if(i < 10) Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_staff_magic0%d.wav", i);
		else Format(iString, sizeof(iString), "vo/halloween_merasmus/sf12_staff_magic%d.wav", i);
		if(FileExists(iString))	{
			PrecacheSound(iString, true);
		}
	}
	
	PrecacheSound("vo/halloween_merasmus/sf12_hide_idles_demo01.wav", true);
	PrecacheSound("vo/halloween_merasmus/sf12_magic_backfire06.wav", true);
	PrecacheSound("vo/halloween_merasmus/sf12_magic_backfire07.wav", true);
	PrecacheSound("vo/halloween_merasmus/sf12_magic_backfire23.wav", true);
	PrecacheSound("vo/halloween_merasmus/sf12_magic_backfire29.wav", true);
	PrecacheSound("vo/halloween_merasmus/sf12_magicwords11.wav", true);
	
	PrecacheSound("misc/halloween/merasmus_appear.wav", true);
	PrecacheSound("misc/halloween/merasmus_death.wav", true);
	PrecacheSound("misc/halloween/merasmus_disappear.wav", true);
	PrecacheSound("misc/halloween/merasmus_float.wav", true);
	PrecacheSound("misc/halloween/merasmus_hiding_explode.wav", true);
	PrecacheSound("misc/halloween/merasmus_spell.wav", true);
	PrecacheSound("misc/halloween/merasmus_stun.wav", true);
}

public void MeraStun_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, MeraStun_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, MeraStun_End);	
	//AddToForward(g_OnAlmostEnd, INVALID_HANDLE, MeraStun_CheckDeadPlayers);
	
	//SetStateAll(true);
	g_attack = true;
	ServerCommand("sm_cvar tf_flamethrower_burstammo 0");
	CreateTimer(0.1, Spawn_MonsterMera);
}
public void MeraStun_justEntered(int client) {
	if (IsValidClient(client)) {
		SDKHook(client, SDKHook_PreThink, Airblast_RemoveLeftClick);

		RemoveClientWeapons(client);
		SetClientClass(client, "pyro");
		TpEnabled(client);
		giveitem(client, 21);
		SetClientSlot(client, 0);
		TF2_AddCondition(client, TFCond_HalloweenSpeedBoost, 20.0);
		TF2_AddCondition(client, TFCond_HalloweenBombHead, 20.0);
		g_attack = true;		
	}	
}

public void MeraStun_End() {	
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_PreThink, Airblast_RemoveLeftClick);
		TpDisabled(i);
		g_attack = false;
	}		
	int monstermera = -1;
	while((monstermera = FindEntityByClassname(monstermera, MERASMUS)) != -1) {
		if(IsValidEntity(monstermera)) {
			Handle g_Event = CreateEvent("merasmus_killed", true);
			FireEvent(g_Event);
			AcceptEntityInput(monstermera, "Kill");
			
		}
	}
	ServerCommand("sm_cvar tf_flamethrower_burstammo 25");
}
public Action Spawn_MonsterMera(Handle hndl, any data)
{
	int monstermera = CreateEntityByName(MERASMUS);
	DispatchKeyValue(monstermera, "modelscale", "0.5");
	DispatchSpawn(monstermera);
	if(IsValidEntity(monstermera)){
		float monmeraLoc[3] = {6.509786, 10.568852, -631.781738};
		TeleportEntity(monstermera, monmeraLoc, NULL_VECTOR, NULL_VECTOR);	   
	}	

	return Plugin_Continue;
}
public Action MerasmusThink(Handle hTimer, int iMerasmus)
{
	if(!IsValidEntity(iMerasmus))
		return Plugin_Stop;
	char sClass[64];
	GetEntityNetClass(iMerasmus,sClass,sizeof(sClass));
	if(!StrEqual(sClass,"CMerasmus"))
		return Plugin_Stop;
	float pos1[3];
	GetEntPropVector(iMerasmus, Prop_Data,"m_vecAbsOrigin",pos1);
	for(int i=1; i<=MaxClients; i++)
	{
		if(!IsClientInGame(i) || !TF2_IsPlayerInCondition(i, TFCond_HalloweenBombHead)) continue;
					
		float pos2[3];
		GetClientEyePosition(i, pos2);
					
		float distance = GetVectorDistance(pos1, pos2);
					
		float dist = 200.0;
					
							
		if(distance < dist)
		{
			SDK_StunMerasmus(iMerasmus, i);
			SetStateClient(i, true, true);
		}
	}
	return Plugin_Continue;
}