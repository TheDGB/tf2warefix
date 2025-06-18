
#define ShipRed_mdl "models/marioragdoll/super mario galaxy/bj ship/bjship.mdl"
#define ShipBlue_mdl "models/marioragdoll/super mario galaxy/bj ship/bjship.mdl"

int RedShip = -1;
int BlueShip = -1;
// new RedShip2;
// new RedShip3;
int Ent_attackStart = false;

public void PirateAttack_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, PirateAttack_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, PirateAttack_OnGameFrame);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, PirateAttack_End);
	//SetStateAll(false);
/* 	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsClientParticipating(i)) {
			g_Mission[i] = GetRandomInt(0,6);
			SetStateClient(i, false, true);
		}
	} */
	Ent_attackStart = true;
	CreateTimer(1.0, CreateRedShip);
	CreateTimer(1.0, CreateBlueShip);
	Ent_attackStart = true;
}
public void PirateAttack_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, PirateAttack_OnMapStart);
}	
public void PirateAttack_OnMapStart() {
	PrecacheModel(ShipRed_mdl, true);
	PrecacheModel(ShipBlue_mdl, true);
}
public void PirateAttack_justEntered(int client) {
	if (IsValidClient(client)) {
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		int columncow = client;
		int rowcow = 0;
		while (columncow > 2) {
		columncow = columncow - 2;
		rowcow = rowcow + 1;
		}
		float pos[3];
		pos[0] = 358.0 + float(columncow*90);
		pos[1] = 2937.0 - float(rowcow*90);
		pos[2] = 349.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 0.0;
		ang[2] = 0.0;

		TeleportEntity(client, pos, ang, vel);
		SetClientClass(client, "demoman");
		RemoveClientWeapons(client);
		NoCollision(true);
		g_attack = true;
		giveitem(client, 265);
		if(GetClientTeam(client) == 2){
			g_Mission[client] = 1;
		}
		if(GetClientTeam(client) == 3){
			g_Mission[client] = 0;
		}
		//SDKHook(client, SDKHook_OnTakeDamage, Ships_warHurt);
		SetStateClient(client, false, true);
		SetEntityHealth(client, 150);
	}
}
public void PirateAttack_End() {
	for (int  i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			//TpDisabled(i);
			SDKUnhook(i, SDKHook_OnTakeDamage, Ships_warHurt);
		}
	}
	if (IsValidEntity(RedShip)) {
		AcceptEntityInput(RedShip, "Kill");
		RedShip = -1;
	}
	if (IsValidEntity(BlueShip)) {
		AcceptEntityInput(BlueShip, "Kill");
		BlueShip = -1;
	}
	g_attack = false;
	Ent_attackStart = false;
}
public Action CreateRedShip(Handle timer){
	if (Ent_attackStart) {
		RedShip = CreateEntityByName("prop_dynamic_override");
		if (IsValidEntity(RedShip)) {
			float ShipLoc[3] = {2503.0, 2880.0, 170.0};
			float ang[3] = {0.0, 180.0, 0.0};

			DispatchKeyValue(RedShip, "modelscale", "1.0");
			DispatchKeyValue(RedShip, "model", ShipRed_mdl);

			DispatchSpawn(RedShip);
			TeleportEntity(RedShip, ShipLoc, ang, NULL_VECTOR);

			SetEntProp(RedShip, Prop_Send, "m_nSolidType", 0); // SOLID_NONE
			SetEntProp(RedShip, Prop_Send, "m_CollisionGroup", 2); // COLLISION_GROUP_DEBRIS
			SetEntProp(RedShip, Prop_Send, "m_usSolidFlags", 4); // FSOLID_NOT_SOLID

			SetEntityRenderColor(RedShip, 255, 0, 0, 255);
		}
	}
	return Plugin_Continue;
}

public Action CreateBlueShip(Handle timer){
	if (Ent_attackStart) {
		BlueShip = CreateEntityByName("prop_dynamic_override");
		if (IsValidEntity(BlueShip)) {
			float ShipLoc[3] = {2503.0, 2080.0, 170.0};
			float ang[3] = {0.0, -180.0, 0.0};

			DispatchKeyValue(BlueShip, "modelscale", "1.0");
			DispatchKeyValue(BlueShip, "model", ShipBlue_mdl);

			DispatchSpawn(BlueShip);
			TeleportEntity(BlueShip, ShipLoc, ang, NULL_VECTOR);

			SetEntProp(BlueShip, Prop_Send, "m_nSolidType", 0); // SOLID_NONE
			SetEntProp(BlueShip, Prop_Send, "m_CollisionGroup", 2); // COLLISION_GROUP_DEBRIS
			SetEntProp(BlueShip, Prop_Send, "m_usSolidFlags", 4); // FSOLID_NOT_SOLID

			SetEntityRenderColor(BlueShip, 0, 255, 255, 255);
		}
	}
	return Plugin_Continue;
}

public void PirateAttack_OnGameFrame() {
	float Apos[3];
	float Bpos[3];
	float Cpos[3];
	if(IsValidEntity(RedShip)){
		GetEntPropVector(RedShip, Prop_Data, "m_vecOrigin", Bpos);
	}
	if(IsValidEntity(BlueShip)){
		GetEntPropVector(BlueShip, Prop_Data, "m_vecOrigin", Cpos);
	}
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			GetEntPropVector(i, Prop_Data, "m_vecOrigin", Apos);
			float Dist;
			if(GetClientTeam(i) == 3){
				Dist = GetVectorDistance(Apos, Bpos);
			}
			if(GetClientTeam(i) == 2){
				Dist = GetVectorDistance(Apos, Cpos);
			}
			if(i <0 || i > MAXPLAYERS)continue;
			if(IsValidClient(i) && IsPlayerAlive(i) && GetEntityFlags(i) & FL_INWATER){
				float vel[3];
				vel[0] = 0.0;
				vel[1] = 0.0;
				vel[2] = 0.0;
				float Fpos[3];
				Fpos[0] = 564.668701;
				Fpos[1] = 2459.562744;
				Fpos[2] = 316.0;
				float ang[3];
				ang[0] = 0.0;
				ang[1] = 0.0;
				ang[2] = 0.0;
				
				TeleportEntity(i, Fpos, ang, vel);
			}
			float TeleDist;
			TeleDist = 150.0;
			if(Dist <= TeleDist)
			{
				if(GetClientTeam(i) == 2){
					float vel[3];
					vel[0] = 0.0;
					vel[1] = 0.0;
					vel[2] = 0.0;
					float Fpos[3];
					Fpos[0] = 564.668701;
					Fpos[1] = 2459.562744;
					Fpos[2] = 316.0;
					float ang[3];
					ang[0] = 0.0;
					ang[1] = 0.0;
					ang[2] = 0.0;
					
					TeleportEntity(i, Fpos, ang, vel);
				}
				if(GetClientTeam(i) == 3){
					float vel[3];
					vel[0] = 0.0;
					vel[1] = 0.0;
					vel[2] = 0.0;
					float Fpos[3];
					Fpos[0] = 564.668701;
					Fpos[1] = 2459.562744;
					Fpos[2] = 316.0;
					float ang[3];
					ang[0] = 0.0;
					ang[1] = 0.0;
					ang[2] = 0.0;
					
					TeleportEntity(i, Fpos, ang, vel);
				}
				CreateTimer(0.1, GiveWinMinigame, i);
			}
		}
	}
}
//blue spawn setpos 2481.270996 2077.162598 305.814301;setang 16.170118 5.610002 0.000000
// red setpos 2483.995361 2878.465088 305.814301;setang -7.985998 -1.320000 0.000000
public Action Ships_warHurt(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	
	if ((IsValidClient(attacker)) || (IsValidClient(victim)))
	damage = 0.0;
	return Plugin_Changed;
}