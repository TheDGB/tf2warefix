

public void SharkGame_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, SharkGame_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, SharkGame_OnGameFrame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, SharkGame_PlayerDeath);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SharkGame_End);
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
	SetConVarInt(FindConVar("mp_autoteambalance"), 0);
		
	int AllPlayers = GetClientCount(true);
	int target = GetRandomInt(1, AllPlayers);
	while ((!IsValidClient(target)) || GetClientTeam(target) < 2 || g_Spawned[target] == false || !IsPlayerAlive(target)) {
		target = GetRandomInt(1, AllPlayers);
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsValidTeam(i)){
			OldTeam[i] = GetClientTeam(i);
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 1);
	g_Mission[target] = 1;
	g_result = 0;
}
public void SharkGame_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[0] < 1204.0){
				if (g_Mission[i] == 0){
					SetStateClient(i, true, true);
				}
				if (g_Mission[i] == 1){
					float vel[3];
					vel[0] = 0.0;
					vel[1] = 0.0;
					vel[2] = 0.0;
					float posback[3];
					posback[0] = 2011.0;
					posback[1] = 2435.0;
					posback[2] = 185.0;
			
					float ang[3];
					ang[0] = 0.0;
					ang[1] = 0.0;
					ang[2] = 0.0;
					TeleportEntity(i, posback, ang, vel);
				}
			}
			if (g_Mission[i] == 1)
			{
				float cpos[3];
				float dpos[3];
				GetEntPropVector(i, Prop_Data, "m_vecOrigin", dpos);
				for(int ii = 1; ii<= MaxClients; ii++)
				{
					if(IsValidClient(ii) && IsPlayerAlive(ii) && g_Mission[ii] == 0)
					{
						GetEntPropVector(ii, Prop_Data, "m_vecOrigin", cpos);
						float Dist = GetVectorDistance(dpos, cpos);
						if(Dist <= 100.0)
						{
							//PrintToChatAll("Kill!");
							FakeClientCommand(ii, "explode");
						}
					}
				}
			}
		}
		if (IsValidClient(i) && (g_Mission[i] == 1) && (g_result >= 3)) SetStateClient(i, true);
	}
}
public void SharkGame_PlayerDeath(int client) {
	if (IsValidClient(client)) {
		if (g_Mission[client] == 0) {
			if (GetClientTeam(client) == 2) ChangeClientTeam(client, 3);
			SetStateClient(client, false);
			g_result += 1;
		}
	}
}

public void SharkGame_justEntered(int client) {
	if (IsValidClient(client)) {
		SDKHook(client, SDKHook_OnTakeDamage, SharkGame_OnTakeDamageClient);
		if (g_Mission[client] == 0) {
			if (GetClientTeam(client) == 3) ChangeClientTeamAlive(client, 2);
			SetClientClass(client, "soldier");
			DisableClientWeapons(client);
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
			int columnmiud = client;
			int rowmiud = 0;
			while (columnmiud > 2) {
			columnmiud = columnmiud - 2;
			rowmiud = rowmiud + 1;
			}
		
			float pos[3];
			pos[0] = 3751.0 + float(columnmiud*55);
			pos[1] = 3101.0 - float(rowmiud*55);
			pos[2] = -35.0;

			float ang[3];
			ang[0] = 0.0;
			ang[1] = -180.0;
			ang[2] = 0.0;
			TeleportEntity(client, pos, ang, vel);
		}
		if (g_Mission[client] == 1) {
			if (GetClientTeam(client) == 2) ChangeClientTeamAlive(client, 3);
			SetClientClass(client, "pyro");
			RemoveClientWeapons(client);
			TF2_RemoveAllWearables(client);
			//giveitem(client, 813);
			// Turn him into a Shark!
			SetVariantString(SHARK_MODEL);
			AcceptEntityInput(client, "SetCustomModel");
			SetVariantInt(1);
			AcceptEntityInput(client, "SetCustomModelRotates");
			//SetEntProp(client, Prop_Send, "m_iObserverMode", 1);
			TF2_RemoveAllWearables(client);
		
			CreateTimer(0.1, Shark_Ragdoll, client);
			float vel[3];
			vel[0] = 0.0;
			vel[1] = 0.0;
			vel[2] = 0.0;
			float pos[3];
			pos[0] = 2011.0;
			pos[1] = 2435.0;
			pos[2] = 185.0;
		
			float ang[3];
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;
			TeleportEntity(client, pos, ang, vel);
			//SDKHook(client, SDKHook_Touch, Shark_OnPlayerTouch);
		}
		g_attack = true;
	}
}
public Action SharkGame_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if ((IsValidClient(attacker)) && (TF2_GetPlayerClass(attacker) == TFClass_Pyro)) {
		damage = 20000.0;
	}
	else
	damage = 0.0;
	return Plugin_Changed;
}
public void SharkGame_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)){
			TF2_RemoveCondition(i, TFCond_Jarated);
			SDKUnhook(i, SDKHook_OnTakeDamage, SharkGame_OnTakeDamageClient);	
			g_attack = false;
			if (g_Mission[i] == 1) {
				SetVariantString("");
				AcceptEntityInput(i, "SetCustomModel");
				SetVariantInt(0);
				AcceptEntityInput(i, "SetCustomModelRotates");
				//SDKUnhook(i, SDKHook_Touch, Shark_OnPlayerTouch);
			}
			ChangeClientTeamAlive(i, OldTeam[i]);
		}
	}
	SetConVarInt(FindConVar("mp_friendlyfire"), 0);
	SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
	SetConVarInt(FindConVar("mp_autoteambalance"), 1);
}
public Action Shark_Ragdoll(Handle timer, any client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}
public void Shark_OnPlayerTouch(int entity, int other) {
	if (status != 2) return;
	if (!StrEqual(minigame, "Shark")) return;
	if ((IsValidEntity(entity)) && (IsValidClient(other))){
		if(entity && other <= MaxClients){
			FakeClientCommand(other, "explode");
		}
	}
}