#include "KeyPointDetector.h"
// https://www.programmersought.com/article/21136770876/
// https://github.com/alexanderb14/Harris-Corner-Detector/blob/master/harris.cpp
// https://github.com/adityaintwala/Harris-Corner-Detection/blob/master/find_harris_corners.py
// https://courses.fit.hcmus.edu.vn/pluginfile.php/159049/mod_resource/content/1/CV_Week_4_VN.pdf
Mat KeyPointDetector::detectHarris(const cv::Mat sourceImage, float k, float threshold, int size, double sigma)
{
	Convolution convolution;
	
	// Đạo hàm Sobel
	Mat dx, dy;
	convolution.setKernel(1);
	convolution.DoConvolution(sourceImage, dx);
	convolution.setKernel(2);
	convolution.DoConvolution(sourceImage, dy);
	// Tìm Ixx, Iyy, Ixy
	Mat Ixx, Iyy, Ixy;
	Ixx = dx.mul(dx);
	Iyy = dy.mul(dy);
	Ixy = dx.mul(dy);
	EdgeDetector edgedetector;
	// Apply filter Gaussian lên Ixx, Iyy, Ixy
	Mat gIxx, gIyy, gIxy;
	edgedetector.GaussBlur(Ixx, gIxx, size, sigma);
	edgedetector.GaussBlur(Iyy, gIyy, size, sigma);
	edgedetector.GaussBlur(Ixy, gIxy, size, sigma);
	Mat R(dy.rows, dy.cols, CV_32F,Scalar(0.0));
	// R = det(M) - k * trace(M)^2
	for (int y = 0; y < dy.rows; y++)
	{
		for (int x = 0; x < dy.cols; x++)
		{
			float a11, a12, a21, a22;
			// |Ixx  Ixy|
			// |Ixy  Iyy|
			a11 = gIxx.at<uchar>(y, x);
			a22 = gIyy.at<uchar>(y, x);
			a12 = gIxy.at<uchar>(y, x);
			a21 = gIxy.at<uchar>(y, x);
			float det = a11 * a22 - a12 * a21; // det(M)
			float trace = a11 + a22; // trace(M)
			R.at<float>(y, x) = abs(det - k * trace * trace);
		}
	}
	
	std::vector<Vec2i> cornerPoints;
	for (int i = 1; i < R.rows - 1; i++)
	{
		for (int j = 1; j < R.cols - 1; j++)
		{
			// Tìm điểm cực trị cục bộ
			double local_max = R.at<float>(i, j);
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					if (R.at<float>(i + x, j + y) > local_max)
					{
						local_max = R.at<float>(i + x, j + y);
					}
				}
			}
			// Chọn điểm cực trị cục bộ và lớn hơn threshold là điểm corner
			if (R.at<float>(i, j) == local_max && local_max > threshold)
			{
				cornerPoints.push_back({ j,i });
			}
		}
	}
	
	Mat destinationImage = sourceImage.clone();
	
	for (int i = 0; i < cornerPoints.size(); ++i)
	{
		circle(destinationImage, cornerPoints[i], 3, Scalar(0,0,255),-1);
	}
	
	return destinationImage;
}
// https://projectsflix.com/opencv/laplacian-blob-detector-using-python/
Mat KeyPointDetector::detectBlobLoG(const cv::Mat sourceImage, int scale_number, double sigma, double threshold)
{
	std::vector<Mat> log_images;
	double k = 1.414;
	Convolution convolution;
	std::vector<double> sigma_arr;
	Mat tempImage;
	std::vector<std::vector<double>> imageVal;
	std::vector<double> tempVec;
	cvtColor(sourceImage, tempImage, COLOR_BGR2GRAY);
	tempImage.convertTo(tempImage, CV_32FC1, 1.0 / 255.0);
	/*
	for (int y = 0; y < tempImage.rows; y++) // Duyệt từng dòng của ảnh
	{
		uchar* pDestinationRow = tempImage.ptr<uchar>(y); // Con trỏ từng dòng ma trận
		for (int x = 0; x < tempImage.cols; x++)
		{
			tempVec.push_back((float)pDestinationRow[x] / 255.0);
		}
		imageVal.push_back(tempVec);
		tempVec.clear();
	}
	*/
	/*
	for (int y = 0; y < imageVal.size(); y++)
	{
		for (int x = 0; x < imageVal[y].size(); x++)
		{
			std::cout << imageVal[y][x] << " ";
		}
		std::cout << std::endl;
	}
	*/
	for (int i = 0; i < scale_number; i++)
	{
		/*
		std::vector<std::vector<double>> temp;
		double y = pow(k, i);
		double sigma_1 = sigma * y;
		sigma_arr.push_back(sigma_1);
		convolution.generateLoGKernel(sigma_1);
		temp = convolution.DoConvolution(imageVal);
		
		for (int y = 0; y < temp.size(); y++)
		{
			for (int x = 0; x < temp[y].size(); x++)
			{
				temp[y].assign(x, temp[y].at(x) * temp[y].at(x));
			}
		}
		*/
		//
		Mat temp;
		double y = pow(k, i);
		double sigma_1 = sigma * y;
		sigma_arr.push_back(sigma_1);
		convolution.generateLoGKernel(sigma_1);
		filter2D(tempImage, temp, -1, convolution.getKernel());
		Mat temp_pad;
		int padding = 1;
		temp_pad.create(temp.rows + 2 * padding, temp.cols + 2 * padding, tempImage.type());
		temp_pad.setTo(Scalar::all(0));
		temp.copyTo(temp_pad(Rect(padding, padding, temp.cols, temp.rows)));
		temp_pad = temp_pad.mul(temp_pad);
		log_images.push_back(temp_pad);
		
		
		
	}
	
	std::vector<Vec3d> keyPoints;
	
	for (int y = 1; y < sourceImage.rows; y++)
	{
		if (y + 2 >= sourceImage.cols) continue;
		for (int x = 1; x < sourceImage.cols; x++)
		{
			if (x + 2 >= sourceImage.cols) continue;
			//std::vector<Mat> slice_img;
			double result = 0.0;
			
			int result_i = 0;
			Point result_loc;
			double lmax;
			Point maxloc;
			for (int image_index = 0; image_index < scale_number; image_index++)
			{
				//slice_img.push_back(log_images[image_index](Range(x - 1, x + 2), Range(y - 1, y + 2)));
				minMaxLoc(log_images[image_index](Range(x - 1, x + 2), Range(y - 1, y + 2)), NULL, &lmax,NULL,&maxloc);
				//std::cout << "lmax: " << lmax << std::endl;
				if (lmax > result)
				{
					result = lmax;
					result_i = image_index;
					result_loc = maxloc;
				}
			}
			//std::cout << "-->" << result << std::endl;
			//std::cout << result_i << "  :  "<<result_loc << std::endl;
			if (result >= 0.03)
			{
				//std::cout << "X: " << result_loc.x << std::endl;
				//std::cout << "Y: " << result_loc.y << std::endl;
				//std::cout << "Z: " << result_i << std::endl;
				keyPoints.push_back({(double)x+result_loc.x-1,(double)y+result_loc.y-1,pow(k,result_i*sigma)});

			}
		}
	}
	
	Mat destinationImage = sourceImage.clone();
	for (int i = 0; i < keyPoints.size(); ++i)
	{
		Point keyPoint;
		keyPoint.x = keyPoints[i][1];
		keyPoint.y = keyPoints[i][0];
		circle(destinationImage, keyPoint, keyPoints[i][2]*1.414, Scalar(0, 0, 255), 0);
	}
	
	return destinationImage;

	//return log_images[0];
	//return Mat();
}

Mat KeyPointDetector::detectBlobDoG(const cv::Mat sourceImage, int scale_number, double sigma, double threshold, double k)
{
	std::vector<Mat> log_images;
	Mat tempPrev, tempCurr;
	Convolution convolution;
	convolution.generateGaussKernel2(pow(k, 0));
	convolution.DoConvolution(sourceImage, tempPrev);
	
	for (int i = 1; i < scale_number; i++)
	{
		Mat temp;
		convolution.generateGaussKernel2(pow(k, i));
		convolution.DoConvolution(sourceImage, tempCurr);
		temp = tempCurr - tempPrev;
		temp = temp.mul(temp);
		log_images.push_back(temp);
		tempPrev.release();
		tempPrev = tempCurr;
		tempCurr.release();
	}
	
	std::vector<Vec3d> keyPoints;

	for (int y = 1; y < sourceImage.rows; y++)
	{
		if (y + 2 >= sourceImage.rows) continue;
		for (int x = 1; x < sourceImage.cols; x++)
		{
			if (x + 2 >= sourceImage.cols) continue;
			double result = 0.0;
			int result_i = 0;
			Point result_loc;
			double lmax;
			Point maxloc;
			for (int image_index = 0; image_index < scale_number-1; image_index++)
			{
				minMaxLoc(log_images[image_index](Range(y - 1, y + 2), Range(x - 1, x + 2)), NULL, &lmax, NULL, &maxloc);
				if (lmax > result)
				{
					result = lmax;
					result_i = image_index;
					result_loc = maxloc;
				}
			}
			if (result >= threshold)
			{
				keyPoints.push_back({ (double)y + result_loc.y - 1,(double)x + result_loc.x - 1,pow(k,(result_i+1) * sigma) });
			}
		}
	}
	Mat destinationImage = sourceImage.clone();
	for (int i = 0; i < keyPoints.size(); ++i)
	{
		Point keyPoint;
		keyPoint.x = keyPoints[i][1];
		keyPoint.y = keyPoints[i][0];
		circle(destinationImage, keyPoint, keyPoints[i][2] * 1.414, Scalar(0, 0, 255), 0);
	}
	
	//return log_images[2];
	return destinationImage;

}
