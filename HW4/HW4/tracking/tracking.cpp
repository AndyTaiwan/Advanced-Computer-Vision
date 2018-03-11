#include <cstdio>
#include <opencv2/opencv.hpp>
#include <iostream>
#include<time.h>

using namespace std;
using namespace cv;

int main(){

	stringstream execution_time;
	double t0, t1;
	
	VideoCapture video("1.avi");
	if (!video.isOpened()){
		return -1;
	}

	Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	VideoWriter writer;
	writer.open("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);

	Mat videoFrame;

	while (true)
	{
		/*-------------------------------------------------------*/
		t0 = clock();//execution time
		video >> videoFrame;
		if (videoFrame.empty()){
			break;
		}
		if (waitKey(33) == 27)//waitKey(33)讓輸出影像每29ms取一個frame，使輸出與輸入時間一樣
		{
			break;
		}
		t1 = clock(); //execution time
		/*-------------------------------------------------------*/
		execution_time << (t1 - t0) << "ms";//execution time
		putText(videoFrame, execution_time.str(), Point(videoFrame.cols - 200, videoFrame.rows - 100), 0, 1, Scalar(0, 0, 255), 3);
		execution_time.str("");//歸零
		imshow("video demo", videoFrame);
		writer.write(videoFrame);//輸出影片
	}
	return 0;
}