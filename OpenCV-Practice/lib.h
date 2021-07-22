#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types_c.h>

using namespace cv;

#define ORIGINAL "Original image"
#define PROCESSED "Processed image"

int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage);