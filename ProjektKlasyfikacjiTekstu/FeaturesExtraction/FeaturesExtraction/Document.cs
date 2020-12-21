using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeaturesExtraction
{
    class Document
    {
        public List<string> content;
        public string document_class;
        public string name;
        public Dictionary<string, int> features = new Dictionary<string, int>();

        public Document(string name, string document_class, List<string> content)
        {
            this.name = name;
            this.document_class = document_class;
            this.content = content;
        }

        public void fillDictionary(List<string> possible_features)
        {
            foreach (string feature in possible_features)
            {
                int count = content.Where(x => x.Equals(feature)).Count();
                features.Add(feature, count);
            }
        }

        public void debugDocument()
        {
            Console.WriteLine("name: [" + name + "]");
            Console.WriteLine("class: [" + document_class + "]");
            Console.WriteLine("features:");
            foreach (KeyValuePair<string, int> entry in features)
            {
                Console.WriteLine("[" + entry.Key + "]["+entry.Value+"]");

            }
            Console.ReadLine();
        }
    }
}
