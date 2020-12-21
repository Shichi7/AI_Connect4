using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeaturesExtraction
{
    class TextClass
    {
        public int doc_count;
        public string class_name;
        public Dictionary<string, ClassFeature> feature_mis;
        
        public TextClass(string class_name)
        {
            feature_mis = new Dictionary<string, ClassFeature>();
            this.class_name = class_name;
            doc_count = 0;
        }

        public void calculateMIs()
        {
            foreach (KeyValuePair<string, ClassFeature> entry in feature_mis)
            {
                entry.Value.calculateMI();
            }
        }
        public void countN01s()
        {
            foreach (KeyValuePair<string, ClassFeature> entry in feature_mis)
            {
                entry.Value.calculateN01(doc_count);
            }
        }
        public void filterFeaturesByMI(int count)
        {
            feature_mis = feature_mis.OrderByDescending(o => o.Value.MI).ToDictionary(i => i.Key, i => i.Value);
            //Dictionary<string, ClassFeature>  temp_dictionary = new Dictionary<string, ClassFeature>();

            int items_count = feature_mis.Count() - count;
            int counter = 0;
            foreach (KeyValuePair<string, ClassFeature> entry in feature_mis.Reverse())
            {
                if (counter<items_count)
                {
                    feature_mis.Remove(entry.Key);
                }
                counter++;
            }
        }

        public void debugFiltered()
        {
            Console.WriteLine("["+class_name+"]");
            foreach (KeyValuePair<string, ClassFeature> entry in feature_mis)
            {
                Console.WriteLine("[" + entry.Key + "]["+entry.Value.MI);
            }
            Console.ReadLine();
        }

    }
}
