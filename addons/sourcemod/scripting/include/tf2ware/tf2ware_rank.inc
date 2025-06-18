/* *********************************************************************

Thanks for people that help me:
- dalto (http://forums.alliedmods.net/member.php?u=29575)


Updates:
 * Display FIX
 * Query FIXES
 * New: OLD STATS deleting
 * Update TABLE on UTF8 and Setting NAMES to UTF8 on DatabaseConnect
 * Changes in Database Connect
 * New: Added "/headhunters"
 * New: played_time, we will creat most active players TOP
 * New: last_active ON this we will base is this player stats not to old.. (we dont want have BIG MySQL DataBase)
   in future, maybe i will creat archive table, that there we will be have those player's, and they will be come back from archive when they will connect.
 * Anti Flood Protection (new userFlood[64];)
 * New announce message on: onClientPutInServer(client)
 * WWW sie Cvar ("sm_lrcss_www",)
 * PHP site nick protection:
 [		ReplaceString(name, sizeof(name), "'", "");
		ReplaceString(name, sizeof(name), "<", "");
		ReplaceString(name, sizeof(name), "\"", "");
]
* Changed SQL_Query on SQL_TQuery Functions, Helper: - dalto
* Added Debug Mode for MySQL Queries: new DEBUG = 0;
* Added: char steamIdSave[64][255]; for Player Disconnect useage in: SQL_TQuery
* Added: if(hndl == INVALID_HANDLE) function check in all: SQL_TQuery

MySQL Query:

CREATE TABLE `tf2ware_rank`(
`rank_id` int(64) NOT NULL auto_increment,
`steamId` varchar(32) NOT NULL default '',
`nick` varchar(128) NOT NULL default '',
`mg_won` int(12) NOT NULL default '0,
`mg_lose` int(12) NOT NULL default '0,
`bossstagewon` int(12) NOT NULL default '0,
`totalwins` int(12) NOT NULL default '0,
`last_active` int(12) NOT NULL default '0,
`played_time` int(12) NOT NULL default '0,
PRIMARY KEY  (`rank_id`)) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

database.cfg

	"tf2ware_rank"
	{
		"driver"			"default"
		"host"				"127.0.0.1"
		"database"			"database_name"
		"user"				"database_user"
		"pass"				"PASSWORD"
		//"timeout"			"0"
		"port"			"3306"
	}

*************************************************************************** */

// KOLOROWE KREDKI
#define YELLOW 0x01
#define GREEN 0x04

// DEBUG MODE (1 = ON, 0 = OFF)

int g_reconnectCounter = 0;
Handle g_hSQL;
// SOME DEFINES
#define MAX_LINE_WIDTH 60

// STATS TIME (SET DAYS AFTER STATS ARE DELETE OF NONACTIVE PLAYERS)
#define PLAYER_STATSOLD 30
char mg_id[255];
char output3[255];
// STATS DEFINATION FOR PLAYERS
int MinigameWon[MAXPLAYERS+1];
int MinigameLose[MAXPLAYERS+1];
int BossStageWon[MAXPLAYERS+1];
int TotalRoundWon[MAXPLAYERS+1];
int userInit[MAXPLAYERS+1];
int userFlood[MAXPLAYERS+1];
int userPtime[MAXPLAYERS+1];
int Mg_Query[MAXPLAYERS+1];
char steamIdSave[MAXPLAYERS+1][255];

// HANDLE OF DATABASE

int ConnectSQL()
{
	if (g_hSQL != INVALID_HANDLE)
		CloseHandle(g_hSQL);
	
	g_hSQL = INVALID_HANDLE;
	
	if (SQL_CheckConfig("tf2ware_rank"))
	{
		SQL_TConnect(ConnectSQLCallback, "tf2ware_rank");
	}
	else
	{
		SetFailState("PLUGIN STOPPED - Reason: no config entry found for 'tf2ware_rank' in databases.cfg - PLUGIN STOPPED");
	}
	
    return 1;
}

public void ConnectSQLCallback(Handle owner, Handle hndl, const char[] error, any data)
{
	if (g_reconnectCounter >= 55)
	{
		SetFailState("PLUGIN STOPPED - Reason: reconnect counter reached max - PLUGIN STOPPED");
		return;
	}
	
	if (hndl == INVALID_HANDLE)
	{
		LogError("[tf2ware_rank.smx] Connection to SQL database has failed, Try #%d, Reason: %s", g_reconnectCounter, error);
		g_reconnectCounter++;
		ConnectSQL();
		return;
	}
	
	char driver[16];
	SQL_GetDriverIdent(owner, driver, sizeof(driver));
	
	g_hSQL = hndl;
	
	if (StrEqual(driver, "mysql", false))
	{
		SQL_TQuery(g_hSQL, CreateSQLTableCallback, "CREATE TABLE IF NOT EXISTS `tf2ware_rank` (...)");
		// resto da lógica
	}	
	
	g_reconnectCounter = 1;
}

public void CreateSQLTableCallback(Handle owner, Handle hndl, const char[] error, any data)
{
    if (owner == INVALID_HANDLE)
    {
        g_reconnectCounter++;
        ConnectSQL();
        return;
    }
    
    if (hndl == INVALID_HANDLE)
    {
        return;
    }
}

public int InitializeClient( int client )
{
	if (!IsFakeClient(client))
	{
		MinigameWon[client]=0;
		MinigameLose[client]=0;
		BossStageWon[client]=0;
		TotalRoundWon[client]=0;
		userFlood[client]=0;
		userPtime[client]=GetTime();
		char steamId[64];
		GetClientAuthId(client, AuthId_Steam2, steamId, sizeof(steamId));
		steamIdSave[client] = steamId;
		//PrintToChatAll("client %s", steamId);
		Mg_Query[client] = 0;
		CreateTimer(1.0, initPlayerBase, client);
	}
	
	return 1;
}

public Action initPlayerBase(Handle timer, any client)
{
    if (g_hSQL != INVALID_HANDLE)
    {
        char buffer[200];
        Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank WHERE steamId = '%s'", steamIdSave[client]);
        SQL_TQuery(g_hSQL, SQLUserLoad, buffer, client);
    }
    return Plugin_Continue; // CORREÇÃO: Retorno explícito
}
public int saveUser(int client){
	if ( !IsFakeClient(client) && userInit[client] == 1)
	{		
		if (g_hSQL != INVALID_HANDLE)
		{
			char buffer[200];
			Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank WHERE steamId = '%s'", steamIdSave[client]);
			//PrintToChatAll("DEBUG: saveUser (%s)", buffer);

			SQL_TQuery(g_hSQL, SQLUserSave, buffer, client);
		}
	}
	
	return 1;
}

public Action Command_Say(int client, int args){
	if(!GetConVarInt(ww_sql))return Plugin_Handled;
	if(IsValidClient(client)){
		char text[192]; char command[64];

		int startidx = 0;

		GetCmdArgString(text, sizeof(text));

		if (text[strlen(text)-1] == '"')
		{		
			text[strlen(text)-1] = '\0';
			startidx = 1;	
		} 	
		if (strcmp(command, "say2", false) == 0)

		startidx += 4;

		if (strcmp(text[startidx], "/rank", false) == 0)	{
			if(userFlood[client] != 1){
				saveUser(client);
				GetMyRank(client);
				userFlood[client]=1;
				CreateTimer(10.0, removeFlood, client);
			} else {
				PrintToChat(client,"%cDo not flood the server!", GREEN);
			}
		} else	if (strcmp(text[startidx], "/top10", false) == 0)
		{		
			if(userFlood[client] != 1){
				saveUser(client);
				showTOP(client);
				userFlood[client]=1;
				CreateTimer(10.0, removeFlood, client);
			} else {
				PrintToChat(client,"%cDo not flood the server!", GREEN);
			}
		} else	if (strcmp(text[startidx], "/mg_list", false) == 0)
		{		
			if(userFlood[client] != 1){
				saveUser(client);
				showTOPHeadHunter(client);
				userFlood[client]=1;
				CreateTimer(10.0, removeFlood, client);
			} else {
				PrintToChat(client,"%cDo not flood the server!", GREEN);
			}
		}
		else if (strcmp(text[startidx], "/wlr", false) == 0)
		{		
			if(userFlood[client] != 1){
				saveUser(client);
				GetMyWLR(client);
				userFlood[client]=1;
				CreateTimer(10.0, removeFlood, client);
			} else {
				PrintToChat(client,"%cDo not flood the server!", GREEN);
			}
		}
	}
	return Plugin_Continue;
}

public Action removeFlood(Handle timer, any client)
{
    userFlood[client] = 0;
    return Plugin_Continue;
}

public int GetMyWLR(int client){
	if (g_hSQL != INVALID_HANDLE)
	{
		if(userInit[client] == 1){

			char buffer[200];
			Format(buffer, sizeof(buffer), "SELECT `mg_won`, `mg_lose`, `bossstagewon` FROM `tf2ware_rank` WHERE `steamId` = '%s' LIMIT 1", steamIdSave[client]);
			//PrintToChatAll("DEBUG: GetMyWLR (%s)", buffer);
			SQL_TQuery(g_hSQL, SQLGetMyWLR, buffer, client);

		} else {

			PrintToChat(client,"%cWait for system load you from database", GREEN);

		}
	} else {
		PrintToChat(client, "%cWLR System is now not avilable", GREEN);
	}
	
	return 1;
}
public int GetMyRank(int client){
	if (g_hSQL != INVALID_HANDLE)
	{
		if(userInit[client] == 1){
			char buffer[200];
			Format(buffer, sizeof(buffer), "SELECT `mg_won`, `mg_lose`, `bossstagewon`, `totalwins` FROM `tf2ware_rank` WHERE `steamId` = '%s' LIMIT 50", steamIdSave[client]);
			//PrintToChatAll("DEBUG: GetMyRank (%s)", buffer);
			SQL_TQuery(g_hSQL, SQLGetMyRank, buffer, client);

		} else {

			PrintToChat(client,"%cWait for system load you from database", GREEN);

		}
	} else {
		PrintToChat(client, "%cRank System is now not avilable", GREEN);
	}
	
	return 1;
}
public int GetWinnerPoints(int client){
	if (g_hSQL != INVALID_HANDLE)
	{
		if(userInit[client] == 1){

			char buffer[200];
			Format(buffer, sizeof(buffer), "SELECT `totalwins` FROM `tf2ware_rank` WHERE `steamId` = '%s' LIMIT 1", steamIdSave[client]);
			//PrintToChatAll("DEBUG: GetMyRank (%s)", buffer);
			SQL_TQuery(g_hSQL, SQLGetWinnerPoints, buffer, client);

		} else {

			PrintToChat(client,"%cWait for system load you from database", GREEN);

		}
	} else {
		PrintToChat(client, "%cRank System is now not avilable", GREEN);
	}
	
	return 1;
}

public int showTOP(int client){

	if (g_hSQL != INVALID_HANDLE)
	{
		char buffer[200];
		//Format(buffer, sizeof(buffer), "SELECT *, (`mg_lose`/`mg_won`) / `played_time` AS rankn FROM `tf2ware_rank` WHERE `mg_won` > 0 AND `mg_lose` > 0 ORDER BY rankn ASC LIMIT 10");
		Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank ORDER BY totalwins DESC LIMIT 10");
		//PrintToChatAll("DEBUG: showTOP (%s)", buffer);
		SQL_TQuery(g_hSQL, SQLTopShow, buffer, client);
	} else {
		PrintToChat(client, "%cRank System is now not avilable", GREEN);
	}
	
	return 1;
}

public int showTOPHeadHunter(int client){

	if (g_hSQL != INVALID_HANDLE)
	{
		char buffer[200];
		Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank WHERE steamId = '%s'", steamIdSave[client]);
		SQL_TQuery(g_hSQL, SQLMg_WinList, buffer, client);
	} else {
		PrintToChat(client, "%cRank System is now not avilable", GREEN);
	}
	
	return 1;
}

public int TopMenu(Menu menu, MenuAction action, int param1, int param2)
{
    return 0;
}

// ================================================================================

public int SQLErrorCheckCallback(Handle owner, Handle hndl, const char[] error, any data)
{
	if(!StrEqual("", error))
	{
		//PrintToChatAll("Last Connect SQL Error: %s", error);
	}
	
	return 1;
}


public void SQLUserLoad(Handle owner, Handle hndl, const char[] error, any client){
	if (!IsClientInGame(client)) return;
	if(SQL_FetchRow(hndl))
	{
		char name[MAX_LINE_WIDTH];
		GetClientName( client, name, sizeof(name) );

		ReplaceString(name, sizeof(name), "'", "");
		ReplaceString(name, sizeof(name), "<", "");
		ReplaceString(name, sizeof(name), "\"", "");

		char buffer[512];
		Format(buffer, sizeof(buffer), "UPDATE tf2ware_rank SET nick = '%s', last_active = '%i' WHERE steamId = '%s'", name, GetTime(), steamIdSave[client]);
		//PrintToChatAll("DEBUG: SQLUserLoad (%s)", buffer);
		SQL_TQuery(g_hSQL, SQLErrorCheckCallback, buffer);

		userInit[client] = 1;
	} else {

		char name[MAX_LINE_WIDTH];
		char buffer[200];

		GetClientName( client, name, sizeof(name) );

		ReplaceString(name, sizeof(name), "'", "");
		ReplaceString(name, sizeof(name), "<", "");
		ReplaceString(name, sizeof(name), "\"", "");

		Format(buffer, sizeof(buffer), "INSERT INTO tf2ware_rank (steamId, nick, last_active) VALUES('%s','%s', '%i')", steamIdSave[client], name, GetTime());
		//PrintToChatAll("DEBUG: SQLUserLoad (%s)", buffer);
		SQL_TQuery(g_hSQL, SQLErrorCheckCallback, buffer);

		userInit[client] = 1;
	}
}

public void SQLUserSave(Handle owner, Handle hndl, const char[] error, any client){
	if(hndl == INVALID_HANDLE)
	{
		LogError(error);
		//PrintToChatAll("Last Connect SQL Error: %s", error);
		return;
	}

	int QueryReadRow_MGWON;
	int QueryReadRow_MGLOSE;
	int QueryReadRow_BOSSWON;
	int QueryReadRow_TOTALROUNDS;
	int QueryReadRow_PTIME;
	int QueryReadRow_MGQUERY;
	int mg_id2 = 8+iMinigame;
	if(SQL_FetchRow(hndl)) 
	{
		QueryReadRow_MGWON=SQL_FetchInt(hndl,3) + MinigameWon[client];
		QueryReadRow_MGLOSE=SQL_FetchInt(hndl,4) + MinigameLose[client];
		QueryReadRow_BOSSWON=SQL_FetchInt(hndl,5) + BossStageWon[client];
		QueryReadRow_TOTALROUNDS=SQL_FetchInt(hndl,6) + TotalRoundWon[client];
		QueryReadRow_PTIME=SQL_FetchInt(hndl,8) + GetTime() - userPtime[client];
		QueryReadRow_MGQUERY=SQL_FetchInt(hndl,mg_id2) + MinigameWon[client];
		MinigameWon[client] = 0;
		MinigameLose[client] = 0;
		BossStageWon[client] = 0;
		TotalRoundWon[client] = 0;
		Mg_Query[client] = 0;
		userPtime[client] = GetTime();
		char buffer[512];
		Format(buffer, sizeof(buffer), "UPDATE tf2ware_rank SET mg_won = '%i', mg_lose = '%i', bossstagewon = '%i', totalwins = '%i', played_time = '%i', %s = '%i' WHERE steamId = '%s'", QueryReadRow_MGWON, QueryReadRow_MGLOSE, QueryReadRow_BOSSWON, QueryReadRow_TOTALROUNDS, QueryReadRow_PTIME, output2, QueryReadRow_MGQUERY, steamIdSave[client]);
		

		//PrintToChatAll("DEBUG: SQLUserSave (%s)", buffer);
		SQL_TQuery(g_hSQL, SQLErrorCheckCallback, buffer);
	}
}

public void SQLGetMyWLR(Handle owner, Handle hndl, const char[] error, any client){
	if(hndl == INVALID_HANDLE)
	{
		LogError(error);
		//PrintToChatAll("Last Connect SQL Error: %s", error);
		return;
	}
    
	int RAmgwon;
	int RAmglose;
	int RABossWon;

	if(SQL_FetchRow(hndl)) 
	{
		RAmgwon=SQL_FetchInt(hndl, 0);
		RAmglose=SQL_FetchInt(hndl, 1);
		RABossWon=SQL_FetchInt(hndl, 2);
		char buffer[512];
		float fmg_won = float(RAmgwon + RABossWon);
		float fmg_lose = float(RAmglose);
		float kpd = 0.0;
		if (fmg_won <= 0 && fmg_lose <= 0)
		{
			return;
		}	
		else if (fmg_won > 0 && fmg_lose <= 0)
		{
			kpd = fmg_won - fmg_lose;
		}
		else if (fmg_won > 0 && fmg_lose > 0)
		{
			kpd = fmg_won / fmg_lose;
		}
		Format(buffer, sizeof(buffer), "WinLoseRatio: %.2f , minigames won: %i, minigames losed: %i, Boss Stages Wons: %i", kpd, RAmgwon, RAmglose , RABossWon);
		PrintToChat(client,"%cWin/Lose Rate (%s)", GREEN, buffer);
	} else {
		PrintToChat(client, "%cYour kpd is not avlilable!", GREEN);
	}
}
public void SQLGetWinnerPoints(Handle owner, Handle hndl, const char[] error, any client){
	if(hndl == INVALID_HANDLE)
	{
		LogError(error);
		//PrintToChatAll("Last Connect SQL Error: %s", error);
		return;
	}
	int RATotalwins;

	if(SQL_FetchRow(hndl)) 
	{
		RATotalwins=SQL_FetchInt(hndl, 0) + TotalRoundWon[client];
		TotalRoundWon[client]=0;
		char buffer[512];
		//PrintToChatAll("DEBUG: SQLGetMyRank (%s)", buffer);
		if (RATotalwins <= 1)
		{	
			CPrintToChatAllEx(client, "%T\x01 %T", "TF2WARE_RANK_PREFIX_COLOR", client, "NOTIFY_VICTORY_FIRST", client, client);
			Format(buffer, sizeof(buffer), "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", LANG_SERVER, "NOTIFY_VICTORY_FIRST", LANG_SERVER, client);
		}
		else
		{
			CPrintToChatAllEx(client, "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", client, "NOTIFY_VICTORY", client, client, RATotalwins);
			Format(buffer, sizeof(buffer), "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", LANG_SERVER, "NOTIFY_VICTORY", LANG_SERVER, client, RATotalwins);
		}
		char savebuffer[255];
		Format(savebuffer, sizeof(savebuffer), "UPDATE tf2ware_rank SET totalwins = '%i' WHERE steamId = '%s'", RATotalwins, steamIdSave[client]);
		SQL_TQuery(g_hSQL, SQL_ErrorCheckCallBack, savebuffer, client);
		Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank ORDER BY totalwins DESC LIMIT 50");
		SQL_TQuery(g_hSQL, SQLShowWinner, savebuffer, client, DBPrio_High);
	} else {
		PrintToChat(client, "%cYour winner event not avlilable!", GREEN);
	}
}
public void SQLGetMyRank(Handle owner, Handle hndl, const char[] error, any client){
	if(hndl == INVALID_HANDLE)
	{
		LogError(error);
		//PrintToChatAll("Last Connect SQL Error: %s", error);
		return;
	}
    
	int RAmgwon;
	int RAmglose;
	int RABossWon;
	int RATotalwins;

	if(SQL_FetchRow(hndl)) 
	{
		RAmgwon=SQL_FetchInt(hndl, 0);
		RAmglose=SQL_FetchInt(hndl, 1);
		RABossWon=SQL_FetchInt(hndl, 2);
		RATotalwins=SQL_FetchInt(hndl, 3);
		char buffer[512];
		Format(buffer, sizeof(buffer), "SELECT * FROM tf2ware_rank ORDER BY `totalwins` DESC LIMIT 50;");
		//PrintToChatAll("DEBUG: SQLGetMyRank (%s)", buffer);
		//SQL_TQuery(g_hSQL, SQLShowRank, buffer, client);
		PrintToChat(client,"%cMinigame Won: %i | Minigame Failed: %i | Boss Stage Won: %i | Total TF2Ware Rounds Wins: %i", GREEN, RAmgwon, RAmglose, RABossWon, RATotalwins);
	} else {
		PrintToChat(client, "%cYour kpd is not avlilable!", GREEN);
	}
}
public int SQLShowRank(Handle owner, Handle hndl, const char[] error, any client){
		if (SQL_HasResultSet(hndl))
		{
			while (SQL_FetchRow(hndl))
			{
				int rows;
				//SQL_FetchString(hndl, 2, name, sizeof(name));
				//totalwins=SQL_FetchInt(hndl,6);
				rows = SQL_GetRowCount(hndl);
				PrintToChat(client,"%cYour rank is: %i.", GREEN, rows);
			}
		}
		return 1;
}
public int SQLShowWinner(Handle owner, Handle hndl, const char[] error, any client){
		if (SQL_HasResultSet(hndl))
		{
			int rows = SQL_GetRowCount(hndl);
			PrintToChat(client,"%cYour rank is: %i.", GREEN, rows);
		}
		return 1;
}
public int SQLShowWLR(Handle owner, Handle hndl, const char[] error, any client){
		if (SQL_HasResultSet(hndl))
		{
			int rows = SQL_GetRowCount(hndl);
			PrintToChat(client,"%cYour rank is: %i.", GREEN, rows);
		}
		return 1;
}

public void SQLTopShow(Handle owner, Handle hndl, const char[] error, any client){

		if(hndl == INVALID_HANDLE)
		{
			LogError(error);
			//PrintToChatAll("Last Connect SQL Error: %s", error);
			return;
		}
		Panel panel = CreatePanel(GetMenuStyleHandle(MenuStyle_Radio));
		char text[128];
		Format(text,127,"TF2ware Rounds Top 10 Players");
		SetPanelTitle(panel,text);

		int row;
		char name[64];
		int totalwins;
		if (SQL_HasResultSet(hndl))
		{
			while (SQL_FetchRow(hndl))
			{
				row++;
				SQL_FetchString(hndl, 2, name, sizeof(name));
				totalwins=SQL_FetchInt(hndl,6);
				Format(text,127,"%d) %s Won %i times!", row, name, totalwins);
				DrawPanelText(panel, text);
			}
		} else {
				Format(text,127,"TF2ware Rounds TOP 10 is empty!");
				DrawPanelText(panel, text);
		}

		DrawPanelItem(panel, " ", ITEMDRAW_SPACER|ITEMDRAW_RAWLINE);

		Format(text,59,"Exit");
		DrawPanelItem(panel, text);
		
		SendPanelToClient(panel, client, TopMenu, 20);
		//panel.Send(client, TopMenu, 20);
	 
		//delete panel

		CloseHandle(panel);

}
public void SQLMg_WinList(Handle owner, Handle hndl, const char[] error, any client){

		if(hndl == INVALID_HANDLE)
		{
			LogError(error);
			//PrintToChatAll("Last Connect SQL Error: %s", error);
			return;
		}
		Menu menu = new Menu(MenuHandler1);
		//menu.SetTitle("%T", "Minigames win details", LANG_SERVER);
		menu.SetTitle("Minigames win details");
		char text[128];

		int row;
		//char name[64];
		char mg_name[64];
		int mg_Query;
		//int ptimed;
		//char textime[64];
		int mg_id2;

		if (SQL_HasResultSet(hndl))
		{
			while (SQL_FetchRow(hndl))
			{
				for (int i=0; i<sizeof(g_name); i++)
				{
					if (StrEqual(g_name[i], ""))
					{
						continue;
					}
					row++;
					if(GetMinigameConfNum(g_name[i], "id") <= 9){
						Format(output3, sizeof(output3), "%1d_%s", GetMinigameConfNum(g_name[i], "id"), g_name[i]);
					}
					if(GetMinigameConfNum(g_name[i], "id") >= 10){
						Format(output3, sizeof(output3), "%2d_%s", GetMinigameConfNum(g_name[i], "id"), g_name[i]);
					}
					if(GetMinigameConfNum(g_name[i], "id") <= 9){
						Format(mg_id, sizeof(mg_id), "%1d", GetMinigameConfNum(g_name[i], "id"));
					}
					if(GetMinigameConfNum(g_name[i], "id") >= 10){
						Format(mg_id, sizeof(mg_id), "%2d", GetMinigameConfNum(g_name[i], "id"));
					}
					Format(mg_name, sizeof(mg_name), "%s", g_name[i]);
					mg_id2 = StringToInt(mg_id);
					//PrintToChatAll("NUMERO DI MERDA %i", mg_id2);
					mg_Query=SQL_FetchInt(hndl, mg_id2 +8);
					Format(text,127," %s : %i", mg_name, mg_Query);
					menu.AddItem(text, text);
				}
			}
		} else {
				Format(text,127,"TOP BossStageWon is empty!");
				menu.AddItem(text, text);
		}
		menu.ExitButton = false;
		menu.Display(client, 20);

}
public int MenuHandler1(Menu menu, MenuAction action, int param1, int param2)
{
    return 0;
}

public int SQL_ErrorCheckCallBack(Handle owner, Handle hndl, const char[] error, any data)
{
    // This is just an errorcallback for function who normally don't return any data
    if(hndl == INVALID_HANDLE)
    {
        SetFailState("Query failed! %s", error);
        LogError("Didint connect to a detabase2!");
    }
	
	return 1;
} 