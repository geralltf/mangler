using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;

namespace mangler_config
{
    public enum NewPropertyStatus
    {
        DialogCancel,
        DialogCreate
    }

    public class NewPropertyModel
    {
        public string Name { get; set; }
    }

    public class Parsing
    {
        public static string SaveValue(string sectionName, string key, object value)
        {
            string val = value.ToString();

            bool keyBinding = (sectionName == "key bindings");

            if (keyBinding)
            {
                // Anything under the Key Bindings section maps to a KeyboardInputKey type.
                val = value.ToString();
            }
            else
            {
                char discriminator = key.ElementAt(0).ToString().ToLower().ElementAt(0);
                switch (discriminator)
                {
                    case 'f':
                        val = value.ToString();
                        break;
                    case 'b':
                        val = ((((bool)value) != false || value != null) ? "1" : "0");
                        break;
                    case 'v':
                        if (key.StartsWith("v2f"))
                        {
                            Vector2 v = (Vector2)value;
                            val = string.Format("{0}, {1}", v.X, v.Y);
                        }
                        if (key.StartsWith("v2u"))
                        {
                            Vector2u v = (Vector2u)value;
                            val = string.Format("{0}, {1}", v.X, v.Y);
                        }
                        if (key.StartsWith("v3f"))
                        {
                            Vector3 v = (Vector3)value;
                            val = string.Format("{0}, {1}, {2}", v.X, v.Y, v.Z);
                        }
                        if (key.StartsWith("v4f"))
                        {
                            Vector4 v = (Vector4)value;
                            val = string.Format("{0}, {1}, {2}, {3}", v.X, v.Y, v.Z, v.W);
                        }
                        break;
                    case 'u':
                        val = value.ToString();
                        break;
                    default:
                        val = value.ToString();
                        break;
                }
            }

            return val;
        }
        public static object ParseDiscriminator(string sectionName, string key, string value)
        {
            object val = value;
            char discriminator = key.ElementAt(0).ToString().ToLower().ElementAt(0);
            switch (discriminator)
            {
                case 'f':
                    float f;
                    if (float.TryParse(value, out f))
                    {
                        val = f;
                    }
                    else
                    {
                        val = 0.0f;
                    }
                    break;
                case 'b':
                    val = (value != null || value != "0" || value.ToLower() != "false");
                    break;
                case 'v':
                    if (key.StartsWith("v2f"))
                    {
                        var lst = parseFloatList(value);
                        val = new Vector2(lst);
                    }
                    if (key.StartsWith("v2u"))
                    {
                        var lst = parseUintList(value);
                        val = new Vector2u(lst);
                    }
                    if (key.StartsWith("v3f"))
                    {
                        var lst = parseFloatList(value);
                        val = new Vector3(lst);
                    }
                    if (key.StartsWith("v4f"))
                    {
                        var lst = parseFloatList(value);
                        val = new Vector4(lst);
                    }
                    break;
                case 'u':
                    uint u;
                    if (uint.TryParse(value, out u))
                    {
                        val = u;
                    }
                    else
                    {
                        val = (uint)0;
                    }
                    break;
                default:
                    val = value;
                    break;
            }
            return val;
        }

        public static object ParseValue(string sectionName, string key, string value)
        {
            object val = value;
            bool keyBinding = (sectionName == "key bindings");

            if (keyBinding)
            {
                // Anything under the Key Bindings section maps to a KeyboardInputKey type.
                try
                {
                    KeyboardInputKey binding;
                    Enum.TryParse<KeyboardInputKey>(value, out binding);
                    val = binding;
                }
                catch
                {
                    val = KeyboardInputKey.Unknown;
                }
            }
            else
            {
                val = ParseDiscriminator(sectionName, key, value);
            }
            return val;
        }

        public static List<float> parseFloatList(string input)
        {
            List<float> lst = new List<float>();
            if(!string.IsNullOrEmpty(input))
            {
                string[] sep = input.Split(',');
                float value;
                foreach (string s in sep)
                {
                    if (float.TryParse(s.Trim(), out value))
                    {
                        lst.Add(value);
                    }
                    else
                    {
                        lst.Add(0);
                    }
                }
            }
            return lst;
        }
        public static List<uint> parseUintList(string input)
        {
            List<uint> lst = new List<uint>();
            if (!string.IsNullOrEmpty(input))
            {
                string[] sep = input.Split(',');
                uint value;
                foreach (string s in sep)
                {
                    if (uint.TryParse(s.Trim(), out value))
                    {
                        lst.Add(value);
                    }
                    else
                    {
                        lst.Add(0);
                    }
                }
            }
            return lst;
        }
    }

    [TypeConverter(typeof(ExpandableObjectConverter))]
    [Serializable()]
    public class Vector2
    {
        [Description("The X Component of Vector2 (Floating point value).")]
        public float X { get; set; }

        [Description("The Y Component of Vector2 (Floating point value).")]
        public float Y { get; set; }

        public Vector2() { }
        public Vector2(List<float> values)
        {
            if (values.Count < 2) return;
            X = values[0];
            Y = values[1];
        }

        public override string ToString()
        {
            return string.Format("Vector2 X:{0} Y:{1}", X, Y);
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [Serializable()]
    public class Vector2u
    {
        [Description("The X Component of Vector2 (Unsigned integer value).")]
        public uint X { get; set; }

        [Description("The Y Component of Vector2 (Unsigned integer value).")]
        public uint Y { get; set; }

        public Vector2u() { }
        public Vector2u(List<uint> values)
        {
            if (values.Count < 2) return;
            X = values[0];
            Y = values[1];
        }

        public override string ToString()
        {
            return string.Format("Vector2u X:{0} Y:{1}", X, Y);
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [Serializable()]
    public class Vector3
    {
        [Description("The X Component of Vector3 (Floating point value).")]
        public float X { get; set; }

        [Description("The Y Component of Vector3 (Floating point value).")]
        public float Y { get; set; }

        [Description("The Z Component of Vector3 (Floating point value).")]
        public float Z { get; set; }

        public Vector3() { }
        public Vector3(List<float> values)
        {
            if (values.Count < 3) return;
            X = values[0];
            Y = values[1];
            Z = values[2];
        }

        public override string ToString()
        {
            return string.Format("Vector3 X:{0} Y:{1} Z:{2}", X, Y, Z);
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [Serializable()]
    public class Vector4
    {
        [Description("The X Component of Vector4 (Floating point value).")]
        public float X { get; set; }

        [Description("The Y Component of Vector4 (Floating point value).")]
        public float Y { get; set; }

        [Description("The Z Component of Vector4 (Floating point value).")]
        public float Z { get; set; }

        [Description("The W Component of Vector4 (Floating point value).")]
        public float W { get; set; }

        public Vector4() { }
        public Vector4(List<float> values)
        {
            if (values.Count < 4) return;
            X = values[0];
            Y = values[1];
            Z = values[2];
            W = values[3];
        }

        public override string ToString()
        {
            return string.Format("Vector4 X:{0} Y:{1} Z:{2} W:{3}", X, Y, Z, W);
        }
    }

    public enum KeyboardInputKey
    {
        Unknown = -1, ///< Unhandled key
        A = 0,        ///< The A key
        B,            ///< The B key
        C,            ///< The C key
        D,            ///< The D key
        E,            ///< The E key
        F,            ///< The F key
        G,            ///< The G key
        H,            ///< The H key
        I,            ///< The I key
        J,            ///< The J key
        K,            ///< The K key
        L,            ///< The L key
        M,            ///< The M key
        N,            ///< The N key
        O,            ///< The O key
        P,            ///< The P key
        Q,            ///< The Q key
        R,            ///< The R key
        S,            ///< The S key
        T,            ///< The T key
        U,            ///< The U key
        V,            ///< The V key
        W,            ///< The W key
        X,            ///< The X key
        Y,            ///< The Y key
        Z,            ///< The Z key
        Num0,         ///< The 0 key
        Num1,         ///< The 1 key
        Num2,         ///< The 2 key
        Num3,         ///< The 3 key
        Num4,         ///< The 4 key
        Num5,         ///< The 5 key
        Num6,         ///< The 6 key
        Num7,         ///< The 7 key
        Num8,         ///< The 8 key
        Num9,         ///< The 9 key
        Escape,       ///< The Escape key
        LControl,     ///< The left Control key
        LShift,       ///< The left Shift key
        LAlt,         ///< The left Alt key
        LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl,     ///< The right Control key
        RShift,       ///< The right Shift key
        RAlt,         ///< The right Alt key
        RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu,         ///< The Menu key
        LBracket,     ///< The [ key
        RBracket,     ///< The ] key
        SemiColon,    ///< The ; key
        Comma,        ///< The , key
        Period,       ///< The . key
        Quote,        ///< The ' key
        Slash,        ///< The / key
        BackSlash,    ///< The \ key
        Tilde,        ///< The ~ key
        Equal,        ///< The = key
        Dash,         ///< The - key
        Space,        ///< The Space key
        Return,       ///< The Return key
        BackSpace,    ///< The Backspace key
        Tab,          ///< The Tabulation key
        PageUp,       ///< The Page up key
        PageDown,     ///< The Page down key
        End,          ///< The End key
        Home,         ///< The Home key
        Insert,       ///< The Insert key
        Delete,       ///< The Delete key
        Add,          ///< The + key
        Subtract,     ///< The - key
        Multiply,     ///< The * key
        Divide,       ///< The / key
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,      ///< The numpad 0 key
        Numpad1,      ///< The numpad 1 key
        Numpad2,      ///< The numpad 2 key
        Numpad3,      ///< The numpad 3 key
        Numpad4,      ///< The numpad 4 key
        Numpad5,      ///< The numpad 5 key
        Numpad6,      ///< The numpad 6 key
        Numpad7,      ///< The numpad 7 key
        Numpad8,      ///< The numpad 8 key
        Numpad9,      ///< The numpad 9 key
        F1,           ///< The F1 key
        F2,           ///< The F2 key
        F3,           ///< The F3 key
        F4,           ///< The F4 key
        F5,           ///< The F5 key
        F6,           ///< The F6 key
        F7,           ///< The F7 key
        F8,           ///< The F8 key
        F9,           ///< The F9 key
        F10,          ///< The F10 key
        F11,          ///< The F11 key
        F12,          ///< The F12 key
        F13,          ///< The F13 key
        F14,          ///< The F14 key
        F15,          ///< The F15 key
        Pause         ///< The Pause key
    }
}
