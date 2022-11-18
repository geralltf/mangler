using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace mangler_config
{
    public class FileView
    {
        public string @baseDirectory;
        public string confFile = null;
        public Dictionary<string, Dictionary<string, string>> database;
        public TreeNode root;

        public TreeNode GetTreeRoot()
        {
            return root;
        }

        public void DbCreateSection(string sectionName)
        {
            database[sectionName] = new Dictionary<string, string>();
        }
    }

    public class ConfigurationView
    {
        public ConfigurationView(IConsole console)
        {
            this.AConsole = console;
        }

        public FileView SelectedFileView = null;
        public List<FileView> Files = new List<FileView>();
        protected const string SYS_DATABASE = "Game.ini";

        public IConsole AConsole;

        /// <summary>
        /// Display properties for a section in the ini within the PropertyGroup control.
        /// </summary>
        /// <param name="sectionName"></param>
        /// <returns></returns>
        public ViewModel GetViewModel(string sectionName, FileView parentView)
        {
            ViewModel viewModel = new ViewModel();
            sectionName = sectionName.ToLower();
            
            if (parentView.database.Any() && parentView.database.ContainsKey(sectionName))
            {
                foreach (var pair in parentView.database[sectionName])
                {
                    try
                    {
                        viewModel[pair.Key] = Parsing.ParseValue(sectionName, pair.Key, pair.Value);
                    }
                    catch
                    {
                        viewModel[pair.Key] = null;
                    }
                }
            }

            return viewModel;
        }

        

        /// <summary>
        /// Load a configuration file settings into the view.
        /// </summary>
        public void LoadConfiguration(ref FileView view, ContextMenuStrip chContextMenu)
        {
            view.root = null;

            if(string.IsNullOrEmpty(view.confFile))
            {
                // Load a Game.ini file somewhere in either the executable path, solution path, or project paths.
                view.confFile = GetConfPath(out view.@baseDirectory);
            }
            
            if (!string.IsNullOrEmpty(view.confFile))
            {
                string newFile = Path.GetFileName(view.confFile);
                string newDir = Path.GetDirectoryName(view.confFile);

                AConsole.WriteLine("LOADING CONFIGURATION {0}..", ConsoleStyle.Underlined, newFile);

                // Parse the configuration file.
                GameConfParser parser = new GameConfParser(view.confFile, AConsole);

                // Build a tree using the parser.
                view.root = BuildConfTree(parser, view, chContextMenu);

                AConsole.WriteLine("CONFIGURATION LOADED{0}", ConsoleStyle.Underlined, "");
                AConsole.WriteLine("-> {0}  in {1}", ConsoleStyle.Regular, newFile, newDir.Replace("\\", "\\\\"));
            }
            else
            {
                AConsole.WriteLine("File not specified. Aborting.", ConsoleStyle.Underlined, "");
            }
        }

        /// <summary>
        /// Overwrite the existing configuration file with new settings.
        /// </summary>
        public void SaveConfiguration(FileView view)
        {
            if (!string.IsNullOrEmpty(view.confFile))
            {
                string newFile = Path.GetFileName(view.confFile);
                string newDir = Path.GetDirectoryName(view.confFile);

                AConsole.WriteLine("SAVING CONFIGURATION {0}..", ConsoleStyle.Underlined, newFile);
                
                GameConfWriter w = new GameConfWriter(view.confFile, AConsole);
                w.WriteDictionary(view.database);

                AConsole.WriteLine("CONFIGURATION SAVED{0}", ConsoleStyle.Underlined, "");
                AConsole.WriteLine("-> {0}  in {1}", ConsoleStyle.Regular, view.confFile, newDir.Replace("\\", "\\\\"));
            }
        }

        public TreeNode BuildConfTree(GameConfParser parser, FileView view, ContextMenuStrip chContextMenu)
        {
            TreeNode root = CreateNode("Settings <" + view.confFile + ">");
            TreeNode n;
            List<IniLine> lines;

            lines = parser.Parse(out view.database);
            lines = null;
            root.Tag = view;

            foreach (var section in view.database)
            {
                if(section.Key != "build")
                {
                    // [Section]
                    n = CreateNode(GameConfWriter.ToTitleCase(section.Key));
                    n.Tag = view;
                    n.ContextMenuStrip = chContextMenu;

                    AppendChild(root, n);
                }
            }

            return root;
        }

        public TreeNode CreateNode(string text)
        {
            TreeNode n = new TreeNode(text);
            return n;
        }

        public void AppendChild(TreeNode parent, TreeNode child)
        {
            parent.Nodes.Add(child);
        }

        public string GetConfPath(out string @baseDirectory)
        {
            // Load a Game.ini file somewhere in either the executable path, solution path, or project paths.
            string path = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            string file0 = System.IO.Path.GetFullPath(path + "/" + SYS_DATABASE); // if a production build the binary is located in the same place as the config file.
            string file1 = System.IO.Path.GetFullPath(path + "/../../../") + SYS_DATABASE; // if debugging may be located in the solution directory.
            string file2 = System.IO.Path.GetFullPath(path + "/../../../athena-source/") + SYS_DATABASE; // if debugging may be located in the project debug/release directory.
            string file = "";

            @baseDirectory = System.IO.Path.GetDirectoryName(file1); // the base or solution directory.

            if (File.Exists(file0))
            {
                file = file0;
            }
            else if (File.Exists(file1))
            {
                file = file1;
            }
            else if (File.Exists(file2))
            {
                file = file2;
            }
            else
            {
                MessageBox.Show(string.Format("Could not find system configuration '{0}' at the base path '{1}'", file0, @baseDirectory));
            }

            if (!string.IsNullOrEmpty(file))
            {
                // Set base directory the same location as the configuration file.
                @baseDirectory = System.IO.Path.GetDirectoryName(file);
            }

            return file;
        }

    }
}
