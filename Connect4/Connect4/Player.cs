using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Connect4
{
    class Player
    {
        int player_number;
        public string algorithm;
        public int depth;
        public string color;
        public int moves;
        public bool win;
        public double total_time;
        public Stopwatch watch;
        public double wage1;
        public double wage2;
        public int winning_rows;
        public double score;
          

        public Player(int number, string algorithm, int depth = 0, string strategy = "")
        {
            watch = new Stopwatch();
            player_number = number;
            this.algorithm = algorithm;
            this.depth = depth;
            if (number==0)
            {
                color = "YELLOW";
            }
            else
            {
                color = "RED";
            }

            wage1 = 1;
            wage2 = -1;

            if (strategy.Equals("PREFER WIN")) wage1 = 2;
            else if (strategy.Equals("ONLY WIN")) wage2 = 0;

            moves = 0;
            total_time = 0;
            win = false;

            winning_rows = 0;
            score = 0;
        }

        public void setWinningRows(int winning_rows, int opponent_rows)
        {
            this.winning_rows = winning_rows;
            score = (double)winning_rows * wage1 + (double)opponent_rows * wage2;
        }

        public string getString()
        {
            string stats = "Player: ["+player_number+"]\n";
            stats += "Color: [" + color+"]\n";
            stats += "Algorithm: [" + algorithm + "]\n";

            if (!algorithm.Equals("human"))
            {
                stats += "Depth: [" + depth + "]\n";
            }

            stats += "\nMoves: [" + moves + "]\n";
            stats += "Time: [" + total_time.ToString("0.00") + "]s\n\n";

            stats += "Winning rows: [" + winning_rows + "]\n";
            if (!algorithm.Equals("human"))
            {
                stats += "Heuristics wages: [" + wage1 + "][" + wage2 + "]\n";
                stats += "Score: [" + score + "]";
            }

            return stats;
        }

        public void moveMade()
        {
            moves++;
            watch.Stop();
            total_time += (watch.ElapsedMilliseconds/1000.0);
            watch.Reset();
        }
    }
}
