#define _CRT_SECURE_NO_WARNINGS 

/*************************************************************/
/*  程式名稱：Image Read/Write/Rotation/Channel_Interchange   */
/*           (using OpenCV function)                         */
/*  作者姓名：卓諭                                            */
/*	學號:106318025                                           */
/*  指導教授：Dr.黃正民                                       */
/*  電子郵件：t106318025@ntut.org.tw                         */
/************************************************************/
#include <cstdio>
#include <opencv2/opencv.hpp>
#define _USE_MATH_DEFINES //定義pi
#include <math.h>


using namespace cv;

int main()
{
	void image1();//Image Read & Write
	void image2();//Image Rotation
	void image3();//Image Channel Interchange
	void image4();//Bonus
	int select;
	while (1)
	{
		printf(" 1.Image Read & Write\n ");
		printf("2.Image Rotation\n ");
		printf("3.Channel Interchange\n ");
		printf("4.Bonus\n ");
		printf(" Please select a number : ");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
		{image1(); break; }
		case 2:
		{image2(); break; }
		case 3:
		{image3(); break; }
		case 4:
		{image4(); break; }
		}
	}

}
/*********************Image Read/Write*************************/
void image1()
{
	/*------------------------讀檔---------------------------*/
	Mat image = imread("InputImage1.bmp", 1);
	/*------------------------寫檔---------------------------*/
	imwrite("CV_code_output_image.bmp", image);

	imshow("lena_out", image);
	waitKey(0);
}
/**************************************************************/
/*********************Image Rotation***************************/
void image2()
{
	/*------------------------讀檔---------------------------*/
	Mat image = imread("InputImage1.bmp", 1);
	bool flag_1 = true;
	bool flag_2 = true;

	float a;
	int b;
	int i;
	int pick;
	int deg;
	while (flag_1)
	{
		printf(" 1.choose your own rotation degree\n ");
		printf("2.Dynamic rotation([Esc] to stop)\n ");
		printf(" Please select a number or enter 0 to exit {2.Image Rotation} : ");
		scanf("%d", &pick);
		if (pick == 0)
		{
			flag_1 = false;
			break;
		}
		switch (pick)
		{
		case 1:
		{
			printf("choose the degree :");
			scanf("%d", &deg);
			a = abs(sin(deg*(M_PI / 180))) + abs(cos(deg * (M_PI / 180)));
			b = a;//將雙精度浮點數捨取小數點成整數

			Mat dst;//宣告一個要輸出的矩陣，矩陣大小由判斷式求得
			if (a > b)//判斷若算出的frame大小有小數點則整個frame長寬各加一
			{
				dst.cols = a * 512 + 1;
				dst.rows = dst.cols;
			}
			else
			{
				dst.cols = a * 512;
				dst.rows = dst.cols;
			}

			Mat dst_ROI(dst.rows, dst.cols, CV_8UC3, Scalar(0));//宣告一個新的矩陣，大小與dst同
			Point2f im_center(dst.cols / 2 - 0.5, dst.rows / 2 - 0.5);//(-0.5)才能讓旋轉中心更為準確
			image.copyTo(dst_ROI(Rect(im_center.x - 512 / 2, im_center.y - 512 / 2, 512, 512)));
			//將原始未旋轉的影像放至剛宣告的新矩陣dst_ROI中我們感興趣範圍內(即矩陣中心512*512範圍)

			Mat rotate = getRotationMatrix2D(im_center, deg, 1.0);
			//取得圖片旋轉or放大的轉換矩陣，旋轉中心依據dst的frame大小而變(一直在dst中間)

			warpAffine(dst_ROI, dst, rotate, dst_ROI.size());
			imwrite("CV_code_rotation.bmp", dst);
			imshow("Rotate", dst);
			waitKey();
			flag_1 = false;
			break;
		}
		case 2:
		{
			while (flag_2)
			{
				for (i = 0; i <= 360; i++)
				{
					a = abs(sin(i*(M_PI / 180))) + abs(cos(i*(M_PI / 180)));
					b = a;//將雙精度浮點數捨取小數點成整數

					Mat dst;//宣告一個要輸出的矩陣，矩陣大小由判斷式求得
					if (a > b)//判斷若算出的frame大小有小數點則整個frame長寬各加一
					{
						dst.cols = a * 512 + 1;
						dst.rows = dst.cols;
					}
					else
					{
						dst.cols = a * 512;
						dst.rows = dst.cols;
					}

					Mat dst_ROI(dst.rows, dst.cols, CV_8UC3, Scalar(0));//宣告一個新的矩陣，大小與dst同
					Point2f im_center(dst.cols / 2 - 0.5, dst.rows / 2 - 0.5);//(-0.5)才能讓旋轉中心更為準確
					image.copyTo(dst_ROI(Rect(im_center.x - 512 / 2, im_center.y - 512 / 2, 512, 512)));
					//將原始未旋轉的影像放至剛宣告的新矩陣dst_ROI中我們感興趣範圍內(即矩陣中心512*512範圍)

					Mat rotate = getRotationMatrix2D(im_center, i, 1.0);
					//取得圖片旋轉or放大的轉換矩陣，旋轉中心依據dst的frame大小而變(一直在dst中間)

					warpAffine(dst_ROI, dst, rotate, dst.size());


					circle(dst, Point(im_center.x + 512 / 2, im_center.y - 512 / 2), 1, Scalar(0, 0, 255), 4, 8, 0);
					circle(dst, Point(im_center.x + 512 / 2, im_center.y + 512 / 2), 1, Scalar(0, 0, 255), 4, 8, 0);
					circle(dst, Point(im_center.x - 512 / 2, im_center.y + 512 / 2), 1, Scalar(0, 0, 255), 4, 8, 0);
					circle(dst, Point(im_center.x - 512 / 2, im_center.y - 512 / 2), 1, Scalar(0, 0, 255), 4, 8, 0);
					line(dst, Point(im_center.x - 512 / 2, im_center.y - 512 / 2), Point(im_center.x + 512 / 2, im_center.y - 512 / 2), Scalar(0, 255, 255), 1, 8, 0);
					line(dst, Point(im_center.x + 512 / 2, im_center.y - 512 / 2), Point(im_center.x + 512 / 2, im_center.y + 512 / 2), Scalar(0, 255, 255), 1, 8, 0);
					line(dst, Point(im_center.x + 512 / 2, im_center.y + 512 / 2), Point(im_center.x - 512 / 2, im_center.y + 512 / 2), Scalar(0, 255, 255), 1, 8, 0);
					line(dst, Point(im_center.x - 512 / 2, im_center.y + 512 / 2), Point(im_center.x - 512 / 2, im_center.y - 512 / 2), Scalar(0, 255, 255), 1, 8, 0);

					circle(dst, Point(dst.cols - 512 * sin((i - 90 * (i / 90))*(M_PI / 180)), 0), 2, Scalar(0, 0, 255), 4, 8, 0);//轉動的右上點
					circle(dst, Point(dst.cols, dst.rows - 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), 2, Scalar(0, 0, 255), 4, 8, 0);//轉動的右下點
					circle(dst, Point(512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180))), dst.rows), 2, Scalar(0, 0, 255), 4, 8, 0);//轉動的左下點
					circle(dst, Point(0, 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), 2, Scalar(0, 0, 255), 4, 8, 0);//轉動的左下點
					line(dst, Point(dst.cols - 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180))), 0), Point(dst.cols, dst.rows - 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), Scalar(255, 255, 0), 1, 8, 0);
					line(dst, Point(dst.cols, dst.rows - 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), Point(512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180))), dst.rows), Scalar(255, 255, 0), 1, 8, 0);
					line(dst, Point(512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180))), dst.rows), Point(0, 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), Scalar(255, 255, 0), 1, 8, 0);
					line(dst, Point(0, 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180)))), Point(dst.cols - 512 * abs(sin((i - 90 * (i / 90))*(M_PI / 180))), 0), Scalar(255, 255, 0), 1, 8, 0);

					imshow("Rotate", dst);
					if (waitKey(35) == 27)
					{
						flag_2 = false;
						break;
					}
				}
				flag_1 = false;
				break;
			}
			break;
		}
		default:
		{
			printf("Are you an illiteracy ? \n");
			system("pause");
			printf("Such a poor guy\n");
			system("pause");
			break;
		}
		}
	}
}
/**************************************************************/
/******************Image Channel Interchange*******************/
void image3()
{
	Mat image = imread("CV_code_rotation.bmp", 1);
	Mat bgr[3];
	Mat rbg[3];
	Mat merge_img;
	split(image, bgr);
	rbg[0] = bgr[1];
	rbg[1] = bgr[2];
	rbg[2] = bgr[0];
	merge(rbg, 3, merge_img);

	imwrite("CV_code_Channel_Interchange.bmp", merge_img);
	imshow("Channel Interchange", merge_img);

	waitKey();
}
/**************************************************************/
/***************************Bonus******************************/
void image4()
{
	Mat cropped = imread("lena_cropped.bmp", 1);

	Mat dst;//宣告一個要輸出的矩陣，矩陣大小由判斷式求得
	dst.cols = cropped.rows;
	dst.rows = cropped.cols;
	Point2f im_center(288 / 2 - 0.5, 288 / 2 - 0.5);//(-0.5)才能讓旋轉中心更為準確
	Mat rotate = getRotationMatrix2D(im_center, 270, 1.0);
	//取得圖片旋轉or放大的轉換矩陣，旋轉中心依據dst的frame大小而變(一直在dst中間)
	warpAffine(cropped, dst, rotate, dst.size());

	imwrite("CV_code_lena_cropped.bmp", cropped);
	imwrite("CV_code_lena_cropped_rotation.bmp", dst);

	imshow("Cropped", cropped);
	imshow("Cropped_rotation", dst);
	waitKey();
}
/**************************************************************/