#include <cstdio>
#include <opencv2/opencv.hpp>
#include <iostream>
#include<time.h>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

Mat firstFrame;

Mat morph(Mat frame);
void calcLines(const Mat &input, std::vector<Vec2f> &lines);
void drawLines(Mat &input, const std::vector<Vec2f> &lines, Point &point1, Point &point2);
void onMouse(int Event, int x, int y, int flags, void* param);
Point P1(-1, -1);
Point P2(-1, -1);
Point PP1, PP2;
Point pp1, pp2;
Point p1, p2;
double t0, t1;
double time_average;
double time_total = 0;
double frameCount = 0;
stringstream execution_time;
int select;
int main()
{
	FILE *filein1, *filein2, *filein3, *filein4, *filein5;
	FILE *fileout1, *fileout2, *fileout3, *fileout4, *fileout5;

	filein1 = fopen("ground_truth_1.txt", "r");
	filein2 = fopen("ground_truth_2.txt", "r");
	filein3 = fopen("ground_truth_3.txt", "r");
	filein4 = fopen("ground_truth_4.txt", "r");
	filein5 = fopen("ground_truth_5.txt", "r");
	fileout1 = fopen("outputError1.txt","a");
	fileout2 = fopen("outputError2.txt", "a");
	fileout3 = fopen("outputError3.txt", "a");
	fileout4 = fopen("outputError4.txt", "a");
	fileout5 = fopen("outputError5.txt", "a");

	char file[10];
	printf("(1)1.avi \n");
	printf("(2)2.avi \n");
	printf("(3)3.avi \n");
	printf("(4)4.avi \n");
	printf("(5)5.avi \n");
	printf("Please press the number to choose the file : ");
	scanf("%d", &select);
	switch (select)
	{
	case 1:
	{sprintf(file, "1.avi"); break; }
	case 2:
	{sprintf(file, "2.avi"); break; }
	case 3:
	{sprintf(file, "3.avi"); break; }
	case 4:
	{sprintf(file, "4.avi"); break; }
	case 5:
	{sprintf(file, "5.avi"); break; }
	}

	VideoCapture video_first(file);
	if (!video_first.isOpened()){
		return -1;
	}

	Size videoSize = Size((int)video_first.get(CV_CAP_PROP_FRAME_WIDTH), (int)video_first.get(CV_CAP_PROP_FRAME_HEIGHT));

	VideoWriter writer;
	writer.open("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);

	video_first >> firstFrame;
	namedWindow("firstFrame", 0);
	setMouseCallback("firstFrame", onMouse, NULL);

	while (true)
	{
		if (P1.x == -1 && P2.x == -1)
		{
			imshow("firstFrame", firstFrame);
		}
		if (P1.x != -1 && P2.x != -1)
		{
			line(firstFrame, P1,P2, Scalar(255, 0, 0), 2);
			PP1 = P1;
			PP2 = P2;
			P1.x = -1;
			P1.y = -1;
			P2.x = -1;
			P2.y = -1;
			imshow("firstFrame", firstFrame);
		}
		if (waitKey(1) == 27)
		{
			break;
		}
	}
	video_first.release();
	VideoCapture video(file);
	Mat videoFrame;
	int Error;
	int a, b, c, d;
		while (true)
		{
			
			frameCount++;
			t0 = clock();//execution time
			video >> videoFrame;

			if (videoFrame.empty())
			{
				break;
			}
			
			Mat morImg = morph(videoFrame);
			vector<Vec2f> lines;
			calcLines(morImg, lines);
			drawLines(videoFrame, lines, PP1, PP2);
			PP1 = p1;
			PP2 = p2;
			switch (select)
			{
			case 1:
			{
				fscanf(filein1, "%d %d %d %d", &a, &b, &c, &d);
				Error = 0.5*(sqrt(pow((PP1.x - a), 2) + pow((PP1.y - b), 2)) +
					sqrt(pow((PP2.x - c), 2) + pow((PP2.y - d), 2)));
				printf("frame%d error = %d\n", frameCount, Error);
				fprintf(fileout1, "%d\n", Error);
				break;
			}
			case 2:
			{
				fscanf(filein2, "%d %d %d %d", &a, &b, &c, &d);
				Error = 0.5*(sqrt(pow((PP1.x - a), 2) + pow((PP1.y - b), 2)) +
					sqrt(pow((PP2.x - c), 2) + pow((PP2.y - d), 2)));
				printf("frame%d error = %d\n", frameCount, Error);
				fprintf(fileout2, "%d\n", Error);
				break;
			}
			case 3:
			{
				fscanf(filein3, "%d %d %d %d", &a, &b, &c, &d);
				Error = 0.5*(sqrt(pow((PP1.x - a), 2) + pow((PP1.y - b), 2)) +
					sqrt(pow((PP2.x - c), 2) + pow((PP2.y - d), 2)));
				printf("frame%d error = %d\n", frameCount, Error);
				fprintf(fileout3, "%d\n", Error);
				break;
			}
			case 4:
			{
				fscanf(filein4, "%d %d %d %d", &a, &b, &c, &d);
				Error = 0.5*(sqrt(pow((PP1.x - a), 2) + pow((PP1.y - b), 2)) +
					sqrt(pow((PP2.x - c), 2) + pow((PP2.y - d), 2)));
				printf("frame%d error = %d\n", frameCount, Error);
				fprintf(fileout4, "%d\n", Error);
				break;
			}
			case 5:
			{
				fscanf(filein5, "%d %d %d %d", &a, &b, &c, &d);
				Error = 0.5*(sqrt(pow((PP1.x - a), 2) + pow((PP1.y - b), 2)) +
					sqrt(pow((PP2.x - c), 2) + pow((PP2.y - d), 2)));
				printf("frame%d error = %d\n", frameCount, Error);
				fprintf(fileout5, "%d\n", Error);
				break;
			}
			}
			//if (getchar() != ' ')//Press space to pause
			//{
			//	playVideo = !playVideo;
			//}

			//execution time
			t1 = clock();
			execution_time << "execution time: " << (t1 - t0) << "ms";//execution time
			putText(videoFrame, execution_time.str(), Point(videoFrame.cols - 500, videoFrame.rows - 50), 0, 1, Scalar(0, 0, 255), 3);
			execution_time.str("");//reset
			if (waitKey(0) == 27)//// Press  ESC on keyboard to exit
			{
				break;
			}
			imshow("video demo", videoFrame);
			imshow("video mor", morImg);
			/*printf("%f\n", t1 - t0);*/
			writer.write(videoFrame);//¿é¥X¼v¤ù
			time_total = time_total + (t1 - t0);
		}
		system("pause");
		fclose(fileout1); fclose(filein1);
		fclose(fileout2); fclose(filein2);
		fclose(fileout3); fclose(filein3);
		fclose(fileout4); fclose(filein4);
		fclose(fileout5); fclose(filein5);
		
		//Average execution time
		time_average = time_total / frameCount;
		printf("time_average = %f ms\n", time_average);
		system("pause");

		// When everything done, release the video capture object
		video.release();

		// Closes all the frames
		destroyAllWindows();

		return 0;
	}
Mat morph(Mat frame)
{
	Mat gray, gray2, diff;
	Mat Struct;
	cvtColor(frame, gray, CV_RGB2GRAY);
	/*medianBlur(gray, gray2, 7);*/
	threshold(gray, gray2, 190, 255, THRESH_BINARY);
	Struct = getStructuringElement(MORPH_RECT, Size(3, 5));
	morphologyEx(gray2, gray2, MORPH_OPEN, Struct);
	/*Struct = getStructuringElement(MORPH_RECT, Size(1, 15));
	morphologyEx(gray2, gray2, MORPH_CLOSE, Struct);*/
	/*Canny(gray2, gray2, 50, 150);*/

	return gray2;
}
void calcLines(const Mat &input, std::vector<Vec2f> &lines)
{
	lines.clear();   
	HoughLines(input, lines, 1, CV_PI / 360, 120);
}
void drawLines(Mat &input, const std::vector<Vec2f> &lines, Point &point1, Point &point2)
{
	int error = INT_MAX;
	float degree;
	for (int i = 0; i < lines.size(); i++)
	{
		float r = lines[i][0];
		float theta = lines[i][1];

		Point pt1(r / cos(theta), 0);
		Point pt2((r - input.rows*sin(theta)) / cos(theta), input.rows);

		double D1 = sqrt(pow(pt1.x - point1.x, 2) + pow(pt1.y - point1.y, 2));
		double D2 = sqrt(pow(pt2.x - point2.x, 2) + pow(pt2.y - point2.y, 2));
		if ((int)D1 + D2 < error)
		{
			error = D1 + D2;
			p1 = pt1;
			p2 = pt2;
			degree = theta;
		}
	}
	if (degree<20*CV_PI / 180.0 || degree>160 * CV_PI / 180.0)
		line(input, p1, p2, Scalar(255, 0, 0), 5);
	else
	{
		line(input, point1, point2, Scalar(255, 0, 0), 5);
		p1 = point1;
		p2 = point2;
	}
		
}

void onMouse(int Event, int x, int y, int flags, void* param)
{
	
	bool flag = false;
	if (Event == CV_EVENT_LBUTTONDOWN)
	{
		pp1.x = x;
		pp1.y = y;
	}
	if (Event == CV_EVENT_LBUTTONUP)
	{
		pp2.x = x;
		pp2.y = y;
		flag = true;
	} 
	if (flag)
	{
		P1.x = pp1.x + (0 - pp1.y)*(pp1.x - pp2.x) / (pp1.y - pp2.y);
		P1.y = 0;
		P2.x = pp1.x + (480 - pp1.y)*(pp1.x - pp2.x) / (pp1.y - pp2.y);
		P2.y = 480;
	}
}