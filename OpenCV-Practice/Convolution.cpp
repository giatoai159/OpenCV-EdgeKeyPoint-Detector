#include "Convolution.h"

void Convolution::generateKernelIndex(int size)
{
	_kernelIndex.clear();
	int offsetX = (size - 1) / 2;
	int offsetY = (size - 1) / 2;
	for (int y = -offsetY; y <= offsetY; y++)
	{
		for (int x = -offsetX; x <= offsetX; x++)
		{
			_kernelIndex.push_back({ x,y });
		}
	}
}
// https://www.geeksforgeeks.org/gaussian-filter-generation-c/
void Convolution::generateGaussKernel(double sigma)
{
	_kernel.clear();
	double r;
	double s = 2.0 * pow(sigma, 2);
	double sum = 0.0;
	const double PI = 3.14159265358979323846;
	// Tính toán kernel
	for (int k_i = 0; k_i < _kernelIndex.size(); k_i++)
	{
		int x = _kernelIndex[k_i][0];
		int y = _kernelIndex[k_i][1];
		r = sqrt(pow(x, 2) + pow(y, 2));
		//r = pow(x, 2) + pow(y, 2);
		_kernel.push_back((exp(-(r * r) / s)) / (PI * s));
	}
	// Chuẩn hóa giá trị
	for (const double& val : _kernel)
		sum += val;
	for (double& val : _kernel)
		val /= sum;

}

void Convolution::generateGaussKernel2(double sigma)
{
	_kernel.clear();
	double r;
	double s = 2.0 * pow(sigma, 2);
	double sum = 0.0;
	const double PI = 3.14159265358979323846;
	int n = (int)ceil(sigma * 6);
	if ((int)floor(n) % 2 == 0)
		n = floor(n) + 1;
	else n = floor(n);
	generateKernelIndex(n);
	// Tính toán kernel
	for (int k_i = 0; k_i < _kernelIndex.size(); k_i++)
	{
		int x = _kernelIndex[k_i][0];
		int y = _kernelIndex[k_i][1];
		r = sqrt(pow(x, 2) + pow(y, 2));
		_kernel.push_back((exp(-(r * r) / s)) / (PI * s));
	}
	// Chuẩn hóa giá trị
	for (const double& val : _kernel)
		sum += val;
	for (double& val : _kernel)
		val /= sum;

}

void Convolution::generateLoGKernel(double sigma)
{
	_kernel.clear();
	_kernelIndex.clear();
	double r;
	double s2 = pow(sigma, 2);
	double s4 = pow(sigma, 4);
	double sum = 0.0;
	const double PI = 3.14159265358979323846;
	double n = ceil(sigma * 6);
	/*
	if ((int)floor(n) % 2 == 0)
		n = floor(n) + 1;
	else n = floor(n);
	generateKernelIndex(n);
	*/
	//std::cout << n<< std::endl;
	// Tính toán kernel
	for (int y = floor(-n / 2); y < floor(n / 2) + 1; y++)
	{
		for (int x = floor(-n / 2); x < floor(n / 2) + 1; x++)
		{
			_kernelIndex.push_back({ x,y });
			double x2 = x * x;
			double y2 = y * y;
			double x_filter = exp(-(x2 / (2.0 * sigma * sigma)));
			double y_filter = exp(-(y2 / (2.0 * sigma * sigma)));
			double final_filter = (-(2 * s2) + (x2 + y2)) * (x_filter * y_filter) * (1 / (2 * PI * s4));
			_kernel.push_back(final_filter);
		}
	}
	/*
	for (int k_i = 0; k_i < _kernelIndex.size(); k_i++)
	{
		int x = _kernelIndex[k_i][0];
		int y = _kernelIndex[k_i][1];
		double x2 = x * x;
		double y2 = y * y;
		double x_filter = exp(-(x2 / (2.0 * sigma * sigma)));
		double y_filter = exp(-(y2 / (2.0 * sigma * sigma)));
		double final_filter = (-(2 * s2) + (x2 + y2)) * (x_filter * y_filter) * (1 / (2 * PI * s4));
		_kernel.push_back(final_filter);
	}
	*/
}



void Convolution::setKernel(int type, int size, double sigma)
{
	switch (type)
	{
	case 1: {
		generateKernelIndex(size);
		_kernel.clear();
		_kernel = { -1,0,1,-2,0,2,-1,0,1 }; // Sobel Horizontal
	}
		  break;
	case 2: { 
		generateKernelIndex(size);
		_kernel.clear();
		_kernel = { -1,-2,-1,0,0,0,1,2,1 }; // Sobel Vertical
	}
		  break;
	case 3: { 
		generateKernelIndex(size);
		_kernel.clear();
		_kernel = { 1,0,-1,1,0,-1,1,0,-1 }; // Prewitt Horizontal
	}
		  break;
	case 4: { 
		generateKernelIndex(size);
		_kernel.clear();
		_kernel = { 1,1,1,0,0,0,-1,-1,-1 }; // Prewitt Vertical
	}
		  break;
	case 5: {
		generateKernelIndex(size);
		_kernel.clear();
		_kernel = { -1,-1,-1,-1,8,-1,-1,-1,-1 }; // Laplace
		//_kernel = { 1,1,1,1,-8,1,1,1,1 }; // Laplace
	}
		  break;
	case 6: {
		generateKernelIndex(size); 
		generateGaussKernel(sigma);
	}
	case 7: {
		//generateKernelIndex(size);
		generateLoGKernel(sigma);
	}
		break;
	default:
		break;
	}
}


// https://www.stdio.vn/computer-vision/phep-tich-chap-trong-xu-ly-anh-convolution-r1vHu1
int Convolution::DoConvolution(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
	// Số dòng cột của ảnh input
	int nRow = sourceImage.rows;
	int nCol = sourceImage.cols;
	// Check ảnh input
	if (nRow <= 0 || nCol <= 0)
		return 0;
	Mat tempImage;
	// Chuyển ảnh màu sang ảnh xám để tính chập
	RGB2GrayScale(sourceImage, tempImage);
	// Tạo ma trận lưu giá trị tính chập
	destinationImage.create(nRow, nCol, CV_8UC1);
	for (int y = 0; y < nRow; y++) // Duyệt từng dòng của ảnh
	{
		uchar* pDestinationRow = (uchar*)destinationImage.ptr<uchar>(y); // Con trỏ từng dòng ma trận
		for (int x = 0; x < nCol; x++)
		{
			double sum = 0.0;
			for (int k_i = 0; k_i < _kernelIndex.size(); k_i++)
			{
				// Xét dòng nằm ngoài ảnh
				int indexRow = y + _kernelIndex[k_i][1]; // offset y của kernel
				if (indexRow <0 || indexRow>nRow - 1) // Bỏ qua nếu index nằm ngoài ảnh 
					continue;
				int indexCol = x + _kernelIndex[k_i][0]; // Tương tự như trên nhưng xét cột nằm ngoài ảnh
				if (indexCol<0 || indexCol>nCol - 1)
					continue;
				
				sum += _kernel[k_i]*tempImage.at<uchar>(indexRow,indexCol); // Edge Detection
			}
			
			if (sum < 0.0)
				sum = 0.0;
			if (sum > 255.0)
				sum = 255.0;
				
			pDestinationRow[x] = sum;
		}
	}
	return 1;
}

std::vector<std::vector<double>> Convolution::DoConvolution(std::vector<std::vector<double>> source)
{

	std::vector<std::vector<double>> imageVal;
	std::vector<double> tempVec;
	for (int y = 0; y < source.size(); y++) // Duyệt từng dòng của ảnh
	{
		for (int x = 0; x < source[y].size(); x++)
		{
			double sum = 0.0;
			//std::cout << _kernelIndex.size();
			for (int k_i = 0; k_i < _kernelIndex.size(); k_i++)
			{
				//std::cout << _kernel[k_i] << "*" << source[y][x] << std::endl;
				// Xét dòng nằm ngoài ảnh
				int indexRow = y + _kernelIndex[k_i][1]; // offset y của kernel
				//std::cout << "y: " << y << "kY: " << _kernelIndex[k_i][1] << "iRow: " << indexRow;
				if (indexRow <0 || indexRow > source.size() - 1) // Bỏ qua nếu index nằm ngoài ảnh 
				{
					//std::cout << "-->Skip" << std::endl;
					continue;
				}
				//std::cout << std::endl;
				int indexCol = x + _kernelIndex[k_i][0]; // Tương tự như trên nhưng xét cột nằm ngoài ảnh
				//std::cout << "x: " << x << "xY: " << _kernelIndex[k_i][0] << "iCol: " << indexCol;
				if (indexCol<0 || indexCol > source[y].size() - 1)
				{
					//std::cout << "-->Skip" << std::endl;
					continue;
				}
				//std::cout << std::endl;
				//std::cout << _kernel[k_i] << "*" << source[indexRow][indexCol]<<"="<< _kernel[k_i] * source[indexRow][indexCol] << std::endl;
				sum += _kernel[k_i] * source[indexRow][indexCol]; 
			}
			tempVec.push_back(sum);
			//std::cout <<"-->" <<sum<<std::endl;
		}
		imageVal.push_back(tempVec);
		tempVec.clear();
	}
	return imageVal;
}
