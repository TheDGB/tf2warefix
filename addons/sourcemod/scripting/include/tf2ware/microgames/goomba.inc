// Goomba module

int g_JumpManTeam = 2;

public void Goomba_OnMinigame() {
	// These are forwards if you need a specific function!
	AddToForward(g_justEntered, INVALID_HANDLE, Goomba_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Goomba_End);
	
	g_JumpManTeam = GetRandomInt(2,3);
}

public void Goomba_justEntered(int client) {
	if (IsValidClient(client)) {		
		if (g_JumpManTeam == GetClientTeam(client)) {
			SetClientClass(client, "scout");
			g_Mission[client] = 0;
		}
		else {
			SetClientClass(client, "heavy");
			g_Mission[client] = 1;
			SDKHook(client, SDKHook_PreThink, Goomba_RemoveJump);
			SetClientSlot(client, 2);
			giveitem(client, 1084);
			SetStateClient(client, true);
			SDKHook(client, SDKHook_PreThink, ResizeThings);
		}
		SDKHook(client, SDKHook_Touch, Goomba_Touch);
		DisableClientWeapons(client);
	}
}

public void Goomba_Touch(int client, int other) {
	if (status == 2) {
		char edictName[32];
		GetEdictClassname(other, edictName, sizeof(edictName));

		if(StrEqual(edictName, "player") && IsValidClient(client) && IsValidClient(other) && IsPlayerAlive(client) && IsPlayerAlive(other) && g_Mission[client] == 0 && g_Mission[other] == 1 && GetClientTeam(client) != GetClientTeam(other)) {
			float ClientPos[3];
			float VictimPos[3];
			GetClientAbsOrigin(client, ClientPos);
			GetClientAbsOrigin(other, VictimPos);

			float vec[3];
			GetEntPropVector(client, Prop_Data, "m_vecAbsVelocity", vec);

			if(vec[2] < -8.0) {
				SetStateClient(other, false, true);
				ForcePlayerSuicide(other);
				SetStateClient(client, true, true);
					
				float vecAng[3]; float vecVel[3];
				GetClientEyeAngles(client, vecAng);
				vecAng[0] = DegToRad(vecAng[0]);
				vecAng[1] = DegToRad(vecAng[1]);
				vecVel[0] = 300.0*Cosine(vecAng[0])*Cosine(vecAng[1]);
				vecVel[1] = 300.0*Cosine(vecAng[0])*Sine(vecAng[1]);
				vecVel[2] = 300.0+100.0;
					
				TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, vecVel);
			}
		} 
	}
}

public void Goomba_RemoveJump(int client) {
	if ((status == 2) && GetConVarBool(ww_enable) && g_enabled) {
		int iButtons = GetClientButtons(client);
		if ((iButtons & IN_JUMP)) {
			iButtons &= ~IN_JUMP;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
			
		}
	}
}

public void Goomba_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			if (!IsPlayerAlive(i)) {
				SetStateClient(i, false, true);
			}
			SDKUnhook(i, SDKHook_Touch, Goomba_Touch);
			SDKUnhook(i, SDKHook_PreThink, Goomba_RemoveJump);
			SDKUnhook(i, SDKHook_PreThink, ResizeThings);
			SetEntPropFloat(i, Prop_Send, "m_flModelScale", 1.0);
		}
	}
}