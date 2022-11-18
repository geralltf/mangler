/*-
* Copyright © Daemon Mad Games, Mr Gerallt G. Franke 2017.
* All rights reserved.
* Description: Athena Engine.
* Date: 18/05/2017
* Licence: For the purposes of marking and educational uses only.
*          All rights reserved to the author.
*          For commercial use please send a cheque to geralltf at gmail.com.
* Abstract: The Athena Engine.
*
*                                      '
*    [ ATHENA V1.17 ]           . ` ;  ::
*                              , : +:;,,@
*                            '#:. '#` + @;
*            ;               :+:,.@#;'.; +,
*            `               '+;.'@'+@@ :@+
*           @              '+;;'+@@'@@' +@'@
*           @              :'::##@+##':@#@@''
*         +:+              #::;;@@,;@@+@##;@@:
*        @'##            ` +;###@#@''#.@@###@@+
*        '#@@            .`@+@#;+@';@@#+@'##@@@++
*       #@@@@            ++@++;;@':+;+:;@+++#@@+#+'
*       #@@@@#          ##@++#++:'+';@#@#:'';+@#;#;@
*      '@@ @@@,        #+#;@#@##;+.,@#@+@+;;''#@#;@+@
*      @#@@@@@#      `;@#@##@+##;#@,#@#@#@;:;;:+@@:@;#,
*      #@@@@@@@#    :'#,:+#'#++@#+#+;@@@@@;''::;'@#+,#'+
*     ,#@@@@@@@@@   ;'+@@;'#@+@#@@@@#+'@@@@;@@##::@;@;,'+
*     @#@@@@@@@@@@  :`+@###+;@';@'+;.`'@@@@@@@#@#@:@,@;:++
*    #@@@@@@@@@@@@@ #@;@#@@+'@'@+#@@@+@@@@@#@'@@#@@;@;@';@'
*    @@@@@    @@@@@@@++++@+'@@@@@@@#@@;+@@@###+@@@@@'#;+;'#@
*   +@@@@@@@@@@@@@@@#+@@@+@+;+@@,+@`@:+@@@###@+` @@@@#';+#;@;
*   @@@@@@@@@@@@@@@@@@;#'##+###@@#+@@@@'@@##@@@@      @';;@'#
*   @#@@@@@@@#@@@@@#@+#@+@#@;@@ FRANKE @@@##@@@#@     ,@:;'@;+
*  #@#@@@@@@@     +@@:#++#+@@@@@#@@@@@@@@@@@@@@@@     ;#@:;'@#+
*  @##@@@@@#       @@@@+#+##'@@@@@++@@#@@@@@@@@@###;   @#@;''#@,
*  @@@@@@@@        '@##'#@;#@@@@@@@@@@@@@@@   @@@@#+`  @@'@;'';@'
* ,@@ @@@,         @#+#@''',#@@@@@@@@@@@@@@@@@@@@@#@#' `@@'@'+''@++;
* @##@@@@          .@##+#+#;@@@@@@@@#@@@@@@#@@@#@@@@+@. @  ;''++'#'++
* @@@@@#           `#@#'+++`,+@@@@@#@@  +. #@@#  @@@##@ @   ;''.`'#+#+
* @@@@'             #@+@+:+`` @@@#@+`      @@@+    @@#++    ,@'+ @'++#
*'@@@@              #@##@#+# #@@@@         @@@'     @@@@'    '@++ '+##+
*@@@                @@@;@#'+,#@@+          @@@.      #@@@     '+'#+;++@'
*@@;                '@#@##'#'@ @           @@@        @@+      '''@'+++#:
*@@;                 ###@@@,#.             +#@        .@#,     ,''#''+++'
*@@                  `'@@'#@#.   ,         ###`        @++      ##' .;++#
*@@                +  :@@#+@,@+  ,     '   @@#+        ;#@:      '+   ++##
*@                  '  ++@.''::; '@   ;    ##@@:        #@:       +;   '++
*@                  `;   @;@#+' #@;  :     @##@+        @#'     +  +    '+
*                    +'; :@'':: +'  #+     +##@@:       @:+        +     +'
*                     @''#';@+++: ,.@      #@@@@+       ##'         .    :#
*                      `@+ `';;+@' ;,       @#@@#@       '+               +
*                       @@@ ',,+# @`        @@#@@#       @;               ,:
*                         #;+.'':,          @@#,@@;      @+                #
*                         :@#+.''`           @# @@@      @#                .
*                         '@#''#;             @ @@#      @+                 `
*                          @#@@#;             @:`@#      @#
*                           @@@+              @: @#      #+
*                            ''               @; '#      '+
*                            ,,               @. ,@      .++
*                            `                @`  @      .@
*                                             +   @       @
*                                                 .        '
**********************************************************  +  */


#ifndef GAMECONF_H
#define GAMECONF_H

#include <map>
#include <regex>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Common.h"

enum GameStartupOption
{
	GAME_RUNTIME_MODE,
	EDITOR_MODE,
	UNIT_TESTING,
	UNIT_TESTING_ONLY,
	HIDE_ERRORS,
	TRACE_MESSAGES, // TODO: Toggle on and off trace messages 
	LOGGING, // TODO: implement logging of trace messages to file
	DEBUG_NOISE,
	CAP_FPS
};

enum AssetTextureName
{
	TEX_CAMERA,
	TEX_CURSOR,
	TEX_CURSOR2,
	TEX_LINET,
	TEX_PAINTT,
	TEX_WHITE,
	TEX_WATER,
	TEX_BLACK,
	TEX_MOUNTAIN,
	TEX_GRASS,
	TEX_FIRE,
	TEX_PLAYER_L,
	TEX_PLAYER_R
};

class GameConfig
{
public:
	int															argc;
	char														**argv;

	std::map<std::string, bool>									Options; // parsed
	std::map<std::string, float>								fValues;
	std::map<std::string, unsigned int>							uValues;
	std::map<std::string, sf::Vector2f>							v2fValues;
	std::map<std::string, sf::Vector3f>							v3fValues;

	// The String Parameters specified at startup. 
	std::map<std::string, std::string>							StartupParameters;

	// The Parameters from game configuration (Game.ini).
	std::map<std::string, std::map<std::string, std::string>>	Game; // Parsed, but all are as string values

																	  // Keys Bindings.
	sf::Keyboard::Key											KEY_QUIT, KEY_LOAD, KEY_SAVE, KEY_NEWFILE, KEY_GENLEVEL, KEY_GENZPVE, KEY_GENZNEG;
	sf::Keyboard::Key											GAME_KEY_PLAYER_LEFT, GAME_KEY_PLAYER_RIGHT, GAME_KEY_PLAYER_JUMP;

	std::map<std::string, sf::Keyboard::Key>					MapStringKey; // All the system keys associated to string values.
	std::map<std::string, sf::Keyboard::Key>					keyBindings; // All the keys specified under "Key Bindings" in ini conf.

																			 // Configuration Constants.
	sf::Vector2f			v2fDispResolution, v2fDispResolutionEditor;
	float					fCameraMoveSpeed;
	sf::Vector2f			fpspos, consolePos;
	unsigned int			fontsizelarge, fontsizemed, fontsizesmall;
	sf::Color				fontColour;
	std::string				fontDefault;
	sf::Vector2f			cursorScale, cellOffset, cellScale, gridPosition, gridSize;
	std::string				assetFolder, buildFolder;
	std::string				gameExecutable;
	std::string				saveFile;
	float					rotationAnglePve, rotationAngleNeg, zoomInFactor, zoomOutFactor;
	sf::Vector2f			physGravity;
	sf::Vector2f			playerScale;
	bool					RestartApplication = false;

	char PATH_SEPARATOR =
#ifdef _WIN32 
		'\\';
#else 
		'/';
#endif

private:
	std::map<std::string, sf::Keyboard::Key>* createBindingMap();

public:

	void ApplyConfiguration()
	{
		// Parse key bindings from what was taken from the configuration file.
		ParseKeyBindings();

		// Finer grained parsing using data from game configuration dictionary.
		v2fDispResolution = GetVector2("display", "v2fdispresolution");
		v2fDispResolutionEditor = GetVector2("display", "v2fdispresolutioneditor");
		fCameraMoveSpeed = GetFloat("navigation", "fcameramovespeed");
		fpspos = GetVector2("hud", "v2ufpsposition");
		consolePos = GetVector2("hud", "v2uconsolepos");

		fontsizelarge = GetUint("fonts", "ufontsizelarge");
		fontsizemed = GetUint("fonts", "ufontsizemed");
		fontsizesmall = GetUint("fonts", "ufontsizesmall");
		fontColour = GetRGB("fonts", "v3ffontcolour");
		fontDefault = Game["fonts"]["sfontdefault"];

		cursorScale = GetVector2("world", "v2fcursorscale");
		cellOffset = GetVector2("world", "v2fcelloffset");
		cellScale = GetVector2("world", "v2fcellscale");
		gridPosition = GetVector2("world", "v2fgridposition");
		gridSize = GetVector2("world", "v2fgridsize");

		assetFolder = Game["game path"]["sassetfolder"];
		buildFolder = Game["game path"]["sbuildfolder"];
		gameExecutable = Game["game path"]["sgameexecutable"];
		saveFile = Game["game path"]["ssavefile"];

		rotationAnglePve = GetFloat("navigation", "frotatefactorpve");
		rotationAngleNeg = GetFloat("navigation", "frotatefactorneg");
		zoomInFactor = GetFloat("navigation", "fzoominfactor");
		zoomOutFactor = GetFloat("navigation", "fzoomoutfactor");
		physGravity = GetVector2("game character", "v2fphysgravity");
		playerScale = GetVector2("game character", "v2fplayerscale");
	}

	void ParseKeyBindings()
	{
		// Apply Key Bindings From Configuration.
		std::map<std::string, std::string>::iterator it;
		std::map<std::string, std::string> bindings = Game["key bindings"];

		// Create a map between a string value and a Key enumeration.
		MapStringKey = *createBindingMap();

		// Iterate through each key binding found in configuration and bind each key to a value in keyBindings map.
		for (it = bindings.begin(); it != bindings.end(); ++it)
		{
			keyBindings[it->first] = MapStringKey[stolower(it->second)];
		}

		// Explicitly set application keys for manually checking later.
		KEY_QUIT = keyBindings["key_quit"];
		KEY_LOAD = keyBindings["key_load"];
		KEY_SAVE = keyBindings["key_save"];
		KEY_NEWFILE = keyBindings["key_newfile"];
		KEY_GENLEVEL = keyBindings["key_genlevel"];
		KEY_GENZPVE = keyBindings["key_genzpve"];
		KEY_GENZNEG = keyBindings["key_genzneg"]; // TODO: could automate this process of key binding
		GAME_KEY_PLAYER_LEFT = keyBindings["game_key_player_left"];
		GAME_KEY_PLAYER_RIGHT = keyBindings["game_key_player_right"];
		GAME_KEY_PLAYER_JUMP = keyBindings["game_key_player_jump"];
	}

	std::vector<std::string> GetList(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		std::string value = Game[category][name];
		std::vector<std::string>	list;
		int							size;
		std::string					token;
		std::istringstream	ss(value);


		while (std::getline(ss, token, ','))
		{
			list.push_back(trim(token));
		}

		return list;
	}

	unsigned int GetUint(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		std::string value = Game[category][name];

		return stoi(value);
	}
	unsigned int GetInt(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		std::string value = Game[category][name];

		return stoi(value);
	}

	float GetFloat(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		std::string value = Game[category][name];

		return stof(value);
	}

	sf::Vector2f GetVector2(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		auto lst = GetList(category, name);
		sf::Vector2f	vec;

		if (lst.size() == 2)
		{
			vec.x = stof(lst[0]);
			vec.y = stof(lst[1]);
		}

		return vec;
	}

	sf::Vector3f GetVector3(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		auto lst = GetList(category, name);
		sf::Vector3f	vec;

		if (lst.size() == 3)
		{
			vec.x = stof(lst[0]);
			vec.y = stof(lst[1]);
			vec.z = stof(lst[2]);
		}

		return vec;
	}
	sf::Color GetRGB(std::string category, std::string name) //HACK: program the GameConfigParser to automatically parse this type
	{
		auto lst = GetList(category, name);
		sf::Color	vec;

		if (lst.size() == 3)
		{
			vec.r = stof(lst[0]) * 255;
			vec.g = stof(lst[1]) * 255;
			vec.b = stof(lst[2]) * 255;
			vec.a = 255;
		}

		return vec;
	}

	sf::VideoMode GetVideoMode()
	{
		sf::VideoMode mode;

		if (GetDisplayStyle() == sf::Style::Fullscreen)
		{
			const std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

			mode = modes.at(0);
		}
		else
		{
			if (IsEnabled(GameStartupOption::EDITOR_MODE))
			{
				mode = sf::VideoMode((int)v2fDispResolutionEditor.x, (int)v2fDispResolutionEditor.y);
			}
			else
			{
				mode = sf::VideoMode((int)v2fDispResolution.x, (int)v2fDispResolution.y);
			}


		}

		return mode;
	}

	std::string GetAppTitle()
	{
		std::string title = "Athena";

		if (IsEnabled(GameStartupOption::EDITOR_MODE))
		{
			title = Game["build"]["seditortitle"];
		}
		else if (IsEnabled(GameStartupOption::GAME_RUNTIME_MODE))
		{
			title = Game["build"]["sgametitle"];
		}

		return title;
	}
	sf::Uint32 GetDisplayStyle()
	{
		sf::Uint32 style = sf::Style::Default;
		std::string conf = "default"; // default|fullscreen|none|resize

		if (IsEnabled(GameStartupOption::EDITOR_MODE))
		{
			conf = Game["display"]["seditordispstyle"];
		}
		else if (IsEnabled(GameStartupOption::GAME_RUNTIME_MODE))
		{
			conf = Game["display"]["sgamedispstyle"];
		}

		if (conf == "default")
		{
			style = sf::Style::Default;
		}
		else if (conf == "fullscreen")
		{
			style = sf::Style::Fullscreen;
		}
		else if (conf == "none")
		{
			style = sf::Style::None;
		}
		else if (conf == "resize")
		{
			style = sf::Style::Resize;
		}

		return style;
	}

	// Check if the specified GameStartupOption has been enabled.
	bool IsEnabled(GameStartupOption option)
	{
		switch (option)
		{
		case GameStartupOption::EDITOR_MODE:
			return Options["editor"];
		case GameStartupOption::GAME_RUNTIME_MODE:
			return !Options["editor"];
		case GameStartupOption::HIDE_ERRORS:
			return Options["hide-errors"];
		case GameStartupOption::UNIT_TESTING:
			return Options["test"];
		case GameStartupOption::UNIT_TESTING_ONLY:
			return Options["tests-only"];
		case GameStartupOption::DEBUG_NOISE:
			return Options["debugnoiseui"] || (Game["debugging"]["bbebugnoiseui"] != "0");
		case GameStartupOption::CAP_FPS:
			if (Game["display"]["bcapframerate"] != "0")
			{
				float framerateFreq = GetFloat("display", "fcapframeratefreg");
				if (framerateFreq > 0)
				{
					return true;
				}
			}
			return false;
		default:
			return false;
		}
	}
	std::string GetAssetFilePath(AssetTextureName asset)
	{
		switch (asset)
		{
		case AssetTextureName::TEX_BLACK:
			return Game["asset list"]["tex_black"];
		case AssetTextureName::TEX_CAMERA:
			return Game["asset list"]["tex_camera"];
		case AssetTextureName::TEX_CURSOR:
			return Game["asset list"]["tex_cursor"];
		case AssetTextureName::TEX_CURSOR2:
			return Game["asset list"]["tex_cursor2"];
		case AssetTextureName::TEX_FIRE:
			return Game["asset list"]["tex_fire"];
		case AssetTextureName::TEX_GRASS:
			return Game["asset list"]["tex_grass"];
		case AssetTextureName::TEX_LINET:
			return Game["asset list"]["tex_linet"];
		case AssetTextureName::TEX_MOUNTAIN:
			return Game["asset list"]["tex_mountain"];
		case AssetTextureName::TEX_PAINTT:
			return Game["asset list"]["tex_paintt"];
		case AssetTextureName::TEX_PLAYER_L:
			return Game["asset list"]["tex_player_l"];
		case AssetTextureName::TEX_PLAYER_R:
			return Game["asset list"]["tex_player_r"];
		case AssetTextureName::TEX_WATER:
			return Game["asset list"]["tex_water"];
		case AssetTextureName::TEX_WHITE:
			return Game["asset list"]["tex_white"];
		default:
			return "";
		}
	}


private:
	// Check if the specified option has been enabled.
	bool IsEnabled(std::string option)
	{
		return Options[option];
	}

};


// Creates a map between string and Key Bindings.
inline std::map<std::string, sf::Keyboard::Key>* GameConfig::createBindingMap()
{
	std::map<std::string, sf::Keyboard::Key>*	_map = new std::map<std::string, sf::Keyboard::Key>();
	(*_map)["a"] = sf::Keyboard::Key::A;
	(*_map)["b"] = sf::Keyboard::Key::B;
	(*_map)["c"] = sf::Keyboard::Key::C;
	(*_map)["d"] = sf::Keyboard::Key::D;
	(*_map)["e"] = sf::Keyboard::Key::E;
	(*_map)["f"] = sf::Keyboard::Key::F;
	(*_map)["g"] = sf::Keyboard::Key::G;
	(*_map)["h"] = sf::Keyboard::Key::H;
	(*_map)["i"] = sf::Keyboard::Key::I;
	(*_map)["j"] = sf::Keyboard::Key::J;
	(*_map)["k"] = sf::Keyboard::Key::K;
	(*_map)["l"] = sf::Keyboard::Key::L;
	(*_map)["m"] = sf::Keyboard::Key::M;
	(*_map)["n"] = sf::Keyboard::Key::N;
	(*_map)["o"] = sf::Keyboard::Key::O;
	(*_map)["p"] = sf::Keyboard::Key::P;
	(*_map)["q"] = sf::Keyboard::Key::Q;
	(*_map)["r"] = sf::Keyboard::Key::R;
	(*_map)["s"] = sf::Keyboard::Key::S;
	(*_map)["t"] = sf::Keyboard::Key::T;
	(*_map)["u"] = sf::Keyboard::Key::U;
	(*_map)["v"] = sf::Keyboard::Key::V;
	(*_map)["w"] = sf::Keyboard::Key::W;
	(*_map)["x"] = sf::Keyboard::Key::X;
	(*_map)["y"] = sf::Keyboard::Key::Y;
	(*_map)["z"] = sf::Keyboard::Key::Z;
	(*_map)["num0"] = sf::Keyboard::Key::Num0;
	(*_map)["num1"] = sf::Keyboard::Key::Num1;
	(*_map)["num2"] = sf::Keyboard::Key::Num2;
	(*_map)["num3"] = sf::Keyboard::Key::Num3;
	(*_map)["num4"] = sf::Keyboard::Key::Num4;
	(*_map)["num5"] = sf::Keyboard::Key::Num5;
	(*_map)["num6"] = sf::Keyboard::Key::Num6;
	(*_map)["num7"] = sf::Keyboard::Key::Num7;
	(*_map)["num8"] = sf::Keyboard::Key::Num8;
	(*_map)["num9"] = sf::Keyboard::Key::Num9;
	(*_map)["escape"] = sf::Keyboard::Key::Escape;
	(*_map)["lcontrol"] = sf::Keyboard::Key::LControl;
	(*_map)["lshift"] = sf::Keyboard::Key::LShift;
	(*_map)["lalt"] = sf::Keyboard::Key::LAlt;
	(*_map)["lsystem"] = sf::Keyboard::Key::LSystem;
	(*_map)["rcontrol"] = sf::Keyboard::Key::RControl;
	(*_map)["rshift"] = sf::Keyboard::Key::RShift;
	(*_map)["ralt"] = sf::Keyboard::Key::RAlt;
	(*_map)["rsystem"] = sf::Keyboard::Key::RSystem;
	(*_map)["menu"] = sf::Keyboard::Key::Menu;
	(*_map)["lbracket"] = sf::Keyboard::Key::LBracket;
	(*_map)["rbracket"] = sf::Keyboard::Key::RBracket;
	(*_map)["semicolon"] = sf::Keyboard::Key::SemiColon;
	(*_map)["comma"] = sf::Keyboard::Key::Comma;
	(*_map)["period"] = sf::Keyboard::Key::Period;
	(*_map)["quote"] = sf::Keyboard::Key::Quote;
	(*_map)["slash"] = sf::Keyboard::Key::Slash;
	(*_map)["backslash"] = sf::Keyboard::Key::BackSlash;
	(*_map)["tilde"] = sf::Keyboard::Key::Tilde;
	(*_map)["equal"] = sf::Keyboard::Key::Equal;
	(*_map)["dash"] = sf::Keyboard::Key::Dash;
	(*_map)["space"] = sf::Keyboard::Key::Space;
	(*_map)["return"] = sf::Keyboard::Key::Return;
	(*_map)["backspace"] = sf::Keyboard::Key::BackSpace;
	(*_map)["tab"] = sf::Keyboard::Key::Tab;
	(*_map)["pageup"] = sf::Keyboard::Key::PageUp;
	(*_map)["pagedown"] = sf::Keyboard::Key::PageDown;
	(*_map)["end"] = sf::Keyboard::Key::End;
	(*_map)["home"] = sf::Keyboard::Key::Home;
	(*_map)["insert"] = sf::Keyboard::Key::Insert;
	(*_map)["delete"] = sf::Keyboard::Key::Delete;
	(*_map)["add"] = sf::Keyboard::Key::Add;
	(*_map)["subtract"] = sf::Keyboard::Key::Subtract;
	(*_map)["multiply"] = sf::Keyboard::Key::Multiply;
	(*_map)["divide"] = sf::Keyboard::Key::Divide;
	(*_map)["left"] = sf::Keyboard::Key::Left;
	(*_map)["right"] = sf::Keyboard::Key::Right;
	(*_map)["up"] = sf::Keyboard::Key::Up;
	(*_map)["down"] = sf::Keyboard::Key::Down;
	(*_map)["numpad0"] = sf::Keyboard::Key::Numpad0;
	(*_map)["numpad1"] = sf::Keyboard::Key::Numpad1;
	(*_map)["numpad2"] = sf::Keyboard::Key::Numpad2;
	(*_map)["numpad3"] = sf::Keyboard::Key::Numpad3;
	(*_map)["numpad4"] = sf::Keyboard::Key::Numpad4;
	(*_map)["numpad5"] = sf::Keyboard::Key::Numpad5;
	(*_map)["numpad6"] = sf::Keyboard::Key::Numpad6;
	(*_map)["numpad7"] = sf::Keyboard::Key::Numpad7;
	(*_map)["numpad8"] = sf::Keyboard::Key::Numpad8;
	(*_map)["numpad9"] = sf::Keyboard::Key::Numpad9;
	(*_map)["f1"] = sf::Keyboard::Key::F1;
	(*_map)["f2"] = sf::Keyboard::Key::F2;
	(*_map)["f3"] = sf::Keyboard::Key::F3;
	(*_map)["f4"] = sf::Keyboard::Key::F4;
	(*_map)["f5"] = sf::Keyboard::Key::F5;
	(*_map)["f6"] = sf::Keyboard::Key::F6;
	(*_map)["f7"] = sf::Keyboard::Key::F7;
	(*_map)["f8"] = sf::Keyboard::Key::F8;
	(*_map)["f9"] = sf::Keyboard::Key::F9;
	(*_map)["f10"] = sf::Keyboard::Key::F10;
	(*_map)["f11"] = sf::Keyboard::Key::F11;
	(*_map)["f12"] = sf::Keyboard::Key::F12;
	(*_map)["f13"] = sf::Keyboard::Key::F13;
	(*_map)["f14"] = sf::Keyboard::Key::F14;
	(*_map)["f15"] = sf::Keyboard::Key::F15;
	(*_map)["pause"] = sf::Keyboard::Key::Pause;
	return _map;
}

#endif