using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace Connect4
{
    class GameplayManager
    {
        MainWindow window;
        public bool game_started;
        int player_turn;
        int mode;
        int checkers_count;
        int[,] board = new int[6,7];
        int[] column_count = new int[7];
        Random random = new Random();
        public GameplayManager(MainWindow window)
        {
            this.window = window;
        }
    
        public void startGame(int mode = 0)
        {
            game_started = true;
            checkers_count = 0;
            this.mode = mode;
            for (int column = 0; column<7; column++)
            {
                for (int row = 0; row < 6; row++)
                {
                    board[row, column] = -1;
                    Image yellow = (Image)(window.FindName("yellow" + row.ToString() + column.ToString()));
                    Image red = (Image)(window.FindName("red" + row.ToString() + column.ToString()));

                    red.Visibility = System.Windows.Visibility.Hidden;
                    yellow.Visibility = System.Windows.Visibility.Hidden;
                }
                column_count[column] = 0;
            }

            randomPlayerTurn();
        }

        private void randomPlayerTurn()
        {
            player_turn = random.Next(2);
            updatePlayerTurnText();
        }

        private void updatePlayerTurnText()
        {
            window.Turn_TextBlock.Text = "PLAYER " + getPlayersColor(player_turn) + " TURN";
        }

        private bool checkCombination(int[] combination)
        {
            bool win = false;

            int count = 1;
            for (int i = 0; i < 6; i++)
            {
                if ((combination[i] != -1) && (combination[i + 1] != -1))
                {
                    if (combination[i] == combination[i + 1])
                    {
                        count++;
                        if (count == 4) win = true;
                    }
                    else count = 1;
                }
                else count = 1;
            }
            return win;
        }

        private int[] getCombination(int combinationType, int last_row, int last_column)
        {
            int[] combination = new int[7];
            int row = -1, column = -1;

            int counter = 0;
            for (int i = -3; i <= 3; i++)
            {
                if (combinationType == 0)
                {
                    row = last_row;
                    column = last_column + i;
                }
                else if (combinationType == 1)
                {
                    row = last_row + i;
                    column = last_column;
                }
                else if (combinationType == 2)
                {
                    row = last_row + i;
                    column = last_column + i;
                }
                else if (combinationType == 3)
                {
                    row = last_row - i;
                    column = last_column + i;
                }

                combination[counter] = -1;
                if ((row >= 0) && (column >= 0))
                {
                    if ((row < 6) && (column < 7))
                    {
                        combination[counter] = board[row, column];
                    }
                }

                counter++;
            }

            return combination;
        }

        private void checkIfFinished(int last_row, int last_column)
        {
            bool win = false;

            int counter = 0;
            while ((!win)&&(counter<4))         
            {
                win = checkCombination(getCombination(counter, last_row, last_column));
                counter++;
            }

            if (win)
            {
                game_started = false;
                player_turn = (player_turn + 1) % 2;
                window.Turn_TextBlock.Text = "PLAYER " + getPlayersColor(player_turn) + " WINS!";
            }
            else if (checkers_count == 42)
            {
                game_started = false;
                window.Turn_TextBlock.Text = "IT IS A TIE!";
            }
        }

        public void newMove(int column)
        {
            int row = column_count[column];
            if (row<6)
            {
                Image checker = (Image)window.FindName("red"+ row + column);
                if (player_turn == 1)
                {
                    checker = (Image)window.FindName("yellow" + row + column);
                }
                checker.Visibility = System.Windows.Visibility.Visible;
                column_count[column]++;
                checkers_count++;
                board[row, column] = player_turn;

                player_turn = (player_turn + 1) % 2;
                updatePlayerTurnText();
                checkIfFinished(row, column);
            }
        }
        private string getPlayersColor(int player)
        {
            string color = "YELLOW";
            if (player == 0)
            {
                color = "RED";
            }
            return color;
        }

    }
}
