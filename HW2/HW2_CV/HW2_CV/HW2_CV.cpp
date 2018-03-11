#include <cstdio>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;
clock_t time0, time1;


void expand(Mat After, Mat Before)
{
	int i, j;
	for (j = 0; j < Before.rows; j++)
	{
		for (i = 0; i < Before.cols; i++)
		{
			After.data[(Before.rows + 2) + (i + 1) + j*(Before.rows + 2)] = Before.data[i + j*Before.rows];//將原圖置中於擴展圖空間

			if (i == 0)
			{
				After.data[(Before.rows + 2) + i + j*(Before.rows + 2)] = 128;//填補左排空像素
				if (j == 0)
				{
					After.data[0] = 128;//填補左上空像素
				}
			}
			if (i == Before.cols - 1)
			{
				After.data[(Before.rows + 2) + (i + 2) + j*(Before.rows + 2)] = 128;//填補右排空像素
				if (j == Before.rows - 1)
				{
					After.data[Before.cols + 1 + (Before.rows + 2) * (Before.cols + 1)] = 128;//填補右下空像素
				}
			}
			if (j == 0)
			{
				After.data[(i + 1) + j*(Before.rows + 2)] = 128;//填補上排空像素
				if (i == Before.cols - 1)
				{
					After.data[Before.cols + 1 + (Before.rows + 2) * 0] = 128;//填補右上空像素
				}
			}
			if (j == Before.rows - 1)
			{
				After.data[(Before.rows + 3) + (i + 3) + j*(Before.rows + 3)] = 128;//填補下排空像素
				if (i == 0)
				{
					After.data[0 + (Before.rows + 2) * Before.cols + 1] = 128;//填補左下空像素
				}
			}
		}
	}
}

int main()
{
	/*----------------------------------------lena512二值化----------------------------------------*/
	Mat lena512 = imread("lena512.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat lena_binarized;
	threshold(lena512, lena_binarized, 128, 255, THRESH_BINARY);
	imwrite("lena_binarized.bmp", lena_binarized);
	imshow("origin", lena512);
	imshow("threshold", lena_binarized);
	waitKey(0);
	/*----------------------------------------找出bounding box lena512.bmp----------------------------------------*/
	time0 = clock();
	RNG rng;
	Mat lena_gray = imread("lena_binarized.bmp", CV_LOAD_IMAGE_GRAYSCALE);//lena512轉灰階
	Mat lena_binarized_Color = imread("lena_binarized.bmp", CV_LOAD_IMAGE_COLOR);//用RGB全彩讀二值化影像
	Mat lena_binarized_Gray = imread("lena_binarized.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat lena514(514, 514, CV_8UC1, Scalar(0));
	Mat edge;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	double area_lena512;

	time0 = clock();//計算時間開始
	/*-----------------------------------------------------------------------------------------------------------*/
	expand(lena514, lena_gray);
	Canny(lena514, edge, 20, 100, 5.5);
	imshow("lena edge", edge);
	findContours(lena_binarized_Gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

	for (int i = 0; i < contours.size(); i++)
	{
		area_lena512 = contourArea(contours[i], false);
		//printf("contourArea=%f\n", area_lena512);
		if ((int)area_lena512 > 1000)
		{
			//drawContours(lena_binarized_Color, contours, i, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), 0), 2, 8, hierarchy);
			Rect bounding = boundingRect(contours[i]);
			rectangle(lena_binarized_Color, bounding, Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	/*-----------------------------------------------------------------------------------------------------------*/
	time1 = clock();//計算時間結束

	printf("lena512.bmp bounding box computational time = %d ms\n", time1 - time0);
	imshow("lena bounding box", lena_binarized_Color);
	waitKey(0);

	/*----------------------------------------找出bounding box test1.bmp----------------------------------------*/

	Mat test1_gray = imread("test1.bmp", CV_LOAD_IMAGE_GRAYSCALE);//test1轉灰階
	Mat test1 = imread("test1.bmp", CV_LOAD_IMAGE_COLOR);//用RGB全彩讀二值化影像
	Mat edge_test1;
	vector<vector<Point>> contours_test1;
	vector<Vec4i> hierarchy_test1;
	double area_test1;

	time0 = clock();//計算時間開始
	/*-----------------------------------------------------------------------------------------------------------*/

	imshow("test1 original", test1_gray);
	Canny(test1_gray, edge_test1, 50, 150, 3);
	imshow("test1 edge", edge_test1);
	findContours(edge_test1, contours_test1, hierarchy_test1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours_test1.size(); i++)
	{
		area_test1 = contourArea(contours_test1[i], false);
		//printf("contourArea=%f\n", area_test1);
		if (area_test1 > 1000)
		{
			drawContours(test1, contours_test1, i, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), 0), 2, 8, hierarchy_test1);
			Rect bounding = boundingRect(contours_test1[i]);
			rectangle(test1, bounding, Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	/*-----------------------------------------------------------------------------------------------------------*/
	time1 = clock();//計算時間結束

	printf("test1.bmp bounding box computational time = %d ms\n", time1 - time0);
	imshow("test1.bmp bounding box", test1);
	waitKey(0);
	/*----------------------------------------找出bounding box test2.bmp----------------------------------------*/
	
	Mat test2_gray = imread("test2.bmp", CV_LOAD_IMAGE_GRAYSCALE);//test2轉灰階
	Mat test2 = imread("test2.bmp", CV_LOAD_IMAGE_COLOR);//用RGB全彩讀二值化影像
	Mat edge_test2;
	vector<vector<Point>> contours_test2;
	vector<Vec4i> hierarchy_test2;
	double area_test2;

	time0 = clock();//計算時間開始
	/*-----------------------------------------------------------------------------------------------------------*/

	imshow("test2 original", test2_gray);
	Canny(test2_gray, edge_test2, 50, 150, 3);
	imshow("test2 edge", edge_test2);
	findContours(edge_test2, contours_test2, hierarchy_test2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours_test2.size(); i++)
	{
		area_test2 = contourArea(contours_test2[i], false);
		//printf("contourArea=%f\n", area_test2);
		if (area_test2 > 1000)
		{
			drawContours(test2, contours_test2, i, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), 0), 2, 8, hierarchy_test2);
			Rect bounding = boundingRect(contours_test2[i]);
			rectangle(test2, bounding, Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	/*-----------------------------------------------------------------------------------------------------------*/
	time1 = clock();//計算時間結束

	printf("test2.bmp bounding box computational time = %d ms\n", time1 - time0);
	imshow("test2.bmp bounding box", test2);
	waitKey(0);
}