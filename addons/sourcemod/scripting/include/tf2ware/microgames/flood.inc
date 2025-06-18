// Flood module

public void Flood_OnMinigame() {
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Flood_PlayerDeath);	
	SetStateAll(true);	
	// Change the clients that are Scout into Demomen
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			TFClassType class = TF2_GetPlayerClass(i);
			if (class == TFClass_Scout) {
				SetClientClass(i, "demoman");
				SetClientSlot(i, 2);
				DisableClientWeapons(i);
			}
		}
	}
	// Spawns a barrel over a trigger to create the floor of death? xD
	float pos[3];
	int prop = CreateEntityByName("prop_physics");	
	if (IsValidEdict(prop)) {
		DispatchKeyValue(prop, "model", "models/props_farm/wooden_barrel.mdl");
		DispatchSpawn(prop);
		pos[0] = -1938.398804;
		pos[1] = 1453.988647;
		pos[2] = -259.074463;
		TeleportEntity(prop, pos, NULL_VECTOR, NULL_VECTOR);
		CreateTimer(0.1, killprop_timer, prop);
	} 
}

public void Flood_PlayerDeath(int client) {
	SetStateClient(client, false, true);
}