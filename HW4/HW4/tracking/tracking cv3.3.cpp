#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include<time.h>
#include "core/core.hpp"      
#include "highgui/highgui.hpp"      
#include "imgproc/imgproc.hpp"  
#include "video/tracking.hpp"  
#include<iostream> 

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
void Meanshift(char *in);
void cvTracker(char *in);
void onMouse(int  event, int  x, int  y, int  flags, void * ustc);  //���Ц^�ը�� 

Mat image;
Mat rectImage;
Mat imageCopy;  //ø�s�x�ήخɥΨӫ�����Ϫ��Ϲ�    
bool  leftButtonDownFlag = false;  //�����������W�Ȱ����񪺼лx��    
Point originalPoint;  //�x�ήذ_�I    
Point processPoint;  //�x�ήز��I    

Mat targetImageHSV;
int  histSize = 200;
float  histR[] = { 0, 255 };
const float  *histRange = histR;
int  channels[] = { 0, 1 };
Mat dstHist;
Rect rect;
vector<Point> pt;  //�O�s�ؼЭy��  

int main()
{
//	stringstream execution_time;
//	double t0, t1;
//	double time_average;
//	double time_total = 0;
//	double frameCount = 0;
//	char filename[256];
//	char fileidx[10];
//	
//	// List of tracker types in OpenCV 3.2
//	// NOTE : GOTURN implementation is buggy and does not work.
//	string trackerTypes[6] = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN" };
//	// vector <string> trackerTypes(types, std::end(types));
//
//	// Create a tracker
//	string trackerType = trackerTypes[3];
//
//	Ptr<Tracker> tracker;
//
////#if (CV_MINOR_VERSION < 3)
////	{
////		tracker = Tracker::create(trackerType);
////	}
////#else
////	{
//		if (trackerType == "BOOSTING")
//			tracker = TrackerBoosting::create();
//		if (trackerType == "MIL")
//			tracker = TrackerMIL::create();
//		if (trackerType == "KCF")
//			tracker = TrackerKCF::create();
//		if (trackerType == "TLD")
//			tracker = TrackerTLD::create();
//		if (trackerType == "MEDIANFLOW")
//			tracker = TrackerMedianFlow::create();
//		if (trackerType == "GOTURN")
//			tracker = TrackerGOTURN::create();
//	}
//#endif
	// Read video
		int select;
		string file[3] = { "1.avi", "2.avi", "3.avi" };
		printf(" Press 1 : 1.avi \n");
		printf(" Press 2 : 2.avi \n");
		printf(" Press 3 : 3.avi \n");
		printf(" Please press the number: ");
		scanf("%d", &select);
		switch (select)
		{
		case 1:
		{cvTracker("1.avi"); break; }
		case 2:
		{Meanshift("2.avi"); break; }
		case 3:
		{cvTracker("3.avi"); break; }
		}
	//	VideoCapture video(file);

	//// Exit if video is not opened
	//if (!video.isOpened())
	//{
	//	cout << "Could not read video file" << endl;
	//	return 1;

	//}

	//// Read first frame
	//Mat frame;
	//bool ok = video.read(frame);

	//// Define initial boundibg box
	//Rect2d bbox(287, 23, 86, 320);

	//// Uncomment the line below to select a different bounding box
	//bbox = selectROI(frame, true);

	//// Display bounding box.
	//rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	//imshow("Tracking", frame);

	//tracker->init(frame, bbox);

	//while (video.read(frame))
	//{
	//	frameCount ++;
	//	/*-------------------------------------------------------*/
	//	t0 = clock();//execution time
	//	// Start timer
	//	double timer = (double)getTickCount();

	//	// Update the tracking result
	//	bool ok = tracker->update(frame, bbox);

	//	// Calculate Frames per second (FPS)
	//	float fps = getTickFrequency() / ((double)getTickCount() - timer);

	//	if (ok)
	//	{
	//		// Tracking success : Draw the tracked object
	//		rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	//	}
	//	else
	//	{
	//		// Tracking failure detected.
	//		putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
	//	}

	//	//execution time
	//	t1 = clock(); 
	//	/*-------------------------------------------------------*/
	//	execution_time << "execution time: " << (t1 - t0) << "ms";//execution time
	//	putText(frame, execution_time.str(), Point(frame.cols - 500, frame.rows - 50), 0, 1, Scalar(0, 0, 255), 3);
	//	execution_time.str("");//reset

	//	// Display tracker type on frame
	//	putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

	//	// Display FPS on frame
	//	putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
	//	
	//	// Display frame.
	//	imshow("Tracking", frame);

	//	time_total = time_total + (t1 - t0);
	//	/*--------------------------------------save image------------------------------------*/
	//	if ((int)frameCount % 30 == 0)
	//	{
	//		Mat cap;
	//		cap = frame.clone();
	//		if (cap.empty())
	//			break;

	//		sprintf(filename, "SAVE_IMG_Frame%.4d.jpg", (int)frameCount);
	//		imwrite(filename, cap);
	//		cout << "save: " << filename << endl;
	//	}
	//	/*------------------------------------------------------------------------------------*/
	//	// Exit if ESC pressed.
	//	int k = waitKey(30);
	//	if (k == 27)
	//	{
	//		break;
	//	}
	//	
	//	
	//}
	////Average execution time
	//time_average = time_total / frameCount;
	//printf("time_average = %f ms\n", time_average);
	//system("pause");
}

void  Meanshift(char *in)
{ 
	stringstream execution_time;
	double t0, t1;
	double time_average;
	double time_total = 0;
	double frameCount = 0;
	char filename[256];
	char fileidx[10];
	VideoCapture video(in);
	double  fps = video.get(CV_CAP_PROP_FPS);  //������W�V�v    
	double  pauseTime = 1000 / fps;  //��T�e���������j    
	namedWindow("Tracking", 0);
	setMouseCallback("Tracking", onMouse);

	Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;
	writer.open("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);
	while (true)
	{
		frameCount++;
		/*-------------------------------------------------------*/
		t0 = clock();//execution time
		if (!leftButtonDownFlag)  //�P�w���Х���S�����U�A�Ĩ�������W�A�_�h�Ȱ�    
		{
			video >> image;
		}
		if (!image.data || waitKey(pauseTime) == 27)   //�Ϲ����ũ�Esc����U�h�X����    
		{
			break;
		}
		if (originalPoint != processPoint&&!leftButtonDownFlag)
		{
			Mat imageHSV;
			Mat calcBackImage;
			cvtColor(image, imageHSV, CV_RGB2HSV);
			calcBackProject(&imageHSV, 2, channels, dstHist, calcBackImage, &histRange);   //�ϦV��v  
			TermCriteria criteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.001);
			meanShift(calcBackImage, rect, criteria);
			Mat imageROI = imageHSV(rect);    //��s�ҪO             
			targetImageHSV = imageHSV(rect);
			calcHist(&imageROI, 2, channels, Mat(), dstHist, 1, &histSize, &histRange);
			normalize(dstHist, dstHist, 0.0, 1.0, NORM_MINMAX);    //�k�@��  
			rectangle(image, rect, Scalar(0, 0, 255), 3);     //�ؼ�ø�s    
			pt.push_back(Point(rect.x + rect.width / 2, rect.y + rect.height / 2));
			/*for (int i = 0; i<pt.size() - 1; i++)
			{
				line(image, pt[i], pt[i + 1], Scalar(0, 255, 0), 2.5);
			}*/
		}
		//execution time
		t1 = clock();
		/*-------------------------------------------------------*/
		execution_time << "execution time: " << (t1 - t0) << "ms";//execution time
		putText(image, execution_time.str(), Point(image.cols - 500, image.rows - 50), 0, 1, Scalar(0, 0, 255), 3);
		execution_time.str("");//reset
		time_total = time_total + (t1 - t0);
		/*--------------------------------------save image------------------------------------*/
		if ((int)frameCount % 30 == 0)
		{
			Mat cap;
			cap = image.clone();
			if (cap.empty())
				break;

			sprintf(filename, "SAVE_IMG_Frame%.4d.jpg", (int)frameCount);
			imwrite(filename, cap);
			cout << "save: " << filename << endl;
		}
		/*------------------------------------------------------------------------------------*/
		writer.write(image);//��X�v��
		imshow("Tracking", image);
		waitKey(1);
	}
	//Average execution time
	time_average = time_total / frameCount;
	printf("time_average = %f ms\n", time_average);
	system("pause");
	/*return  0;*/
}

//���Ц^�ը��      
void  onMouse(int  event, int  x, int  y, int  flags, void  *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		leftButtonDownFlag = true;  //�лx��    
		originalPoint = Point(x, y);   //�]�m������U�I���x�ΰ_�I    
		processPoint = originalPoint;
	}
	if (event == CV_EVENT_MOUSEMOVE&&leftButtonDownFlag)
	{
		imageCopy = image.clone();
		processPoint = Point(x, y);
		if (originalPoint != processPoint)
		{
			//�b�_��Ϲ��Wø�s�x��    
			rectangle(imageCopy, originalPoint, processPoint, Scalar(255, 0, 0), 2);
		}
		imshow("Tracking", imageCopy);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		leftButtonDownFlag = false;
		rect = Rect(originalPoint, processPoint);
		rectImage = image(rect);  //�l�Ϲ����    
  		imshow("Sub Image", rectImage);
		cvtColor(rectImage, targetImageHSV, CV_RGB2HSV);
		imshow("targetImageHSV", targetImageHSV);
		calcHist(&targetImageHSV, 2, channels, Mat(), dstHist, 1, &histSize, &histRange, true, false);
		normalize(dstHist, dstHist, 0, 255, CV_MINMAX);
		imshow("dstHist", dstHist);
	}
}
void cvTracker(char *in)
{
	stringstream execution_time;
	double t0, t1;
	double time_average;
	double time_total = 0;
	double frameCount = 0;
	char filename[256];
	char fileidx[10];

	// List of tracker types in OpenCV 3.2
	// NOTE : GOTURN implementation is buggy and does not work.
	string trackerTypes[6] = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN" };
	// vector <string> trackerTypes(types, std::end(types));
	string trackerType;
	if (in=="1.avi")
		// Create a tracker
		trackerType = trackerTypes[4];
	else if(in == "3.avi")
		// Create a tracker
		trackerType = trackerTypes[3];

	Ptr<Tracker> tracker;

	//#if (CV_MINOR_VERSION < 3)
	//	{
	//		tracker = Tracker::create(trackerType);
	//	}
	//#else
	//	{
	if (trackerType == "BOOSTING")
		tracker = TrackerBoosting::create();
	if (trackerType == "MIL")
		tracker = TrackerMIL::create();
	if (trackerType == "KCF")
		tracker = TrackerKCF::create();
	if (trackerType == "TLD")
		tracker = TrackerTLD::create();
	if (trackerType == "MEDIANFLOW")
		tracker = TrackerMedianFlow::create();
	if (trackerType == "GOTURN")
		tracker = TrackerGOTURN::create();
	//	}
	//#endif
	// Read video

	VideoCapture video(in);

	//// Exit if video is not opened
	//if (!video.isOpened())
	//{
	//	cout << "Could not read video file" << endl;
	//	return 1;
	//}

	// Read first frame
	Mat frame;
	bool ok = video.read(frame);

	// Define initial boundibg box
	Rect2d bbox(287, 23, 86, 320);

	// Uncomment the line below to select a different bounding box
	bbox = selectROI(frame, false);

	// Display bounding box.
	rectangle(frame, bbox, Scalar(0, 0, 255), 2, 1);
	imshow("Tracking", frame);

	tracker->init(frame, bbox);

	Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;
	writer.open("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);
	while (video.read(frame))
	{
		frameCount++;
		/*-------------------------------------------------------*/
		t0 = clock();//execution time
		// Start timer
		double timer = (double)getTickCount();

		// Update the tracking result
		bool ok = tracker->update(frame, bbox);

		// Calculate Frames per second (FPS)
		float fps = getTickFrequency() / ((double)getTickCount() - timer);

		if (ok)
		{
			// Tracking success : Draw the tracked object
			rectangle(frame, bbox, Scalar(0, 0, 255), 2, 1);
		}
		else
		{
			// Tracking failure detected.
			putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}

		//execution time
		t1 = clock();
		/*-------------------------------------------------------*/
		execution_time << "execution time: " << (t1 - t0) << "ms";//execution time
		putText(frame, execution_time.str(), Point(frame.cols - 500, frame.rows - 50), 0, 1, Scalar(0, 0, 255), 3);
		execution_time.str("");//reset

		// Display tracker type on frame
		putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display FPS on frame
		putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);


		// Display frame.
		imshow("Tracking", frame);
		writer.write(frame);//��X�v��
		time_total = time_total + (t1 - t0);
		/*--------------------------------------save image------------------------------------*/
		if ((int)frameCount % 30 == 0)
		{
			Mat cap;
			cap = frame.clone();
			if (cap.empty())
				break;

			sprintf(filename, "SAVE_IMG_Frame%.4d.jpg", (int)frameCount);
			imwrite(filename, cap);
			cout << "save: " << filename << endl;
		}
		/*------------------------------------------------------------------------------------*/
		// Exit if ESC pressed.
		int k = waitKey(30);
		if (k == 27)
		{
			break;
		}


	}
	//Average execution time
	time_average = time_total / frameCount;
	printf("time_average = %f ms\n", time_average);
	system("pause");
}