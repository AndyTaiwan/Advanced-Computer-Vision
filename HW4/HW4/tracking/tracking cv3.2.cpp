#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

using namespace std;
using namespace cv;

int main(){
	// can change to BOOSTING, MIL, KCF (OpenCV 3.1), TLD, MEDIANFLOW, or GOTURN (OpenCV 3.2)
	Ptr<Tracker> tracker = TrackerTLD::create();
	VideoCapture video("3.avi");
	if (!video.isOpened()){
		cerr << "cannot read video!" << endl;
		return -1;
	}
	Mat frame;
	video.read(frame);
	// Define initial boundibg box
	Rect2d bbox(287, 23, 86, 320);
	// Uncomment the line below to select a different bounding box
	bbox = selectROI(frame, true);
	// Display bounding box.
	rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	tracker->init(frame, bbox);
	while (video.read(frame)){
		tracker->update(frame, bbox);
		rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
		imshow("Tracking", frame);
		int k = waitKey(1);
		if (k == 27) break;
	}
}