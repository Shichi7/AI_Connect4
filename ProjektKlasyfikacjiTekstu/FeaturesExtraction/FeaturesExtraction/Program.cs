using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeaturesExtraction
{
    class Program
    {
        static int important_classes_count = 40;
        static string data_folder_name = "data";

        static void Main(string[] args)
        {
            Loader doc_loader = new Loader(data_folder_name);
            doc_loader.featureExtraction(important_classes_count);

            List<Document> loaded_documents = doc_loader.loaded_documents;
            List<string> filtered_features = doc_loader.filteredFeatures;
            List<TextClass> classes = doc_loader.classes;

            ARFFCreator.createFile(loaded_documents, filtered_features, classes);
        }
    }
}
