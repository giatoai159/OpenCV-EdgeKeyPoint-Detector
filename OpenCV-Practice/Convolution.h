#pragma once

#include "lib.h"

class Convolution
{
	/*
	std::vector<Vec2i> _kernelIndex
	{
		{-1,-1},{0,-1},{1,-1},
		{-1,0},{0,0},{1,0},
		{-1,1},{0,1},{1,1}
	}; // 3x3 kernel index
	*/
	std::vector<Vec2i> _kernelIndex;
	std::vector<double> _kernel;
	int _size;
public:
	std::vector<Vec2i> getKernelIndex()
	{
		return _kernelIndex;
	}

	std::vector<double> getKernel()
	{
		return _kernel;
	}

	void setKernel(std::vector<double> kernel)
	{
		_kernel = kernel;
	}
	void generateKernelIndex(int size);
	void generateGaussKernel(double sigma);
	void generateGaussKernel2(double sigma);
	void generateLoGKernel(double sigma);
	/*
	1: Sobel X
	2: Sobel Y
	3: Prewitt X
	4: Prewitt Y
	5: Laplace
	6: Gaussian
	*/
	void setKernel(int type, int size = 3, double sigma = 1);

	int DoConvolution(const cv::Mat& sourceImage, cv::Mat& destinationImage);
	std::vector<std::vector<double>> DoConvolution(std::vector<std::vector<double>> source);
};