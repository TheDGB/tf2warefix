#pragma semicolon 1
#if !defined Team_Spec
#define Team_Spec 1
#endif
#if !defined Team_Red
#define Team_Red 2
#endif
#if !defined Team_Blu
#define Team_Blu 3
#endif

bool g_bInvisible[2049];


static const char strGhostMoans[][64] = 
{
	"vo/halloween_moan1.mp3",
	"vo/halloween_moan2.mp3",
	"vo/halloween_moan3.mp3",
	"vo/halloween_moan4.mp3"
};
static const char strGhostBoos[][64] = 
{
	"vo/halloween_boo1.mp3",
	"vo/halloween_boo2.mp3",
	"vo/halloween_boo3.mp3",
	"vo/halloween_boo4.mp3",
	"vo/halloween_boo5.mp3",
	"vo/halloween_boo6.mp3",
	"vo/halloween_boo7.mp3"
};
static const char strGhostEffects[][64] = 
{
	"vo/halloween_haunted1.mp3",
	"vo/halloween_haunted2.mp3",
	"vo/halloween_haunted3.mp3",
	"vo/halloween_haunted4.mp3",
	"vo/halloween_haunted5.mp3"
};
public void ScaryGhost_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, ScaryGhost_OnMapStart);
}   

public void ScaryGhost_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, ScaryGhost_justEntered);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, ScaryGhost_End);	
	AddToForward(g_OnAlmostEnd, INVALID_HANDLE, ScaryGhost_CheckDeadPlayers);
	SetStateAll(true);
	g_GhostReady = true;
	if(g_GhostReady == true){
		CreateTimer(1.2, iGhostFired);
	}	
}		

public void ScaryGhost_OnMapStart() {
	PrecacheModel("models/humans/group01/female_01.mdl", true); //Simple_bots default model
	PrecacheModel("models/props_halloween/ghost.mdl", true);	//Ghost model itself
	PrecacheModel("ghost_appearation", true);					//Ghost appear & disappear particle

	PrecacheSounds(strGhostMoans, sizeof(strGhostMoans));
	PrecacheSounds(strGhostBoos, sizeof(strGhostBoos));
	PrecacheSounds(strGhostEffects, sizeof(strGhostEffects));
}

public Action iGhostFired(Handle hndl, any data) {
	if(g_GhostReady == true){
		float pos[3]; float ang[3];
		ang[0] = 0.0;
		float rand;
		for (int i=-2; i<=2; i++) {
			for (int j=-2; j<=2; j++) {
				rand = GetRandomFloat(200.0, 500.0);
				pos[2] = rand;
				pos[0] = i*150.0;
				pos[1] = j*150.0;
				int Ghost = CreateEntityByName("simple_bot");
				if(Ghost > 0 || Ghost < 2048){
					DispatchKeyValue(Ghost, "targetname", "spookyghostthatcanberemoved");  
					DispatchSpawn(Ghost);					
					SetEntProp(Ghost, Prop_Data, "m_takedamage", 0, 1);
					SetEntProp(Ghost, Prop_Send, "m_CollisionGroup", 2);
					TeleportEntity(Ghost, pos, NULL_VECTOR, NULL_VECTOR); 
					AttachParticle(Ghost, "ghost_appearation", _, 5.0);
					SetEntityModel(Ghost, "models/props_halloween/ghost.mdl");
					g_bInvisible[Ghost] = false;
					CreateTimer(GetRandomFloat(60.0, 10.0), Timer_ToggleInvis, EntIndexToEntRef(Ghost));
					SDKHook(Ghost, SDKHook_Touch, GhostThink); 
				} 
			}
		}  
	}
	
	return Plugin_Continue;
}
public void ScaryGhost_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "pyro");
		DisableClientWeapons(client);		
	}
}

public void ScaryGhost_CheckDeadPlayers() {
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i) && (!(IsPlayerAlive(i))) && IsClientParticipating(i)) SetStateClient(i, false, true);
	}
}

public void ScaryGhost_End() {
	int ghost = -1;
	while ((ghost = FindEntityByClassname(ghost, "simple_bot")) != -1)
	{
		if (IsValidEntity(ghost))
		{
			char name[32];
			GetEntPropString(ghost, Prop_Data, "m_iName", name, 128, 0);
			if(StrEqual(name, "spookyghostthatcanberemoved")) 
			{				
				AcceptEntityInput(ghost, "Kill");
			}
		}
	}
	g_GhostReady = false;
}

public void GhostThink(int entity)
{
	if (entity <= 0 || entity > 2048) return;
	
	float flLastCall;
	if(GetEngineTime() - 0.2 <= flLastCall)
		return;
	
	flLastCall = GetEngineTime();
	
	int iClient; float vecGhostOrigin[3]; float vecClientOrigin[3]; float flDistance;
	
	if (IsValidEntity(entity) && !g_bInvisible[entity])
	{
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vecGhostOrigin); 
		
		for(iClient = 1; iClient <= MaxClients; iClient++)
		{
			if(IsValidClient(iClient))
			{
				GetClientAbsOrigin(iClient, vecClientOrigin);
				flDistance = GetVectorDistance(vecGhostOrigin, vecClientOrigin);
				if(flDistance < 0)
					flDistance *= -1.0;
				if(flDistance <= 230)
					ScarePlayer(entity, iClient);
			}
		}
	}
}

public Action Timer_ToggleInvis(Handle timer, any entity) 
{
	int ent = EntRefToEntIndex(entity);
	if (ent != INVALID_ENT_REFERENCE && IsValidEntity(ent))
	{
		char sClass[32];
		GetEntityClassname(ent, sClass, sizeof(sClass));
	
		if(StrEqual(sClass, "simple_bot"))
		{
			//new Float:rand = GetRandomFloat(5.0, 10.0);
		
			if(!g_bInvisible[ent])	//Invisible
			{
				AttachParticle(ent, "ghost_appearation", _, 5.0);
					
				SetEntityRenderMode(ent, RENDER_TRANSCOLOR);
				SetEntityRenderColor(ent, _, _, _, 0);
				SetVariantString("ParticleEffectStop");
				AcceptEntityInput(ent, "DispatchEffect");
				
				EmitSoundToAll(strGhostEffects[GetRandomInt(0, sizeof(strGhostEffects)-1)], ent);
				SetEntityModel(ent, "models/humans/group01/female_01.mdl");
					
				CreateTimer(GetRandomFloat(60.0, 10.0), Timer_ToggleInvis, EntIndexToEntRef(ent));
				//PrintToChatAll("[INVISIBLE] Turning visible in %.1f seconds", rand);
				//CreateTimer(rand, Timer_ToggleInvis, EntIndexToEntRef(ent));
				
				g_bInvisible[ent] = true;
			}
			else					//Visible
			{
				SetEntityModel(ent, "models/props_halloween/ghost.mdl");
				
				AttachParticle(ent, "ghost_appearation", _, 5.0);
					
				SetEntityRenderColor(ent, _, _, _, 255);
				SetEntityRenderMode(ent, RENDER_NORMAL);
				
				EmitSoundToAll(strGhostMoans[GetRandomInt(0, sizeof(strGhostMoans)-1)], ent);
				EmitSoundToAll(strGhostEffects[GetRandomInt(0, sizeof(strGhostEffects)-1)], ent);
				
				CreateTimer(GetRandomFloat(60.0, 10.0), Timer_ToggleInvis, EntIndexToEntRef(ent));
				//PrintToChatAll("[VISIBLE] Turning invisible in %.1f seconds", rand);
				//CreateTimer(rand, Timer_ToggleInvis, EntIndexToEntRef(ent));
				
				g_bInvisible[ent] = false;
			}
		}
	}
	
	return Plugin_Continue;
}

stock int AttachParticle(int iEntity, const char[] strParticleEffect, const char[] strAttachPoint ="", float flZOffset=0.0, float flSelfDestruct=0.0) 
{ 
	int iParticle = CreateEntityByName("info_particle_system"); 
	if(!IsValidEdict(iParticle))return 0; 
	float flPos[3]; 
	GetEntPropVector(iEntity, Prop_Send, "m_vecOrigin", flPos); 
	flPos[2] += flZOffset; 
	TeleportEntity(iParticle, flPos, NULL_VECTOR, NULL_VECTOR); 
		
	DispatchKeyValue(iParticle, "targetname", "killme%dp@later");
	DispatchKeyValue(iParticle, "effect_name", strParticleEffect); 
	DispatchSpawn(iParticle); 
			 
	SetVariantString("!activator"); 
	AcceptEntityInput(iParticle, "SetParent", iEntity); 
	ActivateEntity(iParticle); 
			 
	if(strlen(strAttachPoint)) 
	{ 
		SetVariantString(strAttachPoint); 
		AcceptEntityInput(iParticle, "SetParentAttachmentMaintainOffset"); 
	} 
			
	AcceptEntityInput(iParticle, "start");

	if(flSelfDestruct > 0.0) 
	CreateTimer(flSelfDestruct, Timer_DeleteParticle, EntIndexToEntRef(iParticle)); 
	return iParticle;
} 

public Action Timer_DeleteParticle(Handle timer, any iRefEnt) 
{ 
	int iEntity = EntRefToEntIndex(iRefEnt); 
	if(iEntity > MaxClients) 
		AcceptEntityInput(iEntity, "Kill"); 
	 
	return Plugin_Handled; 
}

void ScarePlayer(int iGhost, int iClient)	//From Leonardos 'Be The Ghost' Plugin
{
	static float flLastScare[MAXPLAYERS+1];
	static float flLastBoo;
	
	if(!IsValidEntity(iGhost) || !IsValidClient(iClient))
		return;
	Action result;
	Call_StartForward(fwdCanBeScared);
	Call_PushCell(iGhost);
	Call_PushCell(iClient);
	Call_Finish(result);
	if(result >= Plugin_Handled)
		return;
	
	if((GetEngineTime() - 5.0) <= flLastScare[iClient])
		return;
	flLastScare[iClient] = GetEngineTime();
		
	if( GetEngineTime() - 1.0 > flLastBoo )
	{
		flLastBoo = GetEngineTime();
		EmitSoundToAll( strGhostBoos[ GetRandomInt( 0, sizeof(strGhostBoos)-1 ) ], iGhost );
	}
		
	Handle hData;
	CreateDataTimer( 1.0, Timer_StunPlayer, hData, TIMER_FLAG_NO_MAPCHANGE|TIMER_DATA_HNDL_CLOSE );
	WritePackCell( hData, iClient );
	SetStateClient(iClient, false, true);
}

public Action Timer_StunPlayer(Handle hTimer, any hData)	//From Leonardos 'Be The Ghost' Plugin
{
	ResetPack(hData);
	int iClient = ReadPackCell(hData);
	if(IsValidClient(iClient))
		//TF2_StunPlayer(iClient, 5.0, _, TF_STUNFLAGS_GHOSTSCARE);
		TF2_StunPlayer(iClient, 5.0, _, TF_STUNFLAGS_GHOSTSCARE, 0);
		
	return Plugin_Stop;
}

stock void PrecacheSounds(const char[][] strSounds, int iArraySize)
{
	for(int i = 0; i < iArraySize; i++)
	if(!PrecacheSound(strSounds[i]))
		PrintToChatAll("Faild to precache sound: %s", strSounds[i]);
}