using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace WpfApp
{
    public partial class Window1 : Window
    {
        public ObservableCollection<FragmentItem> FragmentItems { get; } = new ObservableCollection<FragmentItem>();

        public Window1(int totalFragments, int fragmentWidth, int fragmentHeight)
        {
            InitializeComponent();

            for (int i = 0; i < totalFragments; i++)
            {
                FragmentItems.Add(new FragmentItem
                {
                    Width = fragmentWidth,
                    Height = fragmentHeight
                });
            }

            FragmentItemsControl.ItemsSource = FragmentItems;
        }

        public void AddFragment(Fragment fragment, BitmapImage image)
        {
            Dispatcher.Invoke(() =>
            {
                if (fragment.GlobalIndex >= 0 && fragment.GlobalIndex < FragmentItems.Count)
                {
                    var item = FragmentItems[fragment.GlobalIndex];
                    item.Image = image;
                    item.ImageIndex = fragment.Index;
                    item.X = fragment.X;
                    item.Y = fragment.Y;
                }
            });
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);


            foreach (var item in FragmentItems)
            {
                item.DisposeImage();
            }
            FragmentItems.Clear();


            GC.Collect();
            GC.WaitForPendingFinalizers();
        }


        public void ClearAllImages()
        {
            foreach (var item in FragmentItems)
            {
                item.DisposeImage();
            }
        }
    }
}
