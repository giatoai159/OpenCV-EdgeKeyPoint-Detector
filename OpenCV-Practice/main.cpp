#include "lib.h"
#include "EdgeDetector.h"
#include "KeyPointDetector.h"
#include "Convolution.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "<Program name> <Path to image> <Command Argument>" << std::endl;
        std::cout << "--harris <k> <threshold>" << std::endl;
        std::cout << "--dog <scale> <sigma> <threshold>" << std::endl;
        // DESCRIPTIONS GO HERE
        return 0;
    }
    std::string _inputImage = argv[1];
    std::string command = argv[2];
    if (haveImageReader(_inputImage) == false) // Check xem đường dẫn tới ảnh có hợp lệ không
    {
        std::cout << "Path to " << _inputImage << " is invalid.";
        return 0;
    }
    Mat inputImage = imread(_inputImage, IMREAD_COLOR); // Biến lưu ảnh input
    Mat outputImage; // Biến lưu ảnh output
    KeyPointDetector kpdetector;
    if (command == "--harris")
    {
        std::string k = argv[3];
        std::string threshold = argv[4];
        if (stof(k) < 0.04f || stof(k) > 0.06f)
        {
            std::cout << "Gia tri k phai nam giua 0.04 va 0.06. Xin vui long nhap lai.";
            return 0;
        }
        outputImage = kpdetector.detectHarris(inputImage, stof(k), stof(threshold));
    }
    else if (command == "--log")
    {
        outputImage = kpdetector.detectBlobLoG(inputImage,9);

    }
    else if (command == "--dog")
    {
        std::string scale = argv[3];
        std::string sigma = argv[4];
        std::string threshold = argv[5];
        outputImage = kpdetector.detectBlobDoG(inputImage, stoi(scale), stod(sigma), stod(threshold));
    }
    else
    {
        std::cout << "This command doesn't exist.";
        return 0;
    }
    namedWindow(ORIGINAL);
    imshow(ORIGINAL, inputImage);
    namedWindow(PROCESSED);
    imshow(PROCESSED, outputImage);
    waitKey(0);

    return 0;
}