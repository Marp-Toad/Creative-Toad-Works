using System.ComponentModel;
using System.Windows.Media.Imaging;

namespace WpfApp
{
    public class FragmentItem : INotifyPropertyChanged
    {

        private int _imageIndex;
        public int ImageIndex
        {
            get => _imageIndex;
            set
            {
                _imageIndex = value;
                OnPropertyChanged(nameof(ImageIndex));
                OnPropertyChanged(nameof(DisplayText));
            }
        }

        private double _x;
        public double X
        {
            get => _x;
            set
            {
                _x = value;
                OnPropertyChanged(nameof(X));
                OnPropertyChanged(nameof(DisplayText));
            }
        }

        private double _y;
        public double Y
        {
            get => _y;
            set
            {
                _y = value;
                OnPropertyChanged(nameof(Y));
                OnPropertyChanged(nameof(DisplayText));
            }
        }

        public string DisplayText =>
            double.IsNaN(X) ? $"Img: {ImageIndex}\nЗагрузка..."
                            : $"X: {X}, Y: {Y}\nImg: {ImageIndex}";

        private BitmapImage _image;
        public BitmapImage Image
        {
            get => _image;
            set
            {
                _image = value;
                OnPropertyChanged(nameof(Image));
                OnPropertyChanged(nameof(IsPlaceholder));
            }
        }

        public void DisposeImage()
        {
            if (_image != null)
            {
                _image.StreamSource?.Dispose();
                _image = null;
                OnPropertyChanged(nameof(Image));
                OnPropertyChanged(nameof(IsPlaceholder));
            }
        }

        public void Dispose()
        {
            DisposeImage();
            GC.SuppressFinalize(this);
        }

        public int Width { get; set; }
        public int Height { get; set; }
        public bool IsPlaceholder => Image == null;

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}