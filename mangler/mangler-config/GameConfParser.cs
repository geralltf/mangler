// © Gerallt 2017.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;

namespace mangler_config
{
    public enum IniToken
    {
        LINE_UNPROCESSED,
        LINE_EMPTY,
        LINE_COMMENT,
        LINE_SECTION,
        LINE_VALUE,
        LINE_ERROR
    }

    public class IniLine
    {
	    public string section;
        public string key;
        public string value;
    }

    public class GameConfParser
    {
        public GameConfParser(string file, IConsole console)
        {
            this.SystemDatabase = file;
            this.Console = console;
        }

        public IConsole Console { get; private set; }
        public string SystemDatabase { get; set; }
        private int lineno = 0;
        private int errors = 0;
        private bool syntaxError = false;
        private string lastMessage;

        const string LINE_ERROR_INVALID_GRAMMAR = "Foreign grammar in line. INI Format invalid. Correct the line by changing to a valid section or configuration variable.";
        const string LINE_ERROR_INVALID_KVP = "Could not parse key=value";
        const string SYNTAX_ERROR_FORMAT_STRING = "Syntax error in {0} ({1}){3}:\n-> {2}";
        public const char GRAMMAR_COMMENT_HASH = '#';
        public const char GRAMMAR_COMMENT_SEMICOLON = ';';
        public const char GRAMMAR_KVP_ASSIGNMENT = '=';
        public const char GRAMMAR_QUOTATION_BEGIN = '"';
        public const char GRAMMAR_QUOTATION_END = '"';
        public const char GRAMMAR_PARENTHESES_BEGIN = '(';
        public const char GRAMMAR_PARENTHESES_END = ')';
        public const char GRAMMAR_SECTION_BEGIN = '[';
        public const char GRAMMAR_SECTION_END = ']';
        public const string GRAMMAR_COMMENT_INDENTATION = "  ";

        public List<IniLine> Parse(out Dictionary<string, Dictionary<string, string>> dictionarySections)
        {
            List<IniLine> iniLines = new List<IniLine>();
            string[] lines = File.ReadAllLines(SystemDatabase);
            IniLine parsedLine;
            string currSection = string.Empty;
            string additionalMessage;

            dictionarySections = new Dictionary<string, Dictionary<string, string>>();

            foreach (string line in lines)
            {
                lineno++;
                parsedLine = new IniLine();
                switch (ParseLine(line, ref parsedLine))
                {
                    case IniToken.LINE_EMPTY:
                    case IniToken.LINE_COMMENT:
                        break;
                    case IniToken.LINE_SECTION:
                        currSection = parsedLine.section;

                        Console.WriteLine("Loading {0} settings..", ConsoleStyle.Bold, parsedLine.section);

                        dictionarySections[currSection] = new Dictionary<string, string>();

                        break;
                    case IniToken.LINE_VALUE:

                        if (currSection == "")
                            currSection = "General";

                        dictionarySections[currSection][parsedLine.key] = parsedLine.value;

                        Console.WriteLine("\\tab {0} {1}", ConsoleStyle.Regular, parsedLine.key, parsedLine.value);

                        break;

                    case IniToken.LINE_ERROR:
                        errors++;

                        additionalMessage = (lastMessage.Length > 0) ? (" " + lastMessage) : ("");

                        Console.WriteLine(SYNTAX_ERROR_FORMAT_STRING, ConsoleStyle.Regular, SystemDatabase, lineno, line, additionalMessage);
                        
                        break;

                    default:
                        break;
                }
                iniLines.Add(parsedLine);
            }
            return iniLines;
        }

        public IniToken ParseLine(string line, ref IniLine iniLine)
        {
            int len;
            IniToken token;
            string key = string.Empty;
            string value = string.Empty;

            lastMessage = string.Empty;
            iniLine.section = string.Empty;
            iniLine.key = string.Empty;
            iniLine.value = string.Empty;
            len = line.Length;
            token = IniToken.LINE_UNPROCESSED;

            // PARSE INI
            // Parse tokens within the current line.
            if (len < 1)
            {
                token = IniToken.LINE_EMPTY;
            }
            else if (line[0] == GRAMMAR_COMMENT_HASH || line[0] == GRAMMAR_COMMENT_SEMICOLON)
            {
                token = IniToken.LINE_COMMENT;
            }
            else if (line[0] == GRAMMAR_SECTION_BEGIN && line[len - 1] == GRAMMAR_SECTION_END)
            {
                if (GetSection(line, ref iniLine.section))
                {
                    token = IniToken.LINE_SECTION;
                }
                else
                {
                    // Generate syntax error
                    token = IniToken.LINE_ERROR;

                    lastMessage = LINE_ERROR_INVALID_GRAMMAR;
                }
            }
            else if (GetPair(line, ref key, ref value))
            {
                token = IniToken.LINE_VALUE;

                iniLine.key = key;
                iniLine.value = value;
            }
            else
            {
                // Generate syntax error
                token = IniToken.LINE_ERROR;
                lastMessage = LINE_ERROR_INVALID_GRAMMAR;
            }

            if(syntaxError)
            {
                // Generate syntax error
                token = IniToken.LINE_ERROR;

                // Clear the error as we have handled it.
                syntaxError = false;
            }

            return token;
        }

        public bool GetSection(string line, ref string sectionName)
        {
            if (IsComment(line))
            {
                return false;
            }
            MatchCollection matches;
            Regex expr = new Regex("\\[([^\\[\\]]+)\\]");

            matches = expr.Matches(line.Trim());

            if (matches.Count == 1)
            {
                sectionName = matches[0].Value;

                if (sectionName.StartsWith(GRAMMAR_SECTION_BEGIN.ToString()))
                {
                    sectionName = sectionName.Remove(0, 1);
                }
                if (sectionName.EndsWith(GRAMMAR_SECTION_END.ToString()))
                {
                    sectionName = sectionName.Remove(sectionName.Length-1, 1);
                }

                sectionName = sectionName.ToLower();

                return true;
            }
            else
            {
                return false;
            }
        }

        public bool IsComment(string line)
        {
            if (!string.IsNullOrEmpty(line))
            {
                string clone = line.Trim();
                
                if (clone[0] == GRAMMAR_COMMENT_SEMICOLON || clone[0] == GRAMMAR_COMMENT_HASH)
                {
                    return true;
                }
            }
            return false;
        }

        public bool GetPair(string line, ref string key, ref string value)
        {
            if (IsComment(line))
            {
                key = string.Empty;
                value = string.Empty;
                return false;
            }

            var pair = line.Split(GRAMMAR_KVP_ASSIGNMENT);

            if(pair != null && pair.Length == 2)
            {
                key = pair[0].ToLower();
                key = key.Trim();
                key = removeQuotes(key);

                value = pair[1];
                value = value.Trim();
                value = removeQuotes(value);

                if (GameConfWriter.RequiresParentheses(key))
                {
                    value = removeParentheses(value);
                }

                return true;
            }
            else
            {
                syntaxError = true;
                lastMessage = LINE_ERROR_INVALID_KVP;
            }

            key = string.Empty;
            value = string.Empty;
            return false;
        }

        private string removeQuotes(string input)
        {
            if (string.IsNullOrEmpty(input)) return input;

            if (input[0] == GRAMMAR_QUOTATION_BEGIN)
            {
                input = input.Substring(1, input.Length - 1);

                int last = input.Length - 1;

                if (input[last] == GRAMMAR_QUOTATION_END)
                {
                    input = input.Substring(0, last);
                }
            }

            return input;
        }

        private string removeParentheses(string input)
        {
            if (string.IsNullOrEmpty(input)) return input;

            if (input[0] == GRAMMAR_PARENTHESES_BEGIN)
            {
                input = input.Substring(1, input.Length - 1);

                int last = input.Length - 1;

                if (input[last] == GRAMMAR_PARENTHESES_END)
                {
                    input = input.Substring(0, last);
                }
            }

            return input;
        }
    }
}
