int build_action;
bool IsPlayerHavePDA = false;
#if !defined FORCE_GENERATION
#define FORCE_GENERATION (1 << 8)
#endif

public void BuildSentry_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, BuildSentry_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, BuildSentry_End);
	build_action = GetRandomInt(0,3);
	HookEvent("player_builtobject", OnBuildObject, EventHookMode_Post);
	IsPlayerHavePDA = false;
	
	g_attack = true;
}
 public Action OnBuildObject(Event hEvent, const char[] strEventName, bool bDontBroadcast )
{
	int obj = GetEventInt(hEvent, "index");
	int client = GetEntPropEnt(obj, Prop_Send, "m_hBuilder");
	if(IsValidClient(client) && IsPlayerAlive(client) && IsValidEntity(obj))
	{
		char classname[32];
		GetEdictClassname(obj, classname, sizeof(classname));
		if((StrEqual(classname, "obj_sentrygun")) && (build_action == 0))
		{
			SetStateClient(client, true, true);
		}  
		if((StrEqual(classname, "obj_dispenser")) && (build_action == 1))
		{
			SetStateClient(client, true, true);
		}
		if((StrEqual(classname, "obj_teleporter")) && (build_action == 2)){
			switch (TF2_GetObjectMode(obj))
			{
				case TFObjectMode_Entrance:
				{
					SetStateClient(client, true, true);
					return Plugin_Continue;
				}
				case TFObjectMode_Exit:
				{
					SetStateClient(client, false, true);
					return Plugin_Continue;
				}
			}
		}
		if((StrEqual(classname, "obj_teleporter")) && (build_action == 3)){
			switch (TF2_GetObjectMode(obj))
			{
				case TFObjectMode_Entrance:
				{
					SetStateClient(client, false, true);
					return Plugin_Continue;
				}
				case TFObjectMode_Exit:
				{
					SetStateClient(client, true, true);
					return Plugin_Continue;
				}
			}
		}			
	}
	return Plugin_Continue;
}
public void BuildSentry_End() {
	for (int i = 1; i <= MaxClients; i++) {
		SDKUnhook(i, SDKHook_OnTakeDamage, BuildSentry_OnTakeDamageClient);
	}
	DestroyBuildings();
	char weapon[128];
	int maxent = GetMaxEntities();
	for (int i = MaxClients; i < maxent; i++)
	{
		if (IsValidEntity(i))
		{
			GetEdictClassname(i, weapon, sizeof(weapon));
			if ((StrContains(weapon, "tf_weapon_") != -1))
			{
				AcceptEntityInput(i, "Kill");
			}
		}
	}
	g_attack = false;
	UnhookEvent( "player_builtobject", OnBuildObject);
}

public Action BuildSentry_OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
	if (IsValidClient(attacker)) {
		damage = 0.0;
	}
	return Plugin_Changed;
}

public void  BuildSentry_justEntered(int client) {
	if (IsValidClient(client)) {
		g_Mission[client] = build_action;
		RemoveClientWeapons(client);
		SetClientClass(client, "engineer");
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int totalp = GetActivePlayers();
		int posap = 360 / totalp * (g_Id[client]-1);
		
		float pos[3];
		pos[0] = 2359.0 + (Cosine(DegToRad(float(posap)))*370.0);
		pos[1] = -66.0 - (Sine(DegToRad(float(posap)))*370.0);
		pos[2] = -1028.0;
		
		float ang[3];
		ang[0] = 0.0;
		ang[1] = float(180-posap);
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		SDKHook(client, SDKHook_OnTakeDamage, BuildSentry_OnTakeDamageClient);
		int flags = GetEntityFlags(client)|FL_NOTARGET;
		SetEntityFlags(client, flags);
		int metal = 150;
		if(metal < 0) metal = 0;
		if(metal > 150) metal = 150;
		SetEntProp(client, Prop_Data, "m_iAmmo", metal, 4, 3);
		NoCollision(true);
		IsPlayerHavePDA = true;
		if(IsPlayerHavePDA){
			RequestFrame(BlowMind, client);
		}
	}

}

void GivePDA(int client, int itemIndex, char[] classname)
{
    Handle hItem = TF2Items_CreateItem(OVERRIDE_ALL);
    TF2Items_SetFlags(hItem, OVERRIDE_ALL | FORCE_GENERATION);
    TF2Items_SetClassname(hItem, classname);
    TF2Items_SetItemIndex(hItem, itemIndex);
    TF2Items_SetQuality(hItem, 0);
    TF2Items_SetLevel(hItem, 1);
    TF2Items_SetNumAttributes(hItem, 0);
    
    int entity = TF2Items_GiveNamedItem(client, hItem);
    CloseHandle(hItem);
    
    if(IsValidEntity(entity)) {
        EquipPlayerWeapon(client, entity);
    }
}

public void BlowMind(int client)
{
    if(IsValidClient(client)) {
        TF2_RemoveWeaponSlot(client, TFWeaponSlot_PDA);
        TF2_RemoveWeaponSlot(client, TFWeaponSlot_Building);
        
        GivePDA(client, 28, "tf_weapon_builder");
        
        SetEntProp(client, Prop_Data, "m_iAmmo", 150, 4, 3);
		
		IsPlayerHavePDA = true;
		CreateTimer(0.2, BlowMind2, client);
    }
}

public Action BlowMind2(Handle timer, any client)
{
    if (IsValidClient(client)) {
        GivePDA(client, 25, "tf_weapon_pda_engineer_build");
		GivePDA(client, 7, "tf_weapon_wrench");
    }
    return Plugin_Continue;
}