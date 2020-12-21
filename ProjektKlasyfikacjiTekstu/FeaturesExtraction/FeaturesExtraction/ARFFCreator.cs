using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace FeaturesExtraction
{
    class ARFFCreator
    {
        private static string filename_header = "features_";
        private static string filename_ext = ".arff";
        private static Dictionary<string, int> features_indexed;

        public static void createFile(List<Document> loaded_documents, List<string> filtered_features, List<TextClass> classes)
        {
            string filename = filename_header + filtered_features.Count + filename_ext;

            convertToIndexedFeatures(filtered_features);

            string files_content = buildHeader();
            files_content += buildAttributesDefinition(filtered_features, classes);   
            files_content += buildDocumentsFeaturesSparse(loaded_documents);

            saveFile(files_content, filename);

            Console.WriteLine("Zapisano!\n<Wcisnij Enter aby zakończyć>");
            Console.ReadLine();
        }

        private static void convertToIndexedFeatures(List<string> filtered_features)
        {
            features_indexed = new Dictionary<string, int>();
            int index = 1;
            foreach(string feature in filtered_features)
            {
                features_indexed.Add(feature, index);
                index++;
            }
        }
        private static string buildDocumentsFeatures(List<Document> loaded_documents)
        {
            string features_string = "\n@data\n";

            foreach (Document document in loaded_documents)
            {
                features_string += "'" + document.document_class + "', ";

                foreach (KeyValuePair<string, int> entry in document.features)
                {
                    features_string += entry.Value + ", ";
                }

                features_string = features_string.Substring(0, features_string.Length - 2);
                features_string += "\n";
            }

            return features_string;
        }
        private static string buildDocumentsFeaturesSparse(List<Document> loaded_documents)
        {
            string features_string = "\n@data\n";

            foreach (Document document in loaded_documents)
            {
                features_string += "{0 " + document.document_class + ", ";

                foreach (KeyValuePair<string, int> entry in document.features)
                {
                    if (entry.Value!=0)
                    {
                        features_string += features_indexed[entry.Key] +" "+ entry.Value + ", ";
                    }
                }

                features_string = features_string.Substring(0, features_string.Length - 2);
                features_string += "}\n";
            }

            return features_string;
        }

        private static void saveFile(string files_content, string filename)
        {
            try
            {
                if (File.Exists(filename))
                {
                    File.Delete(filename);
                }

                using (StreamWriter sw = File.CreateText(filename))
                {
                    sw.WriteLine(files_content);
                }
            }
            catch (Exception Ex)
            {
                Console.WriteLine(Ex.ToString());
            }
        }
        private static string buildHeader()
        {
            string header = "% 1. Wyekstrahowane cechy dokumentów do dalszej ich klasyfikacji.\n";
            header += "%2. Zajęcia: Sztuczna Inteligencja i inżynieria wiedzy\n";
            header += "%3. Autor: Aleksander Poławski\n\n";
            header += "@RELATION wikipedia_texts\n\n";
            return header;
        }

        private static string buildAttributesDefinition(List<string> features, List<TextClass> classes)
        {
            string definition = "";

            definition += "@ATTRIBUTE class {";

            foreach(TextClass doc_class in classes)
            {
                definition += doc_class.class_name + ", ";
            }
            definition = definition.Substring(0, definition.Length - 2);
            definition += "}\n";

            foreach (string entry in features)
            {
                definition += "@ATTRIBUTE feat_" + entry + " NUMERIC\n";
            }

            return definition;
        }
    }
}
