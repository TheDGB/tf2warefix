// Slender module added by Benoist3012 as special round boss game
//Credits to slender fortress for the page system and slender model and sound

#define SLENDER_SURVIVOR 0
#define SLENDER_SLENDERMAN 1

#define SLENDER_SPY_MODEL "models/arrival/slenderman.mdl" 
#define PAGEGRAB "slender/newgrabpage.wav"
#define MUSIC_INTRO "slender/intro.mp3"
#define MUSIC_SLENDER "gamesound/tf2ware/minigame_50.mp3"
#define PAGE_MODEL "models/slender/sheet.mdl"

//#define MUSIC_GOTPAGES1_SOUND "slender/newambience_1.wav"
//#define MUSIC_GOTPAGES2_SOUND "slender/newambience_2.wav"
//#define MUSIC_GOTPAGES3_SOUND "slender/newambience_3.wav"
//#define MUSIC_GOTPAGES4_SOUND "slender/newambience_4.wav"

bool Slender_ended = false;
Handle g_hRoundTimerSync;
Handle g_hRoundTimer;
Handle g_hMusicTimer;
//new Float:g_flSpeed[MAXPLAYERS];
//new Float:g_flSound[MAXPLAYERS];
int g_iRoundTime;
int g_iPageMax;
int g_iPageCount;
int g_iEntFog;
int Slender_PagePos[][] =
{
	{4466, -6426, 286, 272},
	{4496, -5993, 286, 270},
	{4871, -6017, 342, 180},
	
	{5432, -7156, 242, 180},
	{4760, -6644, 286, 180},
	{5560, -6876, 193, 180},
	
	{5772, -6737, 278, 270},
	{6015, -6117, 131, 180},
	{5929, -5684, 111, 0},

	{4890, -5918, 558, 0},
	{5558, -6544, 674, 180},
	{5558, -5808, 516, 180},
	
	{6066, -6322, 502, 0},
	{6038, -6356, 572, 180},
	{5976, -5974, 570, 90},
	
	{7528, -5592, 338, 94},
	{7960, -5532, 558, 185},
	{7978, -5860, 548, 6},
	
	{7142, -6384, 272, 0},
	{8690, -6426, 478, 180},
	{7402, -5784, 276, 271},
	
	{7096, -6802, 268, 276},
	{6628, -7218, 282, 4}
};

public void Slender_Init()
{
	AddToForward(g_OnMapStart, INVALID_HANDLE, Slender_OnMapStart);
	g_hRoundTimerSync = CreateHudSynchronizer();
}	

public void Slender_OnMapStart()
{
	//precacheSound(MUSIC_GOTPAGES1_SOUND);
	//precacheSound(MUSIC_GOTPAGES2_SOUND);
	//precacheSound(MUSIC_GOTPAGES3_SOUND);
	//precacheSound(MUSIC_GOTPAGES4_SOUND);
	precacheSound(MUSIC_SLENDER);
	precacheSound(PAGEGRAB);
	precacheSound(MUSIC_INTRO);
	PrecacheModel(SLENDER_SPY_MODEL, true);
	PrecacheModel(PAGE_MODEL, true);
	PrecacheModel( "models/weapons/...", true);
}

public void Slender_OnMinigame()
{
	AddCommandListener(SlenderListener_Voice, "voicemenu");
	int iEnt = -1;
	while((iEnt = FindEntityByClassname(iEnt, "*")) != -1 && iEnt < 2048)
	{
		AcceptEntityInput(iEnt,"TurnOff");
	}
	CreateFog();
	/*iEnt = -1;
	while((iEnt = FindEntityByClassname(iEnt, "light")) != -1)
	{
		AcceptEntityInput(iEnt,"Kill");
	}*/
	AddToForward(g_justEntered, INVALID_HANDLE, Slender_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Slender_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Slender_OnGameFrame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Slender_PlayerDeath);
	
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
	SetConVarInt(FindConVar("mp_autoteambalance"), 0);
	
	SetStateAll(true);	
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsValidTeam(i)){
			OldTeam[i] = GetClientTeam(i);
		}
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		g_Mission[i] = SLENDER_SURVIVOR;
	}	
	
	int AllPlayers = GetClientCount(true);
	int target = 0;
	int attempts = 0;
	float fslender = float(GetActivePlayers()) / 8.0;
	int slender = RoundToCeil(fslender);
	if (slender <= 0) slender = 1;
	if (slender >= 3) slender = 2;
	
	// Get the slender!
	for (int i = 1; i <= slender; i++)
	{
		attempts = 0;
		target = GetRandomInt(1, AllPlayers);
		while (((!IsValidClient(target)) || IsClientParticipating(target) == false || IsFakeClient(target) || g_Mission[target] != SLENDER_SURVIVOR) && attempts <= 50)
		{
			target = GetRandomInt(1, AllPlayers);
			attempts += 1;
		}
		if (attempts <= 50)
		{
			g_Mission[target] = SLENDER_SLENDERMAN;
		}
	}
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && IsClientParticipating(i))
		{
			if (g_Mission[i] == SLENDER_SLENDERMAN)
			{
				SetStateClient(i, true);
				TF2_RemoveAllWearables(i);
			}			
			if (g_Mission[i] == SLENDER_SURVIVOR)
			{
				SetStateClient(i, false);
			}		 
		}
	}
	for (int a = 1; a <= 8; a++)
	{
		int random;
		if(a==1)
		{
			random = GetRandomInt(0,2);
		}
		if(a==2)
		{
			random = GetRandomInt(3,5);
		}
		if(a==3)
		{
			random = GetRandomInt(6,8);
		}
		if(a==4)
		{
			random = GetRandomInt(9,11);
		}
		if(a==5)
		{
			random = GetRandomInt(12,14);
		}
		if(a==6)
		{
			random = GetRandomInt(15,17);
		}
		if(a==7)
		{
			random = GetRandomInt(18,20);
		}
		if(a==8)
		{
			random = GetRandomInt(21,22);
		}
		float pos[3];
		float ang[3];
		pos[0] = float(Slender_PagePos[random][0]);
		pos[1] = float(Slender_PagePos[random][1]);
		pos[2] = float(Slender_PagePos[random][2]);
		int page = CreateEntityByName("prop_dynamic_override");
		ang[0]=0.0;
		ang[1]=float(Slender_PagePos[random][3]);
		ang[2]=0.0;
		if (page != -1)
		{
			TeleportEntity(page, pos, ang, NULL_VECTOR);
			DispatchKeyValue(page, "targetname", "tf2ware_page");
			SetEntityModel(page, PAGE_MODEL);
			
			DispatchKeyValue(page, "solid", "2");
			DispatchSpawn(page);
			ActivateEntity(page);
			a-=1;
			SetVariantInt(a);
			AcceptEntityInput(page, "Skin");
			a+=1;
			AcceptEntityInput(page, "EnableCollision");
			SetEntPropFloat(page, Prop_Send, "m_flModelScale", 1.1);
			SDKHook(page, SDKHook_OnTakeDamage, Hook_PageOnTakeDamage);
			//SDKHook(page, SDKHook_SetTransmit, Hook_SlenderObjectSetTransmit);
		}
		int ent = CreateEntityByName("tf_taunt_prop");
		if (ent != -1)
		{
			float flModelScale = GetEntPropFloat(page, Prop_Send, "m_flModelScale");
	
			SetEntityModel(ent, PAGE_MODEL);
			DispatchSpawn(ent);
			ActivateEntity(ent);
			SetEntityRenderMode(ent, RENDER_TRANSCOLOR);
			SetEntityRenderColor(ent, 0, 0, 0, 0);
			SetEntProp(ent, Prop_Send, "m_bGlowEnabled", 1);
			SetEntPropFloat(ent, Prop_Send, "m_flModelScale", flModelScale);
	
			int iFlags = GetEntProp(ent, Prop_Send, "m_fEffects");
			SetEntProp(ent, Prop_Send, "m_fEffects", iFlags | (1 << 0));
		
			SetVariantString("!activator");
			AcceptEntityInput(ent, "SetParent", page);
			SDKHook(ent, SDKHook_SetTransmit, GlowPage);
		}
		g_iPageMax = a;
		g_iPageCount = 0;
	}
	g_attack = true;
	Slender_ended = false;
	g_iRoundTime = RoundToCeil(GetMinigameConfFloat(minigame, "duration"));
	g_iRoundTime -= 12*8; 
	g_hRoundTimer = CreateTimer(1.0, Timer_RoundTime, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)){
			g_hMusicTimer = CreateTimer(28.5, Timer_SlenderMusic, i, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
		}
	}
}

public void Slender_End()
{
	int iEntTimer = -1;
	while((iEntTimer = FindEntityByClassname(iEntTimer, "team_round_timer")) != -1){
		fStartTime = GetTimeRemaining(iEntTimer);
	}
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
	SetConVarInt(FindConVar("mp_autoteambalance"), 1);
	ServerCommand("mp_restartgame 1");
	AcceptEntityInput(g_iEntFog,"Kill");
	char sTargetName[64];
	int iFog = FindEntityByClassname(-1, "env_fog_controller");
	if (iFog != -1)
	{
		GetEntPropString(iFog, Prop_Data, "m_iName", sTargetName, sizeof(sTargetName));
		
		if (sTargetName[0] == '\0')
		{
			strcopy(sTargetName, sizeof(sTargetName), "OriginalMapFog");
			DispatchKeyValue(iFog, "targetname", sTargetName);
		}
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		SDKUnhook(i, SDKHook_OnTakeDamage, Slender_Damage);
		SDKUnhook(i, SDKHook_PreThink, SDKHooks_SlenderOnPreThink);
		if (IsClientInGame(i))
		{
			SetVariantString(sTargetName);
			AcceptEntityInput(i, "SetFogController");
        }
		if (IsValidClient(i) && IsClientParticipating(i))
		{
			SetEntPropFloat(i, Prop_Send, "m_flModelScale", 1.0 );
			//StopSound(i, SND_CHANNEL_SPECIFIC, MUSIC_SLENDER);
			if (IsPlayerAlive(i))
			{
				Ghostbusters_FixSlap(i);
				SetEntityRenderMode(i, RENDER_NORMAL);
				SetEntityRenderColor(i, 255, 255, 255, 255);
				
				int default_fov = GetEntProp(i, Prop_Send, "m_iDefaultFOV"); 
				SetEntProp(i, Prop_Send, "m_iFOV", default_fov);
			}
			
			if (IsValidClient(i) && IsClientParticipating(i))
			{
				SetVariantString("");
				AcceptEntityInput(i, "SetCustomModel");
				SetVariantInt(0);
				AcceptEntityInput(i, "SetCustomModelRotates");			
			}
			ChangeClientTeamAlive(i, OldTeam[i]);
			TF2_StunPlayer(i, 0.0, 0.0, TF_STUNFLAG_SLOWDOWN);
		}
		if (IsValidClient(i)){
			StopSound(i, SND_CHANNEL_SPECIFIC, MUSIC_SLENDER);
		}
	}
	Slender_ended = true;
	RespawnAll();
	int iEnt = -1;
	while((iEnt = FindEntityByClassname(iEnt, "tf_taunt_prop")) != -1 && iEnt < 2048)
	{
		AcceptEntityInput(iEnt,"Kill");
	}
	iEnt = -1;
	while((iEnt = FindEntityByClassname(iEnt, "prop_dynamic_override")) != -1 && iEnt < 2048)
	{
		char name[64];
		GetEntPropString(iEnt, Prop_Data, "m_iName", name, sizeof(name));
		if(strcmp(name, "tf2ware_page") == 0)
		{
			AcceptEntityInput(iEnt,"Kill");
		}
	}
	if(g_hRoundTimer != INVALID_HANDLE)
	{
		//CloseHandle(g_hRoundTimer);
		g_iRoundTime = 0;
		g_hRoundTimer = INVALID_HANDLE;
	}
	CreateTimer(2.0,SetOldMapTime);
}
public Action SetOldMapTime(Handle timer)
{
	//new MapTime = GetConVarInt(FindConVar("mp_timelimit"));
	//new Float:fTotalTime=(GetGameTime() -fStartTime);
	//new value = RoundToCeil(fTotalTime);
	//new NewValue = (MapTime-value);
	int NewValue = fStartTime;
	ServerCommand("mp_timelimit %i",NewValue);
	if(NewValue <=0)
	{
		ServerCommand("mp_timelimit 1");
	}
	
	return Plugin_Continue;
}

public void Slender_justEntered(int client)
{
	if (IsValidClient(client))
	{   
		float pos[3];
		float ang[3];
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		if (g_Mission[client] == SLENDER_SLENDERMAN)
		{
			if (GetClientTeam(client) == 2) ChangeClientTeamAlive(client, 3);
			SetClientClass(client, "spy");			
			RemoveClientWeapons(client);
			giveitem(client, 727);
			SetVariantString(SLENDER_SPY_MODEL);
			AcceptEntityInput(client, "SetCustomModel");
			SetVariantInt(1);
			AcceptEntityInput(client, "SetCustomModelRotates");			   

			CreateTimer(0.1, Slender_Spy_Ragdoll, client);
		
			pos[0] = 5176.0;
			pos[1] = -6473.0;
			pos[2] = 489.0;
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;
			SetEntPropFloat(client, Prop_Send, "m_flModelScale", 1.4 );
			SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 123.0);
			g_flSpeed[client] = 203.0;
		}
		if (g_Mission[client] == SLENDER_SURVIVOR)
		{
			SetVariantString("SlenderFog");
			AcceptEntityInput(client, "SetFogController");
			EmitSoundToClient(client,MUSIC_INTRO);
			if (GetClientTeam(client) == 3) ChangeClientTeamAlive(client, 2);
			SetClientClass(client, "medic");
			pos[0] = 6048.0;
			pos[1] = -5060.0;
			pos[2] = 281.0;
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;
			RemoveClientWeapons(client);
			giveitem(client, 198);
			g_flSpeed[client] = 230.0;			
		}
		TeleportEntity(client, pos, ang, vel);
		SDKHook(client, SDKHook_OnTakeDamage, Slender_Damage);
		SDKHook(client, SDKHook_PreThink, SDKHooks_SlenderOnPreThink);
	}
}
public Action Hook_PageOnTakeDamage(int page, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3], int damagecustom)
{
	if (Slender_ended) return Plugin_Continue;
	
	if (IsValidClient(attacker))
	{
		if (IsClientParticipating(attacker))
		{
			if (damagetype & 0x80) // 0x80 == melee damage
			{
				CollectPage(page, attacker);
			}
		}
	}
	
	return Plugin_Continue;
}
public void CollectPage(int page, int activator)
{
	SetPageCount(g_iPageCount + 1);
	EmitSoundToAll(PAGEGRAB, activator, SNDCHAN_ITEM, SNDLEVEL_SCREAMING);
	
	AcceptEntityInput(page, "FireUser1");
	AcceptEntityInput(page, "Kill");
}
public void SetPageCount(int iNum)
{
	if (iNum > g_iPageMax) iNum = g_iPageMax;
	g_iPageCount = iNum;
	g_iRoundTime+=12;
	//Slenderman get more speed for each page collected
	CreateTimer(0.1, Slender_Check);
	for(int client = 1; client<= MaxClients; client++)
	{
		/*if(IsClientInGame(client) && (g_iPageCount==1 || g_iPageCount==3 || g_iPageCount==5 || g_iPageCount==7))
		{
			StopSound(client, SND_CHANNEL_SPECIFIC, MUSIC_GOTPAGES1_SOUND);
			StopSound(client, SND_CHANNEL_SPECIFIC, MUSIC_GOTPAGES2_SOUND);
			StopSound(client, SND_CHANNEL_SPECIFIC, MUSIC_GOTPAGES3_SOUND);
			StopSound(client, SND_CHANNEL_SPECIFIC, MUSIC_GOTPAGES4_SOUND);
		}*/
		if(IsValidClient(client) && IsPlayerAlive(client) && g_Mission[client] == SLENDER_SLENDERMAN && IsClientParticipating(client))
		{
			g_flSpeed[client]+=5.0;
		}
	}
	/*if(g_iPageCount==1)
	{
		EmitSoundToAll(MUSIC_GOTPAGES1_SOUND);
	}
	if(g_iPageCount==3)
	{
		EmitSoundToAll(MUSIC_GOTPAGES2_SOUND);
	}
	if(g_iPageCount==5)
	{
		EmitSoundToAll(MUSIC_GOTPAGES3_SOUND);
	}
	if(g_iPageCount==7)
	{
		EmitSoundToAll(MUSIC_GOTPAGES4_SOUND);
	}*/
}
public void Slender_OnGameFrame()
{
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i))
		{
			if (GetClientTeam(i) == 3 && g_Mission[i] != SLENDER_SLENDERMAN) ChangeClientTeamAlive(i, 2);
			if (GetClientTeam(i) == 2 && g_Mission[i] == SLENDER_SLENDERMAN) ChangeClientTeamAlive(i, 3);
		
			if (g_Mission[i] == SLENDER_SLENDERMAN)
			{
				float cpos[3];
				float pos[3];
				GetEntPropVector(i, Prop_Data, "m_vecOrigin", pos);
				for(int ii = 1; ii<= MaxClients; ii++)
				{
					if(IsValidClient(ii) && IsPlayerAlive(ii) && g_Mission[ii] == SLENDER_SURVIVOR)
					{
						GetEntPropVector(ii, Prop_Data, "m_vecOrigin", cpos);
						float Dist = GetVectorDistance(pos, cpos);
						if(Dist <= 90.0)
						{
							//PrintToChatAll("Kill!");
							ForcePlayerSuicide(ii);
						}
					}
				}
			}
		}
	}
}

public void Slender_PlayerDeath(int client)
{
	SetStateClient(client, false, true);
	Slender_SetPP(client, false);
	SetVariantString("");
	AcceptEntityInput(client, "SetCustomModel");
	SetVariantInt(0);
	AcceptEntityInput(client, "SetCustomModelRotates");
	CreateTimer(0.0, Slender_Spy_Ragdoll, client);	   
	CreateTimer(0.1, Slender_Check);
}
public void SDKHooks_SlenderOnPreThink(int client)
{
	if(IsValidClient(client) && IsClientParticipating(client) && !Slender_ended) 
	{
		SetSpeed(client, g_flSpeed[client]);
	}
}
public Action Slender_Spy_Ragdoll(Handle timer, any client)
{
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}

public Action Slender_Check(Handle timer)
{
    if (status != 2) return Plugin_Stop;
    if (!StrEqual(minigame, "Slender")) return Plugin_Stop;
    if (Slender_ended) return Plugin_Stop;

	// Did the survivor win?
	bool complete_survivor = true;
	bool complete_survivor_page = false;
	bool complete_slender = true;
	int winner = 0;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SLENDERMAN) && IsPlayerAlive(i)) complete_survivor = false;
		if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SURVIVOR) && IsPlayerAlive(i)) complete_slender = false;
	}
	
	if(g_iPageCount==g_iPageMax)
	{
		complete_survivor_page = true;
	}
	
	if (complete_slender) winner = 2;
	if (complete_survivor) winner = 1;
	if (complete_survivor_page) winner = 3;
	if (winner > 0)
	{
		Slender_ended = true;
		if (winner == 1) CPrintToChatAll("{lightgreen}Slender killed hiself...{green}surviving medics win!");
		if (winner == 2) CPrintToChatAll("{lightgreen}All survivor are dead {green}Slender's team win!");
		if (winner == 3) CPrintToChatAll("{lightgreen}Surivor collected every pages {green}suriving medics win!");
		for (int i = 1; i <= MaxClients; i++)
		{
			if (winner == 1 || winner == 3)
			{
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SURVIVOR) && IsPlayerAlive(i)) SetStateClient(i, true, true);
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SLENDERMAN))
				{
					SetStateClient(i, false, true);
					if (IsPlayerAlive(i)) ForcePlayerSuicide(i);
				}
			}
			if (winner == 2)
			{
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SLENDERMAN) && IsPlayerAlive(i)) SetStateClient(i, true, true);
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SURVIVOR))
				{
					SetStateClient(i, false, true);
					if (IsPlayerAlive(i)) ForcePlayerSuicide(i);
				}
			}
		}
		CreateTimer(3.0, Slender_TEnd);
	}
	
    return Plugin_Stop;
}
public Action Slender_TEnd(Handle timer)
{
    if (status != 2) return Plugin_Stop;
    if (!StrEqual(minigame, "Slender")) return Plugin_Stop;
    
    TriggerTimer(microgametimer);
    return Plugin_Stop;
}
public Action Timer_SlenderMusic(Handle timer)
{
	if (timer != g_hMusicTimer) return Plugin_Stop;
	if (Slender_ended) return Plugin_Stop;
	EmitSoundToAll(MUSIC_SLENDER);
	return Plugin_Continue;
}
public Action Timer_RoundTime(Handle timer)
{
	if (timer != g_hRoundTimer) return Plugin_Stop;
	
	g_iRoundTime--;
	
	int hours; int minutes; int seconds;
	FloatToTimeHMS(float(g_iRoundTime), hours, minutes, seconds);
	
	SetHudTextParams(-1.0, 0.1, 1.0, 127, 167, 141, 255,_,_,1.5, 1.5);
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i) || IsFakeClient(i)) continue;
		ShowSyncHudText(i, g_hRoundTimerSync, "%d/%d\n%d:%02d", g_iPageCount, g_iPageMax, minutes, seconds);
	}
	if(g_iRoundTime==0)
	{
		Slender_ended=true;
		CPrintToChatAll("{lightgreen}Time limit reaches {green} Slender's team win!");
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SLENDERMAN) && IsPlayerAlive(i)) SetStateClient(i, true, true);
			if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == SLENDER_SURVIVOR))
			{
				SetStateClient(i, false, true);
				if (IsPlayerAlive(i)) ForcePlayerSuicide(i);
			}
		}
		CreateTimer(2.5,Slender_TEnd);
		CloseHandle(g_hRoundTimer);
	}
	return Plugin_Continue;
}
stock void FloatToTimeHMS(float time, int &h=0, int &m=0, int &s=0)
{
	s = RoundFloat(time);
	h = s / 3600;
	s -= h * 3600;
	m = s / 60;
	s = s % 60;
}

public void Slender_SetPP(int client, bool input)
{
	if (IsValidClient(client))
	{
		if (input == false)
		{
			TF2_SetPlayerPowerPlay(client, false);
			if (g_Mission[client] == SLENDER_SLENDERMAN) SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 280.0);
			else SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 300.0);
		}
	}
}

public Action Slender_Damage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
	if (IsValidClient(victim) && g_Mission[victim] == SLENDER_SLENDERMAN)
	{
		damage = 0.0;
		return Plugin_Changed;
	}
	if (IsValidClient(victim) && g_Mission[victim] == SLENDER_SURVIVOR)
	{
		damage = 0.0;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

int CreateFog()
{
	int iFog = CreateEntityByName("env_fog_controller");

	if (iFog != -1)
	{
		DispatchKeyValue(iFog, "targetname", "SlenderFog");
		DispatchKeyValue(iFog, "fogenable", "1");
		DispatchKeyValue(iFog, "spawnflags", "1");
		DispatchKeyValue(iFog, "fogblend", "0");
		DispatchKeyValue(iFog, "fogcolor", "0 0 0");
		DispatchKeyValue(iFog, "fogcolor2", "20 20 20");
		DispatchKeyValueFloat(iFog, "fogstart", 100.0);
		DispatchKeyValueFloat(iFog, "fogend", 384.0);
		DispatchKeyValueFloat(iFog, "fogmaxdensity", 1.0);
		DispatchSpawn(iFog);
		
		AcceptEntityInput(iFog, "TurnOn");
	}
	g_iEntFog = iFog;
	return iFog;
}
public Action SlenderListener_Voice(int client, const char[] command, int argc)
{
	if (!Slender_ended){ 
		if(g_Mission[client] == SLENDER_SLENDERMAN)//Block slenderman to use voice command
			return Plugin_Handled;
		else
		return Plugin_Continue;
	}
	return Plugin_Continue;
}
public Action GlowPage(int iEntity,int iClient)
{
	if(IsValidClient(iClient) && g_Mission[iClient] == SLENDER_SLENDERMAN)
		return Plugin_Continue;
	return Plugin_Handled;
}
stock void SetSpeed(int client, float flSpeed)
{
	SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", flSpeed);
}
stock int GetTimeRemaining(int timer)
{
	if (!IsValidEntity(timer))
	{
		return -1;
	}
	
	float flSecondsRemaining;
	
	if (GetEntProp(timer, Prop_Send, "m_bStopWatchTimer") && GetEntProp(timer, Prop_Send, "m_bInCaptureWatchState"))
	{
		flSecondsRemaining = GetEntPropFloat(timer, Prop_Send, "m_flTotalTime");
	}
	else
	{
		if (GetEntProp(timer, Prop_Send, "m_bTimerPaused"))
		{
			flSecondsRemaining = GetEntPropFloat(timer, Prop_Send, "m_flTimeRemaining");
		}
		else
		{
			flSecondsRemaining = GetEntPropFloat(timer, Prop_Send, "m_flTimerEndTime") - GetGameTime();
		}
	}
	
	return RoundFloat(flSecondsRemaining / 60);
}