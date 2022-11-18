using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mangler_config
{
    public partial class NewPropertyDialog : Form, INotifyPropertyChanged
    {
        public NewPropertyDialog()
        {
            InitializeComponent();
        }

        public NewPropertyModel Model { get; set; }
        public NewPropertyStatus Status { get; set; }
        public event PropertyChangedEventHandler PropertyChanged;

        public bool ValidateForm()
        {
            string name = txtName.Text;

            if (string.IsNullOrEmpty(name))
            {
                MessageBox.Show("Name can not be empty.");

                return false;
            }

            return true;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();

            Status = NewPropertyStatus.DialogCancel;

            PropertyChanged(this, new PropertyChangedEventArgs("Status"));
        }

        private void btnCreate_Click(object sender, EventArgs e)
        {
            if (ValidateForm())
            {
                Model = new NewPropertyModel()
                {
                    Name = txtName.Text
                };

                Status = NewPropertyStatus.DialogCreate;

                PropertyChanged(this, new PropertyChangedEventArgs("Model"));
                PropertyChanged(this, new PropertyChangedEventArgs("Status"));

                this.Hide();
            }
        }
    }
}
