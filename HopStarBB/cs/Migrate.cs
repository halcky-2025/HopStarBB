using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cyclon
{
    class Migrate
    {
        public List<OneMigrate> migrations = new List<OneMigrate>();
        public void Start()
        {
            migrations = new List<OneMigrate>();
            migrations.Add(new OneMigrate());
        }
        public void AddModel(String name, ModelObj model, Local local)
        {
            if (migrations.Count == 1) migrations.Last().AddModel(name, model, null, local);
            else migrations.Last().AddModel(name, model, migrations[migrations.Count - 2], local);
        }
    }
    class OneMigrate
    {
        public String fulltext = "";
        public Dictionary<String, ModelObj> models = new Dictionary<string, ModelObj>();
        public Dictionary<String, ModelObj> tables = new Dictionary<string, ModelObj>();
        public void AddModel(String name, ModelObj model, OneMigrate before, Local local)
        {
            models.Add(name, model);
            fulltext += "@" + model.letter.name + ">" + model.rename + "^" + model.version + "\n";
            var blk1 = model.draw.children[0] as Block;
            foreach (var v in blk1.vmapA.Values)
            {
                if (v.type == ObjType.Variable)
                {
                    var vari = v as Variable;
                    if (vari.cls == local.Int) fulltext += " @int " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Short) fulltext = " @short " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Float) fulltext = " @float " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Bool) fulltext = " @bool " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Str) fulltext = " @string " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls.type == ObjType.ArrayType)
                    {
                    }
                    else if (vari.cls.type == ObjType.ModelObj)
                    {
                        fulltext += " @" + vari.cls.letter.name + " " + vari.letter.name + ">" + vari.rename + "\n";
                    }
                }
            }
            var blk2 = model.draw.children[1] as Block;
            foreach (var kv in blk2.vmapA)
            {
                if (kv.Key == "id") continue;
                var v = kv.Value;
                if (v.type == ObjType.Variable)
                {
                    var vari = v as Variable;
                    if (vari.cls == local.Int) fulltext += " @int " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Short) fulltext = " @short " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Float) fulltext = " @float " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Bool) fulltext = " @bool " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls == local.Str) fulltext = " @string " + vari.letter.name + ">" + vari.rename + "\n";
                    else if (vari.cls.type == ObjType.ArrayType)
                    {
                    }
                    else if (vari.cls.type == ObjType.ModelObj)
                    {
                        fulltext += " @" + vari.cls.letter.name + " " + vari.letter.name + ">" + vari.rename + "\n";
                    }
                }
            }
        }
    }

}
