#include "EdgeDetector.h"
#include "Convolution.h"

int EdgeDetector::DetectBySobel(const cv::Mat& sourceImage, cv::Mat& destinationImage, cv::Mat& Gx, cv::Mat& Gy)
{
	Convolution convolution;
	convolution.setKernel(1);
	convolution.DoConvolution(sourceImage, Gx);
	convolution.setKernel(2);
	convolution.DoConvolution(sourceImage, Gy);
	destinationImage = (Gx + Gy) / 2;
	return 1;
}

int EdgeDetector::DetectByPrewitt(const cv::Mat& sourceImage, cv::Mat& destinationImage, cv::Mat& Gx, cv::Mat& Gy)
{
	Convolution convolution;
	convolution.setKernel(3);
	convolution.DoConvolution(sourceImage, Gx);
	convolution.setKernel(4);
	convolution.DoConvolution(sourceImage, Gy);
	destinationImage = (Gx + Gy) / 2;
	return 1;
}

int EdgeDetector::DetectByLaplace(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
	Convolution convolution;
	convolution.setKernel(5);
	convolution.DoConvolution(sourceImage, destinationImage);
	return 1;
}

int EdgeDetector::GaussBlur(const cv::Mat& sourceImage, cv::Mat& destinationImage, int size, double sigma)
{
	Convolution convolution;
	convolution.setKernel(6, size, sigma);
	convolution.DoConvolution(sourceImage, destinationImage);
	return 0;
}
// WIP CANNY
int EdgeDetector::DetectByCanny(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
	EdgeDetector::GaussBlur(sourceImage, destinationImage, 3, 1);
	Convolution convolution;
	convolution.setKernel(5);
	convolution.DoConvolution(destinationImage, destinationImage);
	return 0;
}