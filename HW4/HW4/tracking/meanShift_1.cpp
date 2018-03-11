/*
meanShift算法用於視頻目標跟踪時，採用目標的顏色直方圖作為搜索特徵，通過不斷迭代meanShift向量使得算法收斂於目標的真實位置，從而達到跟踪的目的。傳統的meanShift算法在跟踪中有幾個優勢：
（1）算法計算量不大，在目標區域已知的情況下完全可以做到實時跟踪；
（2）採用核函數直方圖模型，對邊緣遮擋、目標旋轉、變形和背景運動不敏感。
同時，meanShift算法也存在著以下一些缺點：
（1）缺乏必要的模板更新；
（2）跟踪過程中由於窗口寬度大小保持不變，當目標尺度有所變化時，跟踪就會失敗；
（3）當目標速度較快時，跟踪效果不好；
（4）直方圖特徵在目標顏色特徵描述方面略顯匱乏，缺少空間信息；
由於其計算速度快，對目標變形和遮擋有一定的魯棒性，所以，在目標跟踪領域，meanShift算法目前依然受到大家的重視。但考慮到其缺點，在工程實際中也可以對其作出一些改進和調整；例如：
（1）引入一定的目標位置變化的預測機制，從而更進一步減少meanShift跟踪的搜索時間，降低計算量；
（ 2）可以採用一定的方式來增加用於目標匹配的“特徵”；
（3）將傳統meanShift算法中的核函數固定帶寬改為動態變化的帶寬；
（4）採用一定的方式對整體模板進行學習和更新；
*/
#include "core/core.hpp"      
#include "highgui/highgui.hpp"      
#include "imgproc/imgproc.hpp"  
#include "video/tracking.hpp"  
#include<iostream>      

using namespace  cv;
using namespace  std;

Mat image;
Mat rectImage;
Mat imageCopy;  //繪製矩形框時用來拷貝原圖的圖像    
bool  leftButtonDownFlag = false;  //左鍵單擊後視頻暫停播放的標誌位    
Point originalPoint;  //矩形框起點    
Point processPoint;  //矩形框終點    

Mat targetImageHSV;
int  histSize = 200;
float  histR[] = { 0, 255 };
const float  *histRange = histR;
int  channels[] = { 0, 1 };
Mat dstHist;
Rect rect;
vector<Point> pt;  //保存目標軌跡  
void  onMouse(int  event, int  x, int  y, int  flags, void * ustc);  //鼠標回調函數    

int  main()
{
	VideoCapture video("2.avi");
	double  fps = video.get(CV_CAP_PROP_FPS);  //獲取視頻幀率    
	double  pauseTime = 1000 / fps;  //兩幅畫面中間間隔    
	namedWindow("Tracking", 0);
	setMouseCallback("Tracking", onMouse);
	while (true)
	{
		if (!leftButtonDownFlag)  //判定鼠標左鍵沒有按下，採取播放視頻，否則暫停    
		{
			video >> image;
		}
		if (!image.data || waitKey(pauseTime) == 27)   //圖像為空或Esc鍵按下退出播放    
		{
			break;
		}
		if (originalPoint != processPoint&&!leftButtonDownFlag)
		{
			Mat imageHSV;
			Mat calcBackImage;
			cvtColor(image, imageHSV, CV_RGB2HSV);
			calcBackProject(&imageHSV, 2, channels, dstHist, calcBackImage, &histRange);   //反向投影  
			TermCriteria criteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.001);
			meanShift(calcBackImage, rect, criteria);
			Mat imageROI = imageHSV(rect);    //更新模板             
			targetImageHSV = imageHSV(rect);
			calcHist(&imageROI, 2, channels, Mat(), dstHist, 1, &histSize, &histRange);
			normalize(dstHist, dstHist, 0.0, 1.0, NORM_MINMAX);    //歸一化  
			rectangle(image, rect, Scalar(255, 0, 0), 3);     //目標繪製    
			pt.push_back(Point(rect.x + rect.width / 2, rect.y + rect.height / 2));
			for (int i = 0; i<pt.size() - 1; i++)
			{
				line(image, pt[i], pt[i + 1], Scalar(0, 255, 0), 2.5);
			}
		}
		imshow("Tracking", image);
		waitKey(100);
	}
	return  0;
}

//************************************************ *******************//      
//鼠標回調函數      
void  onMouse(int  event, int  x, int  y, int  flags, void  *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		leftButtonDownFlag = true;  //標誌位    
		originalPoint = Point(x, y);   //設置左鍵按下點的矩形起點    
		processPoint = originalPoint;
	}
	if (event == CV_EVENT_MOUSEMOVE&&leftButtonDownFlag)
	{
		imageCopy = image.clone();
		processPoint = Point(x, y);
		if (originalPoint != processPoint)
		{
			//在復制的圖像上繪製矩形    
			rectangle(imageCopy, originalPoint, processPoint, Scalar(255, 0, 0), 2);
		}
		imshow("Tracking", imageCopy);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		leftButtonDownFlag = false;
		rect = Rect(originalPoint, processPoint);
		rectImage = image(rect);  //子圖像顯示    
		imshow("Sub Image", rectImage);
		cvtColor(rectImage, targetImageHSV, CV_RGB2HSV);
		imshow("targetImageHSV", targetImageHSV);
		calcHist(&targetImageHSV, 2, channels, Mat(), dstHist, 1, &histSize, &histRange, true, false);
		normalize(dstHist, dstHist, 0, 255, CV_MINMAX);
		imshow("dstHist", dstHist);
	}
}