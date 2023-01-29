#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
     cv::CommandLineParser parser(argc, argv,
                               "{@input   |lena.jpg|input image}"
                               "{ksize   k|1|ksize (hit 'K' to increase its value at run time)}"
                               "{scale   s|1|scale (hit 'S' to increase its value at run time)}"
                               "{delta   d|0|delta (hit 'D' to increase its value at run time)}"
                               "{help    h|false|show help message}");
  
  cout << "We are open source code snippets from the OpenCV documentation.\n\n";
  parser.printMessage();
  cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values ( ksize will be -1 equal to Scharr function )";

    Mat image, src;
    Mat grad;
    const String window_name = "Sobel LAB";
    int ksize = parser.get<int>("ksize");
    int scale = parser.get<int>("scale");
    int delta = parser.get<int>("delta");
    int ddepth = CV_16S;

    // Import image and save on two variables
    image = imread(argv[1], 1);

    // Check if image is loaded fine
    if (image.empty())
    {
        printf("Error opening image: %s\n", (char *)argv[1]);
        return EXIT_FAILURE;
    }

   for (;;)
  {
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(image, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Grayscale the grayscale_image var
    printf("Original image is %d x %d pels\n", image.rows, image.cols);
    Mat grayscale_image(Size(image.cols, image.rows), CV_8UC1);
    for (unsigned int r = 0; r < image.rows; r++)
        for (unsigned int c = 0; c < image.cols; c++)
        {
            Vec3b intensity = image.at<Vec3b>(r, c);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            grayscale_image.at<uchar>(r, c) = (uchar)((blue + green + red) / 3);
        }


    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(grayscale_image, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    Sobel(grayscale_image, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    
    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    imshow(window_name, grad);
    
    char key = (char)waitKey(0);
    
    if(key == 27)
    {
      return EXIT_SUCCESS;
    }
    if (key == 'k' || key == 'K')
    {
      ksize = ksize < 30 ? ksize+2 : -1;
    }
    if (key == 's' || key == 'S')
    {
      scale++;
    }
    if (key == 'd' || key == 'D')
    {
      delta++;
    }
    if (key == 'r' || key == 'R')
    {
      scale =  1;
      ksize = -1;
      delta =  0;
    }
  }
  return EXIT_SUCCESS;
}