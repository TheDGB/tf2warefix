// Support for game Vocalizations
Handle vTimers[MAXPLAYERS+1] = { INVALID_HANDLE, ... };
int g_ivTimers = 0;
public Action Timer_Vocalize(Handle hTimer, any pack)
{
	// If the Vocalization is succesful make a new timer, otherwise don't
	ResetPack(pack);
	char id[24];
	int game = ReadPackCell(pack);
	if (iMinigame == game && status == 2)
	{
		ReadPackString(pack, id, sizeof(id));
		float mindelay = ReadPackFloat(pack);
		float maxdelay = ReadPackFloat(pack);
		int level = ReadPackCell(pack);
		
		int clients[MAXPLAYERS+1];
		int i=0;
		while(IsPackReadable(pack, 1))
		{
			clients[i] = ReadPackCell(pack);
			i++;
		}
		
		Handle sounds = CreateArray(128);
		KvJumpToKey(MinigameConf, minigame);
		KvJumpToKey(MinigameConf, id);
		KvGotoFirstSubKey(MinigameConf);
		char buffer[128];
		char number[4];
		i=1;
		IntToString(i, number, sizeof(number));
		while (KvJumpToKey(MinigameConf, number))
		{
			KvGoBack(MinigameConf);
			KvGetString(MinigameConf, number, buffer, sizeof(buffer));
			PushArrayString(sounds, buffer);
			i++;
			IntToString(i, number, sizeof(number));
		}
		KvRewind(MinigameConf);
		
		if (GetArraySize(sounds) > 0)
		{
			GetArrayString(sounds, GetRandomInt(0, GetArraySize(sounds)-1), buffer, sizeof(buffer));
			CloseHandle(sounds);
			
			int client = RollFromValid(clients);
			// RollFromValid returns -1 if there are no clients to choose from
			if (client != -1)
			{
				EmitSoundToAll(buffer, client, SNDCHAN_AUTO, level);
			}        
			AddRandVocalization(id, clients, mindelay, maxdelay, level);
		}
	}
	return Plugin_Stop;
}

int RollFromValid(int clients[MAXPLAYERS+1])
{
	Handle list = CreateArray();
	for (int i=0; i<sizeof(clients); i++)
	{
		if (IsValidClient(clients[i]) && IsPlayerAlive(clients[i]))
		{
			PushArrayCell(list, clients[i]);
		}
	}
	int result;
	// Check to avoid "invalid index 0" if array size is <1
	if (GetArraySize(list) > 0)
	{
		result = GetArrayCell(list, GetRandomInt(0, GetArraySize(list)-1));
	}
	else result = -1;
	CloseHandle(list);
	return result;
}

void CleanupAllVocalizations()
{
	g_ivTimers = 0;
}

int AddRandVocalization(char[] strid, int clients[MAXPLAYERS+1], float mindelay=4.0, float maxdelay=8.0, int level = SNDLEVEL_NORMAL) {
	float rand = GetRandomFloat(mindelay, maxdelay);
	Handle pack = CreateDataPack();
	vTimers[g_ivTimers] = CreateDataTimer(rand, Timer_Vocalize, pack);
	WritePackCell(pack, iMinigame);
	WritePackString(pack, strid);
	WritePackFloat(pack, mindelay);
	WritePackFloat(pack, maxdelay);
	WritePackCell(pack, level);
	for (int i=0; i<sizeof(clients); i++)
	{
		if (IsValidClient(clients[i]))
		{
			WritePackCell(pack, clients[i]);
		}
	}    
	g_ivTimers++;
	return g_ivTimers-1;
}