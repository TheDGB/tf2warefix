int DisguiseClassTarget;
int Disguise_reference;
int Model_Example;
#define SCOUT "models/player/scout.mdl"
#define SOLDIER "models/player/soldier.mdl"
#define PYRO "models/player/pyro.mdl"
#define DEMO "models/player/demo.mdl"
#define HEAVY "models/player/heavy.mdl"
#define ENGINEER "models/player/engineer.mdl"
#define MEDIC "models/player/medic.mdl"
#define SNIPER "models/player/sniper.mdl"
#define SPY "models/player/spy.mdl"
/* disguise class : 1 scout , solder 3 , pyro 7 , demo 4, heavy 6, engi 9, medic 5,sniper 2,spy 8 */
public void SpyDisguise_OnMinigame() {
	AddToForward(g_justEntered, INVALID_HANDLE, SpyDisguise_justEntered);
	//AddToForward(g_OnAlmostEnd, INVALID_HANDLE, SpyDisguise_OnAlmostEnd);
	AddToForward(g_OnEndMinigame, INVALID_HANDLE, SpyDisguise_End);

	Model_Example = CreateEntityByName("prop_dynamic");
	if(IsValidEntity(Model_Example)){
		DisguiseClassTarget = GetRandomInt(1, 9);
		if(DisguiseClassTarget == 1){
			SetEntityModel(Model_Example, SCOUT);
			Disguise_reference = 1;
		}
		if(DisguiseClassTarget == 2){
			SetEntityModel(Model_Example, SOLDIER);
			Disguise_reference = 3;
		}
		if(DisguiseClassTarget == 3){
			SetEntityModel(Model_Example, PYRO);
			Disguise_reference = 7;
		}
		if(DisguiseClassTarget == 4){
			SetEntityModel(Model_Example, DEMO);
			Disguise_reference = 4;
		}
		if(DisguiseClassTarget == 5){
			SetEntityModel(Model_Example, HEAVY);
			Disguise_reference = 6;
		}
		if(DisguiseClassTarget == 6){
			SetEntityModel(Model_Example, ENGINEER);
			Disguise_reference = 9;
		}
		if(DisguiseClassTarget == 7){
			SetEntityModel(Model_Example, MEDIC);
			Disguise_reference = 5;
		}
		if(DisguiseClassTarget == 8){
			SetEntityModel(Model_Example, SNIPER);
			Disguise_reference = 2;
		}
		if(DisguiseClassTarget == 9){
			SetEntityModel(Model_Example, SPY);
			Disguise_reference = 8;
		}
		DispatchKeyValue(Model_Example,"skin", "3");
		DispatchSpawn(Model_Example);
		float modelspawnpos[3] = {6.509786, 10.568852, -745.781738};	 
		TeleportEntity(Model_Example, modelspawnpos, NULL_VECTOR, NULL_VECTOR);
	}
}
public void SpyDisguise_justEntered(int client) {
	if (IsValidClient(client)) {
		SetClientClass(client, "spy");
		RemoveClientWeapons(client);
		SetClientSlot(client, 4);
		ForceSpyPDA(client);
		giveitem(client, 24);
		SetClientSlot(client, 4);
		g_attack = false;
	}
}

public void SpyDisguise_End() {
	if (IsValidEntity(Model_Example)) {
		AcceptEntityInput(Model_Example, "Kill");
	}
	for (int i = 1; i <= MaxClients; i++) {
		if(IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i)) {
			if (TF2_IsPlayerInCondition(i, TFCond_Disguised)){
				//int mask = GetEntProp(i, Prop_Send, "m_nMaskClass");
				int dclass = view_as<int>(GetEntProp(i, Prop_Send, "m_nDisguiseClass"));
				//PrintToChatAll("%d", dclass); //TFClassType
				int classplayer = dclass;
				//PrintToChatAll("%s", mask);
				if(classplayer == Disguise_reference){
					SetStateClient(i, true, true);					
				}				
			}
		}
	}
	g_attack = false;
}