// Needlejump module

int g_Needle_Type = 0;

public void Needlejump_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Needlejump_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Needlejump_End);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Needlejump_OnGameFrame);
	AddToForward(g_OnTimerMinigame, INVALID_HANDLE, Needlejump_TimerMinigame);
	
	g_Needle_Type = GetRandomInt(0,3);
	
	SetStateAll(true);
	g_attack = true;
}

public void Needlejump_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			if (g_Mission[i] == 0) PerformNeedleJump(i);
		}
	}
}

public Action Needlejump_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(victim) && status == 2) {
		if (victim != attacker) {
			float fVelocity[3];
			GetEntPropVector(victim, Prop_Data, "m_vecVelocity", fVelocity);
			fVelocity[2] -= 200.0;
			TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, fVelocity);
			damage = 0.0;
			return Plugin_Changed;
		}
		if (g_Mission[victim] == 3){
			if (victim == attacker) {
				damage = 60.0;
				RequestFrame(Boost, victim);
				return Plugin_Changed;
			}
		}
	}
	return Plugin_Continue;
}

public void Needlejump_TimerMinigame(int timer) {
	if ((g_Needle_Type == 0 && timer == 3) || (g_Needle_Type == 1 && timer == 3) || (g_Needle_Type == 2 && timer == 2) || (g_Needle_Type == 3 && timer == 2)) {
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				float pos[3];
				GetClientAbsOrigin(i, pos);
				float forigin[3];
				forigin[0] = pos[0];
				forigin[1] = pos[1];
				forigin[2] = -770.0;
						
				PyroExplode(forigin);
				
				if (pos[2] <= -700.0) {
					ForcePlayerSuicide(i);
					SetStateClient(i, false, true);
				}
			}
		}
	}
}

public void Needlejump_justEntered(int client) {
	if (IsValidClient(client)) {	
		g_Mission[client] = g_Needle_Type;
		if (g_Mission[client] == 0) {
			SetClientClass(client, "medic");
			RemoveClientWeapons(client);
			giveitem(client, 17);
		}
		if (g_Mission[client] == 1) {
			SetClientClass(client, "soldier");
			RemoveClientWeapons(client);
			giveitem(client, 18);
		}
		if (g_Mission[client] == 2) {
			SetClientClass(client, "demoman");
			RemoveClientWeapons(client);
			giveitem(client, 20);
		}
		if (g_Mission[client] == 3) {
			SetClientClass(client, "pyro");
			RemoveClientWeapons(client);
			ClientCommand(client,"slot2");
			giveitem(client, 740);
			ClientCommand(client,"slot2");
		}
		SDKHook(client, SDKHook_OnTakeDamage, Needlejump_OnTakeDamageClient);
		g_NeedleDelay[client] = 50;
	}
}

public void Needlejump_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, Needlejump_OnTakeDamageClient);
	}
	g_attack = false;
}


void PerformNeedleJump(int i) {
	if (g_Mission[i] == 0) {
		float fEyeAngle[3];
		float fVelocity[3];
		if (g_NeedleDelay[i] > 0) g_NeedleDelay[i] -= 1;
		if (IsValidClient(i) && (GetClientButtons(i) & IN_ATTACK) && (g_NeedleDelay[i] <= 0)){
			int iWeapon = GetPlayerWeaponSlot(i, 0);
			if (IsValidEdict(iWeapon) && GetEntData(iWeapon, FindSendPropInfo("CTFWeaponBase", "m_iClip1")) != 0) {
				GetClientEyeAngles(i, fEyeAngle);
				GetEntPropVector(i, Prop_Data, "m_vecVelocity", fVelocity);
				float multi = GetSpeedMultiplier(1.0);
				fVelocity[0] += (10.0 * Cosine(DegToRad(fEyeAngle[1])) * -1.0) / multi;
				fVelocity[1] += (10.0 * Sine(DegToRad(fEyeAngle[1])) * -1.0) / multi;
				fVelocity[2] -= (40.0 * Sine(DegToRad(fEyeAngle[0])) * -1.0) / multi;
				if(FloatAbs(fVelocity[0]) > 400.0){
					if(fVelocity[0] > 0.0)
						fVelocity[0] = 400.0;
					else
						fVelocity[0] = -400.0;
				}
				if(FloatAbs(fVelocity[1]) > 400.0){
					if(fVelocity[1] > 0.0)
						fVelocity[1] = 400.0;
					else
						fVelocity[1] = -400.0;
				}
				if(fVelocity[2] > 400.0)
					fVelocity[2] = 400.0;
				TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, fVelocity);
				g_NeedleDelay[i] = 3;
			}
		}
	}
}
public void Boost(int client)
{
	//new client = GetClientOfUserId(userid);
	float vecClient[3];
	float vecBoost[3];

	GetEntPropVector(client, Prop_Data, "m_vecVelocity", vecClient);

	vecBoost[0] = vecClient[0] * 1.0;
	vecBoost[1] = vecClient[1] * 1.0;
	if(vecClient[2] > 0)
	{
		vecBoost[2] = vecClient[2] * 2.1;
	} else {
		vecBoost[2] = vecClient[2];
	}

	TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, vecBoost);
}