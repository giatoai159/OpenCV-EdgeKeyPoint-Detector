#pragma once

#include "lib.h"

class EdgeDetector
{
public:
	int DetectBySobel(const cv::Mat& sourceImage, cv::Mat& destinationImage, cv::Mat& Gx, cv::Mat& Gy);
	int DetectByPrewitt(const cv::Mat& sourceImage, cv::Mat& destinationImage, cv::Mat& Gx, cv::Mat& Gy);
	int DetectByLaplace(const cv::Mat& sourceImage, cv::Mat& destinationImage);
	int GaussBlur(const cv::Mat& sourceImage, cv::Mat& destinationImage, int size = 3, double sigma = 1);
	int DetectByCanny(const cv::Mat& sourceImage, cv::Mat& destinationImage);
};