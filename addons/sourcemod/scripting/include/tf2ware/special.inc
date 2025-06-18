/*
	1:      Speed Up        Everything is faster
	2:      Don't Touch     Touch other players and fail
	3:      x2 Bosses       Two bosses
	4:      Singleplayer    Everyone is invisible
	5:      Worst is Best   The person with the least points win
	6:      Bonk            Everyone gets a bat to push people with    
	7:      Thirdperson     Forced into thirdperson!    
*/

#define SPECIAL_TOTAL 11
#define MDL_DUCK		"models/items/target_duck.mdl"
bool Special_TwoBosses = false;
char var_special_name[][] = {
	"SUPER_SPEED",
	"NO_TOUCHING",
	"x2_BOSS_BATTLE",
	"SINGLEPLAYER",
	"LEAST_IS_BEST",
	"BONK",
	"THIRDPERSON",
	"FOV_EXPERIENCE",
	"BIG_WEAPONS",
	"FART_TIME",
	"BONUS_DUCK"
};
char var_special_desc[][] = {
	"SUPER_SPEED_DEC",
	"NO_TOUCHING_DEC",
	"x2_BOSS_BATTLE_DEC",
	"SINGLEPLAYER_DEC",
	"LEAST_IS_BEST_DEC",
	"BONK_DEC",
	"THIRDPERSON_DEC",
	"FOV_EXPERIENCE_DEC",
	"BIG_WEAPONS_DEC",
	"FART_TIME_DEC",
	"BONUS_DUCK_DEC"
};
bool g_fartvar = false;
Handle g_fart;

public void Special_BigWeapons(int i) 
{
    if (SpecialRound == 9 && IsValidClient(i)) 
    {
        SetEntPropFloat(i, Prop_Send, "m_flHandScale", 3.0);
    }
}
public void Special_Spawn(int i) 
{
    SetClientFOV(i, 110);
}
void SetClientFOV(int client, int iAmount) 
{
    SetEntProp(client, Prop_Send, "m_iFOV", iAmount);
}

public void Special_NoTouch(int entity, int other) 
{
    if (SpecialRound != 2)
	{
		return;
	}
	if (StrEqual(minigame, "Hugging"))
	{
		return;
	}
	if (StrEqual(minigame, "SniperTarget"))
	{
		return;
	}
	if (StrEqual(minigame, "SoldierTarget"))
	{
		return;
	}
	if (StrEqual(minigame, "Airblast"))
	{
		return;
	}
	if (StrEqual(minigame, "Batboy"))
	{
		return;
	}
	if (StrEqual(minigame, "Mandrill"))
	{
		return;
	}
	char classname[64];
	char classname2[64];
	GetEdictClassname(entity, classname, sizeof(classname));
	GetEdictClassname(other, classname2, sizeof(classname2));
	if (StrEqual(classname, "player") && StrEqual(classname2, "player") && IsValidClient(entity) && IsValidClient(other) && status == 2 && IsPlayerAlive(entity) && IsPlayerAlive(other) && GetClientTeam(entity) != GetClientTeam(other))
	{
		SetStateClient(entity, false, true);
		SetStateClient(other, false, true);
		ForcePlayerSuicide(entity);
		ForcePlayerSuicide(other);
		CPrintToChatEx(entity, other, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_TOUCH_PLAYER", other);
		CPrintToChat(entity, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_TOUCH_WARNING");
		CPrintToChatEx(other, entity, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_TOUCH_PLAYER", entity);
		CPrintToChat(other, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_TOUCH_WARNING");
	}
}

public Action Special_DamagePush(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker) && IsValidClient(victim) && SpecialRound == 6)
	{	
		char Weapon[128];
		GetClientWeapon(attacker, Weapon, sizeof(Weapon));
		if (StrEqual(Weapon, "tf_weapon_bat"))
		{
			float ang[3];
			float vel[3];
			GetClientEyeAngles(attacker, ang);
			vel[0] -= 300.0 * Cosine(DegToRad(ang[1])) * -1.0;
			vel[1] -= 300.0 * Sine(DegToRad(ang[1])) * -1.0;
			vel[2] += 300.0;
			TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, vel);
			damage = 0.0;
		}
	}
	return Plugin_Changed;
}

void ResetSpecialRoundEffect() 
{
	switch (simon_action) {
		case 2: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKUnhook(i, SDKHook_Touch, Special_NoTouch);
				}
			}
		}
		case 4: {
			NoCollision(false);
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SetEntityRenderColor(i, 255, 255, 255, 255);
					SetEntityRenderMode(i, RENDER_NORMAL);				
				}
			}
		}
		case 6: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKUnhook(i, SDKHook_OnTakeDamage, Special_DamagePush);
				}
			}
		}
		case 7: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && !IsFakeClient(i))
				{
					TpDisabled(i);
					g_bThirdPersonEnabled[i] = false;
				}
			}
		}
		case 8: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKUnhook(i, SDKHook_PreThink, Special_Spawn);			
					GetEntProp(i, Prop_Send, "m_iDefaultFOV");
				}
			}
		}
		case 9: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKUnhook(i, SDKHook_PreThink, Special_BigWeapons);
				}
			}
		}
		case 10: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					ClearTimer(g_fart);
					g_fartvar = false;
					g_fart = INVALID_HANDLE;
				}
			}
		}
	}
}

void AddSpecialRoundEffect() 
{
	switch (simon_action) {
		case 2: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKHook(i, SDKHook_Touch, Special_NoTouch);
				}
			}
		}
		case 3: {
			Special_TwoBosses = false;
		}
		case 4: {
			NoCollision(true);
			for (int i = 1; i <= MaxClients; i++) 
			{
				if (IsValidClient(i))
				{
					SetEntityRenderColor(i, 255, 255, 255, 0);
					SetEntityRenderMode(i, RENDER_NONE);
					TF2_RemoveAllWearables(i);				
				}				
			}
		}
		case 6: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKHook(i, SDKHook_OnTakeDamage, Special_DamagePush);
				}
			}
		}
		case 7: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && !IsFakeClient(i))
				{
					TpEnabled(i);
					g_bThirdPersonEnabled[i] = true;
				}
			}
		}
		case 8: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKHook(i, SDKHook_PreThink, Special_Spawn);
				}
			}
		}
		case 9: {
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					SDKHook(i, SDKHook_PreThink, Special_BigWeapons);
				}
			}
		}
		case 10: {
			g_fartvar = true;
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{
					g_fart = CreateTimer(40.0, Timer_Fart, i, TIMER_REPEAT);
				}
			}
		}
		case 11: {
			CreateDuckSpecial();
		}
	}
}

void SpecialPrecache() 
{
	AddFileToDownloadsTable("sound/misc/anxious.wav");
	AddFileToDownloadsTable("sound/misc/blower.wav");
	AddFileToDownloadsTable("sound/misc/common.wav");
	PrecacheSound("misc/anxious.wav", true);
	PrecacheSound("misc/blower.wav", true);
	PrecacheSound("misc/common.wav", true);
}

public Action Timer_Fart(Handle timer, any i) 
{
    if (IsValidClient(i) && g_fartvar) 
    {
		switch (GetRandomInt(0, 2))
		{
			case 0:
			{
				EmitSoundToClient(i, "misc/common.wav");
				AttachParticleFart(i);
			}
			case 1:
			{
				EmitSoundToClient(i, "misc/blower.wav");
				AttachParticleFart(i);
			}
			case 2:
			{
				EmitSoundToClient(i, "misc/anxious.wav");
				AttachParticleFart(i);
			}
		}
	}		
	
    return Plugin_Continue;	
}
public void AttachParticleFart(int i) 
{
	int strIParticle = CreateEntityByName("info_particle_system");
	char strName[128];
	if (IsValidEdict(strIParticle))
	{
		float strflPos[3];
		GetEntPropVector(i, Prop_Send, "m_vecOrigin", strflPos);
		TeleportEntity(strIParticle, strflPos, NULL_VECTOR, NULL_VECTOR);
		Format(strName, sizeof(strName), "target%i", i);
		DispatchKeyValue(i, "targetname", strName);
		DispatchKeyValue(strIParticle, "targetname", "tf2particle");
		DispatchKeyValue(strIParticle, "parentname", strName);
		DispatchKeyValue(strIParticle, "effect_name", "bday_balloon01");
		DispatchSpawn(strIParticle);
		SetVariantString(strName);
		AcceptEntityInput(strIParticle, "SetParent", strIParticle, strIParticle, 0);
		SetVariantString("head");
		AcceptEntityInput(strIParticle, "SetParentAttachment", strIParticle, strIParticle, 0);
		ActivateEntity(strIParticle);
		AcceptEntityInput(strIParticle, "start");
		CreateTimer(5.0, DeleteParticleFart, strIParticle);
	}
	else 
	{
		PrintToChat(i, "\x03 o_O");
	}
}

public Action DeleteParticleFart(Handle timer, any Ent) 
{
    if (!IsValidEntity(Ent)) 
    {
        return Plugin_Continue;
    }
	char cls[25];
	GetEdictClassname(Ent, cls, sizeof(cls));
	if (StrEqual(cls, "info_particle_system", false))
	{
		AcceptEntityInput(Ent, "Kill");
	}

	return Plugin_Continue;
}
void ClearTimer(Handle &timer) 
{
	if(timer != INVALID_HANDLE)
	{
		KillTimer(timer);
		timer = INVALID_HANDLE;
	}
}
void CreateDuckSpecial() 
{
    CreateTimer(0.5, iDuckFired22);
}
public Action iDuckFired22(Handle hndl, any data) 
{
	float pos[3]; float ang[3];
	ang[0] = 35.0;
	float rand;
	for (int i=-2; i<=2; i++)
	{
		for (int j=-2; j<=2; j++)
		{
			rand = GetRandomFloat(-780.0, -770.0);
			pos[2] = rand;
			pos[0] = i*244.0;
			pos[1] = j*244.0;
			DuckFired(pos);
		}
	}  
    return Plugin_Stop; // Explicit return for one-shot timer
}

int DuckFired(float vPosition[3])
{
	int iDuck = CreateEntityByName("tf_halloween_pickup");
	if(IsValidEntity(iDuck))
	{
		DispatchKeyValue(iDuck,"targetname","aether_ducks_bonus");
		DispatchKeyValue(iDuck,"powerup_model","models/items/target_duck.mdl");
		DispatchKeyValue(iDuck,"AutoMaterialize","0");
		DispatchKeyValue(iDuck,"Skin","2");
		DispatchKeyValue(iDuck,"pickup_sound","misc/halloween/duck_pickup_pos_01.wav");
		DispatchKeyValue(iDuck,"StartDisabled","0");
		TeleportEntity(iDuck, vPosition, NULL_VECTOR, NULL_VECTOR);
		SetEntPropFloat(iDuck, Prop_Send, "m_flModelScale", 0.6); 

		DispatchSpawn(iDuck);
		CreateTimer(4.0, KillDucks,iDuck);
		SDKHook(iDuck, SDKHook_Touch, OnTouchDuck);
		return iDuck;
	}
	
	return -0;
}
public Action OnTouchDuck(int entity, int client)
{
	char classname[64];
	GetEdictClassname(entity, classname, sizeof(classname));
	if (StrEqual(classname, "tf_halloween_pickup"))
	{
		if(client <0 || client > MAXPLAYERS)
		{
			return Plugin_Changed;
		}
		if(IsValidEntity(entity) && IsClientParticipating(client))
		{
			PlaySoundDuck(client);
			g_Points[client] += 1;
			SDKUnhook(entity, SDKHook_Touch, OnTouchDuck);
			UpdateHud(0.5);
		}
	}
	return Plugin_Continue;
}
public void PlaySoundDuck(int client) 
{
    if (IsValidClient(client)) 
    {
		EmitSoundToClient(client, SOUND_BONUS, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
	}			 
}   
public Action KillDucks( Handle timer, any entity )
{
	if (IsValidEntity(entity))
	{
		char classname[64];
		GetEdictClassname(entity, classname, sizeof(classname));
		if (StrEqual(classname, "tf_halloween_pickup"))
		{
			AcceptEntityInput(entity, "Kill");
			SDKUnhook(entity, SDKHook_Touch, OnTouchDuck);
		}
	}
	return Plugin_Continue;
}