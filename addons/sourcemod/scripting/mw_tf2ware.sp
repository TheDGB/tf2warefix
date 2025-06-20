//Includes:
#include <sourcemod>
#include <SteamWorks>
#include <clientprefs>
#include <sdktools>
#include <sdktools_sound>
#include <morecolors>
#include <tf2>
#include <tf2_stocks>
#include <sdkhooks>
#include <loghelper>
#include <tf2_hud>
#include <tf2items>
#include <gimme>
#pragma newdecls required

#define MAX_MINIGAMES 63

#define PLUGIN_VERSION "2.7.6"

#define MUSIC2_START "gamesound/tf2ware/tf2ware_intro.mp3"
#define MUSIC2_START_LEN 2.18
#define MUSIC2_WIN "gamesound/tf2ware/tf2ware_win.mp3"
#define MUSIC2_FAIL "gamesound/tf2ware/tf2ware_fail.mp3"
#define MUSIC2_END_LEN 2.2
#define MUSIC2_SPEEDUP "gamesound/tf2ware/tf2ware_speedup.mp3"
#define MUSIC2_SPEEDUP_LEN 3.29
#define MUSIC2_BOSS "gamesound/tf2ware/boss.mp3"
#define MUSIC2_BOSS_LEN 3.9
#define MUSIC2_GAMEOVER "gamesound/tf2ware/warioman_gameover.mp3"
#define MUSIC2_GAMEOVER_LEN 13.17

#define MUSIC_START "gamesound/tf2ware/newintro/warioman_intro.mp3"
#define MUSIC_START_LEN 4.07
#define MUSIC_WIN "gamesound/tf2ware/newintro/warioman_win.mp3"
#define MUSIC_FAIL "gamesound/tf2ware/newintro/warioman_fail.mp3"
#define MUSIC_END_LEN 2.0
#define MUSIC_SPEEDUP "gamesound/tf2ware/newintro/warioman_speedup.mp3"
#define MUSIC_SPEEDUP_LEN 7.1
#define MUSIC_BOSS "gamesound/tf2ware/warioman_boss.mp3"
#define MUSIC_BOSS_LEN 4.2
#define MUSIC_GAMEOVER "gamesound/tf2ware/warioman_gameover.mp3"
#define MUSIC_GAMEOVER_LEN 13.17

#define SOUND_COMPLETE "gamesound/tf2ware/complete_me.mp3"
#define SOUND_COMPLETE_YOU "gamesound/tf2ware/complete_you.mp3"
#define SOUND_MINISCORE "items/pumpkin_drop.wav"
#define SOUND_HEAVY_KISS "vo/heavy_generic01.mp3"
#define MUSIC_WAITING "gamesound/tf2ware/waitingforplayers_new2.mp3"
#define MUSIC_SPECIAL "gamesound/tf2ware/specialround.mp3"
#define MUSIC_SPECIAL_LEN 13.5
#define WIN_VOICE "vo/announcer_victory.mp3"
#define LOSE_VOICE "vo/announcer_you_failed.mp3"
#define SOUND_BONUS "misc/halloween/duck_pickup_pos_01.wav"
#define FROGGER_JUMP "gamesound/tf2ware/frogger_hop.mp3"
#define FROGGER_DEAD "gamesound/tf2ware/frogger_squash.mp3"
#define SOUND_SELECT "gamesound/tf2ware/select.mp3"
#define SHARK_MODEL "models/tf2ware/shark.mdl"
#define BANANA_HEAVY_MODEL "models/tf2ware/banana.mdl"
#define WW_BOMB "pl_hoodoo/alarm_clock_ticking_3.wav"
#define WW_BOMB_SCOUT_MODEL "models/tf2ware/bobomb.mdl"
#define SPYCRAB_SKIN "models/player/spycrabman.mdl"

#define SND_CHANNEL_SPECIFIC 32

#define PARTICLE_WIN_BLUE "teleportedin_blue"
#define PARTICLE_WIN_RED "teleportedin_red"

#define TRAIN_HIT "trainsawlaser/extra/sound1.mp3"
#define TRAIN_HIT_FULL "sound/trainsawlaser/extra/sound1.mp3"
#define TRAIN_RAIN "trainsawlaser/extra/sound2.mp3"
#define TRAIN_RAIN_FULL "sound/trainsawlaser/extra/sound2.mp3"

#define TF2_PLAYER_TAUNTING		   (1 << 7)	   // 128		 Taunting
#define TF2_PLAYERCOND_JARATE		 (1<<22)
#define TF2_PLAYERCOND_TELEGLOW		   (1<<6)

//#define HIDEHUD_INVEHICLE			   ( 1<<10 )

char g_name[MAX_MINIGAMES][24];
Function g_initFuncs[MAX_MINIGAMES];
int	 g_Sprites[MAXPLAYERS + 1];
bool g_bThirdPersonEnabled[MAXPLAYERS+1];
char infoText[512];

// Language strings
char var_lang[][] = {"", "it/"};

// Handles
ConVar g_Cvar_GameDescription;
ConVar ww_enable;
ConVar ww_speed;
ConVar ww_music;
ConVar ww_force;
ConVar ww_log;
ConVar ww_special;
ConVar ww_gamemode;
ConVar ww_advert;
ConVar ww_sql;
ConVar ww_hlstatsx;
ConVar ww_skybox;
ConVar ww_skybox_more;
ConVar TrHud;
//Handle ww_bots;
ConVar ww_force_special;
ConVar g_Cvar_Snowball;
Handle ww_allowedCommands;
int gVelocityOffset;
ConVar cvarDeathCam;
Handle clientCookie;
Handle hEntStack; //train track

Handle g_hSDKStunMerasmus;

ConVar TauntDisable;
int tauntvalueold = -1;


// REPLACE WEAPON
Handle g_hSdkRemoveWearable = INVALID_HANDLE;
Handle microgametimer = INVALID_HANDLE;

// Keyvalues configuration handle
Handle MinigameConf = INVALID_HANDLE;
Handle fwdCanBeScared = INVALID_HANDLE;
Handle g_StartBoss = INVALID_HANDLE;

// Bools
bool g_bSteamWorks = false;
bool g_Complete[MAXPLAYERS+1];
bool g_Spawned[MAXPLAYERS+1];
bool g_ModifiedOverlay[MAXPLAYERS+1];
bool g_attack = false;
bool g_enabled = false;
bool g_first = false;
bool g_waiting = true;
bool g_iMedic[MAXPLAYERS+1] = {false, ...};	//medic Call 
bool g_iCheers[MAXPLAYERS+1] = {false, ...}
bool g_iInspect[MAXPLAYERS+1] = {false, ...}
bool g_BallReady = false;
bool g_BallBackReady = false;
bool g_AirRaidDeflect = false;
bool g_GhostReady = false;
bool g_BossTargetMiniGame = false;
bool g_MultiJumpEnabled = false;
bool g_SoldierTargGameStart = false;
bool g_PortalStart = false;
bool g_SawGameStart = false;
bool g_ClockGameStart = false;
bool g_StartTalpa = false;
bool g_bJouerBoss = false;
bool g_BombRainEnabled = false;
bool g_ParaTargetStart = false;
bool g_wipeout_start = false;
bool sandwich[MAXPLAYERS+1] = {false, ...}
bool inspect = false;
int OldTeam[MAXPLAYERS+1];

// Ints
int g_Ent[MAXPLAYERS+1];
int g_Mission[MAXPLAYERS+1];
int g_NeedleDelay[MAXPLAYERS+1];
int g_Points[MAXPLAYERS+1];
int g_Id[MAXPLAYERS+1];
int g_Winner[MAXPLAYERS+1];
int g_Minipoints[MAXPLAYERS+1];
int g_Country[MAXPLAYERS+1];
int LastSnowBallUsed[MAXPLAYERS+1];
float currentSpeed;
int iMinigame;
int status;
int randommini;
int randomtauntselection;
int randomposspawn;
int RandomClass;
int g_offsCollisionGroup;
int timeleft = 8;
int timeleftmg = 4;
int white;
int g_HaloSprite;
int g_ExplosionSprite;
int g_result = 0;
char g_mathquestion[24];
int g_bomb = 0;
int Roundstarts = 0;
int g_lastminigame = 0;
int g_lastboss = 0;
int g_minigamestotal = 0;
int bossBattle = 0;
int SpecialRound = 0;
bool g_Participating[MAXPLAYERS+1] = {false, ...}
int g_Gamemode = 0;
bool ShowMessage = false;
int GameRules = -1;
int m_bIsInTrainingOffset = -1;
int m_bIsTrainingHUDVisibleOffset = -1;


// Strings
char materialpath[512] = "ww_overlays/";
char Hour[128];
// Name of current minigame being played
char minigame[25];
char output2[255];
// VALID iMinigame FORWARD HANDLERS //////////////
Handle g_OnMapStart;
Handle g_justEntered;
Handle g_OnAlmostEnd;
Handle g_OnTimerMinigame;
Handle g_OnEndMinigame;
Handle g_OnGameFrame_Minigames;
Handle g_PlayerDeath;
float g_flSpeed[MAXPLAYERS];
int fStartTime;
/////////////////////////////////////////
enum struct g_ePlayerInfo
{
    int g_iPlayerColor[4];
    Handle g_hPlayerEntities;
}
g_ePlayerInfo g_PlayerData[MAXPLAYERS+1];
///////////////////////////////////////////////
#define GAMEMODE_NORMAL			0
#define GAMEMODE_WIPEOUT		1
#define GAMEMODE_WIPEOUT_HEIGHT 990.0
#define SOUND_DIR "sound/gamesound/tf2ware/"
#define SUBSOUND_DIR "gamesound/tf2ware/"

#include "tf2ware\microgames\hitenemy.inc"
#include "tf2ware\microgames\spycrab.inc"
#include "tf2ware\microgames\kamikaze.inc"
#include "tf2ware\microgames\math.inc"
#include "tf2ware\microgames\sawrun.inc"
#include "tf2ware\microgames\barrel.inc"
#include "tf2ware\microgames\needlejump.inc"
#include "tf2ware\microgames\hopscotch.inc"
#include "tf2ware\microgames\airblast.inc"
#include "tf2ware\microgames\movement.inc"
#include "tf2ware\microgames\flood.inc"
#include "tf2ware\microgames\simonsays.inc"
#include "tf2ware\microgames\bball.inc"
#include "tf2ware\microgames\hugging.inc"
#include "tf2ware\microgames\redfloor.inc"
#include "tf2ware\microgames\snipertarget.inc"
#include "tf2ware\microgames\airraid.inc"
#include "tf2ware\microgames\jumprope.inc"
#include "tf2ware\microgames\colortext.inc"
#include "tf2ware\microgames\frogger.inc"
#include "tf2ware\microgames\goomba.inc"
#include "tf2ware\microgames\ghostbusters.inc"
#include "tf2ware\microgames\mandrill.inc"
#include "tf2ware\microgames\batboy.inc"
#include "tf2ware\microgames\soldiertarget.inc"
#include "tf2ware\microgames\hitenemyhhh.inc"
#include "tf2ware\microgames\flipperball.inc"
#include "tf2ware\microgames\scareghost.inc"
#include "tf2ware\microgames\monocolus.inc"
#include "tf2ware\microgames\bosstarget.inc"
#include "tf2ware\microgames\catchcube.inc"
#include "tf2ware\microgames\multijump.inc"
#include "tf2ware\microgames\portals.inc"
#include "tf2ware\microgames\avoidtrain.inc"
#include "tf2ware\microgames\bombrain.inc"
#include "tf2ware\microgames\longjumpkart.inc"
#include "tf2ware\microgames\bosskart.inc"
#include "tf2ware\microgames\racekart.inc"
#include "tf2ware\microgames\pushkart.inc"
#include "tf2ware\microgames\tauntkill.inc"
#include "tf2ware\microgames\paracadute.inc"
#include "tf2ware\microgames\waterwar.inc"
#include "tf2ware\microgames\spycrab_limbo.inc"
#include "tf2ware\microgames\giocajouer.inc"
#include "tf2ware\microgames\madgear.inc"
#include "tf2ware\microgames\shark.inc"
#include "tf2ware\microgames\buildsentry.inc"
#include "tf2ware\microgames\grap_the_cow.inc"
#include "tf2ware\microgames\flipperball_back.inc"
#include "tf2ware\microgames\slender.inc"
#include "tf2ware\microgames\clockgame.inc"
#include "tf2ware\microgames\treasure_hunt.inc"
#include "tf2ware\microgames\piggy.inc"
#include "tf2ware\microgames\merastun.inc"
#include "tf2ware\microgames\push_the_ball.inc"
#include "tf2ware\microgames\spycrab_disguise.inc"
#include "tf2ware\microgames\break_the_passtime.inc"
#include "tf2ware\microgames\boss_gp.inc"
#include "tf2ware\microgames\dodgeball.inc"
#include "tf2ware\microgames\streetfighter.inc"
#include "tf2ware\microgames\pickup_plate.inc"
#include "tf2ware\microgames\pirate.inc"
#include "tf2ware\microgames\meetchester.inc"

#include "tf2ware\fireworks.inc"
#include "tf2ware\mw_tf2ware_features.inc"
#include "tf2ware\special.inc"
#include "tf2ware\vocalize.inc"
#include "tf2ware\tf2ware_rank.inc"

public Plugin myinfo = {
	name = "TF2 Ware v2",
	author = "TonyBaretta",
	description = "Wario Ware in Team Fortress 2!",
	version = PLUGIN_VERSION,
	url = "https://www.assembla.com/code/tf2ware/subversion/nodes"
};
char var_funny_names[][] = {
	"FAT_LARD_RUN",
	"MOUSTACHIO",
	"LOVE_STORY",
	"SIZE_MATTERS",
	"ENGINERD",
	"IDLE_FOR_HATS",
	"TF2_BROS_BRAWL",
	"HOT_SPY_ON_ICE"
};
public void OnPluginStart()
{
	// Check for SDKHooks
	if(GetExtensionFileStatus("sdkhooks.ext") < 1)
	{
		SetFailState("SDK Hooks is not loaded.");
	}
	
	// Find collision group offsets
	// Fixed SendProp lookup
	g_offsCollisionGroup = FindSendPropInfo("CBaseEntity", "m_CollisionGroup");
	if (g_offsCollisionGroup == -1)
	{
		PrintToServer("");
	}
	BuildPath(Path_SM, EffectsList, sizeof(EffectsList), "configs/%s", PARTICLESFILEFW);
	BuildPath(Path_SM, FireworkPositions, sizeof(FireworkPositions), "configs/%s", POSITIONFILEFW);
	
	 // ConVars
	CreateConVar("tf2ware_version", PLUGIN_VERSION, "Current TF2WARE version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY);
	ww_enable = CreateConVar("ww_enable", "1", "Enables/Disables TF2 Ware.", FCVAR_PLUGIN);
	ww_force = CreateConVar("ww_force", "0", "Force a certain minigame (0 to not force).", FCVAR_PLUGIN);
	ww_speed = CreateConVar("ww_speed", "1.0", "Speed level.", FCVAR_PLUGIN);
	ww_music = CreateConVar("ww_music_fix", "0", "Apply music fix? Should only be on for localhosts during testing", FCVAR_PLUGIN);
	ww_log = CreateConVar("ww_log", "0", "Log server events?", FCVAR_PLUGIN);
	ww_special = CreateConVar("ww_special", "0", "Next round is Special Round?", FCVAR_PLUGIN);
	ww_gamemode = CreateConVar("ww_gamemode", "-1", "Gamemode -1 disable", FCVAR_PLUGIN);
	ww_advert = CreateConVar("ww_advert", "120.0", "Time interval in seconds between notifications (0 for none)");
	ww_sql = CreateConVar("ww_sql", "1", "enable rank with sql table?", FCVAR_PLUGIN);
	ww_hlstatsx = CreateConVar("ww_hlstatsx", "0", "enable HLSTATSX points? (def 0 )", FCVAR_PLUGIN);
	ww_skybox = CreateConVar("ww_skybox", "1", "Enable Dynamic Skybox based on server time", FCVAR_PLUGIN);
	ww_skybox_more = CreateConVar("ww_skybox_more", "0", "Enable more skyboxes in differents hours  ww_skybox 1 needed", FCVAR_PLUGIN);
	TauntDisable = CreateConVar("ww_disabletaunts", "0", "Disable Taunts 1 disable", FCVAR_PLUGIN);
	g_Cvar_Snowball = CreateConVar("ww_snowballs", "1", "Enable snowball?", FCVAR_NONE, true, 0.0, true, 1.0);
	ww_force_special = CreateConVar("ww_force_special", "0", "Forces a specific Special Round on Special Round", FCVAR_PLUGIN);
	
	// In development !!!
	TrHud = CreateConVar("ww_trhud", "0", "Show Mission message via training hud?", FCVAR_NONE, true, 0.0, true, 1.0);
	
	RegConsoleCmd("say", Command_Say);
	//RegConsoleCmd("jointeam", Command_JoinTeam);
	//RegConsoleCmd("changeteam", Command_JoinTeam);
	if(ww_sql.IntValue)
	{
		ConnectSQL();
	}
	//ww_bots = CreateConVar("ww_bots", "4", "how many bots ?", FCVAR_PLUGIN);
	g_Cvar_GameDescription = CreateConVar("ww_gamedescription", "1.0", "If SteamWorks is loaded, set the Game Description to tf2ware?", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	AutoExecConfig(true, "tf2_ware_v2");
	AddCommandListener(BlockTaunt, "taunt");
	gVelocityOffset = FindSendPropInfo("CBasePlayer", "m_vecVelocity[0]");
	clientCookie = RegClientCookie("tf2ware_wins", "Number of Tf2ware rounds wins", CookieAccess_Protected);
	LoadTranslations("common.phrases.txt");
	LoadTranslations("mw_tf2ware.phrases.txt");
	Format(infoText, sizeof(infoText), "{orange}[TF2Ware]{default}Commands: {green}/top10 {default}show top10 winners, {green}/wlr {default}Show Win lose ratio {green}/mg_list{default} list minigames won {green}/rank {default} Player details");

    m_bIsInTrainingOffset = FindSendPropInfo("CTFPlayer", "m_bIsInTraining");
    m_bIsTrainingHUDVisibleOffset = FindSendPropInfo("CTFPlayer", "m_bIsTrainingHUDVisible");
	if (ww_advert.FloatValue > 0.0 && ww_sql.IntValue && ww_enable.BoolValue) CreateTimer(ww_advert.FloatValue , Notification);
}
public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	MarkNativeAsOptional("Steam_SetGameDescription");
	fwdCanBeScared = CreateGlobalForward("TF2SG_CanBeScared", ET_Hook, Param_Cell, Param_Cell);
	return APLRes_Success;
}
public void OnMapStart()
{
	char map[128];
	GetCurrentMap(map, sizeof(map));
	if (StrContains(map, "tf2ware", false) == 0)
	{
		SetConVarBool(ww_enable, true);
		g_enabled = true;
	}
	if (StrContains(map, "tf2ware", false) == -1)
	{
		SetConVarBool(ww_enable, false);
		g_enabled = false;
		return;
	}
	if(!ww_enable) 
	{
		return;
	}
	if (g_enabled && ww_enable.BoolValue)
	{
		char file[256];
		BuildPath(Path_SM, file, 255, "configs/tf2ware.ini");
		Handle fileh = OpenFile(file, "r");
		if (fileh != INVALID_HANDLE)
		{
			char buffer[256];
			char buffer_full[PLATFORM_MAX_PATH];

			while(ReadFileLine(fileh, buffer, sizeof(buffer)))
			{
				TrimString(buffer);
				if ((StrContains(buffer, "//") == -1) && (!StrEqual(buffer, "")))
				{
					PrintToServer("Reading downloads line :: %s", buffer);
					Format(buffer_full, sizeof(buffer_full), "%s", buffer);
					if (FileExists(buffer_full))
					{
						PrintToServer("Precaching %s", buffer);
						PrecacheDecal(buffer, true);
						AddFileToDownloadsTable(buffer_full);
					}
				}
			}
		}		
		char AdressFireworks[128];
		for(int i=1;i<=5;i++)
		{
			Format(AdressFireworks, sizeof(AdressFireworks), "sound/gamesound/tf2ware/fireworks/firework_explode%i.mp3", i);
			AddFileToDownloadsTable(AdressFireworks);
			Format(AdressFireworks, sizeof(AdressFireworks), "gamesound/tf2ware/fireworks/firework_explode%i.mp3", i);
			PrecacheSound(AdressFireworks);
		}		
		// Add server tag
		AddServerTag("TF2Ware");
		// Load minigames
		char imFile[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, imFile, sizeof(imFile), "configs/minigames.cfg");
			
		MinigameConf = CreateKeyValues("Minigames");
		if (FileToKeyValues(MinigameConf, imFile))
		{
			PrintToServer("Loaded minigames from minigames.cfg");

			KvGotoFirstSubKey(MinigameConf);
			int i=0;
			do
			{
				KvGetSectionName(MinigameConf, g_name[KvGetNum(MinigameConf, "id")-1], 64);
				i++;
			}while (KvGotoNextKey(MinigameConf)); 

			KvRewind(MinigameConf);
		}
		else
		{
			PrintToServer("Failed to load minigames.cfg!");
		}		
		// Add logging
		if (ww_log.BoolValue)
		{
			LogMessage("//////////////////////////////////////////////////////");
			LogMessage("//					   TF2WARE LOG					//");
			LogMessage("//////////////////////////////////////////////////////");
		}
		// Hooks
		HookConVarChange(ww_enable,StartMinigame_cvar);
		//HookConVarChange(ww_disabletaunts,EnableTaunts_cvar);
		HookEvent("post_inventory_application", EventInventoryApplication,	EventHookMode_Post);
		HookEvent("player_death", Player_Death, EventHookMode_Post);
		HookEvent("player_spawn", OnPlayerSpawned, EventHookMode_Post);
		HookEvent("player_team", Player_Team, EventHookMode_Post);
		HookEvent("teamplay_round_start", Event_Roundstart, EventHookMode_PostNoCopy);
		HookEvent("teamplay_game_over", Event_Roundend, EventHookMode_PostNoCopy);
		HookEvent("teamplay_round_stalemate", Event_Roundend, EventHookMode_PostNoCopy);
		HookEvent("teamplay_round_win", Event_Roundend, EventHookMode_PostNoCopy);
		//HookEvent("player_activate", PlayerActive, EventHookMode_Post);
		RegAdminCmd("ww_list", Command_list, ADMFLAG_GENERIC, "Lists all the registered, enabled plugins and their ids");
		RegAdminCmd("ww_give", Command_points, ADMFLAG_GENERIC, "Gives you 20 points - You're a winner! (testing feature)");
		RegAdminCmd("ww_event", Command_event, ADMFLAG_GENERIC, "Starts a debugging event");
		AddNormalSoundHook(view_as<NormalSHook>(SoundHookBlast));
		AddNormalSoundHook(view_as<NormalSHook>(SoundHookBlastDB));
		if(ww_hlstatsx.BoolValue)
		{
			/* HLStatsX events. */
			HookEvent("tf2ware_inc_boss", Event_Win, EventHookMode_Pre);
			HookEvent("tf2ware_inc_pts", Event_Win, EventHookMode_Pre);
			HookEvent("tf2ware_inc_win", Event_GameWin, EventHookMode_Pre);
		}
			
		// Vars
		currentSpeed = ww_speed.FloatValue;
		iMinigame = 1;
		status = 0;
		randommini = 0;
		//randommini2 = 0;
		randomposspawn = 0;
		//randomposspawnboss = 0;
		Roundstarts = 0;
		SetStateAll(false);
		ResetWinners();
		SetMissionAll(0);
			
		// FORWARDS FOR MINIGAMES
		g_OnMapStart = CreateForward(ET_Ignore);
		g_justEntered = CreateForward(ET_Ignore, Param_Cell);
		g_OnAlmostEnd = CreateForward(ET_Ignore);
		g_OnTimerMinigame = CreateForward(ET_Ignore, Param_Cell);
		g_OnEndMinigame = CreateForward(ET_Ignore);
		g_OnGameFrame_Minigames = CreateForward(ET_Ignore);
		g_PlayerDeath = CreateForward(ET_Ignore, Param_Cell);
			
		// MINIGAME REGISTRATION
		RegMinigame("HitEnemy", HitEnemy_OnMinigame);
		RegMinigame("Spycrab", Spycrab_OnMinigame);
		RegMinigame("Kamikaze", Kamikaze_OnMinigame);
		RegMinigame("Math", Math_OnMinigame);
		RegMinigame("SawRun", SawRun_OnMinigame, SawRun_Init);
		RegMinigame("Barrel", Barrel_OnMinigame);
		RegMinigame("Needlejump", Needlejump_OnMinigame);
		RegMinigame("Hopscotch", Hopscotch_OnMinigame);
		RegMinigame("Airblast", Airblast_OnMinigame);
		RegMinigame("Movement", Movement_OnMinigame);
		RegMinigame("Flood", Flood_OnMinigame);
		RegMinigame("SimonSays", SimonSays_OnMinigame);
		RegMinigame("BBall", BBall_OnMinigame);
		RegMinigame("Hugging", Hugging_OnMinigame, Hugging_Init);
		RegMinigame("RedFloor", RedFloor_OnMinigame);
		RegMinigame("SniperTarget", SniperTarget_OnMinigame, SniperTarget_Init);
		RegMinigame("Airraid", Airraid_OnMinigame);
		RegMinigame("JumpRope", JumpRope_OnMinigame);
		RegMinigame("ColorText", ColorText_OnMinigame);
		RegMinigame("Frogger", Frogger_OnMinigame, Frogger_Init);
		RegMinigame("Goomba", Goomba_OnMinigame);
		RegMinigame("Ghostbusters", Ghostbusters_OnMinigame, Ghostbusters_Init);
		RegMinigame("Mandrill", Mandrill_OnMinigame);		
		RegMinigame("Batboy", Batboy_OnMinigame, Batboy_Init);
		RegMinigame("SoldierTarget", SoldierTarget_OnMinigame, SoldierTarget_Init);
		RegMinigame("HHH", HitEnemyhhh_OnMinigame, HitHHH_Init);		
		RegMinigame("Flipperball", Flipper_OnMinigame, Flipper_Init);
		RegMinigame("ScaryGhost", ScaryGhost_OnMinigame, ScaryGhost_Init);
		RegMinigame("Monocolus", Monocolus_OnMinigame);
		RegMinigame("CatchCube", CatchCube_OnMinigame, CatchCube_Init);
		RegMinigame("BossTarget", BossTarget_OnMinigame, BossTarget_Init);
		RegMinigame("MultiJump", MultiJump_OnMinigame);
		RegMinigame("Portals", Portals_OnMinigame, Portals_Init);
		RegMinigame("AvoidTrain", AvoidTrain_OnMinigame);
		RegMinigame("BombRain", BombRain_OnMinigame);
		RegMinigame("LongJumpKart", LongJumpCar_OnMinigame);
		RegMinigame("KartRace", KartRace_OnMinigame);
		RegMinigame("Boss_Kart", BossKart_OnMinigame);
		RegMinigame("KartPush", KartPush_OnMinigame);
		RegMinigame("TauntKill", TauntKill_OnMinigame);
		RegMinigame("Paracadute", Paracadute_OnMinigame, Paracadute_Init);
		RegMinigame("WaterWar", WaterWar_OnMinigame);
		RegMinigame("Spycrab_Limbo", Limbo_OnMinigame, Limbo_Init);
		RegMinigame("Gioca_Jouer", Jouer_OnMinigame);
		RegMinigame("MadGear", MadGear_OnMinigame);
		RegMinigame("Shark", SharkGame_OnMinigame);
		RegMinigame("BuildSentry", BuildSentry_OnMinigame);
		RegMinigame("Grap_the_cow", Grap_Cow_OnMinigame);
		RegMinigame("FlipperBallBack", FlipperBack_OnMinigame, FlipperBack_Init);
		RegMinigame("Slender", Slender_OnMinigame, Slender_Init);
		RegMinigame("ClockGame", ClockGame_OnMinigame);
		RegMinigame("Treasure_Hunt", Treasure_Hunt_OnMinigame, Treasure_Hunt_Init);
		RegMinigame("PiggyBack_Game", PiggyBack_OnMinigame);
		RegMinigame("MeraStun_Game", MeraStun_OnMinigame);
		RegMinigame("PushBall_Game", PushBall_OnMinigame);
		RegMinigame("Spy_Disguise", SpyDisguise_OnMinigame);
		RegMinigame("PassTIme_Break", BreakPassTime_OnMinigame);
		RegMinigame("Gp_Race", GpRace_OnMinigame, GpRace_Init);
		RegMinigame("Dodgeball", Dodgeball_OnMinigame);
		RegMinigame("StreetFighter", StreetFighter_OnMinigame);
		RegMinigame("PickupPlate", Pick_up_plate_OnMinigame);
		RegMinigame("PirateAttack", PirateAttack_OnMinigame);
		RegMinigame("MeetChester", MeetChester_OnMinigame);

		// CHEATS
		//HookConVarChange(FindConVar("sv_cheats"), OnConVarChanged_SvCheats);
		HookConVarChange(g_Cvar_GameDescription, Cvar_GameDescription);
		ww_allowedCommands = CreateArray(64);
		SetCommandFlags("host_timescale", GetCommandFlags("host_timescale") & (~FCVAR_CHEAT));
		SetCommandFlags("phys_timescale", GetCommandFlags("phys_timescale") & (~FCVAR_CHEAT));
		SetCommandFlags("r_screenoverlay", GetCommandFlags("r_screenoverlay") & (~FCVAR_CHEAT));
		if(ww_skybox.BoolValue){
			FormatTime(Hour, sizeof(Hour), "%H", GetTime());
			int hour = StringToInt(Hour);
			if(ww_skybox_more.BoolValue){
				if(hour >=0 && hour <=5)ServerCommand("sv_skyname sky_night_01");
				if(hour >=6 && hour <=9)ServerCommand("sv_skyname sky_well_01");
				if(hour >=10 && hour <=12)ServerCommand("sv_skyname sky_goldrush_01");
				if(hour >=12 && hour <=16)ServerCommand("sv_skyname sky_hydro_01");
				if(hour >=17 && hour <=20)ServerCommand("sv_skyname sky_badlands_01");
				if(hour >=21 && hour <=23)ServerCommand("sv_skyname sky_night_01");
			}
			else{ 
				if(hour >=0 && hour <=5)ServerCommand("sv_skyname sky_night_01");
				if(hour >=19 && hour <=23)ServerCommand("sv_skyname sky_night_01");
				if(hour >=6 && hour <=18)ServerCommand("sv_skyname sky_tf2_04");
			}
		}
		else{ 
			ServerCommand("sv_skyname sky_tf2_04");
		}
			
		PushArrayString(ww_allowedCommands, "host_timescale");
		PushArrayString(ww_allowedCommands, "r_screenoverlay");
		
		DestroyAllBarrels();
			
		// HUD
		ResetScores();
		if(TrHud.BoolValue){
			ShowMessage = false;
		 
			GameRules = FindEntityByClassname(-1, "tf_gamerules");
			if(GameRules <= 0)
			{
				GameRules = CreateEntityByName("tf_gamerules");
			}
				m_bIsInTrainingOffset = FindSendPropInfo("CTFGameRulesProxy", "m_bIsInTraining");
				m_bIsTrainingHUDVisibleOffset = FindSendPropInfo("CTFGameRulesProxy", "m_bIsTrainingHUDVisible");
				
        		// SetEntData(m_bIsInTrainingOffset, 1, 1, true);
        		// SetEntData(m_bIsTrainingHUDVisibleOffset, 1, 1, true);
		}
			
		// Remove Notification Flags
		RemoveNotifyFlag("sv_tags");
		RemoveNotifyFlag("mp_respawnwavetime");
		RemoveNotifyFlag("mp_friendlyfire");
		RemoveNotifyFlag("tf_tournament_hide_domination_icons");
		RemoveNotifyFlag("mp_teams_unbalance_limit");
		RemoveNotifyFlag("mp_autoteambalance");
		RemoveNotifyFlag("mp_match_end_at_timelimit");
		RemoveNotifyFlag("tf_flamethrower_burstammo");
		RemoveNotifyFlag("tf_bot_count");
		RemoveNotifyFlag("tf_grapplinghook_enable");
		RemoveNotifyFlag("spec_freeze_time");
		SetConVarInt(FindConVar("tf_tournament_hide_domination_icons"), 1, true);
		SetConVarInt(FindConVar("mp_friendlyfire"), 1);
		SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
		SetConVarInt(FindConVar("mp_autoteambalance"), 1);
		SetConVarInt(FindConVar("mp_match_end_at_timelimit"), 1);
		ServerCommand("sm_cvar tf_halloween_kart_fast_turn_speed  120");
		ServerCommand("sm_cvar mp_bonusroundtime 5.0");
		//ServerCommand("sv_tags tf2ware, warioware");
		cvarDeathCam = FindConVar("spec_freeze_time");
		HookConVarChange(cvarDeathCam, CVAR_DeathCam_Changed); 
		SetConVarInt(cvarDeathCam, -1);

		if (ww_log.BoolValue)
		{
			LogMessage("Calling OnMapStart Forward");
		}

		Call_StartForward(g_OnMapStart);
		Call_Finish();

		precacheSound(MUSIC_START);
		precacheSound(MUSIC_WIN);
		precacheSound(MUSIC_FAIL);
		precacheSound(MUSIC_SPEEDUP);
		precacheSound(MUSIC_BOSS);
		precacheSound(MUSIC_GAMEOVER);

		precacheSound(MUSIC2_START);
		precacheSound(MUSIC2_WIN);
		precacheSound(MUSIC2_FAIL);
		precacheSound(MUSIC2_SPEEDUP);
		precacheSound(MUSIC2_BOSS);
		precacheSound(MUSIC2_GAMEOVER);

		precacheSound(MUSIC_WAITING);
		precacheSound(MUSIC_SPECIAL);
		precacheSound(SOUND_BONUS);
		precacheSound(WIN_VOICE);
		precacheSound(LOSE_VOICE);

		precacheSound(SOUND_COMPLETE);
		precacheSound(SOUND_COMPLETE_YOU);
		precacheSound(SOUND_MINISCORE);
		precacheSound(SOUND_SELECT);

		precacheSound(FROGGER_JUMP);
		precacheSound(FROGGER_DEAD);

		PrecacheModel("models/props_farm/wooden_barrel.mdl", true);
		PrecacheModel("models/props_farm/gibs/wooden_barrel_break02.mdl", true);
		PrecacheModel("models/props_farm/gibs/wooden_barrel_chunk02.mdl", true);
		PrecacheModel("models/props_farm/gibs/wooden_barrel_chunk04.mdl", true);
		PrecacheModel("models/props_farm/gibs/wooden_barrel_chunk03.mdl", true);
		PrecacheModel("models/props_farm/gibs/wooden_barrel_chunk01.mdl", true);
		PrecacheModel("models/props_2fort/cow001_reference.mdl", true);
		PrecacheModel("models/props_forest/sawblade_moving.mdl", true);
		PrecacheModel("materials/sprites/laser.vmt", true);

		PrecacheModel("models/props_vehicles/train_enginecar.mdl", true);
		PrecacheModel("models/player/demo.mdl", true);

		PrecacheModel("models/humans/group01/female_01.mdl", true); //Simple_bots default model
		PrecacheModel("models/props_halloween/ghost.mdl", true);	//Ghost model itself
		PrecacheModel("ghost_appearation", true);					//Ghost appear & disappear particle

		PrecacheModel("models/props_manor/doorframe_01_door_01.mdl", true);
		PrecacheModel("models/props_gameplay/sign_wood_cap001.mdl", true);

		PrecacheModel(SHARK_MODEL, true);	
		PrecacheModel(BANANA_HEAVY_MODEL, true);
		PrecacheModel(WW_BOMB_SCOUT_MODEL, true);
		PrecacheModel(WW_BOMB_MODEL, true);
		PrecacheSound(WW_BOMB, true);

		PrecacheModel("models/props_training/target_heavy.mdl", true);
		PrecacheModel("models/props/metal_box.mdl", true);
		PrecacheModel("models/props_halloween/hwn_kart_ball01.mdl", true);
		PrecacheModel("models/player/geavy.mdl", true);
		PrecacheModel("models/freak_fortress_2/wario/wario5.mdl",true);
		PrecacheGeneric("materials/ww_overlays/...", true);
		AddFileToDownloadsTable("materials/ww_overlays/...");
		PrecacheModel( "models/weapons/...", true);
		PrecacheModel(SPYCRAB_SKIN, true);

		PrecacheModel("models/player/items/taunts/bumpercar/parts/bumpercar.mdl");
		PrecacheModel("models/player/items/taunts/bumpercar/parts/bumpercar_nolights.mdl");

		PrecacheSound(")weapons/bumper_car_accelerate.wav");
		PrecacheSound(")weapons/bumper_car_decelerate.wav");
		PrecacheSound(")weapons/bumper_car_decelerate_quick.wav");
		PrecacheSound(")weapons/bumper_car_go_loop.wav");
		PrecacheSound(")weapons/bumper_car_hit_ball.wav");
		PrecacheSound(")weapons/bumper_car_hit_ghost.wav");
		PrecacheSound(")weapons/bumper_car_hit_hard.wav");
		PrecacheSound(")weapons/bumper_car_hit_into_air.wav");
		PrecacheSound(")weapons/bumper_car_jump.wav");
		PrecacheSound(")weapons/bumper_car_jump_land.wav");
		PrecacheSound(")weapons/bumper_car_screech.wav");
		PrecacheSound(")weapons/bumper_car_spawn.wav");
		PrecacheSound(")weapons/bumper_car_spawn_from_lava.wav");
		PrecacheSound(")weapons/bumper_car_speed_boost_start.wav");
		PrecacheSound(")weapons/bumper_car_speed_boost_stop.wav");
		PrecacheSound("weapons/knife_swing.wav", true);
		AddFileToDownloadsTable("sound/gamesound/tf2ware/kenryu.mp3");
		PrecacheSound("gamesound/tf2ware/kenryu.mp3", true);

		char name[64];
		for(int i = 1; i <= 8; i++)
		{
			FormatEx(name, sizeof(name), "weapons/bumper_car_hit%d.wav", i);
			PrecacheSound(name);
		}
		hEntStack = CreateStack();		

		char input[512];

		KvGotoFirstSubKey(MinigameConf);
		int id;
		int enable;
		int i=1;
		if (ww_log.BoolValue)
		{
			LogMessage("--Adding the following to downloads table from information in minigames.cfg:", input);
		}
		do
		{
			id = KvGetNum(MinigameConf, "id");
			enable = KvGetNum(MinigameConf, "enable", 1);
			if (enable == 1)
			{
				Format(input, sizeof(input), "gamesound/tf2ware/minigame_%d.mp3", id);
				if (ww_log.BoolValue)
				{
					LogMessage("%s", input);
				}
				precacheSound(input);
			}
			i++;
		}while (KvGotoNextKey(MinigameConf)); 

		KvRewind(MinigameConf);

		white = PrecacheModel("materials/sprites/white.vmt");
		g_HaloSprite = PrecacheModel("materials/sprites/halo01.vmt");
		g_ExplosionSprite = PrecacheModel("sprites/sprite_fire01.vmt");

		PrecacheSound( "ambient/explosions/explode_8.wav", true);
		SetConVarFloat(ww_speed, 1.0);
		ResetScores();
		bossBattle = 0;
		Roundstarts = 0;

		SpecialPrecache();
		SDK_Init();

		if (ww_log.BoolValue)
		{
			LogMessage("Map started");
		}
	}
}
public void OnAllPluginsLoaded()
{
	g_bSteamWorks = LibraryExists("SteamWorks");
}

public void OnLibraryAdded(const char[] name)
{
	if (StrEqual(name, "SteamWorks", false))
	{
		g_bSteamWorks = true;
	}
}

public void OnLibraryRemoved(const char[] name)
{
	if (StrEqual(name, "SteamWorks", false))
	{
		g_bSteamWorks = false;
	}
}
public void OnConfigsExecuted()
{	
		UpdateGameDescription(true);
}
public void Cvar_GameDescription(Handle convar, const char[] oldValue, const char[] newValue)
{
	UpdateGameDescription();
}
void UpdateGameDescription(bool bAddOnly=false)
{
	if (g_bSteamWorks)
	{
		char gamemode[64];
		if (g_Cvar_GameDescription.BoolValue)
		{
			Format(gamemode, sizeof(gamemode), "TF2WARE2 v.%s", PLUGIN_VERSION);
		}
		else if (bAddOnly)
		{
			
			return;
		}
		else
		{
			strcopy(gamemode, sizeof(gamemode), "Team Fortress");
		}
		SteamWorks_SetGameDescription(gamemode);
	}
}
void SDK_Init()
{
	Handle hGamedata = LoadGameConfigFile("merasmus_stun");
	StartPrepSDKCall(SDKCall_Entity);
	PrepSDKCall_SetFromConf(hGamedata, SDKConf_Signature, "CMerasmus::AddStun");
	PrepSDKCall_AddParameter(SDKType_CBasePlayer, SDKPass_Pointer);
	g_hSDKStunMerasmus = EndPrepSDKCall();
	if(g_hSDKStunMerasmus == INVALID_HANDLE)
		PrintToServer("[Benoist Gamedata] Could not find CMerasmus::AddStun!");
	CloseHandle(hGamedata);
}
void SDK_StunMerasmus(int iMerasmus, int iPlayer)
{
	if(g_hSDKStunMerasmus != INVALID_HANDLE)
	{
		SDKCall(g_hSDKStunMerasmus, iMerasmus, iPlayer);
	}
}
public void CVAR_DeathCam_Changed(Handle cvar, const char[] oldVal, const char[] newVal)
{
	if (StringToInt(newVal) != -1)
	{
		SetConVarInt(cvar, -1); 
	}
}
public Action Timer_DisplayVersion(Handle timer, any client)
{
	if (IsValidClient(client))
	{
		SetHudTextParams(0.68,0.73,25.0,255,0,0,0,2,3.0,0.0,3.0);
		ShowHudText(client,1,"v%s", PLUGIN_VERSION);
		char buffer[128];
		PrintToChat(client, "%T", "NOTIFY_TF2WARE_VERSION", client, "\x04", PLUGIN_VERSION,client,	"\x01",client,	"\x03, client");
		Format(buffer, sizeof(buffer), "%T", "NOTIFY_TF2WARE_VERSION", LANG_SERVER, "\x04", PLUGIN_VERSION, LANG_SERVER, "\x01", LANG_SERVER, "\x03", LANG_SERVER);
		if (g_Cvar_Snowball.BoolValue){
			CPrintToChat(client," %t\x01 %t \x04%t \x05%t \x01 %t \x04 %t", "TF2WARE_PREFIX_COLOR", "PRESS", "BUTTON", "ATTACK_BUTTON_3", "TO_USE","SNOWBALLS");
		}
	}
	return Plugin_Handled;
}
public Action BlockTaunt(int client, const char[] command, int argc)
{
	if(TauntDisable.BoolValue)
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}
public Action Command_JoinTeam(int client, int args){
	if(IsValidClient(client) && GetClientTeam(client) == 1){
		int team = GetRandomInt(2,3);
		ChangeClientTeam(client, team);
	}
	
	return Plugin_Handled;
}
public Action Event_Roundstart(Event event,const char[] name,bool dontBroadcast)
{
	if (g_enabled && ww_enable.BoolValue)
	{
		if ( Roundstarts == 0 )
		{
			g_waiting = true;
			SetGameMode();
			RemoveAllParticipants();
			
		}
		if ( Roundstarts == 1 )
		{
			g_waiting = false;
			SetGameMode();
			ResetScores();
			StartMinigame();
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && g_Spawned[i])
				{
					if (!IsFakeClient(i))
					{
						StopSound(i, SND_CHANNEL_SPECIFIC, MUSIC_WAITING);
						SetOverlay(i, "");					
					}
					if (g_Gamemode == GAMEMODE_WIPEOUT) 
					{
						SetWipeoutPosition(i, true);
						if(IsFakeClient(i))
						{
							ServerCommand("tf_bot_quota 1");					
						}
					}
				}
			}
			if (ww_log.BoolValue)
			{
				LogMessage("Waiting-for-players period has ended");
			}
		}
	}
	Roundstarts++;
	
	return Plugin_Continue;
}

public Action Event_Roundend(Event event,const char[] name,bool dontBroadcast)
{
	if (g_enabled && ww_enable.BoolValue)
	{
		g_enabled = false;
		if (ww_log.BoolValue)
		{
			LogMessage("== ROUND ENDED SUCCESSFULLY == ");
		}
		ServerCommand("ww_gamedescription 0.0");
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && (!(IsFakeClient(i))))
			{
				SetOverlay(i,"tf2ware_minigame_gameover");
			}
		}
		SetConVarInt(FindConVar("tf_tournament_hide_domination_icons"), 0, true);
		SetConVarInt(FindConVar("mp_friendlyfire"), 0);
		SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
		SetConVarInt(FindConVar("mp_autoteambalance"), 1);
		SetConVarInt(FindConVar("mp_match_end_at_timelimit"), 0);
	}
	
	return Plugin_Continue;
}

public void OnClientPostAdminCheck(int client)
{
	if (!g_enabled)
	{
		return;
	}
	//UpdateClientCheatValue();
	g_Points[client] = GetAverageScore();
	if (g_Gamemode == GAMEMODE_WIPEOUT)
	{
		g_Points[client] = -1;
	}
	char ip[32];
	GetClientIP(client, ip, sizeof(ip));
	g_Country[client] = 0;
	
	//if (StrEqual(country, "IT"))
	//{
	//	g_Country[client] = 1;
	//}
	if (ww_log.BoolValue)
	{
		LogMessage("Client post admin check. Country: %d", g_Country[client]);
	}
}

public void OnClientPutInServer(int client)
{
	if (!g_enabled)
	{
		return;
	}
	if (ww_log.BoolValue)
	{
		LogMessage("Client put in server and hooked");
	}
	SDKHook(client, SDKHook_PreThink, OnPreThink);
	SDKHook(client, SDKHook_OnTakeDamage, OnTakeDamageClient);
	SDKHook(client, SDKHook_Touch, Special_NoTouch);
	SDKHook(client, SDKHook_OnTakeDamage, Special_DamagePush);
	SDKHook(client, SDKHook_PreThink, Special_BigWeapons);	  
	SDKHook(client, SDKHook_PostThink, OnPostThink);
	//SDKHook(client, SDKHook_OnTakeDamage, TauntKill_OnTakeFix);
	FakeClientCommand(client, "explode");
	char buffer[128];
	PrintToChat(client, "%T", "NOTIFY_TF2WARE_VERSION", client, "\x04", PLUGIN_VERSION,client,	"\x01",client,	"\x03, client");
	Format(buffer, sizeof(buffer), "%T", "NOTIFY_TF2WARE_VERSION", LANG_SERVER, "\x04", PLUGIN_VERSION, LANG_SERVER, "\x01", LANG_SERVER, "\x03", LANG_SERVER);
	if (bossBattle == 1)
	{
		if (IsValidClient(client) && (!(IsFakeClient(client))))
		{
			SetOverlay(client,"tf2ware_waitboss_2");
		}
	}
	if (bossBattle == 0)
	{
		if (IsValidClient(client) && (!(IsFakeClient(client))))
		{
			SetOverlay(client,"tf2ware_welcome_2");
		}
	}
	if(TrHud.BoolValue){
		ChangeClientTeam(client, 1);
		CreateTimer(3.0, respawnclient, client);
		TF2_RespawnPlayer(client);
		InitializeClient(client);
	}
}
public Action respawnclient(Handle timer , any client) {
	if (IsValidClient(client)){
		TF2_RespawnPlayer(client);
	}
	return Plugin_Stop;
}
public Action Notification(Handle hTimer) {
	if (ww_advert.FloatValue > 0.0) {
		if (ww_enable.BoolValue) CPrintToChatAll(infoText); 
		CreateTimer(ww_advert.FloatValue, Notification);
	}
	return Plugin_Stop;
}
public void OnClientDisconnect(int client)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Client disconnected");
	}
	g_Spawned[client] = false;
	g_IsModel[client] = false;
	g_bIsHHH[client] = false;
	ClearHorsemannParticles(client);
	if ((g_Sprites[client]) > 0)
	{
		DestroySprite(client);
	}
	if (g_piggy[client]){
		SDKUnhook(client, SDKHook_PreThink, OnPreThinkPiggy);
		SDKUnhook(client, SDKHook_OnTakeDamage, PiggyBack_Hurt);
		g_piggy[client] = false;
	}
	if ( !IsFakeClient(client) && userInit[client] == 1)
	{		
		if (g_hSQL != INVALID_HANDLE)
		{
			saveUser(client);
			userInit[client] = 0;
		}
	}
}
public void OnPostThink(int client)
{
	int ent; float vOrigin[3]; float vVelocity[3];
	
	for(int i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i)) continue;
		if ((ent = g_Sprites[i]) > 0)
		{
			if (!IsValidEntity(ent))
			{
				g_Sprites[i] = 0;
			}
			else if ((ent = EntRefToEntIndex(ent)) > 0)
			{
				GetClientEyePosition(i, vOrigin);
				vOrigin[2] += 25.0;
				GetEntDataVector(i, gVelocityOffset, vVelocity);
				TeleportEntity(ent, vOrigin, NULL_VECTOR, vVelocity);
			}
		}
	}
}

public Action OnTakeDamageClient(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
	
	if ((g_Winner[victim] >= 1) && (status != 2))
	{
		damage = 0.0;
		return Plugin_Changed;
	}
	
	if (IsValidClient(attacker) && (g_Winner[attacker] == 1) && (g_Winner[victim] == 0) && IsValidClient(victim) && IsPlayerAlive(victim))
	{
		damage = 750.0;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

public int OnPreThink(int client)
{
	int iButtons = GetClientButtons(client);
	float fVelplayer[ 3 ];
	GetEntPropVector(client, Prop_Data, "m_vecVelocity", fVelplayer);
	if ((status != 2) && ww_enable.BoolValue && g_enabled && (g_Winner[client] == 0) && !(SpecialRound == 6 && status != 5))
	{
		if ((iButtons & IN_ATTACK2) || (iButtons & IN_ATTACK))
		{
			iButtons &= ~IN_ATTACK;
			iButtons &= ~IN_ATTACK2;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
		}
	}	
	if ((status == 2) && (g_attack == false || !IsClientParticipating(client)) && ww_enable.BoolValue && g_enabled)
	{
		if ((iButtons & IN_ATTACK2) || (iButtons & IN_ATTACK))
		{
			iButtons &= ~IN_ATTACK;
			iButtons &= ~IN_ATTACK2;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
		}
	}
	if ((status != 2) && ww_enable.BoolValue && g_enabled && (g_Winner[client] == 1) && !(SpecialRound == 6 && status != 5) && !(SpecialRound == 9))
	{
		if (IsValidClient(client))
		{
			SetEntPropFloat(client, Prop_Send, "m_flHandScale", 3.0);
		}
	}
	if ((status != 2) && ww_enable.BoolValue && g_Cvar_Snowball.BoolValue && g_enabled && (g_Winner[client] == 0) && !(SpecialRound == 6 && status != 5)){
		if(GetClientButtons(client) & IN_ATTACK3){
			if((IsValidClient(client)) && (IsPlayerAlive(client))){
				int currentTime = GetTime();
				if (currentTime - LastSnowBallUsed[client] < 1.5)
					return false;
				LastSnowBallUsed[client] = GetTime();
				int iBall = CreateEntityByName("tf_projectile_stun_ball");
				if(IsValidEntity(iBall))
				{
					float vPosition[3];
					float vAngles[3];
					float flSpeed = 1500.0;
					float vVelocity[3];
					float vBuffer[3];
					GetClientEyePosition(client, vPosition);
					GetClientEyeAngles(client, vAngles);
						
					GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
						
					vVelocity[0] = vBuffer[0]*flSpeed;
					vVelocity[1] = vBuffer[1]*flSpeed;
					vVelocity[2] = vBuffer[2]*flSpeed;
					SetEntPropVector(iBall, Prop_Data, "m_vecVelocity", vVelocity);
					SetEntPropEnt(iBall, Prop_Send, "m_hOwnerEntity", client);
					SetEntProp(iBall, Prop_Send, "m_iTeamNum", GetClientTeam(client));
					SetVariantString("OnUser3 !self:FireUser4::3.0:1");
					AcceptEntityInput(iBall, "AddOutput");
					HookSingleEntityOutput(iBall, "OnUser4", BallBreak, false);
					AcceptEntityInput(iBall, "FireUser3");
					DispatchSpawn(iBall);
					EmitSoundToClient(client, "weapons/knife_swing.wav");
					TeleportEntity(iBall, vPosition, vAngles, vVelocity);
					CreateParticle(iBall, "xms_icicle_melt", true, 3.0);
					SetEntityRenderColor(iBall, 190, 251, 250, 255);
				}
			}
			return true;
		}
		return true;
	}
	return true;
}
public Action OnPlayerRunCmd(int client, int &buttons, int &Impulse, float Vel[3], float Angles[3], int &Weapon)
{
	int iButtons = GetClientButtons(client);
	if ((status != 2) && ww_enable.BoolValue && g_enabled && (g_Winner[client] == 0) && !(SpecialRound == 6 && status != 5))
	{
		if ((iButtons & IN_ATTACK2) || (iButtons & IN_ATTACK))
		{
			iButtons &= ~IN_ATTACK;
			iButtons &= ~IN_ATTACK2;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
		}
	}	
	if ((status == 2) && (g_attack == false || !IsClientParticipating(client)) && ww_enable.BoolValue && g_enabled)
	{
		if ((iButtons & IN_ATTACK2) || (iButtons & IN_ATTACK))
		{
			iButtons &= ~IN_ATTACK;
			iButtons &= ~IN_ATTACK2;
			SetEntProp(client, Prop_Data, "m_nButtons", iButtons);
		}
	}
	return Plugin_Continue;
}

public void EventInventoryApplication(Event event, const char[] name, bool dontBroadcast)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Client post inventory");
	}
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (g_Spawned[client] == false && g_waiting && ww_enable.BoolValue && g_enabled && !IsFakeClient(client))
	{
		EmitSoundToClient(client, MUSIC_WAITING, SOUND_FROM_PLAYER, SND_CHANNEL_SPECIFIC);
		SetOverlay(client, "tf2ware_welcome_2");
		CreateTimer(0.25, Timer_DisplayVersion, client);
	}
	g_Spawned[client] = true;
	if (ww_enable.BoolValue && g_enabled)
	{
		SetEntityRenderMode(client, RENDER_NORMAL);
		SetEntityRenderColor(client, 255, 255, 255, 255);
		SetEntProp(client, Prop_Send, "m_bGlowEnabled", 0);
	
		if ((status != 2) && (g_Winner[client] == 0))
		{
			DisableClientWeapons(client);
		}
		if (status == 2 && IsClientParticipating(client))
		{
			Call_StartForward(g_justEntered);
			Call_PushCell(client);
			Call_Finish();
			//CreateSprite(client);
		}		
		if ((status == 2 && g_attack) || (g_Winner[client] > 0) || (SpecialRound == 6))
		{
			SetWeaponState(client, true);
		}
		else
		{
			SetWeaponState(client, false);
		}
		
		//HandlePlayerItems(client);
		
		if (SpecialRound == 4)
		{
			SetEntityRenderMode(client, RENDER_TRANSCOLOR);
			SetEntityRenderColor(client, 255, 255, 255, 0);
		}
		if (SpecialRound == 8)
		{
			SDKHook(client, SDKHook_PreThink, Special_Spawn);
		}
		if (SpecialRound == 9)
		{
			SDKHook(client, SDKHook_PreThink, Special_BigWeapons);
		}
		
		if (g_Gamemode == GAMEMODE_WIPEOUT && g_waiting == false)
		{
			if (status == 2 && IsClientParticipating(client))
			{
				// do nothing
			}
			else
			{
				SetWipeoutPosition(client, true);
			}
			HandleWipeoutLives(client);
		}
	}
}
// public void TF2Items_OnGiveNamedItem_Post(int client, char[] classname, int index, int level, int quality, int entity)
// {
//     switch (index)
//     {
//          case 735, 736, 810, 831, 933, 1102, 1080:
//          CreateTimer(0.1, Timer_RemoveEntity, EntIndexToEntRef(entity));
//     }
// }

public Action Timer_RemoveEntity(Handle timer, any ref) {
    int entity = EntRefToEntIndex(ref);
    if (entity <= MaxClients) return Plugin_Continue;
    AcceptEntityInput(entity, "Kill");
    return Plugin_Continue;
}
void precacheSound(char[] Sounds)
{
	char buffer[128];
	PrecacheSound(Sounds, true);
	Format(buffer, sizeof(buffer), "sound/%s", Sounds);
	AddFileToDownloadsTable(buffer);
}

public void StartMinigame_cvar(Handle cvar, const char[] oldVal, const char[] newVal)
{
	if (ww_enable.BoolValue && g_enabled)
	{
		StartMinigame();
		SetConVarInt(FindConVar("mp_respawnwavetime"), 199);
		SetConVarInt(FindConVar("mp_forcecamera"), 0);
	}
	else
	{
		SetCommandFlags("host_timescale", GetCommandFlags("host_timescale") & (~FCVAR_CHEAT));
		SetCommandFlags("phys_timescale", GetCommandFlags("phys_timescale") & (~FCVAR_CHEAT));
		ServerCommand("host_timescale %f", 1.0);
		ServerCommand("phys_timescale %f", 1.0);
		ResetConVar(FindConVar("mp_respawnwavetime")); 
		ResetConVar(FindConVar("mp_forcecamera")); 
		status = 0;
	}
}
/*
public EnableTaunts_cvar(Handle:cvar, const String:oldVal[], const String:newVal[])
{
	if (ww_disabletaunts.BoolValue)
	{
		g_UnLockTaunt = true;
	}
}
*/
public void OnGameFrame()
{
	if (ww_enable.BoolValue && g_enabled && (status == 2) && (g_OnGameFrame_Minigames != INVALID_HANDLE))
	{
		Call_StartForward(g_OnGameFrame_Minigames);
		Call_Finish();
		
		if (g_Gamemode == GAMEMODE_WIPEOUT && status == 1)
		{
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && IsPlayerAlive(i) && IsClientParticipating(i))
				{
					float pos[3];
					GetClientAbsOrigin(i, pos);
					pos[2] -= 25.0;
					if (pos[2] < GAMEMODE_WIPEOUT_HEIGHT)
					{
						pos[2] = GAMEMODE_WIPEOUT_HEIGHT;
					}
					TeleportEntity(i, pos, NULL_VECTOR, NULL_VECTOR);
				}
			}
		}		
	}
}

public Action StartMinigame_timer(Handle hTimer)
{
	if (status == 0)
	{
		StartMinigame();
	}
	return Plugin_Stop;
}

public Action StartMinigame_timer2(Handle hTimer)
{
	if (status == 10)
	{
		status = 0;
		StartMinigame();
	}
	return Plugin_Stop;
}

int RollMinigame()
{
	if (ww_log.BoolValue && bossBattle != 1)
	{
		LogMessage("Rolling normal microgame...");
	}
	if (ww_log.BoolValue && bossBattle == 1)
	{
		LogMessage("Rolling boss microgame...");
	}
	Handle roll = CreateArray();
	bool accept = false;
	int out = 1;
	int iplayers = GetActivePlayers();
	for (int i = 1; i <= sizeof(g_name); i++)
	{
		if (StrEqual(g_name[i-1], ""))
		{
			continue;
		}
		accept = true;
		int gameisboss = GetMinigameConfNum(g_name[i-1], "boss", 0);
		int minplayers = GetMinigameConfNum(g_name[i-1], "minplayers", 1);
		int maxplayers = GetMinigameConfNum(g_name[i-1], "maxplayers", 2);
		//PrintToChatAll("max , %i min, %i", maxplayers , minplayers);
		if(iplayers > maxplayers)
		{
			accept = true;
		}
		if (iplayers < minplayers)
		{
			accept = false;
		}
		if ((bossBattle == 1) && (!gameisboss))
		{
			accept = false;
		}
		if ((bossBattle != 1) && (gameisboss))
		{
			accept = false;
		}
		if (i == g_lastminigame)
		{
			accept = false;
		}
		if (i == g_lastboss)
		{
			accept = false;
		}
		if (!GetMinigameConfNum(g_name[i-1], "enable", 1))
		{
			accept = false;
		}
		for (int j=0; j<GetMinigameConfNum(g_name[i-1], "chance", 1); j++)
		{
			if (accept)
			{
				PushArrayCell(roll, i);
			}
		}
		if (ww_log.BoolValue && (accept))
		{
			LogMessage("-- Microgame #%d allowed", i);
		}
		if (ww_log.BoolValue && (accept == false))
		{
			LogMessage("-- Microgame #%d NOT allowed", i);
		}
	}
	if (GetArraySize(roll) > 0)
	{
		out = GetArrayCell(roll, GetRandomInt(0, GetArraySize(roll)-1));
		//PrintToChatAll("ID %i", out);
	}
	CloseHandle(roll);
	
	int force = ww_force.IntValue;
	if (force > 0)
	{
		if (force-1 < sizeof(g_name) && !StrEqual(g_name[force-1], ""))
		{
			out = ww_force.IntValue;
		}
		else
		{
			PrintToServer("Warning: Couldn't find a game with id %d, continuing with random roll.", ww_force.IntValue);
		}
	}
	
	if (ww_log.BoolValue)
	{
		LogMessage("Rolled microgame was: %s (id:%d)", g_name[out-1], out);
	}
	if (ww_log.BoolValue)
	{
		LogMessage("Roll end");
	}
	return out;
}

public void Player_Team(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	int oldteam = GetEventInt(event, "oldteam");
	int newteam = GetEventInt(event, "team");
	
	if (ww_log.BoolValue)
	{
		LogMessage("%N changed team", client);
	}
	if (ww_enable.BoolValue && g_enabled)
	{
		CreateTimer(0.1, StartMinigame_timer);
		if (oldteam < 2 && newteam >= 2)
		{
			GiveSpecialRoundInfo();
		}
	}
}

void HandOutPoints()
{
	if (ww_log.BoolValue)
	{
		LogMessage("Handing out points");
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		int points = 1;
		if ((IsValidClient(i)) && IsClientParticipating(i))
		{
			if (g_Complete[i])
			{
				if (g_Gamemode == GAMEMODE_NORMAL)
				{
					if (bossBattle == 1)
					{
						/* Set the points to 5. */
						points = 5;
						if(ww_hlstatsx.BoolValue){
							/* Create the boss win event. */
							Handle hWinEvent = CreateEvent("tf2ware_inc_boss");
							
							if (hWinEvent != null)
							{
								/* Set the information. */
								SetEventInt(hWinEvent, "userid", GetClientUserId(i));
								SetEventInt(hWinEvent, "points", points);
								
								/* Fire the event. */
								FireEvent(hWinEvent);
							}
						}
					}
					else
					{
						if(ww_hlstatsx.BoolValue){
							/* Create the microgame win event. */
							Handle hWinEvent = CreateEvent("tf2ware_inc_pts");
							
							if (hWinEvent != null)
							{
								/* Set the information. */
								SetEventInt(hWinEvent, "userid", GetClientUserId(i));
								SetEventInt(hWinEvent, "points", points);
								
								/* Fire the event. */
								FireEvent(hWinEvent);
							}
						}
					}
					g_Points[i] += points;
				}
			}
			else
			{
				if (g_Gamemode == GAMEMODE_WIPEOUT && g_Points[i] > 0)
				{
					g_Points[i] -= points;
					if (g_Points[i] < 0)
					{
						g_Points[i] = 0;
					}
					HandleWipeoutLives(i, true);
				}
			}
		}
		g_Complete[i] = false;
	}
}

void StartMinigame()
{
	if (ww_enable.BoolValue && g_enabled && (status == 0) && g_waiting == false)
	{
		if (ww_log.BoolValue)
		{
			LogMessage("Starting microgame %s! Status = 0", minigame);
		}
		SetConVarInt(FindConVar("mp_respawnwavetime"), 199);
		SetConVarInt(FindConVar("mp_friendlyfire"), 1);
		
		float MUSIC_INFO_LEN = MUSIC_START_LEN;
		char MUSIC_INFO[PLATFORM_MAX_PATH];
		Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC_START);
		if (g_Gamemode == GAMEMODE_WIPEOUT)
		{
			MUSIC_INFO_LEN = MUSIC2_START_LEN;
			Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC2_START);
		}
		//RespawnAll();
		for (int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i) && !IsPlayerAlive(i) && IsClientParticipating(i)){
				//RespawnClient(i, false, false);
				RespawnClientGeneric(i);
			}
		}
		RemoveAllParticipants();
		UpdateHud(GetSpeedMultiplier(MUSIC_INFO_LEN));
		if (SpecialRound == 4)
		{
			NoCollision(true);
		}

		currentSpeed = ww_speed.FloatValue;
		SetCommandFlags("host_timescale", GetCommandFlags("host_timescale") & (~FCVAR_CHEAT));
		SetCommandFlags("phys_timescale", GetCommandFlags("phys_timescale") & (~FCVAR_CHEAT));
		ServerCommand("host_timescale %f", GetHostMultiplier(1.0));
		ServerCommand("phys_timescale %f", GetHostMultiplier(1.0));
		
		if (g_Gamemode == GAMEMODE_WIPEOUT)
		{
			// Get two people to fight it off
			int personA = GetRandomWipeoutPlayer();
			if (IsValidClient(personA))
			{
				g_Participating[personA] = true;
			}
			int personB = GetRandomWipeoutPlayer();
			if (IsValidClient(personB))
			{
				g_Participating[personB] = true;
			}
			
			int personC = -1;
			if (GetLeftWipeoutPlayers() > 4) personC = GetRandomWipeoutPlayer();
			if (IsValidClient(personC)) g_Participating[personC] = true;
			int TeamRed = GetActivePlayers(2, true);
			int TeamBlu = GetActivePlayers(3, true);
			if(TeamRed == 0){
				if (IsValidClient(personA))
				{
					ChangeClientTeamAlive(personA ,2);
				}				
			}
			if(TeamBlu == 0){
				if (IsValidClient(personA))
				{
					ChangeClientTeamAlive(personA ,3);
				}				
			}			
			if (IsValidClient(personA) == false || IsValidClient(personB) == false)
			{
				status = 4;
				bossBattle = 2;
				CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), Victory_timer);
				return;
			}
			char strMessage[512];
			Format(strMessage, sizeof(strMessage), "%N\n%N", personA, personB);
			
			if (IsValidClient(personC))
			{
				Format(strMessage, sizeof(strMessage), "%s\n%N", strMessage, personC);
			}
			PrintCenterTextAll(strMessage);
		}
		else
		{
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && GetClientTeam(i) >= 2 && g_Spawned[i] == true)
				{
					g_Participating[i] = true;
				}
			}
		}
		
		if (ww_music.BoolValue) EmitSoundToClient(1, MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		else EmitSoundToAll(MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && (!(IsFakeClient(i))))
			{
				SetOverlay(i,"");
				g_Minipoints[i] = 0;
			}
			if (g_Gamemode == GAMEMODE_WIPEOUT){
				if (IsValidClient(i) && (!(IsFakeClient(i)))){
					if(g_wipeout_start){
						SetOverlay(i,"tf2ware_wipeout");
						//PrintToChatAll("debug wipeout overlay Check");
					}
				}
			}
		}
		
		status = 1;
		iMinigame = RollMinigame();
		if(iMinigame <= 0){
			iMinigame = RollMinigame();
		}
		minigame = g_name[iMinigame-1];
		if (bossBattle == 1) g_lastboss = iMinigame;
		else g_lastminigame = iMinigame;
		CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), Game_Start);
		if (SpecialRound == 6) g_attack = true;
		else g_attack = false;
		DestroyAllSprites();
	}
}

public Action Game_Start(Handle hTimer) {
	if (status == 1) {
		if (ww_log.BoolValue) LogMessage("Microgame %s started! Status = 1", minigame);
		
		// Spawn everyone so they can participate
		//RespawnAll();
		DestroyBuildings();
		for (int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i) && !IsPlayerAlive(i) && IsClientParticipating(i)){
				//RespawnClient(i, false, false);
				RespawnClientGeneric(i);
			}
			if (g_Gamemode == GAMEMODE_WIPEOUT){
				if (IsValidClient(i) && (!(IsFakeClient(i)))){
					SetOverlay(i,"");
				}
			}
		}
		if (SpecialRound == 4) NoCollision(true);
		if (SpecialRound == 7) {
			for (int i = 1; i <= MaxClients; i++) {
				if (IsValidClient(i) && !IsFakeClient(i)) {
					g_bThirdPersonEnabled[i] = true;
				}
			}
		}		
		
		if (g_Gamemode == GAMEMODE_WIPEOUT) {
			for (int i2 = 1; i2 <= MaxClients; i2++) {
				if (IsValidClient(i2) && IsPlayerAlive(i2) && IsClientParticipating(i2)) {
					SetEntityMoveType(i2, view_as<MoveType>(MOVETYPE_WALK));
					SetWipeoutPosition(i2, false);
				}
			}
		}
		
		// Play the microgame's music
		char sound[512];
		Format(sound, sizeof(sound), "gamesound/tf2ware/minigame_%d.mp3", iMinigame);
		if (StrEqual(minigame, "Ghostbusters") && GetRandomInt(1,3) == 1) Format(sound, sizeof(sound), "gamesound/tf2ware/minigame_%d_alt.mp3", iMinigame);
		//if (StrEqual(minigame, "BossTarget")) Format(sound, sizeof(sound), "gamesound/tf2ware/minigame_%d.wav", iMinigame);
		int channel = SNDCHAN_AUTO;
		if (GetMinigameConfNum(minigame, "dynamic", 0)) channel = SND_CHANNEL_SPECIFIC;
		if (ww_music.BoolValue) EmitSoundToClient(1, sound, SOUND_FROM_PLAYER, channel, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		else EmitSoundToAll(sound, SOUND_FROM_PLAYER, channel, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		
		// Set everyone's state to fail
		SetStateAll(false);
		
		// The 'x did y first' is untriggered
		g_first = false;
		
		// current proccess
		status = 2;
		
		// Reset everyone's mission
		SetMissionAll(0);
		
		// noone can attack
		if (SpecialRound == 6) g_attack = true;
		else g_attack = false;
		
		// initiate mission
		InitMinigame(iMinigame);
		
		// show the mission text
		PrintMissionText();
		
		// timeleft counter. Let it stay longer on boss battles.
		timeleft = 8;
		if (bossBattle == 1){ 
			CreateTimer(GetSpeedMultiplier(3.0), CountDown_Timer);
			CreateTimer(GetSpeedMultiplier(3.0), CountDown_Timer2);
		}
		else {
			CreateTimer(GetSpeedMultiplier(1.0), CountDown_Timer);
			CreateTimer(GetSpeedMultiplier(1.0), CountDown_Timer2);
		}
		
		// get the lasting time from the cfg
		microgametimer = CreateTimer(GetSpeedMultiplier(GetMinigameConfFloat(minigame, "duration")), EndGame);
		timeleftmg = RoundToFloor(GetSpeedMultiplier(GetMinigameConfFloat(minigame, "duration")));
		if (StrEqual(minigame, "PushBall_Game"))
		{
			timeleft = 40;
		}
		
		// debug
		if (ww_log.BoolValue) LogMessage("Microgame started post");
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && IsPlayerAlive(i)){
				if(TF2_IsPlayerInCondition(i, TFCond_Taunting))
				{
					TF2_RemoveCondition(i, TFCond_Taunting);
				}
				TF2_RemovePlayerDisguise(i);
			}
		}
	}
	return Plugin_Stop;
}

void PrintMissionText()
{
	if (ww_log.BoolValue)
	{
		LogMessage("Printing mission text");
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i))
		{
			if (g_Gamemode == GAMEMODE_WIPEOUT){
				if(IsValidClient(i)){
					if(g_wipeout_start){
						SetOverlay(i,"");
						//PrintToChatAll("debug wipeout overlay disabled");
						g_wipeout_start = false;
					}
				}
			}
			char missionmessage[256];
			char message[256];
			char tittle[256];
			char buffer[128];
			Format(missionmessage, sizeof(missionmessage), "INSTRUCTION_%i_%i", iMinigame, g_Mission[i]+1);
			SetGlobalTransTarget(i);
			Format(message, sizeof(message), "\x04%t", missionmessage, i);
			if(!TrHud.BoolValue){
				PrintToHud(i, "%T", missionmessage, i);
			}
			if(TrHud.BoolValue){
				Format(tittle, sizeof(tittle), "TF2Ware Minigame: %i-20 %s", g_minigamestotal+1, minigame);
				float mg_Timemsg;
				mg_Timemsg = GetMinigameConfFloat(minigame, "duration");
				if(mg_Timemsg >7.0) mg_Timemsg = 7.0;
				if (IsValidClient(i)){
					TrainingMessage(i, tittle, message, mg_Timemsg-1.0);
				}
				if(IsValidClient(i) && GetClientTeam(i) <= 1){
					CPrintToChat(i, "\x01wait to \x04 minigame instruction \x01 is off to join team!");
				}
			}
			Format(buffer, sizeof(buffer), "%T", missionmessage, LANG_SERVER);
			if(iMinigame <= 9){
				Format(output2, sizeof(output2), "%1d_%s", iMinigame, minigame);
			}
			if(iMinigame >= 10){
				Format(output2, sizeof(output2), "%2d_%s", iMinigame, minigame);
			}
		}
	}
}
public Action CountDown_Timer2(Handle hTimer)
{
	if ((status == 2) && (timeleftmg > 0))
	{
		timeleftmg = timeleftmg - 1;
		CreateTimer(GetSpeedMultiplier(1.0), CountDown_Timer2);
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && IsValidClient(ntarget))
			{
				if (StrEqual(minigame, "Dodgeball"))
				{
					if(iHudSpeed < 999){
						SetHudTextParams(0.30, 0.70, 0.9, 0, 255, 0, 255);
						ShowHudText(i, -1, "TimeLeft: %i %t %i %t %N", timeleftmg, "DB_BALL_SPEED", RoundToZero(BallSpeed), "TARGET", ntarget);
					}
					if(iHudSpeed > 1000 && iHudSpeed < 1499){
						SetHudTextParams(0.30, 0.70, 0.9, 255, 222, 0, 255);
						ShowHudText(i, -1, "TimeLeft: %i %t %i %t %N", timeleftmg, "DB_BALL_SPEED", RoundToZero(BallSpeed), "TARGET", ntarget);
					}
					if(iHudSpeed > 1500){
						SetHudTextParams(0.30, 0.70, 0.9, 255, 0, 0, 255);
						ShowHudText(i, -1, "TimeLeft: %i %t %i %t %N", timeleftmg, "DB_BALL_SPEED", RoundToZero(BallSpeed), "TARGET", ntarget);
					}
				}
				else
				if(timeleftmg >7)SetHudTextParams(0.58, 0.70, 1.0, 255, 255, 255, 255);
				if(timeleftmg <=6 && timeleftmg >=4)SetHudTextParams(0.58, 0.70, 1.0, 255, 222, 0, 255);
				if(timeleftmg <=3 && timeleftmg >=0)SetHudTextParams(0.58, 0.70, 1.0, 255, 0, 0, 0);
				ShowHudText(i, 1,"TimeLeft: %i", timeleftmg);
			}
		}
	}
	
	return Plugin_Stop;
}
public Action CountDown_Timer(Handle hTimer)
{
	if ((status == 2) && (timeleft > 0))
	{
		timeleft = timeleft - 1;
		CreateTimer(GetSpeedMultiplier(0.4), CountDown_Timer);
		if (bossBattle != 1)
		{
			Call_StartForward(g_OnTimerMinigame);
			Call_PushCell(timeleft);
			Call_Finish();
		}
		if (timeleft == 2)
		{
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && (!(IsFakeClient(i))) && g_ModifiedOverlay[i] == false)
				{
					SetOverlay(i, "");
				}
			}
		}
	}
	
	return Plugin_Stop;
}

public Action EndGame(Handle hTimer) {
	microgametimer = INVALID_HANDLE;
	if (status == 2) {
		if (ww_log.BoolValue) LogMessage("Microgame %s, (id:%d) ended!", minigame, iMinigame);
		Call_StartForward(g_OnAlmostEnd);
		Call_Finish();
		
		status = 0;
		
		float MUSIC_INFO_LEN = MUSIC_END_LEN;
		char MUSIC_INFO_WIN[PLATFORM_MAX_PATH];
		char MUSIC_INFO_FAIL[PLATFORM_MAX_PATH];
		Format(MUSIC_INFO_WIN, sizeof(MUSIC_INFO_WIN), MUSIC_WIN);
		Format(MUSIC_INFO_FAIL, sizeof(MUSIC_INFO_FAIL), MUSIC_FAIL);
		if (g_Gamemode == GAMEMODE_WIPEOUT) {
			MUSIC_INFO_LEN = MUSIC2_END_LEN;
			Format(MUSIC_INFO_WIN, sizeof(MUSIC_INFO_WIN), MUSIC2_WIN);
			Format(MUSIC_INFO_FAIL, sizeof(MUSIC_INFO_FAIL), MUSIC2_FAIL);
		}
		
		if (SpecialRound == 6) g_attack = true;
		else g_attack = false;
		
		Call_StartForward(g_OnEndMinigame);
		Call_Finish();
		
		CleanupAllVocalizations();
		
		currentSpeed = ww_speed.FloatValue;
		SetCommandFlags("host_timescale", GetCommandFlags("host_timescale") & (~FCVAR_CHEAT));
		SetCommandFlags("phys_timescale", GetCommandFlags("phys_timescale") & (~FCVAR_CHEAT));
		ServerCommand("host_timescale %f", GetHostMultiplier(1.0));
		ServerCommand("phys_timescale %f", GetHostMultiplier(1.0));
		
		char sound[512];
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i)) {
				if (IsClientParticipating(i)) {
					// heal everyone
					//TF2_RespawnPlayer(i); funziona ma lagga
					//TF2_RegeneratePlayer(i);
					
					// Kill their weapons
					DisableClientWeapons(i);
					HealClient(i);
					
					// if client won
					if (g_Complete[i]) {
						Format(sound, sizeof(sound), MUSIC_INFO_WIN);
					}
					
					// if client lost
					if (g_Complete[i] == false) {
						Format(sound, sizeof(sound), MUSIC_INFO_FAIL);
					}
				}
				else {
					Format(sound, sizeof(sound), MUSIC_INFO_WIN);
				}
				char oldsound[512];				
				Format(oldsound, sizeof(oldsound), "gamesound/tf2ware/minigame_%d.mp3", iMinigame);
				if (GetMinigameConfNum(minigame, "dynamic", 0)) StopSound(i, SND_CHANNEL_SPECIFIC, oldsound);
				if(IsValidClient(i)){
				EmitSoundToClient(i, sound, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
				}
			}
		}
		
		// Clear all functions from forwards
		RemoveAllFromForward(g_justEntered, INVALID_HANDLE);
		RemoveAllFromForward(g_OnAlmostEnd, INVALID_HANDLE);
		RemoveAllFromForward(g_OnTimerMinigame, INVALID_HANDLE);
		RemoveAllFromForward(g_OnEndMinigame, INVALID_HANDLE);
		RemoveAllFromForward(g_OnGameFrame_Minigames, INVALID_HANDLE);
		RemoveAllFromForward(g_PlayerDeath, INVALID_HANDLE);
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && !IsFakeClient(i) && IsClientParticipating(i)) {
				if (g_Complete[i]) {
					SetOverlay(i,"tf2ware_minigame_win_2");
					if(ww_sql.IntValue){
						MinigameWon[i]++;
					}
					if(bossBattle == 1){
						if(ww_sql.IntValue){
							BossStageWon[i]++;
						}
					}
					CreateTimer(0.3, WinTimer, i);
				}
				if (g_Complete[i] == false) {
					SetOverlay(i,"tf2ware_minigame_fail_2");
					if(ww_sql.IntValue){
						MinigameLose[i]++;
					}
				}
				saveUser(i);
			}
		}
		
		UpdateHud(GetSpeedMultiplier(MUSIC_INFO_LEN));
		
		bool bHandlePoints = true;
		if (g_Gamemode == GAMEMODE_WIPEOUT) {
			bool bSomeoneWon = false;
			for (int i = 1; i <= MaxClients; i++) {
				if (IsValidClient(i) && IsClientParticipating(i) && g_Complete[i] == true) bSomeoneWon = true;
			}
			if (bSomeoneWon == false && bossBattle == 1 && GetLeftWipeoutPlayers() == 2) {
				bHandlePoints = false;
				CPrintToChatAll("{red}DRAW{default}... playing new boss!");
			}
		}
		if (bHandlePoints) HandOutPoints();
		
		// RESPAWN START
		if (GetMinigameConfNum(minigame, "endrespawn", 0) > 0) RespawnAll();
		else RespawnAll();
		if (g_Gamemode == GAMEMODE_WIPEOUT) {
			for (int i2 = 1; i2 <= MaxClients; i2++) {
				if (IsValidClient(i2) && IsClientParticipating(i2)) {
					SetWipeoutPosition(i2, true);
				}
			}
		}
		
		if (SpecialRound == 4) NoCollision(true);
		else NoCollision(false);
		
		if (SpecialRound == 7) {
			for (int i = 1; i <= MaxClients; i++) {				
				if (IsValidClient(i) && !IsFakeClient(i)){
				g_bThirdPersonEnabled[i] = true;
				}				
			}
		}
		if (SpecialRound == 9) {
		for (int i = 1; i <= MaxClients; i++) {
			if (IsValidClient(i) && !IsFakeClient(i)) SDKHook(i, SDKHook_PreThink, Special_BigWeapons);
			}
		}
		if (SpecialRound == 11) {
			CreateDuckSpecial();
		}
		
		// RESPAWN END
		
		bool speedup = false;
		g_minigamestotal += 1;
		
		if (bossBattle == 1) bossBattle = 2;
		
		if (g_Gamemode == GAMEMODE_WIPEOUT) {
			if ((GetAverageScoreFloat() <= 2.80) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetAverageScoreFloat() <= 2.50) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetAverageScoreFloat() <= 2.20) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetAverageScoreFloat() <= 1.80) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetAverageScoreFloat() <= 1.40) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetAverageScoreFloat() <= 1.0) && (bossBattle == 0) && currentSpeed <= 1.0) speedup = true;
			if ((GetLeftWipeoutPlayers() == 2) && (bossBattle != 1)) {
				speedup = true;
				bossBattle = 1;
			}	
		}
		else {
			if ((g_minigamestotal == 4) && (bossBattle == 0)) speedup = true;
			if ((g_minigamestotal == 8) && (bossBattle == 0)) speedup = true;
			if ((g_minigamestotal == 12) && (bossBattle == 0)) speedup = true;
			if ((g_minigamestotal == 16) && (bossBattle == 0)) speedup = true;
			if ((g_minigamestotal == 19) && (bossBattle == 0)) {
				speedup = true;
				bossBattle = 1;
			}
			if ((g_minigamestotal >= 19) && bossBattle == 2 && SpecialRound == 3 && Special_TwoBosses == false) {
				speedup = true;
				bossBattle = 1;
				Special_TwoBosses = true;
			}
		}
		if (g_Gamemode == GAMEMODE_WIPEOUT && GetLeftWipeoutPlayers() <= 1) {
			status = 4;
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), Victory_timer);
		}
		if (speedup == false) {
			status = 10;
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), StartMinigame_timer2);
			CreateAllSprites();
		}
		if (speedup == true) {
			status = 3;
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), Speedup_timer);
		}
		if (bossBattle == 2 && speedup == false) {
			status = 4;
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), Victory_timer);
		}

	}
	return Plugin_Stop;
}
public Action Speedup_timer(Handle hTimer)
{
	if (status == 3)
	{
		DrawScoresheet();
		RemoveAllParticipants();
		if (bossBattle == 1)
		{
			if (ww_log.BoolValue)
			{
				LogMessage("GETTING READY TO START SOME BOSS");
			}
			float MUSIC_INFO_LEN = MUSIC_BOSS_LEN;
			char MUSIC_INFO[PLATFORM_MAX_PATH];
			Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC_BOSS);
			if (g_Gamemode == GAMEMODE_WIPEOUT)
			{
				MUSIC_INFO_LEN = MUSIC2_BOSS_LEN;
				Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC2_BOSS);
			}
			
			if (ww_log.BoolValue)
			{
				LogMessage("Boss part 2");
			}
		
			// Set the Speed. If special round, we want it to be a tad faster ;)
			if(SpecialRound == 1)
			{
				if((!StrEqual(minigame, "GiocaJouer")) || (!StrEqual(minigame, "Slender")))
				{
					SetConVarFloat(ww_speed, 2.5);
				}
				else
				{
					SetConVarFloat(ww_speed, 1.0);
				}
			}
			else
			{
				SetConVarFloat(ww_speed, 1.0);
			}
			
			if (ww_log.BoolValue)
			{
				LogMessage("Boss part 3");
			}
			currentSpeed = ww_speed.FloatValue;
			SetCommandFlags("host_timescale", GetCommandFlags("host_timescale") & (~FCVAR_CHEAT));
			SetCommandFlags("phys_timescale", GetCommandFlags("phys_timescale") & (~FCVAR_CHEAT));
			ServerCommand("host_timescale %f", GetHostMultiplier(1.0));
			ServerCommand("phys_timescale %f", GetHostMultiplier(1.0));
			if (ww_log.BoolValue)
			{
				LogMessage("Boss part 4");
			}
			
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), StartMinigame_timer2);
			
			if (ww_log.BoolValue)
			{
				LogMessage("Boss part 5");
			}
			
			if (ww_music.BoolValue)
			{
				EmitSoundToClient(1, MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			}
			else
			{
				EmitSoundToAll(MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			}
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && (!(IsFakeClient(i))))
				{
					SetOverlay(i,"tf2ware_minigame_boss_2");
				}
			}	  

			if (ww_log.BoolValue)
			{
				LogMessage("Boss part 6");
			}
			
			UpdateHud(GetSpeedMultiplier(MUSIC_INFO_LEN));
		}
		
		if (ww_log.BoolValue)
		{
			LogMessage("Boss part 7");
		}
		
		if (bossBattle != 1)
		{
			float MUSIC_INFO_LEN = MUSIC_SPEEDUP_LEN;
			char MUSIC_INFO[PLATFORM_MAX_PATH];
			Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC_SPEEDUP);
			for(int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i))
				{	
					TF2_AddCondition(i, TFCond_SpeedBuffAlly, 6.2);
				}
			}
			if (g_Gamemode == GAMEMODE_WIPEOUT)
			{
				MUSIC_INFO_LEN = MUSIC2_SPEEDUP_LEN;
				Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC2_SPEEDUP);
			}
		
			if (ww_music.BoolValue)
			{
				EmitSoundToClient(1, MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			}
			else
			{
				EmitSoundToAll(MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
			}
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsValidClient(i) && (!(IsFakeClient(i))))
				{
					SetOverlay(i,"tf2ware_minigame_speed_2");
				}
			}
			UpdateHud(GetSpeedMultiplier(MUSIC_INFO_LEN));
			SetConVarFloat(ww_speed, ww_speed.FloatValue + 0.5);
			CreateTimer(GetSpeedMultiplier(MUSIC_INFO_LEN), StartMinigame_timer2);
		}
		
		if (ww_log.BoolValue)
		{
			LogMessage("Boss part 8");
		}
		
		status = 10;
		
		if (ww_log.BoolValue)
		{
			LogMessage("Post boss");
		}
	}
	
	return Plugin_Stop;
}

public Action Victory_timer(Handle hTimer)
{
	if ((status == 4) && (bossBattle > 0))
	{
		bossBattle = 0;
		SetConVarFloat(ww_speed, 1.0);
		currentSpeed = ww_speed.FloatValue;
	   
		CreateTimer(GetSpeedMultiplier(13.17), Restartall_timer);
		status = 5;
	   
		float MUSIC_INFO_LEN = MUSIC_GAMEOVER_LEN;
		char MUSIC_INFO[PLATFORM_MAX_PATH];
		Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC_GAMEOVER);
		if (g_Gamemode == GAMEMODE_WIPEOUT)
		{
			MUSIC_INFO_LEN = MUSIC2_GAMEOVER_LEN;
			Format(MUSIC_INFO, sizeof(MUSIC_INFO), MUSIC2_GAMEOVER);
		}
	   
		if (ww_music.BoolValue)
		{
			EmitSoundToClient(1, MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		}
		else
		{
			EmitSoundToAll(MUSIC_INFO, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL,GetSoundMultiplier());
		}
	   
		ResetWinners();
	   
		int targetscore = GetHighestScore();
		if (SpecialRound == 5)
		{
			targetscore = GetLowestScore();
		}
		int winnernumber = 0;
		Handle ArrayWinners = CreateArray();
		char winnerstring_names[512];
		char pointsname[512];
		char buffer[128];
		Format(pointsname, sizeof(pointsname), "%t", "points");
		if (g_Gamemode == GAMEMODE_WIPEOUT)
		{
			Format(pointsname, sizeof(pointsname), "%t", "lives");
		}
	   
		bool bAccepted = false;
		for (int i = 1; i <= MaxClients; i++)
		{
			SetOverlay(i, "");
			if (IsValidClient(i) && (GetClientTeam(i) == 2 || GetClientTeam(i) == 3))
			{
				bAccepted = false;
				if (g_Gamemode == GAMEMODE_WIPEOUT)
				{
					if (g_Points[i] > 0)
					{
						bAccepted = true;
					}
				}
				else
				{
					if (SpecialRound != 5 && g_Points[i] >= targetscore)
					{
						bAccepted = true;
					}
					if (SpecialRound == 5 && g_Points[i] <= targetscore)
					{
						bAccepted = true;
					}
				}
				if (bAccepted)
				{
					g_Winner[i] = 1;
					CreateSprite(i);
					//RespawnClient(i, false, false);
					RespawnClientEnd(i);
					SetWeaponState(i, true);
					winnernumber += 1;
					for (int i3 = 1; i3 <= MaxClients; i3++)
					{
						if (IsValidClient(i3) && g_Winner[i3]){
							EmitSoundToClient(i3, WIN_VOICE);
							if (g_Gamemode == GAMEMODE_WIPEOUT)
							{
								TF2_RespawnPlayer(i3);
							}
							CreateTimer(8.0, Timer_Comments_Win, i3);
						}
						if (IsValidClient(i3) && !g_Winner[i3]){
							EmitSoundToClient(i3, LOSE_VOICE);
							if (g_Gamemode == GAMEMODE_WIPEOUT)
							{
								TF2_RespawnPlayer(i3);
							}
							CreateTimer(8.0, Timer_Comments_Lose, i3);
						}
					}
					PushArrayCell(ArrayWinners, i);
					if(ww_hlstatsx.BoolValue){
				   
						/* Now create the win event. */
						Handle hWinEvent = CreateEvent("tf2ware_inc_win");
					   
						if (hWinEvent != null)
						{
							/* Set the information. */
							SetEventInt(hWinEvent, "userid", GetClientUserId(i));
							SetEventInt(hWinEvent, "total", g_Points[i]);
						   
							/* Set the mode. */
							if (g_Gamemode == GAMEMODE_WIPEOUT)
							{
								SetEventString(hWinEvent, "mode", "wipeout");
							}
							else if (SpecialRound > 0)
							{
								SetEventString(hWinEvent, "mode", "special");
							}
							else
							{
								SetEventString(hWinEvent, "mode", "normal");
							}
						   
							/* Fire the event. */
							FireEvent(hWinEvent);
						}
					}
				}
			}
		}
		for (int i = 0; i < GetArraySize(ArrayWinners); i++) {
			int client = GetArrayCell(ArrayWinners, i);
			if (winnernumber > 1) {
				if (i >= (GetArraySize(ArrayWinners)-1)){
					Format(winnerstring_names, sizeof(winnerstring_names), "%s {olive}%N{green}", winnerstring_names, client);
				}
				else Format(winnerstring_names, sizeof(winnerstring_names), "%s, {olive}%N{green}", winnerstring_names, client);
			}
			else Format(winnerstring_names, sizeof(winnerstring_names), "{olive}%N{green}", client);
			if(ww_sql.IntValue)
			{
				TotalRoundWon[client]++;
				CreateTimer(1.0, LateWinners, client);
			}
			CreateTimer(0.2, GiveBuffVictory, client);
			if(!ww_sql.IntValue){
				AddToClientWinPoints(client);
			}
		}
		if (winnernumber > 1) ReplaceStringEx(winnerstring_names, sizeof(winnerstring_names), ", ", "");
		
		if (winnernumber == 1) CPrintToChatAll("%t %s (%i %s)!", "NOTIFY_WINNER", winnerstring_names, targetscore, pointsname);
		else  CPrintToChatAll("%t %s (%i %s)!", "NOTIFY_WINNERS", winnerstring_names, targetscore, pointsname);
		CloseHandle(ArrayWinners);
		IsEnd = true;
		int TimeNeeded = 13;
		ActiveFireworks();
		CreateTimer(TimeNeeded*1.0, TimerStopFireworks);	   
		if (SpecialRound > 0)
		{
			CPrintToChatAll("%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_SPECIAL_OVER");
			ResetSpecialRoundEffect();
			SpecialRound = 0;
			g_fart = INVALID_HANDLE;
			ClearTimer(g_fart);
			FormatEx(buffer, sizeof(buffer), "%t", "SPECIAL_ROUND_OVER");
			ShowGameText(buffer);
		}
		UpdateHud(GetSpeedMultiplier(MUSIC_INFO_LEN));	   
	}
	return Plugin_Stop;
}
public Action GiveBuffVictory(Handle timer, any client)
{
	if (IsValidClient(client)){
		TF2_AddCondition(client, TFCond_SpeedBuffAlly, 13.5);
	}
	
	return Plugin_Stop;
}
public Action Restartall_timer(Handle hTimer)
{
	if (status == 5)
	{
		bossBattle = 0;
		
		// Set the game speed
		if (SpecialRound == 1)
		{ 
			SetConVarFloat(ww_speed, 2.5);

			if (bossBattle == 1)
			{
				SetConVarFloat(ww_speed, 1.0);
			}
		}
		else
		{
			SetConVarFloat(ww_speed, 1.0);
		}
		
		if (SpecialRound > 0)
		{
			AddSpecialRoundEffect();
		}
		
		currentSpeed = ww_speed.FloatValue;
		ResetScores();
		SetStateAll(false);
		ResetWinners();
		g_minigamestotal = 0;
		DestroyBuildings();
		
		
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && IsPlayerAlive(i)) DisableClientWeapons(i);
		}
		
		// Roll special round
		if ((GetRandomInt(0,10) == 5 || ww_special.BoolValue) && SpecialRound == 0)
		{
			status = 6;
			StartSpecialRound();
		}
		else
		{
			status = 0;
			SetGameMode();
			ResetScores();
			StartMinigame();
		}
	}
	return Plugin_Stop;
}

int var_SpecialRoundRoll = 0;
int var_SpecialRoundCount = 0;

public void StartSpecialRound()
{
	if (status == 6)
	{
		//RespawnAll();
		for (int i = 1; i <= MaxClients; i++) {
			if(IsValidClient(i) && !IsPlayerAlive(i) && IsClientParticipating(i)){
				//RespawnClient(i, false, false);
				RespawnClientGeneric(i);
			}
		}
		SetConVarBool(ww_special, false);
		if (ww_force_special.IntValue <= 0)
		{
			SpecialRound = GetRandomInt(1,SPECIAL_TOTAL);
		}
		else
		{
			SpecialRound = ww_force_special.IntValue;
		}
	
		if (ww_music.BoolValue)
		{
			EmitSoundToClient(1, MUSIC_SPECIAL);
		}
		else
		{
			EmitSoundToAll(MUSIC_SPECIAL);
		}
		
		status = 5;
		CreateTimer(0.1, SpecialRound_timer);
		
		var_SpecialRoundCount = 130;
		
		CreateTimer(GetSpeedMultiplier(MUSIC_SPECIAL_LEN), Restartall_timer);
		
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && (!(IsFakeClient(i))))
			{
				//SetOverlay(i,"");
				SetOverlay(i,"tf2ware_special_round");
			}
		}
	}
}

public Action SpecialRound_timer(Handle hTimer)
{
	if (status == 5 && var_SpecialRoundCount > 0)
	{
		CreateTimer(0.0, SpecialRound_timer);
		
		var_SpecialRoundCount -= 1;
		var_SpecialRoundRoll += 1;
		if (var_SpecialRoundRoll > sizeof(var_special_name)+1)
		{
			var_SpecialRoundRoll = 0;
		}
		char Name[128];
		if (var_SpecialRoundRoll < sizeof(var_special_name))
		{
			FormatEx(Name, sizeof(Name), "%t", var_special_name[var_SpecialRoundRoll]);
		}
		else
		{
			FormatEx(Name, sizeof(Name), "%t", var_funny_names[GetRandomInt(0, sizeof(var_funny_names)-1)]);
		}
		
		if (var_SpecialRoundCount > 0)
		{
			char Text[128];
			FormatEx(Text, sizeof(Text), "%t: %t", "SPECIAL_ROUND", "SPECIAL_ROUND_INFO", Name);
			ShowGameText(Text, "leaderboard_dominated", 1.0);
		}
		
		if (var_SpecialRoundCount == 0)
		{
			if (ww_music.BoolValue)
			{
				EmitSoundToClient(1, SOUND_SELECT);
			}
			else
			{
				EmitSoundToAll(SOUND_SELECT);
			}

			GiveSpecialRoundInfo();
		}	 
	}
	
	return Plugin_Stop;
}

void GiveSpecialRoundInfo()
{
	if (SpecialRound > 0)
	{
		char Text[128];
		FormatEx(Text, sizeof(Text), "%t: %t!\n%t", "SPECIAL_ROUND", var_special_name[SpecialRound-1], var_special_desc[SpecialRound-1]);
		ShowGameText(Text, "leaderboard_dominated");
	}
}

public Action Command_event(int client, int args)
{
	status = 6;
	StartSpecialRound();
	SetConVarBool(ww_enable, true);
	return Plugin_Handled;
}

void SetStateAll(bool value)
{
	for (int i = 1; i <= MaxClients; i++)
	{
		g_Complete[i] = value;
	}
}

void SetMissionAll(int value)
{
	for (int i = 1; i <= MaxClients; i++)
	{
		g_Mission[i] = value;
	}
}

void SetClientSlot(int client, int slot)
{
	if (ww_log.BoolValue)
	{
		LogMessage("Setting client slot");
	}
	int weapon = GetPlayerWeaponSlot(client, slot);
	SetEntPropEnt(client, Prop_Send, "m_hActiveWeapon", weapon);
}

//int RespawnAll(bool force = false, bool savepos = true)
void RespawnAll()
{
	if (ww_log.BoolValue)
	{
		LogMessage("Respawning everyone");
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && (GetClientTeam(i) == 2 || GetClientTeam(i) == 3))
		{
			RespawnClientEnd(i);
		}
	}
}
void RespawnClientEnd(int client){
	if (IsValidClient(client) && IsValidTeam(client) && (g_Spawned[client] == true)) {
		if (g_Gamemode == GAMEMODE_WIPEOUT && g_Points[client] <= 0)return;
		if (g_Gamemode == GAMEMODE_WIPEOUT && g_Participating[client]){
			TF2_RespawnPlayer(client);
		}
		if (g_Gamemode == GAMEMODE_WIPEOUT && IsValidClient(client) && IsValidTeam(client) && !IsPlayerAlive(client) && g_Points[client] >= 1){
			TF2_RespawnPlayer(client);
		}
		if (g_Gamemode == GAMEMODE_NORMAL){
			TF2_RespawnPlayer(client);
		}
		if (SpecialRound == 7) {				
			if (!IsFakeClient(client)){	 
				g_bThirdPersonEnabled[client] = true;			
			}
		}
	}
}
void RespawnClientGeneric(int client){
	if (IsValidClient(client) && IsValidTeam(client) && (g_Spawned[client] == true)) {
		if (!IsPlayerAlive(client)){
			if (g_Gamemode == GAMEMODE_WIPEOUT && g_Points[client] <= 0)return;
			TF2_RespawnPlayer(client);
		}
		if (IsPlayerAlive(client)){
			if(TF2_IsPlayerInCondition(client, TFCond_Taunting))
			{
				TF2_RemoveCondition(client, TFCond_Taunting);
			}
			TF2_RemovePlayerDisguise(client);
			//TF2_RespawnPlayer(client);
			//TF2_RegeneratePlayer(client);
		}
		if (SpecialRound == 7) {				
			if (!IsFakeClient(client)){	 
				g_bThirdPersonEnabled[client] = true;			
			}
		}
	}
}
void RespawnClient(any i, bool force = false, bool savepos = true) {
	float pos[3];
	float vel[3];
	float ang[3];
	int alive = false;
	if (IsValidClient(i) && IsValidTeam(i) && (g_Spawned[i] == true)) {
		bool force2 = false;
		if (!IsPlayerAlive(i)) force2 = true;
		if (force && IsClientParticipating(i)) force2 = true;
		if (g_Gamemode == GAMEMODE_WIPEOUT && g_Points[i] <= 0) force2 = false;
		if (force2) {
			alive = false;
			if (savepos) {
				GetClientAbsOrigin(i, pos);
				GetClientEyeAngles(i, ang);
				GetEntPropVector(i, Prop_Data, "m_vecVelocity", vel);
				if (IsPlayerAlive(i)) alive = true;			
			}
			TF2_RespawnPlayer(i);
			if ((savepos) && (alive)) TeleportEntity(i, pos, ang, vel);			
		}
		TF2_RemovePlayerDisguise(i);
		if (SpecialRound == 7) {				
			if (IsValidClient(i) && !IsFakeClient(i)){	 
				g_bThirdPersonEnabled[i] = true;			
			}
		}
		if(TF2_IsPlayerInCondition(i, TFCond_Taunting))
		{
			TF2_RemoveCondition(i, TFCond_Taunting);
		}
		TF2_RegeneratePlayer(i);		
	}	
}

public void RespawnRedLate(int client)
{
	if(IsValidClient(client)){
		TF2_RespawnPlayer(client);
	}
}
void DrawScoresheet()
{
	int[] clients = new int[MaxClients+1];

	for (int i; i <=MaxClients; i++)
	{
		clients[i] = i;
	}
	SortCustom1D(clients, MaxClients, ScoreGreaterThan);
	char cName[256];
	char Lines[10][256];
	char Sheet[512];
	int count = -0;
	for (int i=MaxClients; i>-0; i--)
	{
		if (count >= 10)
		{
			break;
		}
		if (IsValidClient(clients[i]) && !IsClientObserver(clients[i]))
		{
			GetClientName(clients[i], cName, sizeof(cName));
			Format(Lines[count], 256, "%2d - %s", g_Points[clients[i]], cName);			   
			count++;
		}
	}
	ImplodeStrings(Lines, 10, "\n", Sheet, 512);
	SetHudTextParams(0.01, 0.10, 8.0, 0, 255, 0, 255);
	for (int i; i <=MaxClients; i++)
	{
		if (IsValidClient(i))
		{		
			ShowHudText(i, -1, Sheet);
		}
	}
	SetHudTextParams(0.01, 0.03, 8.0, 255, 255, 255, 255);
	for (int i; i <=MaxClients; i++)
	{
		if (IsValidClient(i))
		{		
			ShowHudText(i, -1, "%t", "ROUND_RANKING");
		}
	}		
}

void SetStateClient(int client, bool value, bool complete=false)
{
	if (IsValidClient(client) && IsClientParticipating(client))
	{
		if (complete && g_Complete[client] != value)
		{
			if (value)
			{
				EmitSoundToClient(client, SOUND_COMPLETE);
				for(int i = 1; i <= MaxClients; i++)
				{
					if (IsValidClient(i) && !IsFakeClient(i))
					{
						EmitSoundToClient(i, SOUND_COMPLETE_YOU, client);
						if (IsClientParticipating(i) && IsPlayerAlive(i) && g_Gamemode == GAMEMODE_WIPEOUT && i != client)
						{
							SetStateClient(i, false, true);
							if(TF2_IsPlayerInCondition(i, view_as<TFCond>(82)))
							{
								SDKHooks_TakeDamage(i, 0, 0, 20000.0, DMG_SLASH);
								CPrintToChatEx(i, client, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_BEATEN", client);
							}
							else
							ForcePlayerSuicide(i);
							CPrintToChatEx(i, client, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_BEATEN", client);
						}
					}
				}
				char effect[128] = PARTICLE_WIN_BLUE;
				if (GetClientTeam(client) == 2) effect = PARTICLE_WIN_RED;
				ClientParticle(client, effect, 8.0);
			}
		}
		g_Complete[client] = value;
	}
}
/*
stock Float:GetSpeedMultiplier(Float:current)
{
	float speed = ((currentSpeed-1.0)/3.5)+1.0;
	speed = current;
	return speed;
}

stock Float:GetHostMultiplier(Float:current)
{
	float speed = ((currentSpeed-1.0)/3.5)+1.0;
	speed = current;
	return speed;
}

GetSoundMultiplier()
{
	new speed = SNDPITCH_NORMAL + RoundFloat((currentSpeed-1.0)/5.0);
	return speed;
}
*/
stock float GetSpeedMultiplier(float count)
{
	float divide = ((currentSpeed-1.0)/10.0)+1.0;
	float speed = count / divide;
	return speed;
}
stock float GetHostMultiplier(float count)
{
	float divide = ((currentSpeed-1.0)/10.0)+1.0;
	float speed = count * divide;
	return speed;
}
int GetSoundMultiplier()
{
	int speed = SNDPITCH_NORMAL + RoundFloat((currentSpeed-1.0)*21.0);
	return speed;
}

void SetOverlay(int i, char overlay[512])
{
	if (IsValidClient(i) && (!(IsFakeClient(i))))
	{
		char language[512];
		char input[512];
		// TRANSLATION
		Format(language, sizeof(language), "");
		
		if (g_Country[i] > 0)
		{
			Format(language, sizeof(language), "/%s",var_lang[g_Country[i]]);
		}
		
		if (StrEqual(overlay, ""))
		{
			Format(input, sizeof(input), "r_screenoverlay \"\"");
		}
		if (!(StrEqual(overlay, "")))
		{
			Format(input, sizeof(input), "r_screenoverlay \"%s%s%s\"", materialpath,language,overlay);
		}
		ClientCommand(i,input);
		g_ModifiedOverlay[i] = true;
	}
}

void UpdateHud(float time)
{
	char add[5];
	char bonus[15];
	char scorename[26];
	int colorR = 255;
	int colorG = 255;
	int colorB = 0;
	Format(bonus, sizeof(bonus), "%t", "DUCK_HUD");
	Format(scorename, sizeof(scorename), "%t", "POINTS_HUD");
	if (g_Gamemode == GAMEMODE_WIPEOUT && SpecialRound != 7)
	{
		Format(scorename, sizeof(scorename), "%t", "LIVES_HUD");
	}
	for(int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i))
		{
			Format(add, sizeof(add), "");
			if (g_Gamemode == GAMEMODE_WIPEOUT)
			{
				if (!g_Complete[i] && IsClientParticipating(i) && bossBattle != 1 && SpecialRound != 7)
				{
					Format(add, sizeof(add), "-1");
				}
				if (!g_Complete[i] && IsClientParticipating(i) && bossBattle == 1 && SpecialRound != 7)
				{
					Format(add, sizeof(add), "-5");
				}
			}
			else
			{
				if (g_Complete[i] && IsClientParticipating(i) && bossBattle != 1 && SpecialRound != 7)
				{
					Format(add, sizeof(add), "+1");
				}
				if (g_Complete[i] && IsClientParticipating(i) && bossBattle == 1 && SpecialRound != 7)
				{
					Format(add, sizeof(add), "+5");
				}
			}
			if(SpecialRound != 11)
			{
				Format(bonus, sizeof(bonus), "%t", "DUCK_HUD");
				Format(scorename, sizeof(scorename), "%t", "POINTS_HUD");
				if (g_Gamemode == GAMEMODE_WIPEOUT && SpecialRound != 7)
				{
					SetHudTextParams(0.01, 0.59, time, colorR, colorG, colorB, 0);
					ShowHudText(i, 1, "%t %i %s", "LIVES_HUD", g_Points[i], add);
				}
				else
				{
					SetHudTextParams(0.01, 0.59, time, colorR, colorG, colorB, 0);
					ShowHudText(i, 1, "%t %i %s", "POINTS_HUD", g_Points[i], add);
				}
			}
			else
			{
				if (g_Gamemode == GAMEMODE_WIPEOUT && SpecialRound != 7)
				{
					SetHudTextParams(0.01, 0.59, time, colorR, colorG, colorB, 0);
					ShowHudText(i, 1, "%t %i %s", "LIVES_HUD", g_Points[i], add);
				}
				else
				{			
					SetHudTextParams(0.01, 0.59, time, colorR, colorG, colorB, 0);
					ShowHudText(i, 1, "%t %i %s %t", "POINTS_HUD", g_Points[i], add, "DUCK_HUD");
				}
			}
			//SetHudTextParams(0.01,0.62,time,colorR,colorG,colorB,0,2,3.0,0.0,3.0);
			//ShowHudText(i, 1, output);
		}
	}
}

public int SortPlayerTimes(int[] elem1, int[] elem2, const int[][] array, Handle hndl)
{
    if(elem1[1] > elem2[1])
    {
        return -1;
    }
    else if(elem1[1] < elem2[1])
    {
        return 1;
    }
    return 0;
} 

void ResetScores()
{
	for(int i = 1; i <= MaxClients; i++)
	{
		if (g_Gamemode == GAMEMODE_WIPEOUT)
		{
			g_Points[i] = 3;
			g_wipeout_start = true;
			//PrintToChatAll("debug wipeout overlay enabled");
		}
		else
		{
			g_Points[i] = 0;
		}
	}
}

int GetHighestScore()
{
	int out = 0;
	
	for(int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && g_Points[i] > out)
		{
			out = g_Points[i];
		}
	}
	return out;
}

int GetLowestScore()
{
	int out = 99;
	
	for(int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && g_Points[i] < out)
		{
			out = g_Points[i];
		}
	}
	return out;
}

int GetAverageScore()
{
	int out = 0;
	int total = 0;
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && (g_Points[i] > 0))
		{
			out += g_Points[i];
			total += 1;
		}
	}
	
	if ((total > 0) && (out > 0)) out = out / total;
	
	return out;
}

stock float GetAverageScoreFloat()
{
	int out = 0;
	float out2 = 0.0;
	int total = 0;
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && (g_Points[i] > 0))
		{
			out += g_Points[i];
			total += 1;
		}
	}
	
	if ((total > 0) && (out > 0))
	{
		out2 = float(out) / float(total);
	}
	
	return out2;
}

void ResetWinners()
{
	for (int i = 1; i <= MaxClients; i++)
	{
		g_Winner[i] = 0;
	}
}

public Action Command_points(int client, int args)
{
	CPrintToChatAll("%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_POINTS", client);
	g_Points[client] += 20;
	g_Points[0] += 20;
	g_Points[1] += 20;
	return Plugin_Handled;
}

public Action Command_list(int client, int args)
{
	PrintToConsole(client, "Listing all registered minigames...");
	char output[128];
	for (int i=0; i<sizeof(g_name); i++)
	{
		if (StrEqual(g_name[i], ""))
		{
			continue;
		}
		if (GetMinigameConfNum(g_name[i], "enable", 1))
		{
			Format(output, sizeof(output), " %2d - %s", GetMinigameConfNum(g_name[i], "id"), g_name[i]);
		}
		else
		{
			Format(output, sizeof(output), " %2d - %s (disabled)", GetMinigameConfNum(g_name[i], "id"), g_name[i]);
		}
		PrintToConsole(client, output);
	}
	
	return Plugin_Handled;
}

void RemoveNotifyFlag(char name[128])
{
	Handle cv1 = FindConVar(name);
	int flags = GetConVarFlags(cv1);
	flags &= ~FCVAR_REPLICATED;
	flags &= ~FCVAR_NOTIFY;
	SetConVarFlags(cv1, flags);
}

void InitMinigame(int id)
{
	GiveId();
	Call_StartFunction(INVALID_HANDLE, g_initFuncs[id-1]);
	Call_Finish();
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && IsClientParticipating(i))
		{
			Call_StartForward(g_justEntered);
			//DestroySprite(i);
			Call_PushCell(i);
			Call_Finish();
		}
	}
}

public void Player_Death(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));	
	if (ww_enable.BoolValue && (status == 2))
	{
	
		if (g_PlayerDeath != INVALID_HANDLE && IsValidClient(client) && IsClientParticipating(client))
		{
			Call_StartForward(g_PlayerDeath);
			Call_PushCell(client);
			Call_Finish();
		}
		if ((g_Sprites[client]) > 0)
		{
			DestroySprite(client);
		}
	}
	TF2_RemoveAllWearables(client);
	//RemoveFakeWeapon(client);
}
public Action KillDrop(int entity)
{
	char weapon[128];
	int maxent = GetMaxEntities();
	for (int i = MaxClients; i < maxent; i++)
	{
		if (IsValidEntity(i))
		{
			GetEdictClassname(i, weapon, sizeof(weapon));
			if ( ( StrContains(weapon, "tf_dropped_weapon") != -1) || (StrContains(weapon, "tf_ammo_pack") != -1))
			{
				if (IsValidEntity(i))
				{
					AcceptEntityInput(i, "Kill");
					SDKUnhook(i, SDKHook_Spawn, KillDrop);
				}
			}
		}
	}	
	return Plugin_Continue;
}
public int ScoreGreaterThan(int left, int right,  const int[] clientids, Handle data)
{
	if (g_Points[left] <= g_Points[right])
	{
		return -1;
	}
	if (g_Points[left] == g_Points[right])
	{
		return 0;
	}
	return 1;
}

// Some convenience functions for parsing the configuration file more simply.
void GotoGameConf(char[] game)
{
	if (!KvJumpToKey(MinigameConf, game))
	{
		PrintToServer("ERROR: Couldn't find requested iMinigame %s in configuration file!", game);
		KvRewind(MinigameConf);
	}
}

float GetMinigameConfFloat(char[] game, char[] key, float def=4.0)
{
	GotoGameConf(game);
	float value = KvGetFloat(MinigameConf, key, def);
	KvGoBack(MinigameConf);
	return value;
}

int GetMinigameConfNum(char[] game, char[] key, int def=0)
{
	GotoGameConf(game);
	int value = KvGetNum(MinigameConf, key, def);
	KvGoBack(MinigameConf);
	return value;
}

int GetRandomWipeoutPlayer()
{
	Handle roll = CreateArray();
	int out = -1;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && IsClientParticipating(i) == false && g_Points[i] > 0)
		{
			PushArrayCell(roll, i);
		}
	}
		
	if (GetArraySize(roll) > 0) out = GetArrayCell(roll, GetRandomInt(0, GetArraySize(roll)-1));
	{
		CloseHandle(roll);
	}
	return out;
}

stock bool IsClientParticipating(int iClient)
{
	if ((g_Participating[iClient] == false) || (GetClientTeam(iClient) == 1))
	{
		return false;
	}
	return true;
}

void SetGameMode()
{
	int iOld = g_Gamemode;
	int iGamemode = ww_gamemode.IntValue;
	if (iGamemode >= 0)
	{
		g_Gamemode = iGamemode;
	}
	else
	{
		g_Gamemode = GAMEMODE_NORMAL;
		int iRoll = GetRandomInt(0, 100);
		if (iRoll <= 5)
		{
			g_Gamemode = GAMEMODE_WIPEOUT;
		}
	}
	
	if (iOld == GAMEMODE_WIPEOUT && g_Gamemode != iOld)
	{
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && IsPlayerAlive(i))
			{
				SetWipeoutPosition(i, false);
			}
		}
	}
	if (g_Gamemode == GAMEMODE_WIPEOUT && g_Gamemode != iOld)
	{
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsValidClient(i) && IsPlayerAlive(i))
			{
				SetWipeoutPosition(i, true);
			}
		}
	}
}

void RemoveAllParticipants()
{
	for (int i = 1; i <= MaxClients; i++)
	{
		g_Participating[i] = false;
	}
}

int GetLeftWipeoutPlayers()
{
	int out = 0;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && GetClientTeam(i) >= 2 && g_Points[i] > 0)
		{
			out ++;
		}
	}
	return out;
}

void SetWipeoutPosition(int iClient, bool bState = false)
{
	float fPos[3];
	float ang[3];
	GetClientAbsOrigin(iClient, fPos);
	if (bState)
	{
		if(IsValidClient(iClient) && GetClientTeam(iClient) == 2){
			//fPos[2] = GAMEMODE_WIPEOUT_HEIGHT;
			fPos[0] = -6399.002441;
			fPos[1] = -14.157020;
			fPos[2] = 388.031311;
			ang[0] = 0.0;
			ang[1] = 0.0;
			ang[2] = 0.0;
		}
		if(IsValidClient(iClient) && GetClientTeam(iClient) == 3){
			//fPos[2] = GAMEMODE_WIPEOUT_HEIGHT;
			fPos[0] = -5846.886230;
			fPos[1] = -16.545832;
			fPos[2] = 388.031311;
			ang[0] = 0.0;
			ang[1] = 180.157020;
			ang[2] = 0.0;
		}
		TeleportEntity(iClient, fPos, ang, NULL_VECTOR);
	}
	else
	{
		//fPos[2] = -70.0;
		int total = 3;
		int posa = 360 / total * (g_Id[iClient]-1);
		
		float bpos[3];
		bpos[0] = -10.875262  + (Cosine(DegToRad(float(posa)))*220.0);
		bpos[1] = 20.072290 - (Sine(DegToRad(float(posa)))*220.0);
		bpos[2] = -680.080322;

		float angb[3];
		angb[0] = 0.0;
		angb[1] = float(180-posa);
		angb[2] = 0.0;
		TeleportEntity(iClient, bpos, angb, NULL_VECTOR);
	}
}

public Action Timer_HandleWOLives(Handle hTimer, any iClient)
{
	HandleWipeoutLives(iClient);
	
	return Plugin_Stop;
}

void HandleWipeoutLives(int iClient, bool bMessage = false)
{
	if (g_Gamemode == GAMEMODE_WIPEOUT && IsValidClient(iClient) && IsPlayerAlive(iClient) && g_Points[iClient] <= 0)
	{
		if (bMessage)
		{
			if (g_Points[iClient] == 0)
			{
				CPrintToChatAllEx(iClient, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_WIPED_OUT", iClient);
			}
			if(g_Points[iClient] < 0)
			{
				CPrintToChat(iClient, "%t\x01: %t", "TF2WARE_PREFIX_COLOR", "NOTIFY_WIPED_WAIT");
			}
		}
		if(TF2_IsPlayerInCondition(iClient, view_as<TFCond>(82)))
		{
			SDKHooks_TakeDamage(iClient, 0, 0, 20000.0, DMG_SLASH);
			CreateTimer(0.2, Timer_HandleWOLives, iClient);
		}
		else
		{
			ForcePlayerSuicide(iClient);
			CreateTimer(0.2, Timer_HandleWOLives, iClient);
		}
	}
}

public Action TF2_CalcIsAttackCritical(int iClient, int iWeapon, char[] StrWeapon, bool &bCrit)
{
	if (g_enabled && ww_enable.BoolValue)
	{
		bCrit = false;
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

bool IsValidTeam(int iClient)
{
	int iTeam = GetClientTeam(iClient);
	if (iTeam == 2 || iTeam == 3)
	{
		return true;
	}
	return false;

}
stock void PrecacheModelEx(char[] strFileName, bool bPreload=false, bool bAddToDownloadTable=false)
{
	PrecacheModel(strFileName, bPreload);
	if (bAddToDownloadTable)
	{
		char strDepFileName[PLATFORM_MAX_PATH];
		Format(strDepFileName, sizeof(strDepFileName), "%s.res", strFileName);
		
		if (FileExists(strDepFileName))
		{
			// Open stream, if possible
			Handle hStream = OpenFile(strDepFileName, "r");
			if (hStream == INVALID_HANDLE)
			{
				LogMessage("Error, can't read file containing model dependencies.");
				return;
			}
			
			while(!IsEndOfFile(hStream))
			{
				char strBuffer[PLATFORM_MAX_PATH];
				ReadFileLine(hStream, strBuffer, sizeof(strBuffer));
				CleanString(strBuffer);
				
				// If file exists...
				if (FileExists(strBuffer, true))
				{
					// Precache depending on type, and add to download table
					if (StrContains(strBuffer, ".vmt", false) != -1)	  PrecacheDecal(strBuffer, true);
					else if (StrContains(strBuffer, ".mdl", false) != -1) PrecacheModel(strBuffer, true);
					else if (StrContains(strBuffer, ".pcf", false) != -1) PrecacheGeneric(strBuffer, true);
					AddFileToDownloadsTable(strBuffer);
				}
			}
			
			// Close file
			CloseHandle(hStream);
		}
	}
}
public void OnEntityCreated(int entity, const char[] classname)
{
	//PrintToChatAll("Entity: %s",classname);

	//if (ww_log.BoolValue)
	//{
	//	LogMessage("Entity Added : %s", classname);
	//}
	if(StrEqual(classname,"merasmus")){
		CreateTimer(0.1, MerasmusThink, entity, TIMER_REPEAT);
	}
	if (IsValidEntity(entity))
	{
		SDKHook(entity, SDKHook_Spawn, KillDrop);
		if (StrEqual(classname, "instanced_scripted_scene", false))
		{
			SDKHook(entity, SDKHook_Spawn, OnSceneStart);
		}
	}
	if (StrEqual(minigame, "BBall") && (StrEqual(classname, "tf_projectile_pipe")))
	{
		SDKHook(entity, SDKHook_StartTouch, Hook_StartTouch_Pipe);		
	}
	if((StrEqual(minigame, "MeetChester")) && (status == 2) && (StrEqual(classname, "prop_dynamic")))
	{
		SDKHook(entity, SDKHook_StartTouch, OnTouchChester);		
	}
	if((StrEqual(minigame, "CatchCube")) && (status == 2) && (StrEqual(classname, "prop_physics")))
	{
		SDKHook(entity, SDKHook_StartTouch, OnTouchCube);		
	}
}
public void OnMapEnd()
{
	ServerCommand("ww_gamedescription 0.0");
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsValidClient(i) && (!(IsFakeClient(i))))
		{
			SetOverlay(i,"");
		}
	}
}
public Action TpEnabled(int iClient)
{
	if (IsValidClient(iClient))
	{
		SetVariantInt(1);
		AcceptEntityInput(iClient, "SetForcedTauntCam");
		g_bThirdPersonEnabled[iClient] = true;
		return Plugin_Handled;
	}
	return Plugin_Continue;
}
public Action TpDisabled(int iClient)
{
	if (IsValidClient(iClient))
	{
		SetVariantInt(0);
		AcceptEntityInput(iClient, "SetForcedTauntCam");
		g_bThirdPersonEnabled[iClient] = false;
		return Plugin_Handled;
	}
	return Plugin_Continue;
}
// Fixed OnPlayerSpawned
public Action OnPlayerSpawned(Event event, const char[] name, bool dontBroadcast)
{
	int userid = GetEventInt(event, "userid");
	int iClient = GetClientOfUserId(userid);

	if (!IsValidClient(iClient))
	{
		return Plugin_Continue;
	}

	if (g_bThirdPersonEnabled[iClient])
	{
		CreateTimer(0.2, SetViewOnSpawn, userid);
	}

	if (g_Cvar_Snowball.BoolValue)
	{
		SetEntityRenderColor(iClient, 255, 255, 255, 255);
	}

	return Plugin_Continue;
}

public Action SetViewOnSpawn(Handle timer, any userid)
{
	int client = GetClientOfUserId(userid);
	if (client != 0)	//Checked g_bThirdPersonEnabled in hook callback, dont need to do it here~
	{
		SetVariantInt(1);
		AcceptEntityInput(client, "SetForcedTauntCam");
	}
	
	return Plugin_Continue;
}
stock void AddToClientWinPoints(int client)
{
	if (!IsValidClient(client))
	{
		return;
	}
	int points = 0;
	char oldpts[64];
	if (!IsFakeClient(client) && AreClientCookiesCached(client))
	{
		GetClientCookie(client, clientCookie, oldpts, sizeof(oldpts));
		points = StringToInt(oldpts);
		points += 1;
		IntToString(points, oldpts, sizeof(oldpts));
		SetClientCookie(client, clientCookie, oldpts);
	}
	if (points <= 1)
	{	
		char buffer[128];
		CPrintToChatAllEx(client, "%T\x01 %T", "TF2WARE_RANK_PREFIX_COLOR", client, "NOTIFY_VICTORY_FIRST", client, client);
		Format(buffer, sizeof(buffer), "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", LANG_SERVER, "NOTIFY_VICTORY_FIRST", LANG_SERVER, client);
	}
	else
	{
		char buffer[128];
		CPrintToChatAllEx(client, "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", client, "NOTIFY_VICTORY", client, client, points);
		Format(buffer, sizeof(buffer), "%T\x01 %T ", "TF2WARE_RANK_PREFIX_COLOR", LANG_SERVER, "NOTIFY_VICTORY", LANG_SERVER, client, points);
	}

	LogMessage("%N victory, wins: %d.", client, points);
}
void RemoveKart(int client)
{
	TF2_RemoveCondition(client, view_as<TFCond>(82));
	TF2_RemoveCondition(client, view_as<TFCond>(83));
}
public void TF2_OnConditionAdded(int client, TFCond condition)
{
	if((IsValidClient(client)) && (IsPlayerAlive(client)) && (status != 2)){
		if (g_Cvar_Snowball.BoolValue){
			if(condition == TFCond_Dazed)
			{
				SetEntityRenderColor(client, 0, 193, 255, 255);
				CreateParticle(client, "xms_icicle_impact_dryice", true, 1.0);
				TF2_RemoveCondition(client, TFCond_Dazed);
			}
		}
	}
}
public void TF2_OnConditionRemoved(int client, TFCond condition)
{
	if((IsValidClient(client)) && (IsPlayerAlive(client))){
		if (g_Cvar_Snowball.BoolValue){
			if(condition == TFCond_Dazed)
			{
				SetEntityRenderColor(client, 255, 255, 255, 255);
			}
		}
	}
}
public Action KartTime(Handle timer, any client)
{
	if (IsValidClient(client) && IsValidTeam(client))
	{

		if(TF2_IsPlayerInCondition(client, TFCond_Taunting))
		{
			TF2_RemoveCondition(client, TFCond_Taunting);
		}
		TF2_AddCondition(client, view_as<TFCond>(82), TFCondDuration_Infinite);
		//TF2_AddCondition(client, view_as<TFCond>(88), 3.0);
		//TeleportEntity(client, NULL_VECTOR, kartloc, NULL_VECTOR);
	}
	
	return Plugin_Stop;
}
public Action WinTimer(Handle timer, any client)
{
	if (IsValidClient(client))
	{
		TF2_AddCondition(client, TFCond_Kritzkrieged, 2.0);
		AddEntityToClient(client, AttachParticleWin(client, "drg_cow_explosioncore_charged", "head", 5.0));
		CreateTimer(4.0, RemoveWinEffect, client);
	}
	
	return Plugin_Stop;
}
public Action LateWinners(Handle timer, any client)
{
	if (IsValidClient(client))
	{
		if(userInit[client] == 1){
			GetWinnerPoints(client);
		}
	}
	
	return Plugin_Stop;
}
public Action RemoveWinEffect(Handle timer, any client)
{
	if (IsValidClient(client))
	{
		DeleteParticleWinner(g_Ent[client]);
	}
	
	return Plugin_Stop;
}
public Action WaterTime(Handle timer, any client)
{
	if (IsValidClient(client))
	{
		float waterloc[3];
		GetClientAbsAngles(client, waterloc);
		SetEntityMoveType(client, MOVETYPE_WALK);
		TF2_AddCondition(client, view_as<TFCond>(86), TFCondDuration_Infinite);
	}
	
	return Plugin_Stop;
}
stock void SetSpeedHorsemann(int client)
{
	SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 800.0);
}
stock void SetSpeedHeavy(int client)
{
	if (StrEqual(minigame, "Ghostbusters"))
	{
		SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 320.0);
	}
	else
	SetEntPropFloat(client, Prop_Send, "m_flMaxspeed", 280.0);
}
public void SDKHooks_SpeedClient(int client)
{
	if(IsValidClient(client))
	{
		if (StrEqual(minigame, "HHH"))
		{
			SetSpeedHorsemann(client);
		}
		else
		SetSpeedHeavy(client);
	}
}
/* HLStats Events. - Roy */
public Action Event_Win(Handle hEvent, const char[] sName, bool bDontBroadcast)
{
	int iClient = GetClientOfUserId(GetEventInt(hEvent, "userid"));
	
	LogPlayerEvent(iClient, "triggered", sName);
	
	return Plugin_Continue;
}

public Action Event_GameWin(Handle hEvent, const char[] sName, bool bDontBroadcast)
{
	int iClient = GetClientOfUserId(GetEventInt(hEvent, "userid"));
	
	LogPlayerEvent(iClient, "triggered", sName);
	
	return Plugin_Continue;
}
public void BallBreak(const char[] output, int caller, int activator, float delay){

	if(caller == -1){
		return;
	}	
	AcceptEntityInput(caller, "Kill");
}
public Action NoBallStunSound(int clients[64], int numClients, char Pathname[PLATFORM_MAX_PATH], int entity, int channel, float volume, int level, int pitch, int flags) 
{
	if(g_Cvar_Snowball.BoolValue){
		if(StrContains(Pathname, "pl_impact_stun.wav", false) != -1)return Plugin_Stop;
		else
		return Plugin_Continue;
	}
	return Plugin_Continue;
}
public Action Timer_Comments_Lose(Handle timer, any iClient)
{
	if(IsValidClient(iClient))
	{  
		int lose_sound = GetRandomInt(1,23);
		char lose_sound_string[256];
		if(lose_sound == 9 || lose_sound == 2 || lose_sound == 17 || lose_sound == 19){
			lose_sound = GetRandomInt(1,23);
			return Plugin_Handled;
		}
		if(lose_sound <= 8){
				Format(lose_sound_string, sizeof(lose_sound_string), "0%i", lose_sound);
		}
		if(lose_sound >= 10){
			Format(lose_sound_string, sizeof(lose_sound_string), "%i", lose_sound);
		}
		char lss_sound[256];
		Format(lss_sound,sizeof(lss_sound),"vo/compmode/cm_admin_callout_no_%s.mp3", lose_sound_string);
		PrecacheSound(lss_sound, true);
		EmitSoundToClient(iClient, lss_sound);
	}
	return Plugin_Handled;
}
public Action Timer_Comments_Win(Handle timer, any iClient)
{
	if(IsValidClient(iClient))
	{  
		int win_sound = GetRandomInt(1,17);
		char win_sound_string[256];
		if(win_sound == 5 || win_sound == 2 || win_sound == 7 || win_sound == 16){
			win_sound = GetRandomInt(1,23);
			return Plugin_Handled;
		}
		if(win_sound <= 9){
				Format(win_sound_string, sizeof(win_sound_string), "0%i", win_sound);
		}
		if(win_sound >= 10){
			Format(win_sound_string, sizeof(win_sound_string), "%i", win_sound);
		}
		char wss_sound[256];
		Format(wss_sound,sizeof(wss_sound),"vo/compmode/cm_admin_callout_yes_%s.mp3", win_sound_string);
		PrecacheSound(wss_sound, true);
		EmitSoundToClient(iClient, wss_sound);
	}
	return Plugin_Handled;
}

stock void TrainingMessage(int client, char[] objective, char[] message, float duration=0.0)
{
/*	if (TimerHUD != INVALID_HANDLE){
		CloseHandle(TimerHUD);
		TimerHUD = INVALID_HANDLE;
	} */
   
	if(GameRules <= 0)
	{
		GameRules = FindEntityByClassname(-1, "tf_gamerules");
		if(GameRules <= 0)
			GameRules = CreateEntityByName("tf_gamerules");
	}
   
	ShowMessage = true;

	ChangeEdictState(GameRules, m_bIsInTrainingOffset);
	ChangeEdictState(GameRules, m_bIsTrainingHUDVisibleOffset);
   
	Handle MessageObj = StartMessageOne("TrainingObjective", client);
	if (MessageObj != INVALID_HANDLE)
	{
		BfWriteString(MessageObj, objective); //Message
		EndMessage();
	}
	 
	Handle Message = StartMessageOne("TrainingMsg", client);
	if (Message != INVALID_HANDLE)
	{
		BfWriteString(Message, message); //Message
		EndMessage();
	}
   
	if(duration > 0.0)
	{
		CreateTimer(GetSpeedMultiplier(duration), ClearHud, client);
	}
	else
	{
		CreateTimer(GetSpeedMultiplier(3.0), ClearHud, client);
	}
}

public Action ClearHud(Handle timer)
{
	ShowMessage = false;
   
   ChangeEdictState(GameRules, m_bIsInTrainingOffset);
   ChangeEdictState(GameRules, m_bIsTrainingHUDVisibleOffset);
	
	return Plugin_Stop;
}
public Action Training_Callback(int entity, const char[] propName, int &iValue, int element)
{
	if(ShowMessage)
	{
		iValue = 1;
	}
	else
	{
		iValue = 0;
	}
	return Plugin_Changed;
} 