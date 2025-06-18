// Mandrill module

public void Mandrill_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Mandrill_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Mandrill_End);	
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Mandrill_OnGameFrame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Mandrill_PlayerDeath);
	NoCollision(true);
	
}
public void Mandrill_OnMapStart() {
    AddFileToDownloadsTable("models/tf2ware/banana.dx80.vtx"); 
    AddFileToDownloadsTable("models/tf2ware/banana.dx90.vtx"); 
    AddFileToDownloadsTable("models/tf2ware/banana.mdl");
    AddFileToDownloadsTable("models/tf2ware/banana.sw.vtx"); 
    AddFileToDownloadsTable("models/tf2ware/banana.vvd"); 
    AddFileToDownloadsTable("materials/tf2ware/banana.vmt"); 
    AddFileToDownloadsTable("materials/tf2ware/banana.vtf"); 

    PrecacheModel(BANANA_HEAVY_MODEL, true);	
}
public void Mandrill_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[0] < -10662.0) SetStateClient(i, true, true);
		}
	}
}


public void Mandrill_justEntered(int client) {
	if (IsValidClient(client)) {		
		SetClientClass(client, "heavy");
		DisableClientWeapons(client);	
		SetVariantString(BANANA_HEAVY_MODEL);		
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(1);
		AcceptEntityInput(client, "SetCustomModelRotates");
		//SetEntProp(client, Prop_Send, "m_iObserverMode", 1);
		TF2_RemoveAllWearables(client);	   

		CreateTimer(0.1, Mandrill_Heavy_Ragdoll, client);
		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;

		float pos[3];
		pos[0] = 2250.513428;
		pos[1] = -8425.510742;
		pos[2] = 2923.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 180.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		NoCollision(true);
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 6.2);
	}
}

public void Mandrill_PlayerDeath(int client) {
	SetStateClient(client, false, true);
	SetVariantString("");
	AcceptEntityInput(client, "SetCustomModel");
	SetVariantInt(0);
	AcceptEntityInput(client, "SetCustomModelRotates");
	CreateTimer(0.0, Mandrill_Heavy_Ragdoll, client);	 
}

public Action Mandrill_Heavy_Ragdoll(Handle timer, any client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}

public void Mandrill_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SetVariantString("");
			AcceptEntityInput(i, "SetCustomModel");
			SetVariantInt(0);
			AcceptEntityInput(i, "SetCustomModelRotates");
		}
	}
}
