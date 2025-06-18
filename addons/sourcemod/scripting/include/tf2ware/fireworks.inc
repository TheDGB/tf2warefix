//credits to Erreur500

int PosMax[3] = {0,0,0};
int PosMin[2] = {0,0};
bool IsEnd = false;
int LineCount = 0;
char EffectsList[PLATFORM_MAX_PATH];
char FireworkPositions[PLATFORM_MAX_PATH];
#define PARTICLESFILEFW		"fireworks_tf2ware_list.txt"
#define POSITIONFILEFW		"fireworks_tf2ware_pos.cfg"


void ActiveFireworks()
{
	LineCount = 0;
	char Line[255];
	Handle file = OpenFile(EffectsList, "rt");
	if (file == INVALID_HANDLE)
	{
		LogError("[FIREWORKS] Could not open file %s", PARTICLESFILEFW);
		CloseHandle(file);
		return;
	}
	while (!IsEndOfFile(file))
	{
		ReadFileLine(file, Line, sizeof(Line));
		LineCount++;
	}
	CloseHandle(file);
	if(LineCount == 0)
	{
		LogMessage("Can't find any particle in %s",PARTICLESFILEFW);
		return;
	}
	Handle kv;
	char CurrentMap[64];
	kv = CreateKeyValues("Fireworks");
	if(!FileToKeyValues(kv, FireworkPositions))
	{
		LogError("Can't open %s file",POSITIONFILEFW);
		CloseHandle(kv);
		return;
	}
	LogMessage("Prepare fireworks !");
	GetCurrentMap(CurrentMap, sizeof(CurrentMap));
	if(!KvJumpToKey(kv, CurrentMap))
	{
		LogMessage("Can't find %s in %s",CurrentMap ,POSITIONFILEFW);
		return;
	}
	PosMin[0] = KvGetNum(kv, "Xmin", 0);
	PosMin[1] = KvGetNum(kv, "Ymin", 0);
	PosMax[0] = KvGetNum(kv, "Xmax", 0);
	PosMax[1] = KvGetNum(kv, "Ymax", 0);
	PosMax[2] = KvGetNum(kv, "Z", 0);
	KvRewind(kv);
	CloseHandle(kv);
	
	Fireworks();
}
void Fireworks()
{	
	CreateTimer(GetRandomFloat(0.01,0.5), TimerSpawnFireworks);
}

public Action TimerSpawnFireworks(Handle timer, any data)
{	
	float Pos[3];
	float Angl[3] = {-90.0,0.0,0.0};
	float Vel[3];
	float vBufferi[3];
	
	int RandNum = GetRandomInt(1, 7);
	for(int nbr = 0; nbr< RandNum; nbr++)
	{
		Pos[0] = GetRandomInt(PosMin[0], PosMax[0]) * 1.0;
		Pos[1] = GetRandomInt(PosMin[1], PosMax[1]) * 1.0;	
		Pos[2] = PosMax[2]*1.0;
			
		GetAngleVectors(Angl, vBufferi, NULL_VECTOR, NULL_VECTOR);
		NormalizeVector(vBufferi, Vel);
		ScaleVector(Vel, 1000.0);
		
		int ent_rocket = CreateEntityByName("tf_projectile_flare");
		SetEntData(ent_rocket, FindSendPropInfo("CTFProjectile_Rocket", "m_iTeamNum"), GetRandomInt(2,3), true);
		char TargetName[16];
		Format(TargetName, sizeof(TargetName), "fireworks_%i",nbr);
		DispatchKeyValue(ent_rocket, "targetname", TargetName);
		DispatchSpawn(ent_rocket);
		TeleportEntity(ent_rocket, Pos, Angl, Vel);
		CreateTimer(0.4, ExplodeProjectile, ent_rocket);
	}	
	if(IsEnd)
		Fireworks();
		
	return Plugin_Continue;
}

public Action ExplodeProjectile(Handle timer, any entity)		//Called went hight skybox
{
	SpawnParticle(entity);
    return Plugin_Continue;
}

void SpawnParticle(int entity)
{
	if(!IsValidEdict(entity))
	{
		return;
	}
		
	char ClassName[64];
	GetEdictClassname(entity, ClassName, sizeof(ClassName));
	
	if(StrEqual(ClassName, "tf_projectile_flare"))	
	{
		char WeapData[10];
		GetEntPropString(entity, Prop_Data, "m_iName", WeapData, sizeof(WeapData));
		if(StrContains(WeapData, "fireworks", true) == -1)
		{
			return;
		}
		
		float Pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", Pos);
		RemoveEdict(entity);
		
		int iParticle 	= CreateEntityByName("info_particle_system");
		if (IsValidEdict(iParticle))
		{		
			float ParticleAng[3];
			ParticleAng[0]	= 0.0;
			ParticleAng[1] 	= GetRandomInt(0, 360) * 1.0;
			ParticleAng[2]	= 0.0;
			
			char Adress[128];
			Format(Adress, sizeof(Adress), "playgamesound gamesound/tf2ware/fireworks/firework_explode%i.mp3", GetRandomInt(1, 5));
			for(int client=1; client<MaxClients; client++)
			{
				if(IsClientInGame(client) && IsClientConnected(client) && !IsClientReplay(client) && !IsClientSourceTV(client))
				{
					ClientCommand(client, Adress);
				}
			}
			
			int Line = GetRandomInt(1, LineCount);
			char strLine[128];
			Handle file = OpenFile(EffectsList, "rt");
			if (file == INVALID_HANDLE)
			{
				LogError("[FIREWORKS] Could not open file %s", PARTICLESFILEFW);
				CloseHandle(file);
				return;
			}
			int CurrentLine = 0;
			while(CurrentLine != Line)
			{
				ReadFileLine(file, strLine, sizeof(strLine));
				CurrentLine++;
			}
			CloseHandle(file);
			TrimString(strLine);
			DispatchKeyValue(iParticle, "effect_name", strLine);
			TeleportEntity(iParticle, Pos, ParticleAng, NULL_VECTOR);
			DispatchSpawn(iParticle);
			ActivateEntity(iParticle);
			AcceptEntityInput(iParticle, "Start");
			
			CreateTimer(0.5, TimerRemoveEdict, iParticle);
		}
	}
}

public Action TimerRemoveEdict(Handle timer, any Edict)
{
    if(IsValidEdict(Edict))
    {
        RemoveEdict(Edict);
    }
    return Plugin_Handled;
}

public Action TimerStopFireworks(Handle timer, any data)
{
    IsEnd = false;
    return Plugin_Handled;
}