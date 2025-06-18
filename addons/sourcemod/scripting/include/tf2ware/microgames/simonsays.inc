// SimonSays module
//#define TF_CONDFLAG_CHARGING		  (1 << 17)


#define A_TAUNT 0
#define A_JUMP 1
#define A_CROUCH 2
#define A_MEDIC 3
#define A_SANDWICH 4
#define A_INSPECT 5


// simon_action == 0 | Taunt
// simon_action == 1 | Jump
// simon_action == 2 | Crouch
// simon_action == 3 | Look Up
// simon_action == 4 | Look Down

// NEED TO ALLOW BUILDINGS

int simon_action;
int simon_or_someone;

public void SimonSays_OnMinigame() {
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, SimonSays_OnGameFrame);
	AddToForward(g_justEntered, INVALID_HANDLE, SimonSays_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SimonSays_End);
	AddCommandListener(MainListener_Voice, "voicemenu");
	inspect = false;
	simon_action = GetRandomInt(0,5);
	simon_or_someone = GetRandomInt(0,1);
	tauntvalueold = TauntDisable.BoolValue;
	if(TauntDisable.BoolValue){
		SetConVarBool(TauntDisable,false);
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			g_Mission[i] = simon_action * 2 + simon_or_someone;
			sandwich[i] = false;
		}
	}
	
	// If simon says it (simon_or_someone == 0) set state to false until proven true
	// Otherwise, set to true until proven false
	SetStateAll(simon_or_someone == 1);
}

public void SimonSays_OnGameFrame() {	
	if (timeleft <= 6) {
		for (int i = 1; i <= MaxClients; i++) {
			// If the player is valid and taunting
			if(IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				if (IsPlayerDoing(i)) {
					if (simon_or_someone == 0) {
					// If simon says it
						SetStateClient(i, true, true);
					}
					else if (g_Complete[i]) {
					// Otherwise, simon didn't say it and if we still haven't lost
						char input[512];
						char message[256];
						char missionmessage[256];
						// Display "Simon didn't say it!"
						Format(input, sizeof(input), "tf2ware_minigame_%d_20", GetMinigameConfNum("SimonSays", "id"));
						SetOverlay(i, input);
						Format(missionmessage, sizeof(missionmessage), "INSTRUCTION_%d_20", GetMinigameConfNum("SimonSays", "id"), LANG_SERVER);
						Format(message, sizeof(message), "%t", missionmessage, LANG_SERVER);
						PrintToHud(i, message);
						SetStateClient(i, false, true);
					}
				}
			}
		}
	}
}
public void SimonSays_justEntered(int client) {
	if (IsValidClient(client)){
		//SDKHook(i, SDKHook_OnTakeDamage, NoDamage_OnTakeDamageClient);
		g_iMedic[client] = false;
		g_iInspect[client] = false;
		if(simon_action == 4){	
			SetClientClass(client, "heavy");
			g_attack = true;
			TF2_RemoveWeaponSlot(client, 0);
			giveitem(client, 331);
			giveitem(client, 42);
			SetEntityHealth(client, 30);	
		}
		if(simon_action == 5){	
			inspect = true;	
		}
	} 
}
int IsPlayerDoing(int client) {
	switch (simon_action) {
		case A_TAUNT: {
			if (TF2_IsPlayerInCondition(client, TFCond_Taunting)) {
				return true;
			}
		}
		case A_JUMP: {
			if (GetClientButtons(client) & IN_JUMP) {
				return true;
			}
		}
		case A_CROUCH: {
			if (GetClientButtons(client) & IN_DUCK) {
				return true;
			}
		}
		case A_MEDIC: {
			if(g_iMedic[client] && g_iMedic[client] == true){
				return true;
			}
		}
		case A_SANDWICH: {
			if ((TF2_IsPlayerInCondition(client, TFCond_Taunting)) && (sandwich[client])) {
				return true;
			}
		}
		case A_INSPECT: {
			if (inspect && g_iInspect[client] && g_iInspect[client] == true) {
				return true;
			}
		}		
	}
	return false;
}

public Action MainListener_Voice(int client, const char[] command, int argc)
{
	char cmd1[32];
	char cmd2[32];
	if(IsValidClient(client)){
		if(argc < 2) return Plugin_Handled;

		GetCmdArg(1, cmd1, sizeof(cmd1));
		GetCmdArg(2, cmd2, sizeof(cmd2));
	  
		// Capture call for medic commands (represented by "voicemenu 0 0").
		if(StrEqual(cmd1, "0") && StrEqual(cmd2, "0") && IsPlayerAlive(client))
		{
			g_iMedic[client] = true;
			return Plugin_Continue;
		}
		else
		g_iMedic[client] = false;
		if(StrEqual(cmd1, "2") && StrEqual(cmd2, "2") && IsPlayerAlive(client))
		{
			g_iCheers[client] = true;
			return Plugin_Continue;
		}
		else
		g_iCheers[client] = false;
	}
	return Plugin_Continue;
}
public Action OnClientCommandKeyValues(int client, KeyValues kv)
{
    char szBuffer[64];
    kv.GetSectionName(szBuffer, sizeof(szBuffer));
    if (StrEqual(szBuffer, "+inspect_server", false) && inspect){
		g_iInspect[client] = true;
	}    
    return Plugin_Continue;
}
public void SimonSays_End() {
	if(tauntvalueold == 1){
		SetConVarBool(TauntDisable,true);
	}
	if(simon_action == 4){
		for (int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i) && IsClientParticipating(i)) {
				sandwich[i] = false;
			}
		}
	}
	if(simon_action == 5){
		inspect = false;
	}
	tauntvalueold = -1;
	g_attack = false;
}