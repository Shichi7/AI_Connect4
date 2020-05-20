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

            player_turn = random.Next(2);
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
                column = random.Next(7);
            }

            if (state.spaceInColumn(column))
            {
                int row = state.newMove(column, player_turn);
                string checker_name = players[player_turn].color.ToLower() + row + column;
                Image checker = (Image)window.FindName(checker_name);
                checker.Visibility = System.Windows.Visibility.Visible;

                players[player_turn].moveMade();

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
    }
}
