using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;
using System.Globalization;

namespace Connect4
{
    class GameplayManager
    {
        MainWindow window;
        GameState state;
        Player[] players = new Player[2];

        bool first_move;
        public bool game_started;
        int player_turn;

        Random random = new Random();
        public GameplayManager(MainWindow window)
        {
            this.window = window;
        }
    
        public void startGame(int mode = 0)
        {
            state = new GameState(window);
            game_started = true;
            first_move = true;

            string cpu1_alg = window.Algorithm1Button.Text;
            int cpu1_depth = int.Parse(window.Depth1Button.Text, NumberStyles.AllowLeadingWhite | NumberStyles.AllowTrailingWhite);
            string cpu2_alg = window.Algorithm2Button.Text;
            int cpu2_depth = int.Parse(window.Depth2Button.Text, NumberStyles.AllowLeadingWhite | NumberStyles.AllowTrailingWhite);
            string cpu1_strategy = window.Strategy1Button.Text;
            string cpu2_strategy = window.Strategy2Button.Text;

            if (mode==0)
            {
                players[0] = new Player(1, "human");
                players[1] = new Player(2, "human");
            }
            else if (mode==1)
            {
                players[0] = new Player(1, "human");
                players[1] = new Player(2, cpu1_alg, cpu1_depth, cpu1_strategy);
            }
            else if (mode==2)
            {
                players[0] = new Player(1, cpu1_alg, cpu1_depth, cpu1_strategy);
                players[1] = new Player(2, cpu2_alg, cpu2_depth, cpu2_strategy);
            }

            window.Turn_TextBlock.Style = window.FindResource("YELLOW" + "TextBox") as Style;

            //player_turn = random.Next(2);
            player_turn = 0;
            players[player_turn].color = "YELLOW";
            players[(player_turn + 1) % 2].color = "RED";
            updatePlayerTurnText();
            players[player_turn].watch.Start();

            window.Stats1_TextBlock.Text = players[0].getString();
            window.Stats2_TextBlock.Text = players[1].getString();

            if (isCPUMove()) newMove();
        }

        private void updatePlayerTurnText()
        {
            window.Turn_TextBlock.Text = "PLAYER " + players[player_turn].color + " TURN";
        }

        private void swapPlayer()
        {
            player_turn = (player_turn + 1) % 2;
            players[player_turn].watch.Start();
            updatePlayerTurnText();
            window.Turn_TextBlock.Style = window.FindResource(players[player_turn].color + "TextBox") as Style;
        }
        public void newMove(int column = -1)
        {
            if (column == -1)
            {
                column = getComputerMove();
            }

            if (state.spaceInColumn(column))
            {
                if (first_move)
                {
                    first_move = false;
                }

                int row = state.newMove(column, player_turn);
                string checker_name = players[player_turn].color.ToLower() + row + column;
                Image checker = (Image)window.FindName(checker_name);
                checker.Visibility = System.Windows.Visibility.Visible;

                players[player_turn].moveMade();

                state.calculateWinningRows();
                players[player_turn].setWinningRows(state.players_winning_rows[player_turn], state.players_winning_rows[(player_turn + 1) % 2]);
                players[(player_turn+1)%2].setWinningRows(state.players_winning_rows[(player_turn + 1) % 2], state.players_winning_rows[player_turn]);

                window.Stats1_TextBlock.Text = players[0].getString();
                window.Stats2_TextBlock.Text = players[1].getString();

                if (state.checkIfFinished(row, column))
                {
                    game_started = false;
                    players[player_turn].win = true;
                    window.Turn_TextBlock.Text = "PLAYER " + players[player_turn].color + " WINS!";
                }
                else if (state.checkIfTied())
                {
                    game_started = false;
                    window.Turn_TextBlock.Text = "IT IS A TIE!";
                }

                if (game_started)
                {
                    swapPlayer();
                }
            }

            if (isCPUMove()) newMove();
        }

        public bool isCPUMove()
        {
            bool cpu_move = false;
            if ((game_started) && (!players[player_turn].algorithm.Equals("human")))
            {
                cpu_move = true;
            }
            return cpu_move;
        }

        public int getComputerMove()
        {
            int column = 0;
            string mode = players[player_turn].algorithm;

            if ((mode.Equals("RANDOM"))||(first_move))
            {
                column = random.Next(7);
            }
            else if (mode.Equals("MINMAX"))
            {
                column = minMaxInit(state, players[player_turn].depth);
            }
            else if (mode.Equals("ALPHA/BETA"))
            {
                column = minMaxInit(state, players[player_turn].depth, true);
            }
            return column;
        }

        public int minMaxInit(GameState state, int depth, bool alphabeta = false)
        {
            double alpha = -int.MaxValue;      
            double beta = int.MaxValue;

            int best_column = 0;
            double best_score = -int.MaxValue;
            int column = 0;
            bool alphabeta_break = false;

            while ((column<7)&&(!alphabeta_break))
            {
                if (state.spaceInColumn(column))
                {
                    int row = state.newMove(column, player_turn);
                    double temp_score = minMax(state, depth - 1, false, row, column, alphabeta, alpha, beta);
                    if (temp_score>best_score)
                    {
                        best_score = temp_score;
                        best_column = column;
                    }

                    if (alphabeta)
                    {
                        if (temp_score>alpha)
                        {
                            alpha = temp_score;
                        }
                        if (beta<=alpha)
                        {
                            alphabeta_break = true;
                        }
                    }

                    state.reverseMove(column);
                }
                column++;
            }

            return best_column;
        }

        public double minMax(GameState state, int depth, bool current_player, int last_row, int last_column, bool alphabeta, double alpha, double beta)
        {
            double score = 0;
            if(state.checkIfFinished(last_row, last_column))
            {
                if (!current_player) score = 10000;

                if (current_player) score = -10000;
            }
            else if (state.checkIfTied())
            {
                score = 0;
            }
            else if (depth == 0)
            {
                state.calculateWinningRows();

                players[player_turn].setWinningRows(state.players_winning_rows[player_turn], state.players_winning_rows[(player_turn+1)%2]);
                score = players[player_turn].score;
            }
            else
            {
                List<double> children_scores = new List<double>();

                int column = 0;
                bool alphabeta_break = false;

                while ((column < 7) && (!alphabeta_break))
                {
                    if (state.spaceInColumn(column))
                    {
                        int row = -1;
                        double temp_score = -1;
                        if(current_player)
                        {
                            row = state.newMove(column, player_turn);
                            temp_score = minMax(state, depth - 1, false, row, column, alphabeta, alpha, beta);
                            children_scores.Add(temp_score);
                            state.reverseMove(column);
                        }
                        else
                        {
                            row = state.newMove(column, (player_turn+1)%2);
                            temp_score = minMax(state, depth - 1, true, row, column, alphabeta, alpha, beta);
                            children_scores.Add(temp_score);
                            state.reverseMove(column);
                        }

                        if (alphabeta)
                        {
                            if ((current_player)&&(temp_score > alpha))
                            {
                                alpha = temp_score;
                            }
                            else if ((!current_player) && (temp_score < beta))
                            {
                                beta = temp_score;
                            }

                            if (beta <= alpha)
                            {
                                alphabeta_break = true;
                            }
                        }
                    }
                    column++;
                }

                if (current_player)
                {
                    score = children_scores.Max();
                }
                else
                {
                    score = children_scores.Min();
                }
            }

            return score;
        }
    }
}
