// Spycrab module

//#define SPRITE_SPYCRAB_SKY "materials/tf2ware/spycrab.vmt"

//new Spycrab_sprite_sky = -1;

float viewang[3];
int player_crabduck[33];
int player_crablook[33];
int player_crabslot[33];
bool g_bLimboControl = false;
bool g_bBeamControl = false;

public void Limbo_Init() {
	AddToForward(g_OnMapStart, INVALID_HANDLE, Limbo_OnMapStart);
}	

public void Limbo_OnMapStart() {
	PrecacheModel("materials/sprites/laser.vmt", true);
}
public void Limbo_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, Limbo_justEntered);
	AddToForward(g_OnGameFrame_Minigames, INVALID_HANDLE, Limbo_OnGameFrame);
	//SetStateAll(true);
	g_bLimboControl = false;
	g_bBeamControl = true;
	NoCollision(true);
	if(g_bBeamControl){
		CreateTimer(0.5, CreateBeam);
	}
}
public Action CreateBeam(Handle timer, any data) {
	float BeamLocation[3] = {1402.148682, -150.497803, -1060.968140};
	float BeamEndLocation[3] = {3222.372803, -150.082001, -1060.968628};
	StartLaser(BeamLocation, BeamEndLocation);
	g_bBeamControl = false;
	
	return Plugin_Continue;
}
public void Limbo_OnGameFrame() {
    static int offset = -1;
    if (offset == -1) {
        offset = FindSendPropInfo("CTFPlayer", "m_fFlags");
    }
    
    if(g_bLimboControl) {
        for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
				float pos[3];
				GetClientAbsOrigin(i, pos);
				if (pos[1] > -150.0){
					SetStateClient(i, true, true);
				}
				//new Float:min = 35.0 * -1;
				char weapon[18];
				int duck; int look; int slot;
				duck = GetEntData(i, offset) & 2;
				look = true;
				GetClientWeapon(i, weapon, sizeof(weapon));
				slot = StrEqual(weapon, "tf_weapon_pda_spy");
				GetClientEyeAngles(i, viewang);
				look = (viewang[0] < -1 * 35.0); // if false, freeze
						/*if (viewang[0] > min) {
							viewang[0] = min - 5.0;
							TeleportEntity(i, NULL_VECTOR, viewang, NULL_VECTOR);
						}*/

				if (duck != player_crabduck[i] || look != player_crablook[i] || slot != player_crabslot[i]) {
					if (GetEntityMoveType(i) == MOVETYPE_WALK) {
						PrintHintText(i, "Spycrabs must CROUCH and LOOK UP with the disguise kit!");
						//play_sound(i, SOUND_NOCRAB);
					}
					SetEntityMoveType(i, (duck && look && slot) ? MOVETYPE_WALK : MOVETYPE_NONE);
					player_crabduck[i] = duck;
					player_crablook[i] = look;
					player_crabslot[i] = slot;
				}
			}
		}
	}
}

public Action KillLimboBeam(Handle timer, any ent ){
	if(IsValidEntity(ent)){
		AcceptEntityInput( ent, "kill" );		
	}
	return Plugin_Continue;
}
public void Limbo_justEntered(int client) {

	if (IsValidClient(client)) {
		RemoveClientWeapons(client);
		SetClientClass(client, "spy");
		SetVariantString(SPYCRAB_SKIN);
		AcceptEntityInput(client, "SetCustomModel");
		SetVariantInt(1);
		SetEntProp(client, Prop_Send, "m_bUseClassAnimations",1);
		SetEntProp(client, Prop_Send, "m_nBody", 0);
		float vel[3];
		vel[0] = 0.0;
		vel[1] = 0.0;
		vel[2] = 0.0;
		
		int columnas = client;
		int rowas = 0;
		
		float pos[3];
		pos[0] = 1564.0 + float(columnas*63);
		pos[1] = -691.0 - float(rowas*63);
		pos[2] = -1060.0;

		float ang[3];
		ang[0] = 0.0;
		ang[1] = 90.0;
		ang[2] = 0.0;
		TeleportEntity(client, pos, ang, vel);
		g_attack = false;
		CreateTimer(0.5, GivePad, client);
		SetEntityMoveType(client, MOVETYPE_NONE);
		NoCollision(true);
		CreateTimer(0.8, ControlStart, client);
	}
}
public Action GivePad( Handle timer, any client)
{
	if (IsValidClient(client)){
		giveitem(client, 27);
	}
	
	return Plugin_Continue;
}
public Action ControlStart( Handle timer, any client)
{
	g_bLimboControl = true;
	if (IsValidClient(client) && IsPlayerAlive(client) && IsClientParticipating(client)) {
		SetEntityMoveType(client, MOVETYPE_NONE);
	}
	return Plugin_Continue;
}
public void Limbo_End() {

	g_bLimboControl = false;
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
	for (int i = 1; i <= MaxClients; i++) {
		if (IsValidClient(i)) {
			SetVariantString("");
			AcceptEntityInput(i, "SetCustomModel");
			SetVariantInt(0);
		}
	}
}