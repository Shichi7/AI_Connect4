using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;
using System.Windows;

namespace Connect4
{
    class Animations
    {
        public static bool animation_working = false;
        private static Random rand = new Random();
        public static DoubleAnimation getBasicFadeAnimation(DependencyObject target, bool fade_in = true, int milliseconds = 250, double acceleration_ratio = 0.4)
        {
            DoubleAnimation animation = new DoubleAnimation
            {
                From = 1,
                To = 0,
                Duration = TimeSpan.FromMilliseconds(milliseconds),
                AccelerationRatio = acceleration_ratio
            };

            if (fade_in)
            {
                animation.From = 0;
                animation.To = 1;
            }

            Storyboard.SetTargetProperty(animation, new PropertyPath("Opacity"));
            Storyboard.SetTarget(animation, target);
            return animation;
        }
        public static Storyboard createFadeStoryboard(DependencyObject[] controls, bool fade_in = true, int milliseconds = 250, double acceleration_ratio = 0.4)
        {
            Storyboard board = new Storyboard();
            foreach (DependencyObject control in controls)
            {
                board.Children.Add(getBasicFadeAnimation(control, fade_in, milliseconds, acceleration_ratio));
            }

            return board;
        }
    }
}
