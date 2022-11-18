using System;
using System.Globalization;
using System.Reflection;
using System.Windows.Forms;

namespace mangler_config
{
    public interface IConsole
    {
        void WriteLine(string format, ConsoleStyle style, params object[] objs);
    }
    public enum ConsoleStyle
    {
        Bold,
        Italics,
        Underlined,
        Regular
    }

    public partial class MainWindow : Form, IConsole
    {
        ConfigurationView view;
        string txtstdout = "";
        string selectedSection = "";
        int dirtyItems = 0;

        public MainWindow()
        {
            InitializeComponent();

            view = new ConfigurationView(this);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            view.SelectedFileView = new FileView();
            LoadFile(ref view.SelectedFileView);

            tvSections.NodeMouseClick += TvSections_NodeMouseClick;
            tvSections.AfterSelect += TvSections_AfterSelect;
            properties.PropertyValueChanged += Properties_PropertyValueChanged;
        }

        private void Properties_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            var map = view.SelectedFileView.database[selectedSection.ToLower()];
            string propertyKey = e.ChangedItem.Label;

            Type gridEntryType = e.ChangedItem.GetType().Assembly.GetType("System.Windows.Forms.PropertyGridInternal.PropertyDescriptorGridEntry").BaseType;
            var piPropertyDepth = gridEntryType.GetProperty("PropertyDepth");

            // Get the depth of the property in the PropertyGrid
            Int32 propertyDepth = (Int32)piPropertyDepth.GetValue(e.ChangedItem);
            
            if (propertyDepth == 0) // this is a root level property, so nothing needs changing.
            {
                map[propertyKey] = Parsing.SaveValue(selectedSection, propertyKey, e.ChangedItem.Value);
            }
            else if (propertyDepth == 1) // this is not a root level property, so the root level property in the map needs syncing.
            {
                var piInstance = gridEntryType.GetProperty("Instance");
                object propertyInst = piInstance.GetValue(e.ChangedItem);

                propertyKey = e.ChangedItem.Parent.Label;

                map[propertyKey] = Parsing.SaveValue(selectedSection, propertyKey, propertyInst);
            }
            else
            {
                throw new Exception("unhandled property depth.");
            }

            dirtyItems++;
        }

        public void UpdatePropertiesView()
        {
            if (tvSections.SelectedNode == null)
            {
                properties.SelectedObject = null;
            }
            else
            {
                selectedSection = tvSections.SelectedNode.Text;

                properties.SelectedObject = view.GetViewModel(selectedSection, view.SelectedFileView);
            }
        }

        public void ApplyChanges()
        {
            this.WriteLine("################################################", ConsoleStyle.Bold, "");

            if(dirtyItems > 0)
            {
                SaveFileDialog diag = new SaveFileDialog();
                diag.FileName = System.IO.Path.GetFileName(view.SelectedFileView.confFile) + "-new.ini";
                diag.InitialDirectory = view.SelectedFileView.@baseDirectory;
                diag.CheckPathExists = true;
                diag.Filter = "Athena Game Configuration File|*.ini";

                if (diag.ShowDialog() == DialogResult.OK)
                {
                    view.SelectedFileView.confFile = diag.FileName;

                    if(System.IO.File.Exists(diag.FileName))
                    {
                        this.WriteLine("File alredy exists at this location. Set to Overwrite.", ConsoleStyle.Underlined, "");
                    }

                    view.SaveConfiguration(view.SelectedFileView);
                }

                this.WriteLine("Saved {0} changes.", ConsoleStyle.Regular, dirtyItems);

                dirtyItems = 0;
            }
            else
            {
                this.WriteLine("No changes detected.", ConsoleStyle.Regular, dirtyItems);
            }
        }

        private void TvSections_AfterSelect(object sender, TreeViewEventArgs e)
        {
            UpdatePropertiesView();
        }

        private void TvSections_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            view.SelectedFileView = (FileView)e.Node.Tag;

            UpdatePropertiesView();
        }

        private void applyChangesToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            ApplyChanges();
        }

        private void applyChangesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var node = getSourceTreeNode(sender);

            if(node != null)
            {
                var fileView = (FileView)node.Tag;
                view.SelectedFileView = fileView;
            }
            
            ApplyChanges();
        }

        void LoadFile(ref FileView fileView)
        {
            this.WriteLine("################################################", ConsoleStyle.Bold, "");

            // Load Configuration
            view.LoadConfiguration(ref fileView, contextMenuStrip2);

            // Display configuration tree.
            var root = fileView.GetTreeRoot();

            root.ContextMenuStrip = contextMenuStrip1;
            tvSections.Nodes.Add(root);
            tvSections.CollapseAll();
            root.Expand();

            view.SelectedFileView = fileView;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // TODO: check for any changes before quitting

            Application.Exit();
        }

        private void loadConfToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog diag = new OpenFileDialog();
            diag.InitialDirectory = view.SelectedFileView.@baseDirectory;
            diag.CheckFileExists = true;
            diag.Filter = "Athena Game Configuration File|*.ini";

            if (diag.ShowDialog() == DialogResult.OK)
            {
                var fileView = new FileView();

                fileView.confFile = diag.FileName;

                LoadFile(ref fileView);
            }
        }

        TreeNode getSourceTreeNode(object sender)
        {
            // Try to cast the sender to a ToolStripItem
            //ToolStripItem menuItem = sender as ToolStripItem;
            //if (menuItem != null)
            //{
            //    // Retrieve the ContextMenuStrip that owns this ToolStripItem
            //    ContextMenuStrip owner = menuItem.Owner as ContextMenuStrip;
            //    if (owner != null)
            //    {
            //        // Get the control that is displaying this context menu
            //        Control sourceControl = owner.SourceControl;

            //        // Tey to cast to tree view
            //        TreeView treeView = (TreeView)sourceControl;

            //        return treeView.SelectedNode;
            //    }
            //}
            return tvSections.SelectedNode;
            //return null;
        }

        void closeView(object sender)
        {
            var node = getSourceTreeNode(sender);

            if (node != null && node.Parent == null)
            {
                node.Remove();
            }
        }

        private void closeViewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            closeView(sender);
        }

        private void closeViewToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            closeView(sender);
        }

        private void newSectionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewPropertyDialog diag = new NewPropertyDialog();
            
            TreeNode node = tvSections.SelectedNode;

            diag.PropertyChanged += (object _sender, System.ComponentModel.PropertyChangedEventArgs _e) =>
            {
                switch(_e.PropertyName)
                {
                    case "Status":

                        if(diag.Status == NewPropertyStatus.DialogCreate)
                        {
                            string sectionName = diag.Model.Name.ToLower();
                            string title = GameConfWriter.ToTitleCase(sectionName);
                            // Create the new section in the tree.
                            var newNode = view.CreateNode(title);
                            newNode.Tag = view.SelectedFileView;
                            newNode.Text = title;
                            newNode.ContextMenuStrip = contextMenuStrip2;

                            if (node != null)
                            {
                                node.Nodes.Add(newNode);
                            }

                            // Create the section in the dictionary
                            view.SelectedFileView.DbCreateSection(sectionName);

                            dirtyItems++;
                        }

                        diag.Dispose();
                        diag = null;
                        break;

                }
            
            };
            diag.Show();
        }

        private void newPropertyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = tvSections.SelectedNode;

            if(node.Level > 0)
            {
                FileView fileView = (FileView)node.Tag;
                string sectionName = node.Text.ToLower();

                NewPropertyDialog diag = new NewPropertyDialog();

                diag.PropertyChanged += (object _sender, System.ComponentModel.PropertyChangedEventArgs _e) =>
                {
                    switch (_e.PropertyName)
                    {
                        case "Status":

                            if (diag.Status == NewPropertyStatus.DialogCreate)
                            {
                                string propName = diag.Model.Name.ToLower().Replace(" ","");

                                // Create the property in the dictionary
                                fileView.database[sectionName][propName] = "";

                                UpdatePropertiesView();

                                dirtyItems++;
                            }

                            diag.Dispose();
                            diag = null;
                            break;

                    }

                };
                diag.Show();
            }
        }

        public void WriteLine(string format, ConsoleStyle style, params object[] objs)
        {
            string sModS = "";
            string sModE = "";
            switch (style)
            {
                case ConsoleStyle.Regular:
                    sModE = sModS = "";
                    break;
                case ConsoleStyle.Bold:
                    sModS = @" {\b ";
                    sModE = @"} ";
                    break;
                case ConsoleStyle.Italics:
                    sModS = @" {\i ";
                    sModE = @"} ";
                    break;
                case ConsoleStyle.Underlined:
                    sModS = @" {\ul ";
                    sModE = @"} ";
                    break;
            }

            string msg = string.Format(CultureInfo.CurrentCulture, format, objs);

            if (!msg.StartsWith(" "))
            {
                msg = " " + msg;
            }

            txtstdout += sModS + msg + sModE + @"\par";

            txtConsole.Rtf = @"{\rtf1\ansi " + txtstdout + "}";

            txtConsole.SelectionStart = txtConsole.Text.Length;
            txtConsole.ScrollToCaret();

        }
    }
}
