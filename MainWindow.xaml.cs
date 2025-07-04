using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;

namespace WpfApp
{
    public partial class MainWindow : Window
    {
        private readonly HttpClient _httpClient = new HttpClient();
        private readonly Random _random = new Random();
        private List<string> imageUrls = new List<string>
        {
            "https://upload.wikimedia.org/wikipedia/commons/9/9d/Stones-in-water.jpg",
            "http://www.kryzuy.com/wp-content/uploads/2015/12/KryzUy.Com-Sumilon-Island-Blue-Water-Resort-2.jpg",
            "https://img.freepik.com/free-photo/reflection-of-rocky-mountains-and-sky-on-beautiful-lake_23-2148153610.jpg",
            "https://img.freepik.com/free-photo/vestrahorn-mountains-in-stokksnes-iceland_335224-667.jpg",
            "https://img.freepik.com/free-photo/pier-at-a-lake-in-hallstatt-austria_181624-44201.jpg"
        };
        public ObservableCollection<Fragment> Fragments { get; } = new ObservableCollection<Fragment>();
        public ObservableCollection<Fragment> TempFragments { get; } = new ObservableCollection<Fragment>();

        public MainWindow()
        {
            InitializeComponent();
            _httpClient.DefaultRequestHeaders.UserAgent.ParseAdd("Mozilla/5.0");
            FragmentsDataGrid.ItemsSource = Fragments;
        }

        private bool ValidateInputs(out int fragWidth, out int fragHeight, out int count)
        {
            fragWidth = fragHeight = count = 0;

            if (!int.TryParse(FragWidth.Text, out fragWidth) || fragWidth <= 0)
            {
                MessageBox.Show("Некорректная ширина фрагмента!");
                return false;
            }

            if (!int.TryParse(FragHeight.Text, out fragHeight) || fragHeight <= 0)
            {
                MessageBox.Show("Некорректная высота фрагмента!");
                return false;
            }

            if (!int.TryParse(FragCount.Text, out count) || count <= 0)
            {
                MessageBox.Show("Некорректное количество фрагментов!");
                return false;
            }

            return true;
        }
        private void GenerateFragmentIndices(int count)
        {
            TempFragments.Clear();

            for (int i = 0; i < count; i++)
            {
                TempFragments.Add(new Fragment
                {
                    Index = _random.Next(imageUrls.Count),
                    X = -1,
                    Y = -1,
                    GlobalIndex = i
                });
            }
        }
        private async Task<Image> LoadImage(int index)
        {
            string imageUrl = imageUrls[index].Trim();
            using var response = await _httpClient.GetAsync(imageUrl);
            response.EnsureSuccessStatusCode();

            using var stream = await response.Content.ReadAsStreamAsync();
            return await Image.LoadAsync(stream);
        }
        private async Task ProcessFragments(Window1 window1, int fragWidth, int fragHeight)
        {
            var groups = TempFragments.GroupBy(f => f.Index);

            foreach (var group in groups)
            {
                using (var sourceImage = await LoadImage(group.Key))
                {
                    foreach (var fragment in group)
                    {
                        try
                        {
                            if (sourceImage.Width < fragWidth || sourceImage.Height < fragHeight)
                            {
                                MessageBox.Show($"Изображение {fragment.Index} слишком маленькое");
                                continue;
                            }

                            int x = _random.Next(0, sourceImage.Width - fragWidth);
                            int y = _random.Next(0, sourceImage.Height - fragHeight);
                            fragment.X = x;
                            fragment.Y = y;

                            using var croppedImage = sourceImage.Clone(i =>
                                i.Crop(new SixLabors.ImageSharp.Rectangle(x, y, fragWidth, fragHeight)));

                            using var ms = new MemoryStream();
                            await croppedImage.SaveAsPngAsync(ms);
                            ms.Position = 0;

                            var bmp = new BitmapImage();
                            bmp.BeginInit();
                            bmp.CacheOption = BitmapCacheOption.OnLoad;
                            bmp.DecodePixelWidth = fragWidth;   // Оптимизация памяти
                            bmp.DecodePixelHeight = fragHeight; // Оптимизация памяти
                            bmp.StreamSource = ms;
                            bmp.EndInit();
                            bmp.Freeze();

                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                Fragments.Add(fragment);
                                window1.AddFragment(fragment, bmp);
                            });

                            // Явно освобождаем ресурсы
                            bmp = null;
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show($"Ошибка фрагмента {fragment.Index}: {ex.Message}");
                        }
                    }
                }
            }
        }
        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!ValidateInputs(out int width, out int height, out int count))
                return;

            Fragments.Clear();
            TempFragments.Clear();

            GenerateFragmentIndices(count);
            Window1 window1 = new Window1(count, width, height);
            window1.Owner = this; // Привязываем к главному окну
            window1.Closed += (s, args) =>
            {
                window1.ClearAllImages();
                window1 = null;
            };
            window1.Show();
            await ProcessFragments(window1, width, height);
        }
    }
}