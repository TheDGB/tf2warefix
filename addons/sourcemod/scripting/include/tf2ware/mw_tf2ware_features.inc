int g_PlayerVisibleWeapon[MAXPLAYERS+1] = {-1, ...};
//#define COLLISION_GROUP_PUSHAWAY 17
stock bool IsValidClient(int iClient) {
	if (iClient <= 0) return false;
	if (iClient > MaxClients) return false;
	if (!IsClientConnected(iClient)) return false;
	return IsClientInGame(iClient);
}

void RegMinigame(char[] game_name, Function initFunc, Function onMinigameLoad = INVALID_FUNCTION)
{
	for (int i=0; i<sizeof(g_name); i++)
	{
		if (StrEqual(game_name, g_name[i]))
		{
			if (ww_log.BoolValue)
			{
				LogMessage("Registered minigame %s, id %d", game_name, i+1);
			}
			g_initFuncs[i] = initFunc;
			
			if (onMinigameLoad != INVALID_FUNCTION)
			{
				if (ww_log.BoolValue)
				{
					LogMessage("--Running onMinigameLoad function for %s", game_name);
				}
				Call_StartFunction(INVALID_HANDLE, onMinigameLoad);
				Call_Finish();
			}
			return;
		}
	}
	PrintToServer("Warning: Attempted to register minigame %s and failed--configuration not found", game_name);
}

void GiveId()
{
	int id = 0;
	for (int client = 1; client <= MaxClients; client++)
	{
		if(IsValidClient(client) && IsClientParticipating(client))
		{
			id += 1;
			g_Id[client] = id;
		}
	}
}

void NoCollision(bool input)
{
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidEntity(i) && IsValidClient(i))
		{
			if (input)
			{
				SetEntData(i, g_offsCollisionGroup, 2, 4, true); // NO COLLISION
			}
			//if (input)
			//{
			//	SetEntProp(i, Prop_Data, "m_CollisionGroup", COLLISION_GROUP_PUSHAWAY);
			//}
			if (input == false)
			{
				SetEntData(i, g_offsCollisionGroup, 5, 4, true); // BRING BACK COLLISION
			}
		}
	}
}
void PyroExplode(float vec1[3])
{
	int color[4]={188,220,255,200};
	EmitSoundFromOrigin("ambient/explosions/explode_8.mp3", vec1);
	TE_SetupExplosion(vec1, g_ExplosionSprite, 10.0, 1, 0, 0, 750); // 600
	TE_SendToAll();
	TE_SetupBeamRingPoint(vec1, 10.0, 500.0, white, g_HaloSprite, 0, 10, 0.6, 10.0, 0.5, color, 10, 0);
	TE_SendToAll();
}
stock void GetEntityAbsOrigin(int entity, float origin[3])
{
	//decl Float:mins[3], Float:maxs[3];
	GetEntPropVector(entity,Prop_Send,"m_vecOrigin",origin);
/*	GetEntPropVector(entity,Prop_Send,"m_vecMins",mins);
	GetEntPropVector(entity,Prop_Send,"m_vecMaxs",maxs);
	origin[0] += (mins[0] + maxs[0]) * 0.5;
	origin[1] += (mins[1] + maxs[1]) * 0.5;
	origin[2] += (mins[2] + maxs[2]) * 0.5; */
}
void ExplosionDamage(float origin[3])
{
	int maxplayers = MaxClients;
	
	float PlayerVec[3];
	float distance;
	for (int i = 1; i <= maxplayers; i++)
	{
		if( !IsClientInGame(i) || !IsPlayerAlive(i) )
		{
			continue;
		}
		GetClientAbsOrigin(i, PlayerVec);
		
		distance = GetVectorDistance(origin, PlayerVec, true);
		if(distance > 100000.0)
		{
			continue;
		}
		
		int dmg = RoundFloat(100000.0 - distance) / 50;
		int health = GetClientHealth(i) - dmg;
		
		if (health > 0)
		{
			SetEntityHealth(i,health);
		}
		if (health <= 0)
		{
			ForcePlayerSuicide(i);
		}
	}
}

void EmitSoundFromOrigin(const char[] sound,const float orig[3])
{
	EmitSoundToAll(sound,SOUND_FROM_WORLD,SNDCHAN_AUTO,SNDLEVEL_NORMAL,SND_NOFLAGS,SNDVOL_NORMAL,SNDPITCH_NORMAL,-1,orig,NULL_VECTOR,true,0.0);
}
void SetClientClass(int i, char tfclass[128])
{
	if (IsValidClient(i) && (IsPlayerAlive(i) && IsClientParticipating(i)))
	{
		TFClassType input = TFClass_Scout;
		if (StrEqual(tfclass,"soldier")) input = TFClass_Soldier;
		if (StrEqual(tfclass,"spy")) input = TFClass_Spy;
		if (StrEqual(tfclass,"demoman")) input = TFClass_DemoMan;
		if (StrEqual(tfclass,"sniper")) input = TFClass_Sniper;
		if (StrEqual(tfclass,"medic")) input = TFClass_Medic;
		if (StrEqual(tfclass,"engineer")) input = TFClass_Engineer;
		if (StrEqual(tfclass,"heavy")) input = TFClass_Heavy;
		if (StrEqual(tfclass,"pyro")) input = TFClass_Pyro;
		if (IsValidClient(i)){
			if (TF2_GetPlayerClass(i) != input)
			{
				TF2_SetPlayerClass(i, input);
				SetVariantString("");
				AcceptEntityInput(i, "SetCustomModel");
				RequestFrame(resupplyclient, i);
			}
			if(TF2_IsPlayerInCondition(i, TFCond_Taunting))
			{
				TF2_RemoveCondition(i, TFCond_Taunting);
			}
		}
	}
}
public void resupplyclient(int i)
{
	if (IsValidClient(i))
	{
		TF2_RegeneratePlayer(i);
	}
}

void SetClientClassJouer(int i, char tfclass[128])
{
	if (IsValidClient(i) && (IsPlayerAlive(i) && IsValidTeam(i)))
	{
		TFClassType input = TFClass_Scout;
		if (StrEqual(tfclass,"soldier")) input = TFClass_Soldier;
		if (StrEqual(tfclass,"spy")) input = TFClass_Spy;
		if (StrEqual(tfclass,"demoman")) input = TFClass_DemoMan;
		if (StrEqual(tfclass,"sniper")) input = TFClass_Sniper;
		if (StrEqual(tfclass,"medic")) input = TFClass_Medic;
		if (StrEqual(tfclass,"engineer")) input = TFClass_Engineer;
		if (StrEqual(tfclass,"heavy")) input = TFClass_Heavy;
		if (StrEqual(tfclass,"pyro")) input = TFClass_Pyro;
		if (IsValidClient(i))
		{
			if (TF2_GetPlayerClass(i) != input)
			{
				TF2_SetPlayerClass(i, input);
				SetVariantString("");
				AcceptEntityInput(i, "SetCustomModel");
				float pos[3];
				float vel[3];
				float ang[3];
				GetClientAbsAngles(i, pos);
				GetClientEyeAngles(i, ang);
				GetEntPropVector(i, Prop_Data, "m_vecVelocity", vel);
				//HandlePlayerItems(i);
			}
			HealClient(i);		
		}
	}
}

void HealClient(int i)
{
	TFClassType class = TF2_GetPlayerClass(i);
	int health = 125;
	if (class == TFClass_Soldier)
	{
		health = 200;
	}
	if (class == TFClass_Pyro)
	{
		health = 175;
	}
	if (class == TFClass_DemoMan)
	{
		health = 175;
	}
	if (class == TFClass_Heavy)
	{
		health = 300;
	}
	if (class == TFClass_Medic)
	{
		health = 150;
	}
	
	SetEntityHealth(i, health);
}

stock void ClientParticle(int client, char effect[128], float time)
{
	float strflVec[3];
	GetClientEyePosition(client, strflVec);
	
	int strIParticle = CreateEntityByName("info_particle_system");
	char strName[128];
	if (IsValidEdict(strIParticle))
	{
		float strflPos[3];
		GetEntPropVector(client, Prop_Send, "m_vecOrigin", strflPos);
		TeleportEntity(strIParticle, strflPos, NULL_VECTOR, NULL_VECTOR);
		
		Format(strName, sizeof(strName), "target%i", client);
		DispatchKeyValue(client, "targetname", strName);
		
		DispatchKeyValue(strIParticle, "targetname", "tf2particle");
		DispatchKeyValue(strIParticle, "parentname", strName);
		DispatchKeyValue(strIParticle, "effect_name", effect);
		DispatchSpawn(strIParticle);
		SetVariantString(strName);
		AcceptEntityInput(strIParticle, "SetParent", strIParticle, strIParticle, 0);
		//SetVariantString("head");
		//AcceptEntityInput(strIParticle, "SetParentAttachment", strIParticle, strIParticle, 0);
		ActivateEntity(strIParticle);
		AcceptEntityInput(strIParticle, "start");
		
		CreateTimer(time, killprop_timer, strIParticle);
	}
}
public Action killprop_timer(Handle hTimer, any prop)
{
	if (IsValidEntity(prop))
	{
		AcceptEntityInput(prop, "Kill" );
	}
	return Plugin_Stop;
}
stock int GetActivePlayers(int team = 0, bool mustbealive = false)
{
	int output = 0;
	for (int client = 1; client <= MaxClients; client++)
	{
		if (IsValidClient(client) && g_Spawned[client])
		{
			if (((team == 0 && GetClientTeam(client) >= 2) || (team > 0 && GetClientTeam(client) == team)) && (mustbealive == false || IsPlayerAlive(client)) && g_Participating[client] == true) output += 1;
		}
	}
	return output;
}

stock bool IsPlayerTaunting(int client)
{
	int pcond = GetEntProp(client, Prop_Send, "m_nPlayerCond");
	if (pcond & TF2_PLAYER_TAUNTING) 
	{
		return true;
	}
	return false;
}  


public void RemoveClientWeapons(int i)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Removing all client weapons");
	}
	RemoveFakeWeapon(i);
	if (IsValidClient(i) && (IsPlayerAlive(i)) && (g_Winner[i] == 0))
	{
		SetClientSlot(i, 0);
		for (int j=0; j<=5; j++)
		{
			TF2_RemoveWeaponSlot(i, j);
		}
		SetWeaponState(i, false);
	}
}
public void SetWeaponAmmo(int client, int slot1, int slot2)
{
    // Replaced FindSendPropOffs with FindSendPropInfo
    int offset1 = FindSendPropInfo("CTFPlayer", "m_hActiveWeapon");
    int ActiveWeapon = GetEntDataEnt2(client, offset1);
    if(IsValidEntity(ActiveWeapon))
    {
        int offset2 = FindSendPropInfo("CBaseCombatWeapon", "m_iClip1");
        SetEntData(ActiveWeapon, offset2, slot1, 4);
        
        int offset3 = FindSendPropInfo("CTFPlayer", "m_iAmmo");
        SetEntData(client, offset3 + 4, slot2, 4);
        SetEntData(client, offset3 + 8, slot2, 4);
    }
}

public Action SetWeaponState(int client, bool input)
{
    TFClassType class = TF2_GetPlayerClass(client);
	// FIXED: Replace deprecated FindSendPropOffs
    int offset = FindSendPropInfo("CTFPlayer", "m_hActiveWeapon");
    int ActiveWeapon = GetEntDataEnt2(client, offset);
    int iEntity = g_PlayerVisibleWeapon[client];
	if (IsValidEntity(ActiveWeapon))
	{
		if (input == true)
		{
			
			SetEntityRenderColor(ActiveWeapon, 255, 255, 255, 255);
			SetEntityRenderMode(ActiveWeapon, RENDER_NORMAL);
			SetEntProp(client, Prop_Send, "m_bDrawViewmodel", 1);
			g_attack = true;
		}
		else
		{
			
			SetEntityRenderColor(ActiveWeapon, 255, 255, 255, 255);
			SetEntityRenderMode(ActiveWeapon, RENDER_NORMAL);
			SetWeaponAmmo(client, 0, 0);
			SetEntProp(client, Prop_Send, "m_bDrawViewmodel", 1);
			if (class == TFClass_Heavy)
			{
				TF2_RemoveWeaponSlot(client, 2);
				giveitem(client, 331);
				SetClientSlot(client, 2);
			}
			if (class == TFClass_Engineer)
			{
				int metal = 1;
				if(metal < 0) metal = 0;
				if(metal > 1) metal = 1;
				SetEntProp(client, Prop_Data, "m_iAmmo", metal, 4, 3);
			}
			g_attack = false;
		}
	}
	if (iEntity > 0 && IsValidEntity(iEntity))
	{
		if (input == true)
		{
			SetEntityRenderColor(iEntity, 255, 255, 255, 255);
			SetEntityRenderMode(iEntity, RENDER_NORMAL);
		}
		else
		{
			SetEntityRenderColor(iEntity, 255, 255, 255, 0);
			SetEntityRenderMode(iEntity, RENDER_TRANSCOLOR);
		}
	}
	
	return Plugin_Continue;
}

public Action GiveClientWeapon(int client, int slot, bool input)
{
	if (IsValidClient(client) && IsPlayerAlive(client))
	{	
		RemoveAllWeapons();		   
		if (input == true)
		{
			TFClassType class = TF2_GetPlayerClass(client);
			if (class == TFClass_Scout) giveitem(client, 939);
			if (class == TFClass_Soldier) giveitem(client, 939);
			if (class == TFClass_Pyro) giveitem(client, 939);
			if (class == TFClass_DemoMan) giveitem(client, 939);
			if (class == TFClass_Heavy) giveitem(client, 939);
			if (class == TFClass_Engineer) giveitem(client, 939);
			if (class == TFClass_Medic) giveitem(client, 939);
			if (class == TFClass_Sniper) giveitem(client, 939);
			if (class == TFClass_Spy) giveitem(client, 939);	
		}	
		RemoveFakeWeapon(client);
		SetWeaponState(client, input);
	}
	
	return Plugin_Continue;
}

void RemoveFakeWeapon(int &iClient)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Removing all client weapons");
	}
	int iEntity = g_PlayerVisibleWeapon[iClient];
	g_PlayerVisibleWeapon[iClient] = -1;
	
	if (iEntity > 0 && IsValidEdict(iEntity))
	{
		RemoveEdict(iEntity);
	}
}

public Action RemoveClientSlot(int i, int slot)
{
	if (IsValidClient(i) && (IsPlayerAlive(i)))
	{
		TF2_RemoveWeaponSlot(i, slot);
	}
	
	return Plugin_Continue;
}

public Action RemoveAllWeapons()
{
	if (ww_log.BoolValue)
	{
		LogMessage("Removing all weapons");
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && (IsPlayerAlive(i) && IsValidTeam(i)))
		{
			RemoveClientWeapons(i);
		}
	}
	
	return Plugin_Continue;
}

public void DisableClientWeapons(int client)
{
	if (IsValidClient(client) && (IsPlayerAlive(client) && IsValidTeam(client)))
	{
		RemoveClientWeapons(client);
		
		if (SpecialRound == 6 && (!(StrEqual(minigame, "SimonSays") && simon_action == 0)))
		{
			giveitem(client, 660);
			SetClientSlot(client, 2);
		}
	
		else
		{
			TFClassType class = TF2_GetPlayerClass(client);
			if (class == TFClass_Scout) giveitem(client, 939);
			if (class == TFClass_Soldier) giveitem(client, 939);
			if (class == TFClass_Pyro) giveitem(client, 939);
			if (class == TFClass_DemoMan) giveitem(client, 939);
			if (class == TFClass_Heavy) giveitem(client, 939);
			if (class == TFClass_Engineer) giveitem(client, 939);
			if (class == TFClass_Medic) giveitem(client, 939);
			if (class == TFClass_Sniper) giveitem(client, 939);
			if (class == TFClass_Spy) giveitem(client, 939);
			SetClientSlot(client, 2);
			SetWeaponState(client, false);
			g_attack = false;
		}
	}
}

// This function is meant to handle player wearables.
// Ideally it would call some kind of way to re-add a player's wearables, but until we find a way to do that, removing them will do.
public Action HandlePlayerItems(int iClient)
{
	//RemoveHeadgear(iClient);
	//RemoveDemoShield(iClient);
	RemovePowerupBottle(iClient);	
	SetEntProp(iClient, Prop_Send, "m_nBody", 0);
	
	return Plugin_Continue;
}
/*
	RemoveHeadgear(iClient)
	{
		if (!IsValidClient(iClient))
		{
			return;
		}
		if (!IsPlayerAlive(iClient))
		{
			return;
		}

		new iEntity = -1;
		while ((iEntity = FindEntityByClassname2(iEntity, "tf_wearable")) != -1)
		{
			if (IsItemEntity(iEntity))
			{
				RemoveItemEntity(iClient, iEntity);
			}
		}
	}

	RemoveDemoShield(iClient)
	{
		if (!IsValidClient(iClient))
		{
			return;
		}
		if (!IsPlayerAlive(iClient))
		{
			return;
		}

		new iEntity = -1;
		while ((iEntity = FindEntityByClassname2(iEntity, "tf_wearable_demoshield")) != -1)
		{
			if (IsItemEntity(iEntity))
			{
				RemoveItemEntity(iClient, iEntity);
			}
		}
	}
	*/
public Action SetRandomClass(int client)
{
	RandomClass = GetRandomInt(0,8);
	if (IsValidClient(client) && (IsPlayerAlive(client) && IsValidTeam(client) ))
	{
		if (RandomClass == 0)
		{		
			SetClientClassJouer(client, "scout");
		}
		if (RandomClass == 1)
		{
			SetClientClassJouer(client, "soldier");
		}
		if (RandomClass == 2)
		{
			SetClientClassJouer(client, "pyro");  
		}
		if (RandomClass == 3)
		{
			SetClientClassJouer(client, "demoman");
		}
		if (RandomClass == 4)
		{
			SetClientClassJouer(client, "heavy"); 
		}
		if (RandomClass == 5)
		{
			SetClientClassJouer(client, "engineer"); 
		}
		if (RandomClass == 6)
		{
			SetClientClassJouer(client, "medic");
		}
		if (RandomClass == 7)
		{
			SetClientClassJouer(client, "sniper");	
		}
		if (RandomClass == 8)
		{
			SetClientClassJouer(client, "spy");	 
		}
	}
	
	return Plugin_Continue;
}
void RemovePowerupBottle(int iClient)
{
	if (!IsValidClient(iClient))
	{
		return;
	}
	if (!IsPlayerAlive(iClient))
	{
		return;
	}
	int iEntity = -1;
	while ((iEntity = FindEntityByClassname2(iEntity, "tf_powerup_bottle")) != -1)
	{
		if (IsItemEntity(iEntity))
		{
			RemoveItemEntity(iClient, iEntity);
		}
	}
}
stock void RemoveItemEntity(int iClient, int iEntity)
{
	if (IsValidClient(iClient) && IsItemEntity(iEntity))
	{
		if (GetEntPropEnt(iEntity, Prop_Send, "m_hOwnerEntity") == iClient)
		{
			SDKCall(g_hSdkRemoveWearable, iClient, iEntity);			
		}
		RemoveEdict(iEntity);
	}
}

stock bool IsItemEntity(int iEntity)
{
	if (iEntity > 0)
	{
		if (IsValidEdict(iEntity))
		{
			char strClassname[32];
			GetEdictClassname(iEntity, strClassname, sizeof(strClassname));
			if (StrEqual(strClassname, "tf_wearable", false))
			{
				return true;
			}
			if (StrEqual(strClassname, "tf_wearable_demoshield", false))
			{
				return true;
			}
			if (StrEqual(strClassname, "tf_usableitem", false))
			{
				return true;
			}
			if (StrEqual(strClassname, "tf_powerup_bottle", false))
			{
				return true;			
			}
		}
	}
	return false;
}

Handle GameTextHandle = INVALID_HANDLE;
void ShowGameText(const char[] strMessage, const char[] strIcon = "leaderboard_dominated", float life = 35.0)
{
	if (GameTextHandle != INVALID_HANDLE)
	{
		TriggerTimer(GameTextHandle);
	}
	int iEntity = CreateEntityByName("game_text_tf");
	DispatchKeyValue(iEntity,"message", strMessage);
	DispatchKeyValue(iEntity,"display_to_team", "0");
	DispatchKeyValue(iEntity,"icon", strIcon);
	DispatchKeyValue(iEntity,"targetname", "game_text1");
	DispatchKeyValue(iEntity,"background", "0");
	DispatchSpawn(iEntity);
	AcceptEntityInput(iEntity, "Display", iEntity, iEntity);
	GameTextHandle = CreateTimer(life, KillGameText, EntIndexToEntRef(iEntity));
}

public Action KillGameText(Handle hTimer, any iEntityRef)
{
	int iEntity = view_as<int>(EntRefToEntIndex(iEntityRef));
	if ((iEntity > 0) && IsValidEntity(iEntity))
	{
		AcceptEntityInput(iEntity, "kill");
	}
	
	GameTextHandle = INVALID_HANDLE;
	
	return Plugin_Stop;
}

stock int ChangeClientTeamAlive(int client, int team)
{
	int currentteam = GetClientTeam(client);
	if((currentteam  == 1) || (currentteam == 0))return false;
	SetEntProp(client, Prop_Send, "m_lifeState", 2);
	ChangeClientTeam(client, team);
	SetEntProp(client, Prop_Send, "m_lifeState", 0);
	return true;
}


public int GetHealtarget(int client)
{
	if(!IsValidClient(client) || TF2_GetPlayerClass(client) != TFClass_Medic)
	{
		return 0;
	}
	
	int weapon = GetPlayerWeaponSlot(client, 1);
	if (weapon == -1)
	{
		return 0;
	}
		
	char classname[64];
	GetEdictClassname(weapon, classname, 64);
	if(!StrEqual(classname, "tf_weapon_medigun"))
	{
		return 0;
	}
	
	return GetEntDataEnt2(weapon, FindSendPropInfo("CWeaponMedigun", "m_hHealingTarget"));
}

int GetClientMaxHealth(int iClient)
{
	return GetEntProp(GetPlayerResourceEntity(), Prop_Send, "m_iMaxHealth", _, iClient); 
}

stock int FindEntityByClassname2(int startEnt, const char[] classname)
{
	/* If startEnt isn't valid shifting it back to the nearest valid one */
	while (startEnt > -1 && !IsValidEntity(startEnt))
	{
		startEnt--;
	}
	return FindEntityByClassname(startEnt, classname);
}

stock void CreateAllSprites()
{
	for (int client = 1; client <= MaxClients; client++)
	{
		if (IsValidClient(client))
		{
			CreateSprite(client);
		}
	}
}


stock void CreateSprite(int client)
{
	if (IsValidClient(client))
	{
		DestroySprite(client);
		if (IsPlayerAlive(client) && g_Points[client] > 0)
		{
			
			// Sprite name
			int points = g_Points[client];
			if (points > 19)
			{
				points = 19;
			}
			if (g_Winner[client] > 0)
			{
				points = 99;
			}
			char sprite[256];
			Format(sprite, sizeof(sprite), "materials/ww_overlays/tf2ware_points%d.vmt", points);
			
			// Client ent
			char szTemp[64]; 
			Format(szTemp, sizeof(szTemp), "client%i", client);
			DispatchKeyValue(client, "targetname", szTemp);
			float vOrigin[3];
			GetClientAbsOrigin(client, vOrigin);
				
			vOrigin[2] += 100;
			
			int ent = CreateEntityByName("env_sprite_oriented");
			if (IsValidEntity(ent))
			{			 
				DispatchKeyValue(ent, "classname", "env_sprite_oriented");
				DispatchKeyValue(ent, "spawnflags", "1");
				DispatchKeyValue(ent, "framerate", "1");
				DispatchKeyValue(ent, "scale", "0.2");
				DispatchKeyValue(ent, "rendermode", "1");
				DispatchKeyValue(ent, "rendercolor", "255 255 255");
				DispatchKeyValue(ent, "model", sprite);
				DispatchKeyValue(ent, "parentname", szTemp);
				SetVariantString("ATTACH_NORMAL");
				SetVariantString("!activator");
				//AcceptEntityInput(ent, "SetParentAttachmentMaintainOffset", client, ent, 0); 
				DispatchSpawn(ent);
				TeleportEntity(ent, vOrigin, NULL_VECTOR, NULL_VECTOR);
				
				
				g_Sprites[client] = ent;
			}
		}
	}
}

stock void SetSpriteParent(int ent, int client)
{
    SetVariantString("!activator");
    AcceptEntityInput(ent, "SetParentAttachmentMaintainOffset", client, ent, 1);
}

stock void DestroyAllSprites()
{
	for (int client = 1; client <= MaxClients; client++)
	{
		if (IsValidClient(client))
		{
			DestroySprite(client);
		}
	}
}

stock void DestroySprite(int client)
{
	if (IsValidClient(client))
	{
		int ent = g_Sprites[client];
		if ((g_Sprites[client] > 0) && IsValidEntity(ent))
		{			
			AcceptEntityInput(ent, "kill");
			g_Sprites[client] = 0;
		}
	}
}
public Action updatePosition(Handle timer)
{
	int ent; float vOrigin[3]; float vVelocity[3];
	for(int i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i))
		{
			continue;
		}
		if ((ent = g_Sprites[i]) > 0)
		{
			if (!IsValidEntity(ent))
			{
				g_Sprites[i] = 0;
			}
			else
			{
				if ((ent = EntRefToEntIndex(ent)) > 0)
				{
					GetClientEyePosition(i, vOrigin);
					vOrigin[2] += 25.0;
					GetEntDataVector(i, gVelocityOffset, vVelocity);
					TeleportEntity(ent, vOrigin, NULL_VECTOR, vVelocity);
				}
			}
		}
	}
	return Plugin_Continue;
}
stock void TF2_RemoveAllWearables(int client)
{
	int i = -1;
	while ((i = FindEntityByClassname(i, "tf_wearable*")) != -1)
	{
		if (client != GetEntPropEnt(i, Prop_Send, "m_hOwnerEntity")) continue;
		AcceptEntityInput(i, "Kill");
	}
}

void AddEntityToClient(int client, int iEntity)
{
    if(iEntity > MaxClients)
    {
        if(g_PlayerData[client].g_hPlayerEntities == INVALID_HANDLE) // Correto
        {
            g_PlayerData[client].g_hPlayerEntities = CreateArray(); // Correto
        }
        PushArrayCell(g_PlayerData[client].g_hPlayerEntities, EntIndexToEntRef(iEntity)); // Correto
    }
}
void DeleteParticleWinner(any particle)
{
	if (IsValidEntity(particle))
	{
		char classname[256];
		GetEdictClassname(particle, classname, sizeof(classname));
		if (StrEqual(classname, "info_particle_system", false))
		{
			RemoveEdict(particle);
		}
	}
}
int AttachParticleWin(int iEntity, const char[] strParticleEffect, const char[] strAttachPoint="", float flOffsetZ=0.0)
{
	int iParticle = CreateEntityByName("info_particle_system");
	if(iParticle > MaxClients && IsValidEntity(iParticle))
	{
		float flPos[3];
		GetEntPropVector(iEntity, Prop_Send, "m_vecOrigin", flPos);
		flPos[2] += flOffsetZ;
		
		TeleportEntity(iParticle, flPos, NULL_VECTOR, NULL_VECTOR);
		
		DispatchKeyValue(iParticle, "effect_name", strParticleEffect);
		DispatchSpawn(iParticle);
		
		SetVariantString("!activator");
		AcceptEntityInput(iParticle, "SetParent", iEntity);
		ActivateEntity(iParticle);
		
		if(strlen(strAttachPoint))
		{
			SetVariantString(strAttachPoint);
			AcceptEntityInput(iParticle, "SetParentAttachmentMaintainOffset");
		}		
		AcceptEntityInput(iParticle, "start");
		g_Ent[iEntity] = iParticle;
		return iParticle;
	}	
	return 0;
}
stock void StartLaser(float start[3], float end[3])
{
	int ent = CreateEntityByName("env_beam");
	//new String:strClassname[50];;
	if (ent != -1)
	{
		TeleportEntity(ent, start, NULL_VECTOR, NULL_VECTOR);
		SetEntityModel(ent, "sprites/laser.vmt");
		SetEntPropVector(ent, Prop_Data, "m_vecEndPos", end);
		DispatchKeyValue(ent, "targetname", "beam");
		DispatchKeyValue(ent, "rendercolor", "255 255 0");
		DispatchKeyValue(ent, "renderamt", "100");
		DispatchSpawn(ent);
		SetEntPropFloat(ent, Prop_Data, "m_fWidth", 7.0); 
		SetEntPropFloat(ent, Prop_Data, "m_fEndWidth", 7.0);
		ActivateEntity(ent);
		AcceptEntityInput(ent, "TurnOn");
	}
	//GetEdictClassname(ent, strClassname, sizeof(strClassname));
	if (StrEqual(minigame, "PushBall_Game"))
	{
		CreateTimer(18.5, KillLimboBeam, ent);
	}
	if (StrEqual(minigame, "Spycrab_Limbo"))
	{
		CreateTimer(10.5, KillLimboBeam, ent);
	}
}
void ForceGrapplingHook(int client)
{
	if (IsValidClient(client))
	{
		giveitem(client, 1152);
	}
}

void ForceFlamePyro(int client)
{
	if (IsValidClient(client))
	{
		giveitem(client, 659);
	}
}

void ForceScorchShotPyro(int client)
{
	if (IsValidClient(client))
	{
		giveitem(client, 740);
	}
}

void ForceHotHand(int client)
{
	if (IsValidClient(client))
	{
		giveitem(client, 1181);
	}
}

void ForceSpyPDA(int client)
{
	if (IsValidClient(client))
	{
		giveitem(client, 27);
	}
}

public void ResizeThings(int i)
{
	if (IsValidClient(i))
	{
		SetEntPropFloat(i, Prop_Send, "m_flModelScale", 0.7);
	}
}
public Action RemoveOv(Handle timer, any client)
{
	for(int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i)) {
			SetOverlay(i, "");
		}
	}
	
	return Plugin_Continue;
}
stock void DestroyBuildings()
{
	char strObjects[3][] = {"obj_sentrygun","obj_dispenser","obj_teleporter"};
	for( int o = 0; o < sizeof(strObjects); o++ ){
		int iEnt = -1;
		while( ( iEnt = FindEntityByClassname( iEnt, strObjects[o] ) ) != -1 )
		if( IsValidEdict(iEnt) )
		{
			SetEntityHealth( iEnt, 100 );
			SetVariantInt( 1488 );
			AcceptEntityInput( iEnt, "RemoveHealth" );
		}
	}
}
stock int CreateParticle(int iEntity, char[] sParticle, bool bAttach = false, float time)
{
	int iParticle = CreateEntityByName("info_particle_system");
	if (IsValidEdict(iParticle))
	{
		float fPosition[3];
		GetEntPropVector(iEntity, Prop_Send, "m_vecOrigin", fPosition);
		
		TeleportEntity(iParticle, fPosition, NULL_VECTOR, NULL_VECTOR);
		DispatchKeyValue(iParticle, "effect_name", sParticle);
		
		if (bAttach)
		{
			SetVariantString("!activator");
			AcceptEntityInput(iParticle, "SetParent", iEntity, iParticle, 0);			
		}

		DispatchSpawn(iParticle);
		ActivateEntity(iParticle);
		AcceptEntityInput(iParticle, "Start");
		CreateTimer(time, DeleteParticle, iParticle);
	}
	return iParticle;
}
public Action DeleteParticle(Handle timer, any particle)
{
	if (IsValidEntity(particle))
	{
		char classN[64];
		GetEdictClassname(particle, classN, sizeof(classN));
		if (StrEqual(classN, "info_particle_system", false))
		{
			RemoveEdict(particle);
		}
	}
	
	return Plugin_Continue;
}
public void RemoveClientWeaponsSpy(int i)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Removing all client weapons");
	}
	RemoveFakeWeapon(i);
	if (IsValidClient(i) && (IsPlayerAlive(i)) && (g_Winner[i] == 0))
	{
		SetClientSlot(i, 0);
		for (int j=0; j<=2; j++)
		{
			TF2_RemoveWeaponSlot(i, j);
		}
		SetWeaponState(i, false);
	}
}
int GetRandomPlayerTeam(int team)
{
	int clients[MAXPLAYERS+1]; int clientCount;
	for (int i = 1; i <= MaxClients; i++)
		if (IsClientInGame(i) && GetClientTeam(i) == team && IsClientParticipating(i))
		clients[clientCount++] = i;
	return (clientCount == 0) ? -1 : clients[GetRandomInt(0, clientCount-1)];
}
public void Remove_Ragdoll(int client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
}
void ClientExplode(int client, float vec1[3])
{
	int color[4]={188,220,255,200};
	PrecacheSound("mvm/sentrybuster/mvm_sentrybuster_explode.wav",true);
	if(IsValidClient(client)){
		EmitSoundToAll("mvm/sentrybuster/mvm_sentrybuster_explode.wav", SOUND_FROM_PLAYER, SNDCHAN_STATIC);
	}
	TE_SetupExplosion(vec1, g_ExplosionSprite, 10.0, 1, 0, 0, 750);
	TE_SendToAll();
	TE_SetupBeamRingPoint(vec1, 10.0, 500.0, white, g_HaloSprite, 0, 10, 0.6, 10.0, 0.5, color, 10, 0);
	TE_SendToAll();
}
int GetPlayerCountTeam(int team)
{
    int players_team;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsValidClient(i) && IsPlayerAlive(i) && GetClientTeam(i) == team && IsClientParticipating(i))
            players_team++;
    }
    return players_team;
}
int GetPlayersAlive()
{
    int players_total;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsValidClient(i) && IsPlayerAlive(i) && GetClientTeam(i) >= 2 && IsClientParticipating(i))
            players_total++;
    }
    return players_total;
}
public Action OnSceneStart(int entity)
{
	int client = GetEntPropEnt(entity, Prop_Data, "m_hOwner");
	char scenefile[128];
	GetEntPropString(entity, Prop_Data, "m_iszSceneFile", scenefile, sizeof(scenefile));
	if(StrEqual(minigame,"StreetFighter")){
		if (StrEqual(scenefile, "scenes/player/pyro/low/taunt02.vcd")){
			if ((GetEntityFlags(client) & FL_ONGROUND) ) 
			{ 
				if (!IsValidClient(client)) return Plugin_Continue;
				if (!IsPlayerAlive(client)) return Plugin_Continue;
				if (TF2_GetPlayerClass(client) != TFClass_Pyro) return Plugin_Continue;
				if (TF2_IsPlayerInCondition(client, TFCond_Taunting)) return Plugin_Continue;
				CreateTimer(2.0, FireBall, client);
				CreateTimer(0.1, AddCond, client);
			}
		}
	}
	if((StrEqual(minigame,"SimonSays")) || (StrEqual(minigame,"PickupPlate"))){
		if (StrEqual(scenefile, "scenes/player/heavy/low/taunt04.vcd")){
			if ((GetEntityFlags(client) & FL_ONGROUND) ) 
			{
				if (!IsValidClient(client)) return Plugin_Continue;
				if (!IsPlayerAlive(client)) return Plugin_Continue;
				if (TF2_GetPlayerClass(client) != TFClass_Heavy) return Plugin_Continue;
				if (TF2_IsPlayerInCondition(client, TFCond_Taunting)) return Plugin_Continue;
				sandwich[client] = true;
			}
		}
	}
	return Plugin_Continue;
}
stock int GetRandomPlayer()
{
	int[] clients = new int[MaxClients];
	int clientCount;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i) && (GetClientTeam(i) > 1))
		{
			clients[clientCount++] = i;
		}
	}
	return (clientCount == 0) ? -1 : clients[GetRandomInt(0, clientCount-1)];
}

stock int GetItemDefIndex(int iItem) {
	return GetEntProp(iItem, Prop_Send, "m_iItemDefinitionIndex");
}
stock void MoveForward(const float vPos[3], const float vAng[3], float vReturn[3], float fDistance)
{
    float vDir[3];
    GetAngleVectors(vAng, vDir, NULL_VECTOR, NULL_VECTOR);
    vReturn = vPos;
    vReturn[0] += vDir[0] * fDistance;
    vReturn[1] += vDir[1] * fDistance;
}