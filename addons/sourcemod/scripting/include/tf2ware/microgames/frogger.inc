// TEMPLATE module

#define FROGGER_MODEL "models/props_2fort/frog.mdl"
int LastJump[MAXPLAYERS+1];
int g_Frogger_Timer = 0;
bool g_Frogger_Goal = false;
int g_Frogger_Pos[MAXPLAYERS+1] = {0};

public void Frogger_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Frogger_OnMapStart);
}

public void Frogger_OnMinigame() {
	// These are forwards if you need a specific function!
	AddToForward(g_justEntered, INVALID_HANDLE, Frogger_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Frogger_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Frogger_OnGameFrame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Frogger_PlayerDeath);
		
	//SetCameraState(true);
	NoCollision(true);
	g_Frogger_Timer = 11;
	g_Frogger_Goal = false;
		
	for (int i = 1; i <= MaxClients; i++) {
		g_Frogger_Pos[i] = 0;
	}
}

public void Frogger_OnMapStart() {
	PrecacheModel(FROGGER_MODEL, true);
	precacheSound("vo/announcer_ends_10sec.mp3");	
	precacheSound("vo/announcer_ends_9sec.mp3");
	precacheSound("vo/announcer_ends_8sec.mp3");
	precacheSound("vo/announcer_ends_7sec.mp3");
	precacheSound("vo/announcer_ends_6sec.mp3");	
	precacheSound("vo/announcer_ends_5sec.mp3");
	precacheSound("vo/announcer_ends_4sec.mp3");
	precacheSound("vo/announcer_ends_3sec.mp3");
	precacheSound("vo/announcer_ends_2sec.mp3");
	precacheSound("vo/announcer_ends_1sec.mp3");
}

public Action Frogger_Timer(Handle hTimer) {
	if (status == 2) {
		g_Frogger_Timer -= 1;
		
		if (g_Frogger_Timer > 0) {
			char sound[512];
			Format(sound, sizeof(sound), "vo/announcer_ends_%dsec.mp3", g_Frogger_Timer);
			if (GetConVarBool(ww_music) == true) EmitSoundToClient(1, sound);
			else EmitSoundToAll(sound);
			if (g_Frogger_Timer < 5) CPrintToChatAll("{red}%d{default}!", g_Frogger_Timer);
			CreateTimer(1.0, Frogger_Timer);
		}
		else {
			if (microgametimer != INVALID_HANDLE) TriggerTimer(microgametimer);
		}
	}
	return Plugin_Handled;
}

public void Frogger_End() {
	// Let's turn them back
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SetVariantString("");
			AcceptEntityInput(i, "SetCustomModel");
			SetVariantInt(0);
			AcceptEntityInput(i, "SetCustomModelRotates");
			TpDisabled(i);
		}
	}
}

public void Frogger_justEntered(int client) {
	if (IsValidClient(client)) {
		// Change the client's class?
		SetClientClass(client, "medic");
		DisableClientWeapons(client);
			
		// Turn him into a frog!
		SetVariantString(FROGGER_MODEL);
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(1);
		AcceptEntityInput(client, "SetCustomModelRotates");
		//SetEntProp(client, Prop_Send, "m_iObserverMode", 1);
		TF2_RemoveAllWearables(client);
		Frogger_Position(client);
		NoCollision(true);
		
		CreateTimer(0.1, Frogger_Ragdoll, client);
		TpEnabled(client);
	}
}

public void Frogger_Position(int client) {
	float vel[3];
	vel[0] = 0.0;
	vel[1] = 0.0;
	vel[2] = 0.0;
	
	float ang[3];
	ang[0] = 0.0;
	ang[1] = 90.0;
	ang[2] = 0.0;
		
	float pos[3]; 
	
	int checkpoint = g_Frogger_Pos[client];
	if (checkpoint == 99) {
		pos[0] = 11449.0;
		pos[1] = 1126.0;
		pos[2] = 2139.0;
	}
	if (checkpoint == 8) {
		pos[0] = 12364.0;
		pos[1] = 174.0;
		pos[2] = 1981.0;
		ang[1] = 0.0;
	}
	if (checkpoint == 7) {
		pos[0] = 11767.0;
		pos[1] = 148.0;
		pos[2] = 2030.0;
		ang[1] = 0.0;
	}
	if (checkpoint == 6) {
		pos[0] = 10892.0;
		pos[1] = 175.0;
		pos[2] = 2005.0;
		ang[1] = 0.0;
	}
	if (checkpoint == 5) {
		pos[0] = 11415.0;
		pos[1] = -826.0;
		pos[2] = 1975.0;
	}
	if (checkpoint == 4) {
		pos[0] = 11549.0;
		pos[1] = -1621.0;
		pos[2] = 1875.0;
	}
	if (checkpoint == 3) {
		pos[0] = 11489.0;
		pos[1] = -2313.0;
		pos[2] = 1894.0;
	}
	if (checkpoint == 2) {
		pos[0] = 11458.0;
		pos[1] = -3177.0;
		pos[2] = 1859.0;
	}
	if (checkpoint == 1) {

		pos[0] = 11502.0;
		pos[1] = -4611.0;
		pos[2] = 1836.0;
	}
	if (checkpoint == 0) {
		int columnm = client;
		int rowm = 0;
		while (columnm > 7) {
			columnm = columnm - 7;
			rowm = rowm + 1;
		}
	
		pos[0] = 11320.0  + float(columnm*55);
		pos[1] = -6302.0 + float(rowm*55);
		pos[2] = 1869.0;
	}
	TeleportEntity(client, pos, ang, vel);
	TpEnabled(client);
}

public void Frogger_PlayerDeath(int client) {
	if (IsValidClient(client) && status == 2) {
		SetVariantString("");
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(0);
		AcceptEntityInput(client, "SetCustomModelRotates");
		SetEntProp(client, Prop_Send, "m_iObserverMode", 0);
		if (IsValidClient(client)){
			//EmitSoundToClient(client, FROGGER_DEAD, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			EmitSoundToClient(client, FROGGER_DEAD);
		}	
		CreateTimer(1.0, Frogger_Respawn, client);
		
		CreateTimer(0.0, Frogger_Ragdoll, client);
	}
}

public Action Frogger_Ragdoll(Handle timer, any client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}

public Action Frogger_Respawn(Handle hTimer, any client) {
    if (status == 2) {
        RespawnClient(client, true, false);
        TF2_RemoveAllWearables(client);		
    }
    return Plugin_Handled;
}

public void Frogger_OnGameFrame() {
	if (status != 2) return;
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			if (GetClientButtons(i) & IN_JUMP) {
				PlaySoundFroggerJump(i);
			}
			float pos[3];
			GetClientAbsOrigin(i, pos);
			
			// checkpoints
			if (pos[1] >= -4611.0 && g_Frogger_Pos[i] < 1) g_Frogger_Pos[i] = 1;
			if (pos[1] >= -3177.0 && g_Frogger_Pos[i] < 2) g_Frogger_Pos[i] = 2;
			if (pos[1] >= -2313.0 && g_Frogger_Pos[i] < 3) g_Frogger_Pos[i] = 3;
			if (pos[1] >= -1566.0 && g_Frogger_Pos[i] < 4) g_Frogger_Pos[i] = 4;
			if (pos[1] >= -826.0 && g_Frogger_Pos[i] < 5) g_Frogger_Pos[i] = 5;
			if (pos[1] >= 53.0 && pos[0] >= 10683.0 && g_Frogger_Pos[i] < 6) g_Frogger_Pos[i] = 6;
			if (pos[1] >= 30.0 && pos[0] >= 11720.0 && g_Frogger_Pos[i] < 7) g_Frogger_Pos[i] = 7;
			if (pos[1] >= 350.0 && pos[0] >= 12186.0 && g_Frogger_Pos[i] < 8) g_Frogger_Pos[i] = 8;
			
			// goal
			if (pos[1] > 995.0 && g_Complete[i] == false ) {
				bool bWin = true;
				if (g_Gamemode == GAMEMODE_WIPEOUT && g_Frogger_Goal) bWin = false;
				
				if (bWin) {
					SetStateClient(i, true, true);
					float FinalFrogLoc[3] = {2035.366211, 2603.637695, 588.160889};
					float FinalFrogAng[3] = {0.0, 180.0, 0.0};
					TeleportEntity(i, FinalFrogLoc, FinalFrogAng, NULL_VECTOR);
					
					CPrintToChatAllEx(i, "{teamcolor}%N{default} made it to the goal first!", i);
					g_Frogger_Pos[i] = 99;
					if (!g_Frogger_Goal) {
						if (g_Gamemode == GAMEMODE_WIPEOUT) {
							CreateTimer(0.1, Frogger_Timer);
							g_Frogger_Timer = 0;
						} else {
							CreateTimer(0.0, Frogger_Timer);
							CPrintToChatAll("The boss will end in {red}10 seconds{default}!");
						}
					}
					g_Frogger_Goal = true;
				}
			}
		}
	}
}
public void PlaySoundFroggerJump(int client){
	int CheckTime = GetTime();
	if (CheckTime - LastJump[client] < 1)return;
	LastJump[client] = GetTime();
	//EmitSoundToClient(client, FROGGER_JUMP, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
	EmitSoundToClient(client, FROGGER_JUMP);
} 