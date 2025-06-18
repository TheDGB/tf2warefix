public void Spycrab_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Spycrab_justEntered);
	AddToForward(g_OnTimerMinigame, INVALID_HANDLE, Spycrab_TimerMinigame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Spycrab_End);

	SetStateAll(true);
	PrecacheModel(SPYCRAB_SKIN, true);
}

public void Spycrab_TimerMinigame(int timer) {
	if (timer == 3) {
		float min;
		float ang[3];
		int duck;
		
		for (int i = 1; i <= MaxClients; i++) {
			min = -35.0;
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				if (GetEntProp(i, Prop_Send, "m_nPlayerCond") & 16) SetEntProp(i, Prop_Send, "m_nPlayerCond", 0);
				SetEntPropFloat(i, Prop_Send, "m_flCloakMeter", 0.0);
				TF2_RemovePlayerDisguise(i);
				
				// FIX: Replaced deprecated FindSendPropOffs with GetEntProp
				duck = GetEntProp(i, Prop_Send, "m_fFlags") & 2;
			  
				GetClientEyeAngles(i, ang);
				if (ang[0] > min || duck == 0) {
					ForcePlayerSuicide(i);
					SetStateClient(i, false, true);

					if (g_Country[i] == 1) {
						if (ang[0] > min && duck == 0) CPrintToChat(i, "{default}Le spycrab {red}si abbassano {default}e {red}guardano su{default}!");
						else if (ang[0] > min) CPrintToChat(i, "{default}Le spycrab {default}si abbassano e {red}guardano su{default}!");
						else CPrintToChat(i, "{default}Le spycrab {red}si abbassano{default} e guardano su!");
					} else {
						if (ang[0] > min && duck == 0) CPrintToChat(i, "{default}Spycrabs {red}crouch {default}and {red}look up{default}!");
						else if (ang[0] > min) CPrintToChat(i, "{default}Spycrabs {default}crouch and {red}look up{default}!");
						else CPrintToChat(i, "{default}Spycrabs {red}crouch{default} and look up!");
					}
				}
			}
		}
	}
}

public void Spycrab_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "spy");
		SetVariantString(SPYCRAB_SKIN);
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(1);
		SetEntProp(client, Prop_Send, "m_bUseClassAnimations",1);
		SetEntProp(client, Prop_Send, "m_nBody", 0);
		RemoveClientWeapons(client);
		giveitem(client, 27);
		g_attack = false;
	}
}
public void Spycrab_End(int client) {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SetVariantString("");
			AcceptEntityInput(i, "SetCustomModel");
			SetVariantInt(0);
		}
	}

}