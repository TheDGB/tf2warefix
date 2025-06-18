// SawRun module
public void SawRun_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, SawRun_OnMapStart);
}	
public void SawRun_OnMapStart() {
	PrecacheModel("models/props_forest/sawblade_moving.mdl", true);	
}
public void SawRun_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, SawRun_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SawRun_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, SawRun_OnGameFrame);
	g_SawGameStart = true;
}

public void SawRun_OnGameFrame() {
	if(g_SawGameStart == true){
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				float pos[3];
				GetClientAbsOrigin(i, pos);
				if (pos[1] > -3055.0) SetStateClient(i, true, true);
			}
		}
	}
}

public void SawRun_justEntered(int client) {
	if (IsValidClient(client)) {		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columng = client;
		int rowg = 0;
		while (columng > 8) {
			columng = columng - 8;
			rowg = rowg + 1;
		}
		
		float pos[3];
		pos[0] = 4171.0 + float(columng*55);
		pos[1] = -3919.0 - float(rowg*55);
		pos[2] = -334.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "scout");
		DisableClientWeapons(client);
		NoCollision(true);
	}
}

public void SawRun_End() {
	g_SawGameStart = false;
}