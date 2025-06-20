// Movement module

public void Movement_OnMinigame() {
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Movement_OnGameFrame);
	
	SetStateAll(true);
	int mission = GetRandomInt(0,1);
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			g_Mission[i] = mission;
		}
	}
}

public void Movement_OnGameFrame() {
	if (timeleft <= 5) {
		float fVelocity[3];
		float speed;
		float limit;
		speed = 0.0;
		limit = 0.0;
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				limit = GetEntPropFloat(i, Prop_Send, "m_flMaxspeed") - 100.0;
				GetEntPropVector(i, Prop_Data, "m_vecVelocity", fVelocity);
				speed = GetVectorLength(fVelocity);
				if ((g_Mission[i] == 0) && (speed < limit) && (g_Complete[i]) && (IsPlayerAlive(i))) {
					SetStateClient(i, false, true);
					ForcePlayerSuicide(i);
				}
				if ((g_Mission[i] >= 1) && (speed > 100.0) && (g_Complete[i]) && (IsPlayerAlive(i))) {
					SetStateClient(i, false, true);
					ForcePlayerSuicide(i);
				}
			}
		}
	}
}