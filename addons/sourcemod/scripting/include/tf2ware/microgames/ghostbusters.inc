// Ghostbusters module

#define GHOSTBUSTERS_MEDIC 0
#define GHOSTBUSTERS_SPY 1 
#define GHOSTBUSTERS_HEAVY 2

#define GHOSTBUSTERS_SPY_MODEL "models/props_halloween/ghost.mdl" 
#define GHOSTSLAUGH "sound/vo/..."
#define MUSIC_GHOSTB "gamesound/tf2ware/minigame_25_alt.mp3"

bool Ghostbusters_pp[MAXPLAYERS+1] = {false};
int Ghostbusters_target[MAXPLAYERS+1] = {0};
int Ghostbusters_hurt[MAXPLAYERS+1] = {0};
int Ghostbusters_hp[MAXPLAYERS+1] = {0};
bool Ghostbusters_ended = false;

public void Ghostbusters_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Ghostbusters_OnMapStart);
}	

public void Ghostbusters_OnMapStart() {
	PrecacheSound(GHOSTSLAUGH, true);
	precacheSound(MUSIC_GHOSTB);
	PrecacheModel(GHOSTBUSTERS_SPY_MODEL, true);
	PrecacheModel( "models/weapons/...", true);
}

public void Ghostbusters_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Ghostbusters_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Ghostbusters_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Ghostbusters_OnGameFrame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Ghostbusters_PlayerDeath);
	
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
	SetConVarInt(FindConVar("mp_autoteambalance"), 0);
	
	SetStateAll(true);
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsValidTeam(i)){
			OldTeam[i] = GetClientTeam(i);
		}
	}	
	
	for (int i = 1; i <= MaxClients; i++) {
		g_Mission[i] = GHOSTBUSTERS_SPY;
	}

	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i))
		SetEntProp(i, Prop_Send, "m_bGlowEnabled", 1);
	}	
	
	int AllPlayers = GetClientCount(true);
	int target = 0;
	int attempts = 0;
	float fmedics = float(GetActivePlayers()) / 3.5;
	int medics = RoundToCeil(fmedics);
	int heavies = GetActivePlayers() / 4;
	if (medics <= 1) medics = 2;
	if (medics >= 8) medics = 7;
	if (heavies <= 0) heavies = 1;
	if (heavies >= 4) heavies = 3;
	int medicclients[MAXPLAYERS+1];
	int spyclients[MAXPLAYERS+1];
	int heavyclients[MAXPLAYERS+1];
	
	// Get the medics!
	for (int i = 1; i <= medics; i++) {
		attempts = 0;
		target = GetRandomInt(1, AllPlayers);
		while (((!IsValidClient(target)) || IsClientParticipating(target) == false || IsFakeClient(target) || g_Mission[target] != GHOSTBUSTERS_SPY) && attempts <= 50) {
			target = GetRandomInt(1, AllPlayers);
			attempts += 1;
		}
		if (attempts <= 50) {
			g_Mission[target] = GHOSTBUSTERS_MEDIC;
		}
	}
	
	// Get the heavies!
	for (int i = 1; i <= heavies; i++) {
		attempts = 0;
		target = GetRandomInt(1, AllPlayers);
		while (((!IsValidClient(target)) || IsClientParticipating(target) == false || g_Mission[target] != GHOSTBUSTERS_SPY) && attempts <= 50) {
			target = GetRandomInt(1, AllPlayers);
			attempts += 1;
		}
		if (attempts <= 50) {
			g_Mission[target] = GHOSTBUSTERS_HEAVY;
		}
	}
	
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			if (g_Mission[i] == GHOSTBUSTERS_SPY) {
				SetStateClient(i, true);
				spyclients[i-1] = i;
				TF2_RemoveAllWearables(i);
			}			
			if (g_Mission[i] == GHOSTBUSTERS_MEDIC) {
				SetStateClient(i, false);
				medicclients[i-1] = i;
			}
			if (g_Mission[i] == GHOSTBUSTERS_HEAVY) {
				SetStateClient(i, true);
				heavyclients[i-1] = i;
			}			 
		}
	}
	
	AddRandVocalization("ghostslaugh", spyclients, 2.0, 4.0, SNDLEVEL_NORMAL);
	
	g_attack = true;
	Ghostbusters_ended = false;
}

public void Ghostbusters_End() {
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
	SetConVarInt(FindConVar("mp_autoteambalance"), 1);
	
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, Ghostbusters_Damage);
		if (IsValidClient(i) && IsClientParticipating(i)) {
			SDKUnhook(i, SDKHook_PreThink, SDKHooks_SpeedClient);
			StopSound(i, SND_CHANNEL_SPECIFIC, MUSIC_GHOSTB);
			if (IsPlayerAlive(i)) {
				Ghostbusters_FixSlap(i);
				SetEntityRenderMode(i, RENDER_NORMAL);
				SetEntityRenderColor(i, 255, 255, 255, 255);
				
				int default_fov = GetEntProp(i, Prop_Send, "m_iDefaultFOV"); 
				SetEntProp(i, Prop_Send, "m_iFOV", default_fov);
			}
			
			if (IsValidClient(i) && IsClientParticipating(i)) {
				SetVariantString("");
				AcceptEntityInput(i, "SetCustomModel");
				SetVariantInt(0);
				AcceptEntityInput(i, "SetCustomModelRotates");
				ChangeClientTeamAlive(i, OldTeam[i]);
			}			
		}
		CreateTimer(1.0, Timer_StopParticles, i, TIMER_FLAG_NO_MAPCHANGE);	
	}
	RespawnAll();
}

int Ghostbusters_Pos[][] = {
	{4584, -6177, 283, -90},
	{5454, -6982, 283, 127},
	{7818, -5809, 283, -155},
	{5091, -6524, 699, 49},
	{5899, -5131, 91, -51},
	{7061, -5766, 555, -161},
	{6792, -6408, 283, 176},
	{7887, -5564, 555, -134},
	{5025, -5759, 555, -37},
	{4430, -6694, 283, 73},
	{5450, -7109, 283, 142},
	{4628, -6701, 283, -49},
	{7533, -6969, 283, 170},
	{6479, -6868, 323, -15},
	{5877, -7137, 283, 93},
	{5893, -6554, 283, -180},
	{5880, -6262, 91, 179},
	{6049, -5584, 91, 3},
	{7093, -6974, 283, -53},
	{7864, -5853, 548, -141}
};

public void Ghostbusters_justEntered(int client) {
	if (IsValidClient(client)) {   
		float pos[3];
		float ang[3];
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		Ghostbusters_hp[client] = GetClientHealth(client);
		
		if (g_Mission[client] == GHOSTBUSTERS_SPY) {
			if (GetClientTeam(client) == 2) ChangeClientTeamAlive(client, 3);
			SetClientClass(client, "spy");			
			RemoveClientWeapons(client);

			SetVariantString(GHOSTBUSTERS_SPY_MODEL);
			AcceptEntityInput(client, "SetCustomModel");
			SetVariantInt(1);
			AcceptEntityInput(client, "SetCustomModelRotates");			   

			CreateTimer(0.1, Ghostbusters_Spy_Ragdoll, client);
			
			int id = Ghostbusters_GetGroupId(client, g_Mission[client]) -1;
			
			if (id >= sizeof(Ghostbusters_Pos)) id = 0;
		
			pos[0] = float(Ghostbusters_Pos[id][0]);
			pos[1] = float(Ghostbusters_Pos[id][1]);
			pos[2] = float(Ghostbusters_Pos[id][2]);
			ang[0] = 0.0;
			ang[1] = float(Ghostbusters_Pos[id][3]);
			ang[2] = 0.0;
			
			SetEntityRenderMode(client, RENDER_TRANSCOLOR);
			SetEntityRenderColor(client, 255, 255, 255, 60);
			SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 280.0);
			TF2_RemoveAllWearables(client);			
		}
		if (g_Mission[client] == GHOSTBUSTERS_MEDIC) {
			if (GetClientTeam(client) == 3) ChangeClientTeamAlive(client, 3);
			SetClientClass(client, "medic");		
			RemoveClientWeapons(client);
			giveitem(client, 663);
			pos[0] = 5973.0;
			pos[1] = -5230.0;
			pos[2] = 348.0;
			ang[0] = 0.0;
			ang[1] = -92.0;
			ang[2] = 0.0;
			SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 300.0);			
		}
		if (g_Mission[client] == GHOSTBUSTERS_HEAVY) {
			if (GetClientTeam(client) == 3) ChangeClientTeamAlive(client, 2);
			SetClientClass(client, "heavy");			
			RemoveClientWeapons(client);
			giveitem(client, 5);
			pos[0] = 7680.0;
			pos[1] = -5933.0;
			pos[2] = 555.0;
			ang[0] = 0.0;
			ang[1] = -149.0;
			ang[2] = 0.0;
			SetEntityRenderColor(client, 0, 255, 0, 255);
			SDKHook(client, SDKHook_PreThink, SDKHooks_SpeedClient);			
		}
		
		TeleportEntity(client, pos, ang, vel);
		SetEntProp(client, Prop_Send, "m_bGlowEnabled", 1);
		Ghostbusters_pp[client] = false;
		Ghostbusters_target[client] = 0;
		SDKHook(client, SDKHook_OnTakeDamage, Ghostbusters_Damage);
	}
}

int Ghostbusters_GetGroupId(int client, int id) {
	int out = 0;
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			if (g_Mission[i] == id) out += 1;
			if (i == client) return out;
		}
	}
	return 1;
}

public void Ghostbusters_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			if (GetClientTeam(i) == 2 && g_Mission[i] != GHOSTBUSTERS_HEAVY) ChangeClientTeamAlive(i, 3);
			if (GetClientTeam(i) == 3 && g_Mission[i] == GHOSTBUSTERS_HEAVY) ChangeClientTeamAlive(i, 2);
		
			if (g_Mission[i] == GHOSTBUSTERS_MEDIC) {
				int target = GetHealtarget(i);
				if (IsValidClient(target) && IsPlayerAlive(target) && g_Mission[target] != GHOSTBUSTERS_MEDIC) {
					Ghostbusters_SetPP(i, true);
					Ghostbusters_SetPP(target, true);
					Ghostbusters_target[i] = target;
					
					if (Ghostbusters_hurt[target] <= 0) {
						Ghostbusters_Slap(i);
						//Ghostbusters_Slap(target);
						int default_fov = GetEntProp(target, Prop_Send, "m_iDefaultFOV"); 
						int health = Ghostbusters_hp[target];
						int new_fov = default_fov + RoundFloat(float(125 - health)*0.75);
						SetEntProp(target, Prop_Send, "m_iFOV", new_fov);
					
						int newhealth = health - 3;
						
						if (newhealth > 0)
						{
							SetEntityHealth(target, newhealth);
							Ghostbusters_hp[target] = newhealth;
						}
						else {
							Ghostbusters_SetPP(target, false);
							ForcePlayerSuicide(target);
						}
						Ghostbusters_hurt[target] = 6;
					}
					else Ghostbusters_hurt[target] -= 1;
				}
				else {
					Ghostbusters_SetPP(i, false);
					int target2 = Ghostbusters_target[i];
					if (target2 != 0) {
						if (IsValidClient(target2)) Ghostbusters_SetPP(target2, false);
						Ghostbusters_target[i] = 0;
					}
				}
			}
		}
	}
}

public void Ghostbusters_PlayerDeath(int client) {
	SetStateClient(client, false, true);
	Ghostbusters_SetPP(client, false);
	SetVariantString("");
	AcceptEntityInput(client, "SetCustomModel");
	SetVariantInt(0);
	AcceptEntityInput(client, "SetCustomModelRotates");
	CreateTimer(0.0, Ghostbusters_Spy_Ragdoll, client);	   
	CreateTimer(0.1, Ghostbusters_Check);
}

public Action Ghostbusters_Spy_Ragdoll(Handle timer, any client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}

public Action Timer_StopParticles(Handle timer, any client)
{
	if(!IsValidClient(client))
		return Plugin_Continue;

	SetVariantString("ParticleEffectStop");
	AcceptEntityInput(client, "DispatchEffect");
	CreateTimer(1.0, Timer_StopParticles, client, TIMER_FLAG_NO_MAPCHANGE);
	return Plugin_Continue;
}

public Action Ghostbusters_Check(Handle timer) {
	if (status != 2) return Plugin_Continue;
	if (!StrEqual(minigame, "Ghostbusters")) return Plugin_Continue;
	if (Ghostbusters_ended == true) return Plugin_Continue;

	// Did the medics win?
	bool complete_medic = true;
	bool complete_spy = true;
	int winner = 0;
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == GHOSTBUSTERS_SPY) && IsPlayerAlive(i)) complete_medic = false;
		if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == GHOSTBUSTERS_MEDIC) && IsPlayerAlive(i)) complete_spy = false;
	}
	
	if (complete_spy) winner = 2;
	if (complete_medic) winner = 1;
	if (winner > 0) {
		Ghostbusters_ended = true;
		if (winner == 1) CPrintToChatAll("{lightgreen}All ghost spies are dead... {green}surviving medics win!");
		if (winner == 2) CPrintToChatAll("{lightgreen}All medics are dead... {green}surviving undeads win!");
		for (int i = 1; i <= MaxClients; i++) {
			if (winner == 1) {
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == GHOSTBUSTERS_MEDIC) && IsPlayerAlive(i)) SetStateClient(i, true, true);
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] != GHOSTBUSTERS_MEDIC)) {
					SetStateClient(i, false, true);
					if (IsPlayerAlive(i)) ForcePlayerSuicide(i);
				}
			}
			if (winner == 2) {
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] != GHOSTBUSTERS_MEDIC) && IsPlayerAlive(i)) SetStateClient(i, true, true);
				if (IsValidClient(i) && IsClientParticipating(i) && (g_Mission[i] == GHOSTBUSTERS_MEDIC)) {
					SetStateClient(i, false, true);
					if (IsPlayerAlive(i)) ForcePlayerSuicide(i);
				}
			}
		}
		CreateTimer(3.0, Ghostbusters_TEnd);
	}
	
	return Plugin_Continue;
}

public Action Ghostbusters_TEnd(Handle timer) {
	if (status != 2) return Plugin_Continue;
	if (!StrEqual(minigame, "Ghostbusters")) return Plugin_Continue;
	
	TriggerTimer(microgametimer);
	
	return Plugin_Continue;
}

public void Ghostbusters_Slap(int client) {
	float fEye[3];
	GetClientEyeAngles(client, fEye);
	fEye[0] += GetRandomFloat(-10.0,10.0) * 0.1;
	fEye[1] += GetRandomFloat(-10.0,10.0) * 0.1;
	fEye[2] += GetRandomFloat(-10.0,10.0) * 0.1;
	TeleportEntity(client, NULL_VECTOR, fEye, NULL_VECTOR);
}

public void Ghostbusters_FixSlap(int client) {
	float fEye[3];
	GetClientEyeAngles(client, fEye);
	fEye[2] = 0.0;
	TeleportEntity(client, NULL_VECTOR, fEye, NULL_VECTOR);
}

public void Ghostbusters_SetPP(int client, bool input) {
	if (IsValidClient(client)) {
		if (input == true && Ghostbusters_pp[client] == false) {
			TF2_SetPlayerPowerPlay(client, true);
			Ghostbusters_pp[client] = true;
			SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 500.0);
		}
		if (input == false && Ghostbusters_pp[client] == true) {
			TF2_SetPlayerPowerPlay(client, false);
			Ghostbusters_pp[client] = false;
			if (g_Mission[client] == GHOSTBUSTERS_SPY) SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 280.0);
			else SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 300.0);
			Ghostbusters_FixSlap(client);
		}
	}
}

public Action Ghostbusters_Damage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(victim) && IsValidClient(attacker) && g_Mission[victim] == GHOSTBUSTERS_SPY && g_Mission[attacker] == GHOSTBUSTERS_HEAVY) {
		damage = 0.0;
		return Plugin_Changed;
	}
	if (IsValidClient(victim) && IsValidClient(attacker) && g_Mission[victim] == GHOSTBUSTERS_MEDIC && g_Mission[attacker] == GHOSTBUSTERS_HEAVY) {
		damage = 125.0;
		return Plugin_Changed;
	}
	if (IsValidClient(victim) && IsValidClient(attacker) && g_Mission[victim] == GHOSTBUSTERS_HEAVY) {
		damage = 0.0;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}