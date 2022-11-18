using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Dynamic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mangler_config
{
    /// <summary>
    /// Maintains a collection of properties in a dictionary that the PropertyGroup control can access by reflection.
    /// </summary>
    public class ViewModel : DynamicObject, ICustomTypeDescriptor, INotifyPropertyChanged
    {
        /// <summary>
        /// The Properties.
        /// </summary>
        public readonly IDictionary<string, object> dynamicProperties = new Dictionary<string, object>();

        public event PropertyChangedEventHandler PropertyChanged;
        
        public object this[string key]
        {
            get
            {
                return dynamicProperties[key];
            }
            set
            {
                dynamicProperties[key] = value;
            }
        }

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            var memberName = binder.Name;
            return dynamicProperties.TryGetValue(memberName, out result);
        }

        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            var memberName = binder.Name;
            dynamicProperties[memberName] = value;
            return true;
        }

        public PropertyDescriptorCollection GetProperties()
        {
            var attributes = new Attribute[0];
            var properties = dynamicProperties
                .Select(pair => new DynamicPropertyDescriptor(this,
                    pair.Key, pair.Value.GetType(), attributes));
            return new PropertyDescriptorCollection(properties.ToArray());
        }

        //public string GetClassName()
        //{
        //    return GetType().Name;
        //}
        public String GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        private void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged == null)
            {
                return;
            }

            var eventArgs = new PropertyChangedEventArgs(propertyName);
            PropertyChanged(this, eventArgs);
        }

        private void NotifyToRefreshAllProperties()
        {
            OnPropertyChanged(String.Empty);
        }


        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        public String GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }
    }

    public class DynamicPropertyDescriptor : PropertyDescriptor
    {
        private readonly ViewModel businessObject;
        private readonly Type propertyType;

        public DynamicPropertyDescriptor(ViewModel businessObject,string propertyName, Type propertyType, Attribute[] propertyAttributes)
            : base(propertyName, propertyAttributes)
        {
            this.businessObject = businessObject;
            this.propertyType = propertyType;
        }

        public override bool CanResetValue(object component)
        {
            return true;
        }

        public override object GetValue(object component)
        {
            return businessObject.dynamicProperties[Name];
        }

        public override void ResetValue(object component)
        {
        }

        public override void SetValue(object component, object value)
        {
            businessObject.dynamicProperties[Name] = value;
        }

        public override bool ShouldSerializeValue(object component)
        {
            return false;
        }

        public override Type ComponentType
        {
            get { return typeof(ViewModel); }
        }

        public override bool IsReadOnly
        {
            get { return false; }
        }

        public override Type PropertyType
        {
            get
            {
                return propertyType;
            }
        }
    }
}
