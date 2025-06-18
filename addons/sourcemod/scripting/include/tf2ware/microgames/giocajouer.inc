char g_szSprayerID[MAXPLAYERS + 1][32];
float g_vecSprayLocation[MAXPLAYERS + 1][3];
bool g_bDontMoveJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bMoveJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bJumpJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bSalutareJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bCrouchJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bSwimJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bGoRight[MAXPLAYERS + 1] = {false, ...};
bool g_bSprayJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bSpycrabJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bClaxonJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bJumpCrouchJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bCheersJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bCallMedicJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bRocketJumpJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bDisguiseJouer[MAXPLAYERS + 1] = {false, ...};
bool g_bSlayRocketLoser[MAXPLAYERS + 1] = {false, ...};
bool g_JouerFast = false;

public int Jouer_OnMinigame() {
    AddToForward(g_justEntered, INVALID_HANDLE, Jouer_justEntered);
    AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Jouer_OnGameFrame);
    AddToForward(g_OnEndMinigame, INVALID_HANDLE, Jouer_End);
    AddTempEntHook("Player Decal", PlayerSpray);
    AddCommandListener(MainListener_Voice, "voicemenu");
    g_bJouerBoss = true;
    return 0;
}
public void Jouer_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, Jouer_Hurt);
	}
	g_bJouerBoss = false;
}

public Action Jouer_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(victim) && status == 2) {
		if (victim != attacker) {
			float fVelocity[3];
			GetEntPropVector(victim, Prop_Data, "m_vecVelocity", fVelocity);
			fVelocity[2] -= 200.0;
			TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, fVelocity);
			damage = 0.0;
			return Plugin_Changed;
		}
	}
	return Plugin_Continue;
}

public void Jouer_justEntered(int client) {
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
	
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int totaljouer = GetActivePlayers();
		int posjouer = 360 / totaljouer * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 2307.0 + (Cosine(DegToRad(float(posjouer)))*220.0);
		pos[1] = -52.0 - (Sine(DegToRad(float(posjouer)))*220.0);
		pos[2] = -1038.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(posjouer);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);	
		RemoveClientWeapons(client);
		SetClientClass(client, "sniper");
		giveitem(client, 664);
		g_Minipoints[client] = 0;		
		CreateTimer(4.5, JouerStart, client);
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, Jouer_Hurt);
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 20.0);		
		
	}
}
public Action Istructions(Handle timer, any client) {
    if ((IsValidClient(client)) && (g_bJouerBoss)) {
        SetOverlay(client, "gioca_help2");
    }
    return Plugin_Stop;
}
public Action JouerStart(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "");
		CreateTimer(0.6, Jouer1, client);
	}
	return Plugin_Stop;
}
public Action Jouer1(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_2");
		CreateTimer(0.4, Jouer2, client);
	}
	return Plugin_Stop;
}
public Action Jouer2(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_3");
		CreateTimer(0.4, Jouer3, client);
	}
	return Plugin_Stop;
}
public Action Jouer3(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_4");
		CreateTimer(0.4, Jouer4, client);
	}
	return Plugin_Stop;
}
public Action Jouer4(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_5");
		CreateTimer(0.4, Jouer5, client);
	}
	return Plugin_Stop;
}
public Action Jouer5(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_6");
		CreateTimer(0.4, Jouer6, client);
	}
	return Plugin_Stop;
}
public Action Jouer6(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_7");
		CreateTimer(0.4, Jouer7, client);
	}
	return Plugin_Stop;
}
public Action Jouer7(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_8");
		CreateTimer(0.4, Jouer8, client);
	}
	return Plugin_Stop;
}
public Action Jouer8(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		//SetOverlay(client, "");
		SetOverlay(client, "tf2ware_minigame_44_9");
		CreateTimer(0.4, Jouer9, client);
	}
	return Plugin_Stop;
}
public Action Jouer9(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		SetOverlay(client, "");
		if(g_JouerFast){
			CreateTimer(14.5, JouerGame1, client);
			CreateTimer(1.5, Istructions, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(13.5, JouerGame1, client);
			CreateTimer(1.5, Istructions, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame1(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		if(TF2_IsPlayerInCondition(client, TFCond_Taunting)){
			TF2_RemoveCondition(client, TFCond_Taunting);
		}
		g_bDontMoveJouer[client] = true;
		SetOverlay(client, "nomove");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame2, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame2, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame2(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bDontMoveJouer[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "taunt");
		g_bSalutareJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame3, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame3, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame3(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bSalutareJouer[client] = false;
		if(TF2_IsPlayerInCondition(client, TFCond_Taunting)){
			TF2_RemoveCondition(client, TFCond_Taunting);
		}
		g_bJumpJouer[client] = true;
		SetOverlay(client, "");
		SetOverlay(client, "jump");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame4, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame4, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame4(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bJumpJouer[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "crouch");
		g_bCrouchJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame5, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame5, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame5(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bCrouchJouer[client] = false;
		g_bMoveJouer[client] = true;
		SetOverlay(client, "");
		SetOverlay(client, "moving");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame6, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame6, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame6(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bMoveJouer[client] = false;
		SetOverlay(client, "");
		g_bSwimJouer[client] = true;
		SetOverlay(client, "swimming");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame7, client);
			TF2_AddCondition(client, view_as<TFCond>(86), 1.75);
		}
		else
		if(!g_JouerFast){
			TF2_AddCondition(client, view_as<TFCond>(86), 3.5);
			CreateTimer(3.5, JouerGame7, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame7(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bSwimJouer[client] = false;
		SetOverlay(client, "");
		g_bGoRight[client] = true;
		SetOverlay(client, "goright");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame8, client);
		}
		else		
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame8, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame8(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bGoRight[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "spray");
		g_bSprayJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame9, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame9, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame9(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		TF2_RemoveCondition(client, TFCond_Disguising);
		TF2_RemoveCondition(client, TFCond_Disguised);
		g_bSprayJouer[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "spycrab");
		SetClientClassJouer(client, "spy");
		RemoveClientWeapons(client);
		ClientCommand(client, "slot3");
		giveitem(client, 27);
		g_bSpycrabJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame10, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame10, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame10(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_iMedic[client] = false;
		g_bSpycrabJouer[client] = false;
		SetEntityMoveType(client, MOVETYPE_WALK);
		SetOverlay(client, "");
		SetOverlay(client, "clacson");
		g_bClaxonJouer[client] = true;
		if(g_JouerFast){
			TF2_AddCondition(client, view_as<TFCond>(82), 1.50);
			CreateTimer(1.75, JouerGame11, client);
		}
		else
		if(!g_JouerFast){
			TF2_AddCondition(client, view_as<TFCond>(82), 3.0);
			CreateTimer(3.5, JouerGame11, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame11(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_iMedic[client] = false;
		g_bClaxonJouer[client] = false;
		g_bJumpCrouchJouer[client] = true;
		SetOverlay(client, "");
		SetOverlay(client, "jumpcrouch");
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame12, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame12, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame12(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_iMedic[client] = false;
		g_iCheers[client] = false;
		g_bJumpCrouchJouer[client] = false;
		g_bCheersJouer[client] = true;
		SetOverlay(client, "");
		SetOverlay(client, "cheers");
		g_bCheersJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame13, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame13, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame13(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_iMedic[client] = false;
		g_iCheers[client] = false;
		g_bCheersJouer[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "callmedic");
		g_bCallMedicJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame14, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame14, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame14(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		TF2_RemoveCondition(client, TFCond_Disguising);
		TF2_RemoveCondition(client, TFCond_Disguised);
		g_iMedic[client] = false;
		g_iCheers[client] = false;
		g_bCallMedicJouer[client] = false;
		g_bDisguiseJouer[client] = true;
		SetOverlay(client, "");
		SetOverlay(client, "disguise");
		PrintToChat(client, "USE B BUTTON BY DEAFAULT");
		ClientCommand(client, "slot3");
		giveitem(client, 27);
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame15, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame15, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame15(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		TF2_RemoveCondition(client, TFCond_Disguising);
		TF2_RemoveCondition(client, TFCond_Disguised);
		g_bDisguiseJouer[client] = false;
		SetOverlay(client, "");
		SetOverlay(client, "taunt");
		g_bSalutareJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame16, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame16, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame16(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bSalutareJouer[client] = false;
		TF2_RemoveCondition(client, TFCond_Taunting);
		SetClientClassJouer(client, "soldier");
		RemoveClientWeapons(client);
		giveitem(client, 237);
		ClientCommand(client, "slot1");
		SetOverlay(client, "");
		SetOverlay(client, "retaunt");
		g_bSalutareJouer[client] = true;
		if(g_JouerFast){
			CreateTimer(1.75, JouerGame17, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame17, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame17(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		TF2_RemoveCondition(client, TFCond_Taunting);
		g_bSalutareJouer[client] = false;
		SetOverlay(client, "");
		g_bRocketJumpJouer[client] = true;
		g_attack = true;
		g_bSlayRocketLoser[client] = true;
		SetOverlay(client, "rocketjump");
		if(g_JouerFast){
			CreateTimer(2.0, JouerGame19, client);
		}
		else
		if(!g_JouerFast){
			CreateTimer(3.5, JouerGame18, client);
		}
	}
	return Plugin_Stop;
}
public Action JouerGame18(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bSlayRocketLoser[client] = false;
		g_attack = false;
		g_bRocketJumpJouer[client] = false;
		SetOverlay(client, "");
		CreateTimer(0.0, Jouer1, client);
		g_JouerFast = true;
	}
	return Plugin_Stop;
}
public Action JouerGame19(Handle timer, any client)
{
	if ((IsValidClient(client)) && (g_bJouerBoss)) {
		g_bSlayRocketLoser[client] = false;
		g_attack = false;
		SetOverlay(client, "");
		g_bRocketJumpJouer[client] = false;
		g_JouerFast = false;
		if (g_Minipoints[client] >= 32) SetStateClient(client, true, true);
	}
	return Plugin_Stop;
}
public Action JouerGameRocketEnd(Handle timer, any client) {
	if ((IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) || (IsFakeClient(client) && IsPlayerAlive(client) && IsClientParticipating(client))) {
		float pos[3];
		GetClientAbsOrigin(client, pos);
		float forigin[3];
		forigin[0] = pos[0];
		forigin[1] = pos[1];
		forigin[2] = -995.0;
						
		PyroExplode(forigin);
		if(g_bRocketJumpJouer[client]){
			if (pos[2] <= -995.0) {
				ForcePlayerSuicide(client);
				SetStateClient(client, false, true);
				g_bSlayRocketLoser[client] = false;
				return Plugin_Handled;
			}
			else
			PlaySound(client);
			g_Minipoints[client] += 1;
			g_bRocketJumpJouer[client] = false;
			g_bSlayRocketLoser[client] = false;
			return Plugin_Continue;
		}
	}
	return Plugin_Continue;
}
public int Jouer_OnGameFrame() {
	for (int client = 1; client <= MaxClients; client++) {
		if ((g_bDontMoveJouer[client]) || (g_bMoveJouer[client]) || (g_bSwimJouer[client])) {
			float fVelocity[3];
			float speed;
			float limit;
			speed = 0.0;
			limit = 0.0;
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if(TF2_IsPlayerInCondition(client, TFCond_Taunting)){
					TF2_RemoveCondition(client, TFCond_Taunting);
				}
				limit = GetEntPropFloat(client, Prop_Send, "m_flMaxspeed") - 100.0;
				GetEntPropVector(client, Prop_Data, "m_vecVelocity", fVelocity);
				speed = GetVectorLength(fVelocity);
				if((g_bMoveJouer[client]) || (g_bSwimJouer[client])){
					if ((speed < limit) && (IsPlayerAlive(client))) {
						if (!g_Complete[client]){	
							return false;
						}
					}
					else
					if (!g_Complete[client]){
						PlaySound(client);
						g_Minipoints[client] += 1;
						g_bMoveJouer[client] = false;
						g_bSwimJouer[client] = false;
						return true;
					}
				}
				if(g_bDontMoveJouer[client]){
					if ((speed > 100.0) && (IsPlayerAlive(client))) {
						if (!g_Complete[client]){					
							return false;
						}
					}
					else
					if (!g_Complete[client]){			
						PlaySound(client);
						g_Minipoints[client] += 1;
						g_bDontMoveJouer[client] = false;
						return true;
					}
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bJumpJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if (GetClientButtons(client) & IN_JUMP) {
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bJumpJouer[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bGoRight[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if (GetClientButtons(client) & IN_MOVERIGHT) {
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bGoRight[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bSalutareJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				TF2_RemoveCondition(client, TFCond_Disguising);
				TF2_RemoveCondition(client, TFCond_Disguised);
				if (TF2_IsPlayerInCondition(client, TFCond_Taunting)) {
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bSalutareJouer[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bCrouchJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if (GetClientButtons(client) & IN_DUCK) {
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bCrouchJouer[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client) && g_bSpycrabJouer[client]) {
			TF2_RemoveCondition(client, TFCond_Disguising);
			TF2_RemoveCondition(client, TFCond_Disguised);
			//new Float:min = 35.0 * -1;
			char weapon[18];
			int duck; int look; int slot;
			duck = GetEntProp(client, Prop_Send, "m_fFlags") & 2;
			look = true;
			GetClientWeapon(client, weapon, sizeof(weapon));
			slot = StrEqual(weapon, "tf_weapon_pda_spy");
			GetClientEyeAngles(client, viewang);
			look = (viewang[0] < -1 * 35.0); // if false, freeze
						/*if (viewang[0] > min) {
							viewang[0] = min - 5.0;
							TeleportEntity(client, NULL_VECTOR, viewang, NULL_VECTOR);
						}*/

			if (duck != player_crabduck[client] || look != player_crablook[client] || slot != player_crabslot[client]) {
				if (GetEntityMoveType(client) == MOVETYPE_WALK) {
						PrintHintText(client, "Spycrabs must CROUCH and LOOK UP with the disguise kit!");
							//play_sound(client, SOUND_NOCRAB);
				}
				SetEntityMoveType(client, (duck && look && slot) ? MOVETYPE_WALK : MOVETYPE_NONE);
				player_crabduck[client] = duck;
				player_crablook[client] = look;
				player_crabslot[client] = slot;
				if (duck == player_crabduck[client] || look == player_crablook[client]) {
					if (GetEntityMoveType(client) == MOVETYPE_WALK){
						PlaySound(client);
						g_Minipoints[client] += 1;
						g_bSpycrabJouer[client] = false;
						return true;
					}
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if((g_bClaxonJouer[client]) || (g_bCallMedicJouer[client])){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				TF2_RemoveCondition(client, TFCond_Disguising);
				TF2_RemoveCondition(client, TFCond_Disguised);
				if(g_iMedic[client]){
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bClaxonJouer[client] = false;
					g_bCallMedicJouer[client] = false;
					//g_iMedic[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bJumpCrouchJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if ((GetClientButtons(client) & IN_JUMP) && (GetClientButtons(client) & IN_DUCK)) {
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bJumpCrouchJouer[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bCheersJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if(g_iCheers[client]){
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bCheersJouer[client] = false;
					g_iMedic[client] = false;
					g_iCheers[client] = false;
					//g_bCallMedic[client] = false;
					return true;
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if(g_bDisguiseJouer[client]){
			if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
				if(TF2_IsPlayerInCondition(client, TFCond_Disguising)){
					PlaySound(client);
					g_Minipoints[client] += 1;
					g_bDisguiseJouer[client] = false;
					TF2_RemoveCondition(client, TFCond_Disguising);
					TF2_RemoveCondition(client, TFCond_Disguised);
				}
			}
		}
	}
	for (int client = 1; client <= MaxClients; client++) {
		if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
			if(g_bSlayRocketLoser[client]){
				CreateTimer(2.0, JouerGameRocketEnd, client);
				g_bSlayRocketLoser[client] = false;
				return true;
			}
		}
	}
	return Plugin_Continue;
}
public Action PlayerSpray(const char[] szTempEntityName, const arrClients[], int iClientCount, float flDelay) {
	int client = TE_ReadNum("m_nPlayer");

	if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client) && g_bSprayJouer[client]) {
		TE_ReadVector("m_vecOrigin", g_vecSprayLocation[client]);
		char player_authid[32];
		GetClientAuthId(client, AuthId_Steam2, player_authid, sizeof(player_authid));
		player_authid = g_szSprayerID[client];
		PlaySound(client);
		g_Minipoints[client] += 1;
		g_bSprayJouer[client] = false;
		return Plugin_Continue;
	}
	else
	return Plugin_Continue;
}