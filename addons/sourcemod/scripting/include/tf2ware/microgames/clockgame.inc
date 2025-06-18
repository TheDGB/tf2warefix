// clock module
bool g_Gate[MAXPLAYERS+1];
bool g_Gate2[MAXPLAYERS+1];
//new Handle:g_hClockEndCheck;
public void ClockGame_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, ClockGame_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, ClockGame_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, ClockGame_OnGameFrame);
	g_ClockGameStart = true;
	CreateTimer(20.5, EndAlive_Timer);
}

public void ClockGame_OnGameFrame() {
	if(g_ClockGameStart == true){
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				float pos[3];
				GetClientAbsOrigin(i, pos);
				if (pos[1] > 2050.0){
					g_Gate[i] = true;
					if(g_Gate2[i]){
						SetStateClient(i, true, true);
					}
				}
			}
		}
	}
}
public Action EndAlive_Timer(Handle hTimer) {
	if (!StrEqual(minigame, "ClockGame")) return Plugin_Stop;
	
	for (int i = 1; i <= MaxClients; i++) {
		if(g_Gate[i] && IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)){
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
										
			float posend2[3];
			posend2[0] = -6193.0;
			posend2[1] = 901.0;
			posend2[2] = 778.0;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = 90.0;
			ang[2] = 0.0;
			TeleportEntity(i, posend2, ang, vel);
			//SetClientClass(i, "demoman");
			SetEntityHealth(i, 40);
			DisableClientWeapons(i);
			NoCollision(true);
			g_Gate[i] = false;
			g_Gate2[i] = true;
		}
	}
	return Plugin_Stop;
}
public void ClockGame_justEntered(int client) {
	if (IsValidClient(client)) {
		g_Gate[client] = false;
		g_Gate2[client] = false;
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		float pos[3];
		pos[0] = -6193.0;
		pos[1] = 901.0;
		pos[2] = 778.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "scout");
		SetEntityHealth(client, 60);
		DisableClientWeapons(client);
		NoCollision(true);
	}
}

public void ClockGame_End() {
	// if(g_hClockEndCheck != INVALID_HANDLE){
		// CloseHandle(g_hClockEndCheck);
		// g_hClockEndCheck = INVALID_HANDLE;
	// }
	g_ClockGameStart = false;
}