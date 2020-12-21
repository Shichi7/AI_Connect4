using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FeaturesExtraction
{
    class ClassFeature
    {
        public string name;
        public double N11;
        public double N10;
        public double N01;
        public double N00;

        public double MI;

        public ClassFeature(string name)
        {
            this.name = name;
            MI = 0;
        }

        public void calculateN01(int doc_count)
        {
            N01 = doc_count - N11;
        }

        public void calculateMI()
        {
            double N = N00 + N10 + N01 + N11;
            double N1 = N10 + N11;
            double N0 = N01 + N00;
            MI = (N11 / N) * Math.Log(N * N11 / (N1 * (N01 + N11)), 2);
            MI += (N01 / N) * Math.Log(N * N01 / (N0 * (N01 + N11)), 2);
            MI += (N10 / N) * Math.Log(N * N10 / (N1 * (N00 + N10)), 2);
            MI += (N00 / N) * Math.Log(N * N00 / (N0 * (N00 + N10)), 2);
            if (Double.IsNaN(MI))
            {
                MI = 0;
            }
        }
    }
}
