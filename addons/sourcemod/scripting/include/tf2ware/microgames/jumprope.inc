// JumpRope module

int jumprope_speed = 0;
int jumprope_drainhp = 8;
int jumprope_drainhpboost = 40;
bool g_bJumpRopeActivePlayer[MAXPLAYERS+1] = { false, ... };
bool g_bJumpRopePassivePlayer[MAXPLAYERS+1] = { false, ... };

public void JumpRope_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, JumpRope_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, JumpRope_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, JumpRope_End);	
	AddToForward(g_PlayerDeath, INVALID_HANDLE, JumpRope_PlayerDeath);
	
	SetStateAll(true);
	
	jumprope_speed = 0;
	CreateTimer(5.0, JumpRope_NewRound);
	CreateTimer(0.5, JumpRope_Drain);
}
public void JumpRope_OnGameFrame() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			float pos[3];
			GetClientAbsOrigin(i, pos);
			if (pos[1] > -5401.0){
				g_bJumpRopeActivePlayer[i] = true;
				g_bJumpRopePassivePlayer[i] = false;
			}
			if (g_bJumpRopeActivePlayer[i])
			{
				if (pos[1] < -5401.0){
					g_bJumpRopePassivePlayer[i] = true;
				}
			}
		}
	}
}
public void JumpRope_justEntered(int client) {
	if (IsValidClient(client)) {		
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnr = client;
		int rowr = 0;
		while (columnr > 17) {
			columnr = columnr - 17;
			rowr = rowr + 1;
		}
		
		float pos[3];
		pos[0] = 614.0	- float(columnr*55);
		pos[1] = -5534.0 - float(rowr*55);
		pos[2] = -795.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "pyro");
		DisableClientWeapons(client);
		NoCollision(true);
		CreateTimer(0.3, SetHealthRope, client);
	}
}
public Action SetHealthRope(Handle timer, any client) {
    if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
        int initialhealth = 85;
        SetEntityHealth(client, initialhealth);
    }
    return Plugin_Continue;
}
public Action JumpRope_NewRound(Handle timer) {
    if (!StrEqual(minigame, "JumpRope") || status != 2)
        return Plugin_Stop;

    jumprope_speed += 1;
    float timescale = (GetHostMultiplier(1.0) + float(jumprope_speed)*0.03);
    ServerCommand("host_timescale %f", timescale);
    CreateTimer((5.0 / timescale), JumpRope_NewRound);

    int activeplayers = 0;
    for (int i = 1; i <= MaxClients; i++) {
        if (IsValidClient(i) && g_Complete[i] && IsClientParticipating(i)) {
            activeplayers += 1;
        }
    }
    if (activeplayers <= 1) TriggerTimer(microgametimer);

    return Plugin_Continue;
}

public Action JumpRope_Drain(Handle timer) {
    if (!StrEqual(minigame, "JumpRope") || status != 2)
        return Plugin_Stop;

    for (int i = 1; i <= MaxClients; i++) {
        if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
            int health = GetClientHealth(i);
            if (health > jumprope_drainhp)
                SetEntityHealth(i, health - jumprope_drainhp);
            if (g_bJumpRopePassivePlayer[i] && health > jumprope_drainhp) {
                SetEntityHealth(i, health - jumprope_drainhpboost);
                PrintToChat(i, "\x03 Go back to the rope!");
            }
            if (health < 8)
                ForcePlayerSuicide(i);
        }
    }

    float timescale = (GetHostMultiplier(1.0) + float(jumprope_speed)*0.02);
    CreateTimer(1.0 / timescale, JumpRope_Drain);

    return Plugin_Continue;
}

public void JumpRope_PlayerDeath(int client) {
		SetStateClient(client, false);
		g_bJumpRopeActivePlayer[client] = false;
		g_bJumpRopePassivePlayer[client] = false;
}
public void JumpRope_End() {
	for (int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			g_bJumpRopeActivePlayer[i] = false;
			g_bJumpRopePassivePlayer[i] = false;
			
		}
	}
}