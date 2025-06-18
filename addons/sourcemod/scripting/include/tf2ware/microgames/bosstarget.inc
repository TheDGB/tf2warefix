//GiftTarget

#define BOSS_TARGET "models/props_halloween/halloween_gift.mdl"

int g_iBosss[1500];
int g_iCurrentBossTarget;

public void BossTarget_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, BossTarget_OnMapStart);
}	

public void BossTarget_OnMapStart() {

	AddFileToDownloadsTable("models/props_halloween/halloween_gift.mdl");
	PrecacheModel(BOSS_TARGET, true);
}

public void BossTarget_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BossTarget_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BossTarget_End);
	g_attack = true;
	g_BossTargetMiniGame = true;
	if(g_BossTargetMiniGame == true){
		g_StartBoss = CreateTimer(1.8, Timer_BossSpam, _, TIMER_REPEAT);
	}
}

public Action Timer_BossSpam(Handle timer) {
	if(g_BossTargetMiniGame == true){
		int BossPrinted; int LastBoss = 0;
		LastBoss = BossPrinted;
		while(BossPrinted == LastBoss)
		{
			BossPrinted = GetRandomInt(0, 13);
		} 
		if (BossPrinted >= 25) {		
			BossPrinted = 0;
			return Plugin_Stop;
		}
	
		int iBoss = CreateEntityByName("prop_physics_override");
		if(IsValidEntity(iBoss)){
			float angboss[3];
			angboss[0] = 0.0;
			angboss[1] = 90.0;
			angboss[2] = 0.0;
			DispatchKeyValue(iBoss, "model", "models/props_halloween/halloween_gift.mdl");
			DispatchKeyValue(iBoss, "disableshadows", "1");					
			DispatchKeyValue(iBoss, "modelscale", "1.0");		
			DispatchSpawn(iBoss);
			SetEntityMoveType(iBoss, MOVETYPE_NONE);	
			switch (GetRandomInt(0, 13))
			{
				case 0:
				{
					float hwB0Loc[3] = {1561.630859, -3725.528076, 120.0};
					TeleportEntity(iBoss, hwB0Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 1:
				{
					float hwB1Loc[3] = {3036.461914, -3655.968750, 120.0};
					TeleportEntity(iBoss, hwB1Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 2:
				{
					float hwB2Loc[3] = {2543.535889, -3655.968750, 120.0};
					TeleportEntity(iBoss, hwB2Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 3:
				{
					float hwB3Loc[3] = {2288.456787, -3895.968750, 120.0};
					TeleportEntity(iBoss, hwB3Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 4:
				{
					float hwB4Loc[3] = {1719.658081, -3735.968750, 120.0};
					TeleportEntity(iBoss, hwB4Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 5:
				{
					float hwB5Loc[3] = {2703.139648, -4023.968750, 120.0};
					TeleportEntity(iBoss, hwB5Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 6:
				{
					float hwB6Loc[3] = {2412.620361, -4014.617188, 120.0};
					TeleportEntity(iBoss, hwB6Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 7:
				{
					float hwB7Loc[3] = {1776.241943, -4023.968750, 120.0};
					TeleportEntity(iBoss, hwB7Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 8:
				{
					float hwB8Loc[3] = {2686.875488, -3655.968750, 120.0};
					TeleportEntity(iBoss, hwB8Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 9:
				{
					float hwB9Loc[3] = {2124.051758, -3895.968750, 120.0};
					TeleportEntity(iBoss, hwB9Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 10:
				{
					float hwB10Loc[3] = {1758.119263, -3735.999512, 120.0};
					TeleportEntity(iBoss, hwB10Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 11:
				{
					float hwB11Loc[3] = {1957.286133, -4023.486816, 120.0};
					TeleportEntity(iBoss, hwB11Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 12:
				{
					float hwB12Loc[3] = {2742.419434, -4021.792480, 120.0};
					TeleportEntity(iBoss, hwB12Loc, NULL_VECTOR, NULL_VECTOR);
				}
				case 13:
				{
					float hwB13Loc[3] = {2185.667236, -3894.608643, 120.0};
					TeleportEntity(iBoss, hwB13Loc, NULL_VECTOR, NULL_VECTOR);
				}
		
			}
	
			SDKHook(iBoss, SDKHook_OnTakeDamage, BossTarget_Hurt);
			BossPrinted++;
			if(IsValidEntity(iBoss)){
				CreateTimer(1.8, Timer_BossKill, iBoss);
			}		
		
		}
		return Plugin_Handled;
	}
	return Plugin_Continue;
}
public Action Timer_BossKill(Handle timer, any iBoss)
{
	if(IsValidEntity(iBoss))
	{
		AcceptEntityInput(iBoss, "Kill");
	}
	
	return Plugin_Continue;
}
public void BossTarget_End() {
	char strClassname[50];	
	for(int i=0; i<g_iCurrentBossTarget; i++)
	{
		if(IsValidEntity(g_iBosss[i]))
		{
			GetEdictClassname(g_iBosss[i], strClassname, sizeof(strClassname));
			if(strcmp(strClassname, "prop_physics_override") == 0)
			{				
				AcceptEntityInput(g_iBosss[i], "Kill");	
			}
		}		
		g_iBosss[i] = -1;
	}	
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, BossTarget_HurtClient);
		//g_StartBoss = false;
	}
	g_attack = false;	
	if(g_StartBoss != INVALID_HANDLE)
	{
		KillTimer(g_StartBoss);
		g_StartBoss = INVALID_HANDLE;
	}
	g_iCurrentBossTarget = -1;
	g_BossTargetMiniGame = false;

}


public Action BossTarget_Hurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	if(status == 2){
		if(attacker <0 || attacker > MAXPLAYERS)return Plugin_Changed;
		if (g_Complete[attacker] == false){		
			PlaySound2(attacker);
			g_Minipoints[attacker] += 1;
			//AcceptEntityInput(iBoss, "Kill" );
			if (g_Minipoints[attacker] >= 10) SetStateClient(attacker, true, true);
		}		
	}
	return Plugin_Changed;
}
public Action BossTarget_HurtClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	damage = 0.0;
	return Plugin_Changed;
}
public void BossTarget_justEntered(int client) {
		g_BossTargetMiniGame = true;
		if (IsValidClient(client)) {			
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnas = client;
		int rowas = 0;
		
		float pos[3];
		pos[0] = 1443.321 + float(columnas*63);
		pos[1] = -5256.0 - float(rowas*63);
		pos[2] = 166.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "sniper");
		RemoveClientWeapons(client);
		giveitem(client, 664);
		
		g_Minipoints[client] = 0;				
		
		// No damage
		SDKHook(client, SDKHook_OnTakeDamage, BossTarget_HurtClient);		
	}
}


public void PlaySound2(int client){
	if (IsValidClient(client)){
		EmitSoundToClient(client, SOUND_MINISCORE, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
	}			 
}