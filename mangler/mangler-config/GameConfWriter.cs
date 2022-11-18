using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;

namespace mangler_config
{
    public class GameConfWriter
    {
        public GameConfWriter(string file, IConsole console)
        {
            this.SystemDatabase = file;
            this.Console = console;
        }
        public string SystemDatabase { get; set; }
        public IConsole Console { get; private set; }

        private FileStream fs;
        private StringBuilder sb;

        public void Open()
        {
            if (fs != null)
            {
                return;
            }
            fs = File.OpenWrite(SystemDatabase);
            sb = new StringBuilder();
        }

        public void Close()
        {
            Open();
            string s = sb.ToString();
            byte[] bytes = UTF8Encoding.UTF8.GetBytes(s);
            fs.Write(bytes, 0, bytes.Length);
            if(fs!= null)
            {
                fs.Close();
            }
        }

        public void WriteDictionary(Dictionary<string, Dictionary<string, string>> database)
        {
            Open();
            Delimit();
            Comment("Athena Engine");
            Comment("System Configuration");

            if (!database.ContainsKey("build"))
            {
                Section("BUILD");
                Value("SVERSION", "1.17.0920.5;", IsQuoted: false);
                Value("SNAME", "Athena Engine", IsQuoted: false);
                Value("SEditorTitle", "Athena Editor");
                Value("SGameTitle", "Athena Runtime Game Mode");
                Value("SAuthor", "(c) Deathproof Interactive 2017");
                Nop();
            }

            float keys = database.Keys.Count;
            float progress;
            int section_i = 0;
            foreach (var section in database)
            {
                section_i++;
                progress = (section_i / keys) * 100;

                //AConsole.WriteLine("{0:N0}%", ConsoleStyle.Bold, progress);

                string name = section.Key.ToLower();

                if (name == "build")
                {
                    name = name.ToUpper();
                }
                else
                {
                    name = ToTitleCase(name);
                }

                Section(name);
                foreach (var setting in section.Value)
                {
                    string key = setting.Key;
                    if (name == "build" || name == "asset list" || name == "key bindings")
                    {
                        key = key.ToUpper();
                    }

                    Value(key, setting.Value, IsQuoteable(setting.Key), RequiresParentheses(setting.Key));
                }
                Nop();
            }

            Delimit();
            Close();
        }

        public static string ToTitleCase(string input)
        {
            return System.Threading.Thread.CurrentThread.CurrentCulture.TextInfo.ToTitleCase(input);
        }

        public static bool IsQuoteable(string name)
        {
            // if the following conditions require quotable values.
            return //name.StartsWith("s") 
                name.StartsWith("v4f") || name.StartsWith("v3f") || name.StartsWith("v2f") || name.StartsWith("v2u")
                || name.StartsWith("lst") || name.StartsWith("tex_")
                || name.EndsWith("warning") || name.EndsWith("error");
        }

        public static bool RequiresParentheses(string name)
        {
            // if the following conditions require quotable values.
            return //name.StartsWith("s") 
                name.StartsWith("v4f") || name.StartsWith("v3f") || name.StartsWith("v2f") 
                || name.StartsWith("v2u") || name.StartsWith("lst");
        }

        public void Delimit()
        {
            sb.Append(GameConfParser.GRAMMAR_COMMENT_SEMICOLON);
            sb.Append(GameConfParser.GRAMMAR_COMMENT_SEMICOLON);
            sb.Append("#########################################################");
            sb.AppendLine();
        }

        public void Comment(string comment)
        {
            Append(IniToken.LINE_COMMENT, IsQuoted: false, ReqParentheses: false, parameter: comment);
        }

        public void Section(string comment)
        {
            Append(IniToken.LINE_SECTION, IsQuoted: false, ReqParentheses: false, parameter: comment);
        }

        public void Value(string Name, string Value, bool IsQuoted = false, bool Parentheses = false)
        {
            Append(IniToken.LINE_VALUE, IsQuoted, Parentheses, Name, Value);
        }

        public void Nop()
        {
            sb.AppendLine();
        }

        public void Append(IniToken Type, bool IsQuoted, bool ReqParentheses, params string[] parameter)
        {
            switch(Type)
            {
                case IniToken.LINE_COMMENT:
                    sb.Append(GameConfParser.GRAMMAR_COMMENT_SEMICOLON);
                    sb.Append(GameConfParser.GRAMMAR_COMMENT_SEMICOLON);
                    sb.Append(GameConfParser.GRAMMAR_COMMENT_INDENTATION);
                    foreach(string par in parameter)
                    {
                        sb.Append(par);
                    }
                    sb.AppendLine();
                    break;
                case IniToken.LINE_SECTION:
                    sb.Append(GameConfParser.GRAMMAR_SECTION_BEGIN);
                    foreach (string par in parameter)
                    {
                        sb.Append(par);
                    }
                    sb.Append(GameConfParser.GRAMMAR_SECTION_END);
                    sb.AppendLine();
                    break;
                case IniToken.LINE_VALUE:
                    sb.Append(parameter[0]); // Key/Name
                    sb.Append(GameConfParser.GRAMMAR_KVP_ASSIGNMENT);
                    if (ReqParentheses)
                    {
                        sb.Append(GameConfParser.GRAMMAR_PARENTHESES_BEGIN);
                    }
                    else if (IsQuoted)
                    {
                        sb.Append(GameConfParser.GRAMMAR_QUOTATION_BEGIN);
                    }

                    sb.Append(parameter[1]); // Value
                    if (ReqParentheses)
                    {
                        sb.Append(GameConfParser.GRAMMAR_PARENTHESES_END);
                    }
                    else if (IsQuoted)
                    {
                        sb.Append(GameConfParser.GRAMMAR_QUOTATION_END);
                    }
                    sb.AppendLine();
                    break;

            }
        }
    }
}
