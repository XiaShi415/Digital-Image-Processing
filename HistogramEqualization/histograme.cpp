#include<iostream>  
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
using namespace std;
using namespace cv;
int maxNum(uint* iarray);
Mat hisTr(Mat img, int high, int width, uint* fre);
Mat hisDis(Mat img, int high, int width, uint* fre);
int main()
{
	uint fre[3][256] = { 0 };
	Mat img = imread("mytest3.jpg");
	if (img.empty())
		return -1;
	imshow("原图", img);


	int high = img.rows;
	int width = img.cols;

	if (img.channels()== 1)//判断是否为灰图
	{												//灰图处理
		imshow("原始B通道", img);					
		imshow("原始R通道直方图", hisDis(img, high, width, fre[0]));//显示直方图
		img = hisTr(img, high, width, fre[0]);						//均衡处理
		imshow("均衡后B通道", img);
		imshow("均衡后R通道直方图", hisDis(img, high, width, fre[0]));
	}
	else											//彩图处理
	{
		Mat channel[3];
		split(img, channel);						 //分割颜色通道

		imshow("原始R通道", channel[0]);
		imshow("原始G通道", channel[1]);
		imshow("原始B通道", channel[2]);
																			
		imshow("原始R通道直方图", hisDis(channel[0], high, width, fre[0]));//显示直方图
		imshow("原始G通道直方图", hisDis(channel[1], high, width, fre[1]));
		imshow("原始B通道直方图", hisDis(channel[2], high, width, fre[2]));

		channel[0] = hisTr(channel[0], high, width, fre[0]); //均衡处理
		channel[1] = hisTr(channel[1], high, width, fre[1]);
		channel[2] = hisTr(channel[2], high, width, fre[2]);

		imshow("均衡后R通道", channel[0]);
		imshow("均衡后G通道", channel[1]);
		imshow("均衡后B通道", channel[2]);

		imshow("均衡后R通道直方图", hisDis(channel[0], high, width, fre[0]));
		imshow("均衡后G通道直方图", hisDis(channel[1], high, width, fre[1]));
		imshow("均衡后B通道直方图", hisDis(channel[2], high, width, fre[2]));

		merge(channel, 3, img);					// 将三通道合并
		imshow("均衡后图像", img);
	}
	
	waitKey();
	return 0;
}

/* 寻找数组中最大值 */
int maxNum(uint* iarray)
{
	int max = 0;
	for (int i = 0; i < 256; i++)
	{
		if (iarray[i] > max)
			max = iarray[i];
	}
	return max;
}
/* 直方图显示 */
Mat hisDis(Mat img, int high, int width, uint* fre) {

	Mat hist_img = Mat::zeros(256, 256, CV_8UC3);
	for (int j = 0; j < high; j++)			//计算频数
	{
		uchar* Val = img.ptr< uchar>(j);
		for (int i = 0; i < width; i++)
		{
			fre[(uint)Val[i]]++;
		}
	}
	int tt = maxNum(fre);					//画直方图
	for (int i = 0; i <256; i++)
	{
		rectangle(hist_img, Point(i, 255 - (fre[i] * 255 / tt)),
			Point((i + 1), 255),
			CV_RGB(0, 255, 0));
	}
	return(hist_img);
}
/* 直方图均衡处理 */
Mat hisTr(Mat img, int high, int width, uint* fre) {
	float frequueccy[256] = { 0 };
	float fresum[256] = { 0 };
	Mat himg = Mat::zeros(high, width, CV_8UC3);
	for (int i = 0; i < 256; i++)
	{
		frequueccy[i] = fre[i] / (high*width*1.0);	//计算频率
		if (i == 0) {
			fresum[i] = frequueccy[i];				//计算频率分布函数
		}
		else {
			fresum[i] = fresum[i - 1] + frequueccy[i];
		}
	}

	for (int i = 0; i < 256; i++)			 //将像素值重新映射到均衡后的区域
	{
		fresum[i] = fresum[i] * 255 + 0.5;
	}

	int tmp = 0;
	for (int j = 0; j < high; j++)
	{
		uchar* Val = img.ptr< uchar>(j);
		for (int i = 0; i < width; i++)
		{
			tmp = Val[i];
			Val[i] = fresum[tmp];
		}
	}
	return(img);
}