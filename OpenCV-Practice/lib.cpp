#include "lib.h"

int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		return 1;
	int width = sourceImage.cols, height = sourceImage.rows;
	int sourceChannels = sourceImage.channels();
	destinationImage = Mat(Size(width, height), CV_8UC1);
	int destinationChannels = destinationImage.channels();

	for (int y = 0; y < height; y++)
	{
		uchar* pSourceRow = (uchar*)(sourceImage.ptr<uchar>(y));
		uchar* pDestinationRow = (uchar*)(destinationImage.ptr<uchar>(y));
		for (int x = 0; x < width; x++, pSourceRow += sourceChannels, pDestinationRow += destinationChannels)
		{
			uchar B = pSourceRow[0];
			uchar G = pSourceRow[1];
			uchar R = pSourceRow[2];
			uchar gray = (uchar)((0.299 * R) + (0.587 * G) + (0.114 * B));
			//uchar gray = (uchar)((R + G + B) / 3);
			pDestinationRow[0] = gray;
		}
	}
	return 0;
}
