// Hit Enemy module
#define HHH "models/bots/headless_hatman.mdl"
#define AXE "models/weapons/c_models/c_bigaxe/c_bigaxe.mdl"
bool g_IsModel[MAXPLAYERS+1] = { false, ... };
bool g_bIsHHH[MAXPLAYERS + 1] = { false, ... };
int g_iHHHParticle[MAXPLAYERS + 1][3];
int monster_type = -1;

public void HitHHH_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, HHH_OnMapStart);
}

public void HHH_OnMapStart() {
	PrecacheModel(HHH, true);
	PrecacheModel(AXE, true);
	HookEvent("player_death", Event_Death2,  EventHookMode_Post);
}


public void HitEnemyhhh_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, HitEnemyhhh_justEntered);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, HitEnemyhhh_CheckDeadPlayers);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, HitEnemyhhh_End);
	SetStateAll(true);
	monster_type = GetRandomInt(0, 2);
	g_attack = true;
		
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
}

public void HitEnemyhhh_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if(monster_type == 0){
			RemoveModel(i);
			SetClientClass(i, "sniper");
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			SDKUnhook(i, SDKHook_OnTakeDamage, HitEnemyhhh_OnTakeDamageClient);
		}
		if(monster_type == 1 ){
			RemoveModel(i);
			ClearHorsemannParticles(i);
			SDKUnhook(i, SDKHook_OnTakeDamage, HitEnemyhhh_OnTakeDamageClient);       
			SetClientClass(i, "demoman");	
			g_bIsHHH[i] = false;
			DisableClientWeapons(i);	
			g_attack = false;
			TpDisabled(i);
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
		}
		if(monster_type == 2){
			RemoveModel(i);
			SetClientClass(i, "sniper");
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			SDKUnhook(i, SDKHook_OnTakeDamage, HitEnemyhhh_OnTakeDamageClient);
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);	
}
public void HitEnemyhhh_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}
public Action HitEnemyhhh_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		SetStateClient(attacker, true, true);
		damage = 500.0;
	}
	return Plugin_Changed;
}


public void HitEnemyhhh_justEntered(int client) {
	if (IsValidClient(client)) {
		RemoveClientWeapons(client);
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int totalr = GetActivePlayers();
		int posar = 360 / totalr * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 36.399994  + (Cosine(DegToRad(float(posar)))*220.0);
		pos[1] = -1792.500854 - (Sine(DegToRad(float(posar)))*220.0);
		pos[2] = 459.432190;
		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posar);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);		    
		NoCollision(true);
		SDKHook(client, SDKHook_OnTakeDamage, HitEnemyhhh_OnTakeDamageClient);
		TF2_RemoveAllWearables(client);
		SDKHook(client, SDKHook_PreThink, SDKHooks_SpeedClient);
		if(monster_type == 1 ){
			SetClientClass(client, "demoman");
			g_bIsHHH[client] = true;    
			RequestFrame(DoHorse, client);
		}
		if(monster_type == 0 ){
			SetClientClass(client, "sniper");
			RequestFrame(BeSkeleton, client);
			giveitem(client, 939);
			TF2_SetHealth(client, 1000);
		}
		if(monster_type == 2 ){
			SetClientClass(client, "sniper");
			RequestFrame(BeMerasmus, client);
			giveitem(client, 3);
			TF2_SetHealth(client, 1000);
		}
	}
}
public void DoHorse(int client)
{
	if (IsValidClient(client)){
		MakeHorsemann(client);
	}
}
public void Event_Death2(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	int deathflags = GetEventInt(event, "death_flags");
	if (!(deathflags & TF_DEATHFLAG_DEADRINGER))
	{
		if (IsValidClient(client) && g_bIsHHH[client])
		{
//			DoHorsemannDeath(client);
			ClearHorsemannParticles(client);
			g_bIsHHH[client] = false;			
			SetClientClass(client, "demoman");			
			RemoveClientWeapons(client);
			TpDisabled(client);			
		}
	}
}
public void SetModel(int client, const char[] model)
{
    if (IsValidClient(client) && IsPlayerAlive(client))
    {
        SetVariantString(model);
        AcceptEntityInput(client, "SetCustomModel");
        DispatchKeyValue(client, "modelscale", "0.7");
        SetEntProp(client, Prop_Send, "m_bUseClassAnimations", 1);
        g_IsModel[client] = true;
    }
}
public void RemoveModel(int client)
{
    if (IsValidClient(client))
    {
        SetVariantString("");
        AcceptEntityInput(client, "SetCustomModel");
        g_IsModel[client] = false;
    }
}
stock void ClearHorsemannParticles(int client)
{
	for (int i = 0; i < 3; i++)
	{
		int ent = EntRefToEntIndex(g_iHHHParticle[client][i]);
		if (ent > MaxClients && IsValidEntity(ent)) AcceptEntityInput(ent, "Kill");
		g_iHHHParticle[client][i] = INVALID_ENT_REFERENCE;
	}
}
stock void DoHorsemannParticles(int client)
{
	ClearHorsemannParticles(client);
	int blefteye = MakeParticle(client, "halloween_boss_eye_glow", "lefteye");
	if (IsValidEntity(blefteye)){
		g_iHHHParticle[client][0] = EntIndexToEntRef(blefteye);
	}
	int brighteye = MakeParticle(client, "halloween_boss_eye_glow", "righteye");
	if (IsValidEntity(brighteye))
	{
		g_iHHHParticle[client][1] = EntIndexToEntRef(brighteye);
	}
}
stock int MakeParticle(int client, char[] effect, char[] attachment)
{
		float pos[3];
		float ang[3];
		char buffer[128];
		GetEntPropVector(client, Prop_Send, "m_vecOrigin", pos);
		GetClientEyeAngles(client, ang);
		ang[0] *= -1;
		ang[1] += 180.0;
		if (ang[1] > 180.0) ang[1] -= 360.0;
		ang[2] = 0.0;
	//	GetAngleVectors(ang, pos2, NULL_VECTOR, NULL_VECTOR);
		int particle = CreateEntityByName("info_particle_system");
		if (!IsValidEntity(particle)) return -1;
		TeleportEntity(particle, pos, ang, NULL_VECTOR);
		DispatchKeyValue(particle, "effect_name", effect);
		SetVariantString("!activator");
		AcceptEntityInput(particle, "SetParent", client, particle, 0);
		if (attachment[0] != '\0')
		{
			SetVariantString(attachment);
			AcceptEntityInput(particle, "SetParentAttachmentMaintainOffset", particle, particle, 0);
		}
		Format(buffer, sizeof(buffer), "%s_%s%d", effect, attachment, particle);
		DispatchKeyValue(particle, "targetname", buffer);
		DispatchSpawn(particle);
		ActivateEntity(particle);
		SetEntPropEnt(particle, Prop_Send, "m_hOwnerEntity", client);
		AcceptEntityInput(particle, "Start");
		return particle;
}
void MakeHorsemann(int client)
{
	int ragdoll = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
	if (ragdoll > MaxClients && IsValidEntity(ragdoll)) AcceptEntityInput(ragdoll, "Kill");
	char weaponname[32];
	GetClientWeapon(client, weaponname, sizeof(weaponname));
	if (strcmp(weaponname, "tf_weapon_minigun", false) == 0) 
	{
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iWeaponState", 0);
	}
	TF2_SwitchtoSlot(client, TFWeaponSlot_Melee);
	CreateTimer(0.0, Timer_Switch, client);
	SetModel(client, HHH);
	TpEnabled(client);
	DoHorsemannParticles(client);
	RemoveClientWeapons(client);
	TF2_SetHealth(client, 1000);
	g_bIsHHH[client] = true;
}
stock void TF2_SetHealth(int client, int NewHealth)
{
	SetEntProp(client, Prop_Send, "m_iHealth", NewHealth, 1);
	SetEntProp(client, Prop_Data, "m_iHealth", NewHealth, 1);
}
public Action Timer_Switch(Handle timer, any client)
{
    if (IsValidClient(client))
    {
        giveitem(client, 266);
        SDKHook(client, SDKHook_PreThink, SDKHooks_SpeedClient);
    }
    return Plugin_Stop;
}
stock void TF2_SwitchtoSlot(int client, int slot)
{
    if (slot >= 0 && slot <= 5 && IsClientInGame(client) && IsPlayerAlive(client))
    {
        char classname[64];
        int wep = GetPlayerWeaponSlot(client, slot);
        if (wep > MaxClients && IsValidEdict(wep) && GetEdictClassname(wep, classname, sizeof(classname)))
        {
            FakeClientCommandEx(client, "use %s", classname);
            SetEntPropEnt(client, Prop_Send, "m_hActiveWeapon", wep);
        }
    }
}
public void BeSkeleton(int client){
	if (IsValidClient(client)){
		SetVariantString("models/bots/skeleton_sniper/skeleton_sniper.mdl");
		AcceptEntityInput(client, "SetCustomModel");
		SetEntProp(client, Prop_Send, "m_bUseClassAnimations", 1);
	}		
}
public void BeMerasmus(int client){
	if (IsValidClient(client)){
		SetVariantString("models/bots/merasmus/merasmus.mdl");
		DispatchKeyValue(client, "modelscale", "0.6");
		AcceptEntityInput(client, "SetCustomModel");
		SetEntProp(client, Prop_Send, "m_bUseClassAnimations", 1);
	}		
}