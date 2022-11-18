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

#ifndef ConfParser_H
#define ConfParser_H

#include <algorithm>
#include <regex>
#include <assert.h>
#include "Common.h"

enum IniToken
{
	LINE_UNPROCESSED,
	LINE_EMPTY,
	LINE_COMMENT,
	LINE_SECTION,
	LINE_VALUE,
	LINE_ERROR
};

class IniLine
{
public:
	std::string section;
	std::string key;
	std::string value;
};

class ConfParser
{
public:

	static void ConfParser_tests();

	static bool IsComment(std::string line);
	static bool GetSection(std::string line, std::string & sectionName);
	static bool GetPair(std::string line, std::string & key, std::string & value);

public:

	static IniLine* ConfParser::ParseLine(std::string line, IniToken& token)
	{
		IniLine* parsedLine;
		token = ParseLine(line, parsedLine);
		return parsedLine;
	}

	static void ConfParser::ParseLine(std::string line, IniToken& token, IniLine* iniLine)
	{
		token = ParseLine(line, iniLine);
	}

	static IniToken ConfParser::ParseLine(std::string line, IniLine* iniLine)
	{
		size_t			len;
		IniToken		token;
		std::locale		loc;
		std::string		key;
		std::string		value;

		iniLine->section = "";
		iniLine->key = "";
		iniLine->value = "";
		len = line.length();
		token = IniToken::LINE_UNPROCESSED;

		// PARSE INI
		// Parse tokens within the current line.
		if (len < 1)
		{
			token = IniToken::LINE_EMPTY;
		}
		else if (line[0] == '#' || line[0] == ';')
		{
			token = IniToken::LINE_COMMENT;
		}
		else if (line[0] == '[' && line[len - 1] == ']')
		{
			if (GetSection(line, iniLine->section))
			{
				token = IniToken::LINE_SECTION;
			}
			else
			{
				// Generate syntax error
				token = IniToken::LINE_ERROR;
			}
		}
		else if (GetPair(line, key, value))
		{
			token = IniToken::LINE_VALUE;

			iniLine->key = key;
			iniLine->value = value;
		}
		else
		{
			// Generate syntax error
			token = IniToken::LINE_ERROR;
		}

		return token;
	}
};

#endif