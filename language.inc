#include <open.mp>

#if !defined PP_SYNTAX_ON_EXIT
    #define PP_SYNTAX_ON_EXIT
#endif

#if !defined PP_SYNTAX_ON_INIT
    #define PP_SYNTAX_ON_INIT
#endif

#include <PawnPlus>

#include <YSI_Coding\y_va>
#include <YSI_Data\y_iterate>

#if !defined MAX_LANGS
    #define MAX_LANGS 30
#endif

#if !defined MAX_LANG_NAME
    #define MAX_LANG_NAME 40
#endif

static 
    Pool: Language_gsList,
    Map: Language_gsMap[MAX_LANGS],
    Language: Language_gsPlayer[MAX_PLAYERS];

// --
// API
// --

/**
 * <library>language</library>
 * <summary>Initializes a language.</summary>
 * <param name="name">Language name.</param>
 * <returns>Language index, -1 if language exists.</returns>
 */
stock Language: Language_Init(const string: name[])
{
    //name is just in case, you can write whatever, maps are points
    if(pool_find_str(Language_gsList, name) != -1)
    {
        return Language: -1;
    }
    new Language: idx = Language: pool_add_str(Language_gsList, name);
    Language_gsMap[_:idx] = map_new();
    return idx;
}

/**
 * <library>language</library>
 * <summary>Removes a language.</summary>
 * <param name="id">Language id (index).</param>
 * <returns>False if language is invalid, otherwise true if removed.</returns>
 */
stock bool: Language_Remove(Language: id)
{
    if(map_valid(Language_gsMap[_:id]))
    {
        return false;
    }
    map_delete_deep(Language_gsMap[_:id]);
    pool_remove(Language_gsList, _:id);
    return true;
}

/**
 * <library>language</library>
 * <summary>Removes a language.</summary>
 * <param name="id">Language id (index).</param>
 * <param name="key">Key.</param>
 * <param name="text_value">Text to send.</param>
 * <returns>False if language is invalid, otherwise true if added.</returns>
 */
stock bool: Language_AddKey(Language: id, const string: key[], const string: text_value[])
{
    if(!map_valid(Language_gsMap[_:id]))
    {
        return false;
    }
    map_str_add_str(Language_gsMap[_:id], key, text_value);
    return true;
}

/**
 * <library>language</library>
 * <summary>Sets player's language.</summary>
 * <param name="playerid">Player we set language to.</param>
 * <param name="id">Language we set.</param>
 * <returns>No return values.</returns>
 */
stock void: Language_SetPlayer(playerid, Language: id)
{
    Language_gsPlayer[playerid] = id;
}

/**
 * <library>language</library>
 * <summary>Gets player's language.</summary>
 * <param name="playerid">Player we get language from.</param>
 * <returns>Player's language.</returns>
 */
stock Language: Language_GetPlayer(playerid)
{
    return Language_gsPlayer[playerid];
}

/**
 * <library>language</library>
 * <summary>Gets language name.</summary>
 * <param name="id">Language id.</param>
 * <param name="name">Destination to store the name.</param>
 * <returns>False if language doesn't exist, otherwise true.</returns>
 */
stock Language_GetName(Language: id, name[MAX_LANG_NAME], len=sizeof(name))
{
    if(!pool_has(Language_gsList, _:id))
    {
        return false;
    }
    pool_get_str(Language_gsList, _:id, name, len);
    return true;
}

// --
// Initializers and finalizers
// --

pawn_on_init[lang]
{
    Language_gsList = pool_new();
}

pawn_on_exit[lang]
{
    pool_delete_deep(Language_gsList);
}

// --
// Hooks
// --

//SendClientMessage
stock __language_SendClientMessage(playerid, colour, const format[], OPEN_MP_TAGS: ...)
{
    new Language: lang = Language_gsPlayer[playerid];
    if(map_has_str_key(Language_gsMap[_:lang], format))
    {
        new text[144];
        map_str_get_str(Language_gsMap[_:lang], format, text);
        return SendClientMessage(playerid, colour, text, ___(3));
    }
    else
    {
        return SendClientMessage(playerid, colour, format, ___(3));
    }
}

#if defined _ALS_SendClientMessage
    #undef SendClientMessage
#else
    #define _ALS_SendClientMessage
#endif

#define SendClientMessage __language_SendClientMessage

//SendClientMessageToAll
stock __language_SendClientMessageToAll(colour, const format[], OPEN_MP_TAGS: ...)
{
    foreach(new i : Player)
    {
        __language_SendClientMessage(i, colour, format, ___(2));
    }
}

#if defined _ALS_SendClientMessageToAll
    #undef SendClientMessageToAll
#else
    #define _ALS_SendClientMessageToAll
#endif

#define SendClientMessageToAll __language_SendClientMessageToAll

// SendPlayerMessageToPlayer
stock __language_SendPlayerMessageToPlayer(playerid, senderid, const format[], OPEN_MP_TAGS:...)
{
    new Language: lang = Language_gsPlayer[playerid];
    if(map_has_str_key(Language_gsMap[_:lang], format))
    {
        new text[144];
        map_str_get_str(Language_gsMap[_:lang], format, text);
        return SendPlayerMessageToPlayer(playerid, senderid, text, ___(3));
    }
    else
    {
        return SendPlayerMessageToPlayer(playerid, senderid, format, ___(3));
    }
}

#if defined _ALS_SendPlayerMessageToPlayer
    #undef SendPlayerMessageToPlayer
#else
    #define _ALS_SendPlayerMessageToPlayer
#endif

#define SendPlayerMessageToPlayer __language_SendPlayerMessageToPlayer

//GameTextForPlayer
stock __language_GameTextForPlayer(playerid, const string[], time, style, OPEN_MP_TAGS: ...)
{
    new Language: lang = Language_gsPlayer[playerid];
    if(map_has_str_key(Language_gsMap[_:lang], string))
    {
        new text[144];
        map_str_get_str(Language_gsMap[_:lang], string, text);
        return GameTextForPlayer(playerid, text, time, style, ___(4));
    }
    else
    {
        return GameTextForPlayer(playerid, string, time, style, ___(4));
    }
}

#if defined _ALS_GameTextForPlayer
    #undef GameTextForPlayer
#else
    #define _ALS_GameTextForPlayer
#endif

#define GameTextForPlayer __language_GameTextForPlayer

//GameTextForAll
stock __language_GameTextForAll(const string[], time, style, OPEN_MP_TAGS: ...)
{
    foreach(new i : Player)
    {
        __language_GameTextForPlayer(i, string, time, style, ___(3));
    }
}

#if defined _ALS_GameTextForAll
    #undef GameTextForAll
#else
    #define _ALS_GameTextForAll
#endif

#define GameTextForAll __language_GameTextForAll

// I'm still figuring out how to make dialog language available if you find any bug report as soon as possible

stock __language_ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE: style, const caption[], const info[], const button1[], const button2[])
{
    new Language: lang = Language_gsPlayer[playerid];
    new 
        dialog_caption[60],
        dialog_info[3800],
        dialog_button1[30],
        dialog_button2[30];

    //caption
    if(map_has_str_key(Language_gsMap[_:lang], caption))
    {
        map_str_get_str(Language_gsMap[_:lang], caption, dialog_caption);
    }
    else
    {
        strcopy(dialog_caption, caption);
    }
    //info
    if(map_has_str_key(Language_gsMap[_:lang], info))
    {
        map_str_get_str(Language_gsMap[_:lang], info, dialog_info);
    }
    else
    {
        strcopy(dialog_info, info);
    }
    //button1
    if(map_has_str_key(Language_gsMap[_:lang], button1))
    {
        map_str_get_str(Language_gsMap[_:lang], button1, dialog_button1);
    }
    else
    {
        strcopy(dialog_button1, button1);
    }
    //button2
    if(map_has_str_key(Language_gsMap[_:lang], button2))
    {
        map_str_get_str(Language_gsMap[_:lang], button2, dialog_button2);
    }
    else
    {
        strcopy(dialog_button2, button2);
    }

    return ShowPlayerDialog(playerid, dialogid, style, dialog_caption, dialog_info, dialog_button1, dialog_button2);
}

#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif

#define ShowPlayerDialog __language_ShowPlayerDialog