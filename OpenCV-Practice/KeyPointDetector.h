#pragma once

#include "Convolution.h"
#include "EdgeDetector.h"
#include "lib.h"

class KeyPointDetector
{
public:
	Mat detectHarris(const cv::Mat sourceImage, float k = 0.04f, float threshold = 10000.0f, int size = 3, double sigma = 1.0);
	Mat detectBlobLoG(const cv::Mat sourceImage, int scale_number, double sigma=1.0, double threshold = 10000.0);
	Mat detectBlobDoG(const cv::Mat sourceImage, int scale_number, double sigma = 1.0, double threshold = 175, double k = 1.414);
};

