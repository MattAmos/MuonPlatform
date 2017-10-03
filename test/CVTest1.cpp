#include <iostream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
 
int main (int argc, const char * argv[])
{
    VideoCapture cap(0);
    if (!cap.isOpened()){
        std::cout << "Unable to open video capture. Exiting..." << std::endl;
        return -1;
    }
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
 
    Mat img, img_gray;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 
    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
    while (true)
    {
        cap >> img;
        if (!img.data){
            std::cout << "No image data. Continuing..." << std::endl;
            continue;
        }
        
        vector<Rect> found, found_filtered;
        cvtColor(img, img_gray, CV_BGR2GRAY);
        equalizeHist(img_gray, img_gray);
        hog.detectMultiScale(img_gray, found, 0, Size(8,8), Size(32,32), 1.05, 2);

        size_t i, j;
        for (i=0; i<found.size(); i++){
            Rect r = found[i];
            for (j=0; j<found.size(); j++){
                if (j!=i && (r & found[j]) == r){
                    std::cout << "Found something." << std::endl;
                    break;
                }
            }
            if (j==found.size()){
                found_filtered.push_back(r);
            }
        }
        for (i=0; i<found_filtered.size(); i++){
            Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.06);
            r.height = cvRound(r.height*0.9);
            rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        }
        imshow("video capture", img);
        int k = waitKey(1) & 0xFF;
        if (k == 27){
            std::cout << "Exiting..." << std::endl;
            break;
        }
    }
    return 0;
}