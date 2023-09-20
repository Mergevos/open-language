# language
[![Mergevos](https://img.shields.io/badge/Mergevos-open--language-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/open-language)

Minimalistic multi language library.

## Installation

Simply install to your project:

```bash
sampctl install Mergevos/open-language
```

Include in your code and begin using the library:

```pawn
#include <language>
```

## Usage

Simply declare a new language like:
```pawn
new Language: serbian;
new Language: english;

public OnGameModeInit()
{
	serbian = Language_Init("Serbian");
	english = Language_Init("English");
	return 1;
}
```
Add a translation key like:
```pawn
Language_AddKey(serbian, "TESTKEY", "Supak %d");
Language_AddKey(serbian, "BUTTON_YES", "Daa");
Language_AddKey(serbian, "INFO", "Ja sam novi mali peder\nvolim vas");

```

Everytime a `SendClientMessage` was sent with this key as text, it will send the value. This also goes for `GameTextForPlayer`, `SendPlayerMessageToPlayer` and soon, I hope for `ShowPlayerDialog`.
No new send functions, only hooks.
Full example:
```pawn

public OnPlayerSpawn(playerid)
{
	Language_SetPlayer(playerid, serbian);
	
	Language_AddKey(english, "TESTKEY", "Ass %d");
	Language_AddKey(english, "BUTTON_YES", "YES");
	Language_AddKey(english, "INFO", "I'm small gay\nlove yall");

	SendClientMessage(playerid, -1, "Testiramo scm %d %s", 0, ReturnPlayerName(playerid));
	SendClientMessage(playerid, -1, "TESTKEY", 33);

	SendPlayerMessageToPlayer(playerid, playerid, "testiramo spmp");
	SendPlayerMessageToPlayer(playerid, playerid, "INFO");

	GameTextForPlayer(playerid, "BUTTON_YES", 3000, 4);

    // we are testing english language from here
	Language_SetPlayer(playerid, english);

	SendClientMessage(playerid, -1, "Testiramo scm %d %s", 0, ReturnPlayerName(playerid));
	SendClientMessage(playerid, -1, "TESTKEY", 33);

	SendPlayerMessageToPlayer(playerid, playerid, "testiramo spmp");
	SendPlayerMessageToPlayer(playerid, playerid, "INFO");

	GameTextForPlayer(playerid, "BUTTON_YES", 3000, 4);

	new lang[MAX_LANG_NAME];
	Language_GetName(Language_GetPlayer(playerid), lang);
	SendClientMessage(playerid, -1, "Player's language is %s", lang);

	
	return 1;
}
```

## Testing


To test, simply run the package:

```bash
sampctl package run
```
