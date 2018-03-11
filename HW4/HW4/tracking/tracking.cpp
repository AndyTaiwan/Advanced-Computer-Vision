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
		if (waitKey(33) == 27)//waitKey(33)����X�v���C29ms���@��frame�A�Ͽ�X�P��J�ɶ��@��
		{
			break;
		}
		t1 = clock(); //execution time
		/*-------------------------------------------------------*/
		execution_time << (t1 - t0) << "ms";//execution time
		putText(videoFrame, execution_time.str(), Point(videoFrame.cols - 200, videoFrame.rows - 100), 0, 1, Scalar(0, 0, 255), 3);
		execution_time.str("");//�k�s
		imshow("video demo", videoFrame);
		writer.write(videoFrame);//��X�v��
	}
	return 0;
}