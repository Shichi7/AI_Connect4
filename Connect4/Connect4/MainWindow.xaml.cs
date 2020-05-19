using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Animation;

namespace Connect4
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        GameplayManager manager;
        int current_column = 0;
        static double font_scale = 0.03;
        int window_size = 1;
        public MainWindow()
        {
            InitializeComponent();
            toggleWindowSize();
            fillCheckersGrid();
            fillArrowsGrid();
            manager = new GameplayManager(this);
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            centerWindow();
        }

        private void centerWindow()
        {
            double screenWidth = SystemParameters.PrimaryScreenWidth;
            double screenHeight = SystemParameters.PrimaryScreenHeight;
            Left = (screenWidth / 2) - (Width / 2);
            Top = (screenHeight / 2) - (Height / 2);
        }
        private void setFontSize()
        {
            Application.Current.Resources["GlobalFontSize"] = Height * font_scale;
            Application.Current.Resources["TitleFontSize"] = 1.3 * Height * font_scale;
            Application.Current.Resources["SecondaryFontSize"] = 0.8 * Height * font_scale;
        }

        public void toggleWindowSize()
        {
            double small_window_percentage = 50;
            double large_window_percentage = 100;

            double ratio_x = 16;
            double ratio_y = 9;

            window_size = (window_size + 1) % 2;

            double percentage = small_window_percentage;
            if (window_size == 1)
            {
                percentage = large_window_percentage;
            }

            double screenHeight = SystemParameters.PrimaryScreenHeight;
            double screenWidth = SystemParameters.PrimaryScreenWidth;

            if (screenHeight < screenWidth)
            {
                double temp_height = screenHeight / 100;
                Height = percentage * temp_height;
                Width = ratio_x * Height / ratio_y;
            }
            else
            {
                double temp_width = screenWidth / 100;
                Width = percentage * temp_width;
                Height = ratio_y * Width / ratio_x;
            }

            setFontSize();
            centerWindow();

            Storyboard fadeIn = getFadeInStoryboard();
            fadeIn.Begin();
        }
        public Storyboard getFadeInStoryboard(int millis = 300)
        {
            Control[] control = { this };
            Storyboard board = Animations.createFadeStoryboard(control, true, millis);
            return board;
        }
        public Storyboard getFadeOutStoryboard(int millis = 300)
        {
            Control[] control = { this };
            Storyboard board = Animations.createFadeStoryboard(control, false, millis);
            return board;
        }
        public void quitGame(object sender = null, EventArgs e = null)
        {
            this.Close();
        }

        public void updateCurrentColumn (int new_column)
        {
            Image arrow = (Image)(this.FindName("arrow" + current_column));
            arrow.Visibility = Visibility.Hidden;

            current_column = new_column;

            arrow = (Image)this.FindName("arrow" + new_column);
            arrow.Visibility = Visibility.Visible;
        }

        private void fillArrowsGrid()
        {
            Grid arrows_grid = Arrows_Grid;
            BitmapImage arrow_bitmap = new BitmapImage();
            arrow_bitmap.BeginInit();
            arrow_bitmap.UriSource = new Uri("Resources/arrow.png", UriKind.Relative);
            arrow_bitmap.EndInit();

            for (int i = 0; i<7; i++)
            {
                string name = "arrow"+i;
                Image arrow = new Image();
                arrow.Source = arrow_bitmap;
                arrow.Name = name;
                RegisterName(name, arrow);
                Grid.SetColumn(arrow, i);
                arrows_grid.Children.Add(arrow);
                arrow.Visibility = Visibility.Hidden;
             }
        }
        private void fillCheckersGrid()
        {
            Grid checkers_grid = Checkers_Grid;
            BitmapImage red_checker_bitmap = new BitmapImage();
            BitmapImage yellow_checker_bitmap = new BitmapImage();

            red_checker_bitmap.BeginInit();
            yellow_checker_bitmap.BeginInit();
            red_checker_bitmap.UriSource = new Uri("Resources/red_checker.png", UriKind.Relative);
            yellow_checker_bitmap.UriSource = new Uri("Resources/yellow_checker.png", UriKind.Relative);
            red_checker_bitmap.EndInit();
            yellow_checker_bitmap.EndInit();

            for (int row = 0; row<6; row++)
            {
                for (int column = 0; column<7; column++)
                {
                    Image red_checker = new Image();
                    Image yellow_checker = new Image();
                    red_checker.Source = red_checker_bitmap;
                    yellow_checker.Source = yellow_checker_bitmap;

                    string name_appendix = (5-row).ToString() + column.ToString();
                    string red_name = "red" + name_appendix;
                    string yellow_name = "yellow" + name_appendix;

                    red_checker.Name = red_name;
                    yellow_checker.Name = yellow_name;

                    RegisterName(red_name, red_checker);
                    RegisterName(yellow_name, yellow_checker);

                    Grid.SetRow(red_checker, row);
                    Grid.SetRow(yellow_checker, row);
                    Grid.SetColumn(red_checker, column);
                    Grid.SetColumn(yellow_checker, column);
                    checkers_grid.Children.Add(red_checker);
                    checkers_grid.Children.Add(yellow_checker);
                    red_checker.Visibility = Visibility.Hidden;
                    yellow_checker.Visibility = Visibility.Hidden;
                }
            }
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {
                Storyboard fadeOut = getFadeOutStoryboard();
                fadeOut.Completed += new EventHandler(quitGame);
                fadeOut.Begin();
            }
            else if (e.Key == Key.LeftShift)
            {
                toggleWindowSize();
            }
        }
     

        private void Board_wall_PreviewMouseMove(object sender, MouseEventArgs e)
        {

            Image image = (Image)sender;
            var point = Mouse.GetPosition(image);

            Grid grid = Checkers_Grid;

            int column = 0;
            ColumnDefinition definition = grid.ColumnDefinitions[0];
            double column_width = definition.ActualWidth;
            column = (int)(point.X / column_width);

            column = column > 6 ? 6 : column;
            column = column < 0 ? 0 : column;

            if (current_column!=column)
            {
                updateCurrentColumn(column);
            }
        }

        private void NewGameButton_Click(object sender, RoutedEventArgs e)
        {
            manager.startGame();
        }

        private void Board_wall_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (manager.game_started)
            {
                manager.newMove(current_column);
            }
        }
    }
}
