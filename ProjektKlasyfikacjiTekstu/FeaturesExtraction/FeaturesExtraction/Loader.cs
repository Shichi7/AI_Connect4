using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace FeaturesExtraction
{
    class Loader
    {
        static bool limitless = true;
        static int load_doc_limit = 800;

        static int minimal_feature_length = 4;

        private string data_path;
        public List<Document> loaded_documents = new List<Document>();
        public List<TextClass> classes = new List<TextClass>();
        public Dictionary<string, int> all_features = new Dictionary<string, int>();

        public List<string> filteredFeatures = new List<String>();

        public Loader(string data_path)
        {
            this.data_path = data_path + "\\";
        }

        public void featureExtraction(int important_classes_count)
        {
            List<string> list_files = Directory.GetFiles(data_path).ToList();

            loadClassess(list_files);
            loadWordsToClasses(list_files);
            countN00sAndN01s();
            calculateMIs(important_classes_count);
            combineFilteredFeatures();
            //debugFilteredFeatures();
            fillDocumentDictionaries();
            //debugDocuments();

            all_features.Clear();

            Console.WriteLine("Ekstrakcja zakończona <wcisnij Enter aby kontunować>");
            Console.ReadLine();
        }

        private void fillDocumentDictionaries()
        {
            foreach(Document doc in loaded_documents)
            {
                doc.fillDictionary(filteredFeatures);
            }
        }

        private void combineFilteredFeatures()
        {
            foreach (TextClass text_class in classes)
            {
                foreach (KeyValuePair<string, ClassFeature> entry in text_class.feature_mis)
                {
                    if (!filteredFeatures.Contains(entry.Key))
                    {
                        filteredFeatures.Add(entry.Key);
                    }
                }
            }
        }
        
        private void debugDocuments()
        {
            Console.Write("Wczytano: [" + loaded_documents.Count + "] dokumentów:");
            foreach (Document doc in loaded_documents)
            {
                doc.debugDocument();
            }
        }
        private void debugFilteredFeatures()
        {
            Console.Write("Wyselekcjonowano: [" + filteredFeatures.Count + "] cech:");
            foreach (string feature in filteredFeatures)
            {
                Console.WriteLine("[" + feature + "]");
            }
            Console.ReadLine();
        }
        private void loadClassess(List<string> list_files)
        {
            foreach (string full_filename in list_files)
            {
                string filename = full_filename.Split('\\')[1];
                string doc_class = filename.Split('_')[0];

                TextClass current_class = classes.SingleOrDefault(item => item.class_name.Equals(doc_class));

                if (current_class == null)
                {
                    current_class = new TextClass(doc_class);
                    classes.Add(current_class);
                }

                current_class.doc_count++;
            }

            //Console.Write("Loaded: " + classes.Count + " klas");
            //Console.ReadLine();
        }

        private void loadWordsToClasses(List<string> list_files)
        {
            int doc_limit = 0;
            foreach (string full_filename in list_files)
            {
                if ((limitless) || (doc_limit < load_doc_limit))
                {
                    string filename = full_filename.Split('\\')[1];
                    string doc_class = filename.Split('_')[0];

                    string content = File.ReadAllText(full_filename);

                    var punctuation = content.Where(Char.IsPunctuation).Distinct().ToArray();

                    List<string> words = content.Split().Select(x => x.Trim(punctuation)).ToList();
                    words = words.Select(i => i.ToLower()).ToList();
                    words = words.Where(i => i.Length >= minimal_feature_length).ToList();

                    loaded_documents.Add(new Document(filename, doc_class, words));

                    List<string> unique_words = words.Distinct().ToList();

                    foreach (string word in words)
                    {
                        if (!all_features.ContainsKey(word))
                        {
                            all_features.Add(word, 1);
                            foreach (TextClass text_class in classes)
                            {
                                text_class.feature_mis.Add(word, new ClassFeature(word));
                            }
                        }
                        else
                        {
                            all_features[word]++;
                        }
                    }

                    foreach (string word in unique_words)
                    {
                        foreach (TextClass text_class in classes)
                        {
                            if (text_class.class_name.Equals(doc_class))
                            {
                                text_class.feature_mis[word].N11++;
                            }
                            else
                            {
                                text_class.feature_mis[word].N10++;
                            }
                        }
                    }
                }
                doc_limit++;
            }
        }
        private void countN00sAndN01s()
        {
            foreach (TextClass text_class in classes)
            {
                text_class.countN01s();
            }

            foreach (KeyValuePair<string, int> entry in all_features)
            {
                foreach (TextClass current_class in classes)
                {
                    double N00 = 0;
                    foreach (TextClass text_class in classes)
                    {
                        if (!text_class.class_name.Equals(current_class.class_name))
                        {
                            N00 += text_class.feature_mis[entry.Key].N01;
                        }
                    }
                    current_class.feature_mis[entry.Key].N00 = N00;
                }
            }
        }

        private void calculateMIs(int important_classes_count)
        {
            foreach (TextClass current_class in classes)
            {
                current_class.calculateMIs();
                current_class.filterFeaturesByMI(important_classes_count);
                //current_class.debugFiltered(); 
            }
        }
    }
}
