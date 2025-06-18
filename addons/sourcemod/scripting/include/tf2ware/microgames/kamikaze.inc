// Kamikaze module
#define WW_BOMB_MODEL "models/props_trainyard/cart_bomb_separate.mdl"
#if !defined WW_BOMB_SCOUT_MODEL
#define WW_BOMB_SCOUT_MODEL "models/tf2ware/bobomb.mdl"
#endif
#define PARTICLE_BOMB "cinefx_goldrush_embers"
#define PARTICLE_EXPLODE "cinefx_goldrush_initial_smoke"

public void Kamikaze_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Kamikaze_OnMapStart);
}

public void Kamikaze_OnMapStart() {
	PrecacheModel(WW_BOMB_MODEL, true);
	precacheSound(WW_BOMB);
}

public void Kamikaze_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Kamikaze_justEntered);
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, Kamikaze_OnAlmostEnd);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, Kamikaze_End);
	AddToForward(g_OnTimerMinigame, INVALID_HANDLE, Kamikaze_TimerMinigame);
	AddToForward(g_PlayerDeath, INVALID_HANDLE, Kamikaze_PlayerDeath);

	randommini = GetRandomInt(0, 2);

	int AllPlayers = GetClientCount(true);
	int target = GetRandomInt(1, AllPlayers);
	while ((!IsValidClient(target)) || GetClientTeam(target) < 2 || g_Spawned[target] == false || !IsPlayerAlive(target)) {
		target = GetRandomInt(1, AllPlayers);
	}

	EmitSoundToClient(target, WW_BOMB, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
	ClientParticle(target, PARTICLE_BOMB, 2.0);

	g_Mission[target] = 1;
	g_result = 0;
	SetStateAll(true);
	SetStateClient(target, false);
}
public void Kamikaze_End() {
	if ((g_bomb > 0) && IsValidEntity(g_bomb)) {
		AcceptEntityInput(g_bomb, "Kill" );
		g_bomb = 0;
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SetVariantString("");
			AcceptEntityInput(i, "SetCustomModel");
			SetVariantInt(0);
			AcceptEntityInput(i, "SetCustomModelRotates");
		}
	}
}

public void Kamikaze_OnAlmostEnd() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (g_Mission[i] == 1) && (g_result >= 2)) SetStateClient(i, true);
	}
}

public void Kamikaze_PlayerDeath(int client) {
	if (g_Mission[client] == 0) {
		SetStateClient(client, false);
		g_result += 1;
	}
	if (g_Mission[client] == 1) {
		SetVariantString("");
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(0);
		AcceptEntityInput(client, "SetCustomModelRotates");
		CreateTimer(0.0, Kamikaze_Ragdoll, client);
	}
}

public void Kamikaze_TimerMinigame(int timer) {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && (g_Mission[i] == 1)) {
			if (timer > 2) {
				ClientParticle(i, PARTICLE_BOMB, 2.0);
			}
			if (timer == 2) {
				ClientParticle(i, PARTICLE_EXPLODE, 8.0);
				float pos[3];
				GetClientAbsOrigin(i, pos);

				PyroExplode(pos);
				ExplosionDamage(pos);
				if ((g_bomb > 0) && IsValidEntity(g_bomb)) {
					AcceptEntityInput(g_bomb, "Kill" );
					g_bomb = 0;
				}
			}
		}
	}
}
public Action Kamikaze_Ragdoll(Handle timer, any client) {
	if(IsValidClient(client)) {
		int rag = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");
		if (rag > MaxClients && IsValidEntity(rag))
			AcceptEntityInput(rag, "Kill");
	}
	return Plugin_Handled;
}
public void Kamikaze_justEntered(int client) {
	if (IsValidClient(client)) {
		if (g_Mission[client] == 0) {
			SetClientClass(client, "heavy");
			DisableClientWeapons(client);
		}
		if (g_Mission[client] == 1) {
			SetClientClass(client, "scout");
			DisableClientWeapons(client);
			TF2_RemoveAllWearables(client);
			giveitem(client, 939);

			SetVariantString(WW_BOMB_SCOUT_MODEL);
			AcceptEntityInput(client, "SetCustomModel");
			SetVariantInt(1);
			AcceptEntityInput(client, "SetCustomModelRotates");
			//SetEntProp(client, Prop_Send, "m_iObserverMode", 1);
			TF2_RemoveAllWearables(client);
		
			CreateTimer(0.1, Kamikaze_Ragdoll, client);

			if ((g_bomb > 0) && IsValidEntity(g_bomb)) {
				AcceptEntityInput(g_bomb, "Kill" );
				g_bomb = 0;
			}

			g_bomb = CreateEntityByName("prop_dynamic_override");

			if (IsValidEntity(g_bomb)) {
				char strName[32];
				Format(strName, sizeof(strName), "target%i", client);
				DispatchKeyValue(client, "targetname", strName);
				DispatchKeyValue(g_bomb, "model", WW_BOMB_MODEL);
				DispatchSpawn(g_bomb);
				AcceptEntityInput(g_bomb, "TurnOn", g_bomb, g_bomb, 0);

				DispatchKeyValue(g_bomb, "parentname", strName);
				SetVariantString("!activator");
				AcceptEntityInput(g_bomb, "SetParent", client, g_bomb, 0);
				SetVariantString("flag");
				AcceptEntityInput(g_bomb, "SetParentAttachment", client, g_bomb, 0);
			}
		}
	}
}