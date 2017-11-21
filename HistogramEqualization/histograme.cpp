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
	imshow("ԭͼ", img);


	int high = img.rows;
	int width = img.cols;

	if (img.channels()== 1)//�ж��Ƿ�Ϊ��ͼ
	{												//��ͼ����
		imshow("ԭʼBͨ��", img);					
		imshow("ԭʼRͨ��ֱ��ͼ", hisDis(img, high, width, fre[0]));//��ʾֱ��ͼ
		img = hisTr(img, high, width, fre[0]);						//���⴦��
		imshow("�����Bͨ��", img);
		imshow("�����Rͨ��ֱ��ͼ", hisDis(img, high, width, fre[0]));
	}
	else											//��ͼ����
	{
		Mat channel[3];
		split(img, channel);						 //�ָ���ɫͨ��

		imshow("ԭʼRͨ��", channel[0]);
		imshow("ԭʼGͨ��", channel[1]);
		imshow("ԭʼBͨ��", channel[2]);
																			
		imshow("ԭʼRͨ��ֱ��ͼ", hisDis(channel[0], high, width, fre[0]));//��ʾֱ��ͼ
		imshow("ԭʼGͨ��ֱ��ͼ", hisDis(channel[1], high, width, fre[1]));
		imshow("ԭʼBͨ��ֱ��ͼ", hisDis(channel[2], high, width, fre[2]));

		channel[0] = hisTr(channel[0], high, width, fre[0]); //���⴦��
		channel[1] = hisTr(channel[1], high, width, fre[1]);
		channel[2] = hisTr(channel[2], high, width, fre[2]);

		imshow("�����Rͨ��", channel[0]);
		imshow("�����Gͨ��", channel[1]);
		imshow("�����Bͨ��", channel[2]);

		imshow("�����Rͨ��ֱ��ͼ", hisDis(channel[0], high, width, fre[0]));
		imshow("�����Gͨ��ֱ��ͼ", hisDis(channel[1], high, width, fre[1]));
		imshow("�����Bͨ��ֱ��ͼ", hisDis(channel[2], high, width, fre[2]));

		merge(channel, 3, img);					// ����ͨ���ϲ�
		imshow("�����ͼ��", img);
	}
	
	waitKey();
	return 0;
}

/* Ѱ�����������ֵ */
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
/* ֱ��ͼ��ʾ */
Mat hisDis(Mat img, int high, int width, uint* fre) {

	Mat hist_img = Mat::zeros(256, 256, CV_8UC3);
	for (int j = 0; j < high; j++)			//����Ƶ��
	{
		uchar* Val = img.ptr< uchar>(j);
		for (int i = 0; i < width; i++)
		{
			fre[(uint)Val[i]]++;
		}
	}
	int tt = maxNum(fre);					//��ֱ��ͼ
	for (int i = 0; i <256; i++)
	{
		rectangle(hist_img, Point(i, 255 - (fre[i] * 255 / tt)),
			Point((i + 1), 255),
			CV_RGB(0, 255, 0));
	}
	return(hist_img);
}
/* ֱ��ͼ���⴦�� */
Mat hisTr(Mat img, int high, int width, uint* fre) {
	float frequueccy[256] = { 0 };
	float fresum[256] = { 0 };
	Mat himg = Mat::zeros(high, width, CV_8UC3);
	for (int i = 0; i < 256; i++)
	{
		frequueccy[i] = fre[i] / (high*width*1.0);	//����Ƶ��
		if (i == 0) {
			fresum[i] = frequueccy[i];				//����Ƶ�ʷֲ�����
		}
		else {
			fresum[i] = fresum[i - 1] + frequueccy[i];
		}
	}

	for (int i = 0; i < 256; i++)			 //������ֵ����ӳ�䵽����������
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