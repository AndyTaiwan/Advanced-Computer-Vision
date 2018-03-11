#include <cstdio>
#include <opencv2/opencv.hpp>
//#include <highgui.h>
//#include <cv.h>
//#include <stdio.h>
using namespace cv;
void main()
{
	IplImage*  capimg = NULL;
	CvCapture* capture = NULL;
	int loop = 1;
	int file_idx = 0;
	int key;

	char filename[256];
	char fileidx[10];

	//img = cvLoadImage( img_file_name, 1 );
	/*capture = cvCaptureFromCAM(0);*/
	VideoCapture capture("1.avi");
	if (!capture)
	{
		fprintf(stderr, "unable to create camera 0 \n");
		exit(1);
	}

	cvNamedWindow("camera0", 1);

	while (loop)
	{
		capimg = cvQueryFrame(capture);
		cvShowImage("camera0", capimg);

		key = cvWaitKey(5);
		if (key == 27)
		{
			loop = 0;
		}
		else if (key == 's' || key == 'S')
		{
			sprintf(fileidx, "%03d", file_idx++);
			strcpy(filename, "SAVE_IMG");
			strcat(filename, fileidx);
			strcat(filename, ".bmp");

			cvSaveImage(filename, capimg);
			printf("Image file %s saved.\n", filename);
		}
	}

	cvDestroyWindow("camera0");
	cvReleaseCapture(&capture);
}