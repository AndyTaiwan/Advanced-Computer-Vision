#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;

//計算原始圖像點位在經過矩陣變換後在目標圖像上對應位置  
Point2f getTransformPoint(const  Point2f originalPoint, const  Mat &transformMaxtri);

int main()
{
	FILE *filein;
	FILE *fileout;
	vector<KeyPoint>keypoint_left, keypoint_right;
	Mat left;
	Mat right;
	Mat Descriptor_left, Descriptor_right;
	
	
	/*************************************讀檔*************************************/
	left = imread("left.bmp");
	right = imread("right.bmp");

	// Step 1: 提取特徵點 (SURF)
	int minHessian = 400;

	SurfFeatureDetector surfDetector(minHessian);

	std::vector<KeyPoint> keypoints_left, keypoints_right;

	surfDetector.detect(left, keypoints_left);
	surfDetector.detect(right, keypoints_right);
	
	// Step 2: 特徵點描述，為之後的特徵點匹配做準備
	SurfDescriptorExtractor surfExtractor;

	Mat descriptors_left, descriptors_right;

	surfExtractor.compute(left, keypoints_left, descriptors_left);
	surfExtractor.compute(right, keypoints_right, descriptors_right);

	// Step 3: 獲得匹配特徵點，並提取最優配對
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_left, descriptors_right, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	//for (int i = 0; i < descriptors_left.rows; i++)
	//{
	//	double dist = matches[i].distance;
	//	if (dist < min_dist) min_dist = dist;
	//	if (dist > max_dist) max_dist = dist;
	//}

	//printf("-- Max dist : %f \n", max_dist);
	//printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_left.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(left, keypoints_left, right, keypoints_right,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_left[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_right[good_matches[i].trainIdx].pt);
	}
	/*************************************************************************************************************/
	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(left.cols, 0);
	obj_corners[2] = cvPoint(left.cols, left.rows); obj_corners[3] = cvPoint(0, left.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(left.cols, 0), scene_corners[1] + Point2f(left.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(left.cols, 0), scene_corners[2] + Point2f(left.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(left.cols, 0), scene_corners[3] + Point2f(left.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(left.cols, 0), scene_corners[0] + Point2f(left.cols, 0), Scalar(0, 255, 0), 4);

	
	//-- Draw keypoints
	Mat img_keypoints_left; Mat img_keypoints_right;

	drawKeypoints(left, keypoints_left, img_keypoints_left, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(right, keypoints_right, img_keypoints_right, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	/**********************************顯示影像***********************************/

	//-- Show detected (drawn) keypoints
	imshow("keypoints_left", img_keypoints_left);
	imshow("img_keypoints_right", img_keypoints_right);


	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	/*waitKey();*/
	/************************************寫檔************************************/
	imwrite("keypoints_left.bmp", img_keypoints_left);
	imwrite("img_keypoints_right.bmp", img_keypoints_right);
	imwrite("Corresponding_Pairs & Warping.bmp", img_matches);
	/*************************************************************************************************************/
	//獲取圖像1到圖像2的投影映射矩陣，尺寸為3*3  
	Mat homo = findHomography(obj, scene, CV_RANSAC);
	Mat adjustMat = (Mat_< double >(3, 3) << 1.0, 0, left.cols, 0, 1.0, 0, 0, 0, 1.0);
	Mat adjustHomo = adjustMat*homo;
	//獲取最佳配對點在原始圖像和矩陣變換後圖像上的對應位置，用於圖像拼接點的定位  
	Point2f originalLinkPoint, targetLinkPoint, basedImagePoint;
	originalLinkPoint = keypoints_left[matches[0].queryIdx].pt;
	targetLinkPoint = getTransformPoint(originalLinkPoint, adjustHomo);
	basedImagePoint = keypoints_right[matches[0].trainIdx].pt;

	//圖像配準  
	Mat imageTransform1;
	warpPerspective(left, imageTransform1, adjustMat*homo, Size(right.cols + left.cols + 110, right.rows));

	//在最佳匹配點左側的重疊區域進行累加，是銜接穩定過渡，消除突變  
	Mat image1Overlap, image2Overlap;  //圖1和圖2的重疊部分     
	image1Overlap = imageTransform1(Rect(Point(targetLinkPoint.x - basedImagePoint.x, 0), Point(targetLinkPoint.x, right.rows)));
	image2Overlap = right(Rect(0, 0, image1Overlap.cols, image1Overlap.rows));
	Mat image1ROICopy = image1Overlap.clone();   //複製一份圖1的重疊部分  
	for (int i = 0; i<image1Overlap.rows; i++)
	{
		for (int j = 0; j<image1Overlap.cols; j++)
		{
			double  weight;
			weight = (double)j / image1Overlap.cols;   //隨距離改變而改變的疊加係數  
			image1Overlap.at<Vec3b>(i, j)[0] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[0] + weight*image2Overlap.at<Vec3b>(i, j)[0];
			image1Overlap.at<Vec3b>(i, j)[1] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[1] + weight*image2Overlap.at<Vec3b>(i, j)[1];
			image1Overlap.at<Vec3b>(i, j)[2] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[2] + weight*image2Overlap.at<Vec3b>(i, j)[2];
		}
	}
	Mat ROIMat = right(Rect(Point(image1Overlap.cols, 0), Point(right.cols, right.rows)));   //圖2中不重合的部分  
	ROIMat.copyTo(Mat(imageTransform1, Rect(targetLinkPoint.x, 0, ROIMat.cols, right.rows)));  //不重合的部分直接銜接上去  
	imshow("Images Stitching", imageTransform1);
	imwrite("Images Stitching.bmp", imageTransform1);
	waitKey(); 
	return  0;
}

//計算原始圖像點位在經過矩陣變換後在目標圖像上對應位置  
Point2f getTransformPoint(const  Point2f originalPoint, const  Mat &transformMaxtri)
{
	Mat originelP, targetP;
	originelP = (Mat_< double >(3, 1) << originalPoint.x, originalPoint.y, 1.0);
	targetP = transformMaxtri*originelP;
	float  x = targetP.at< double >(0, 0) / targetP.at< double >(2, 0);
	float  y = targetP.at< double >(1, 0) / targetP.at< double >(2, 0);
	return  Point2f(x, y);
}

