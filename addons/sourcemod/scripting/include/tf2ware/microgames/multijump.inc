
int jumpoffset;

public void MultiJump_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, MultiJump_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, MultiJump_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, MultiJump_OnGameFrame);
	jumpoffset = FindSendPropInfo("CTFPlayer", "m_iAirDash");
	g_MultiJumpEnabled = true;
}

public void MultiJump_OnGameFrame() {
	if(g_MultiJumpEnabled == true){
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				SetEntData(i, jumpoffset, 0);
				float posend[3];
				//if(i <0 || i > MAXPLAYERS)return Plugin_Changed;
				GetClientAbsOrigin(i, posend);
				if (posend[2] > 513.0) SetStateClient(i, true, true);
			}
		}
	}
}

public void MultiJump_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "scout");
		DisableClientWeapons(client);
		NoCollision(true);
	}
}

public void MultiJump_End() {
	g_MultiJumpEnabled = false;
}