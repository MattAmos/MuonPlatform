
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <unistd.h>
#include <raspicam/raspicam_cv.h>
#include <sstream>
#include <string> 
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;
 
int main (int argc, const char * argv[])
{
	int nb_img = 0;
    string name = "";
    string line = "";
    string face_cascade_name = "/home/pi/MuonPlatform/test/haarcascade_frontalface_alt.xml";
    CascadeClassifier face_cascade;

    if(!face_cascade.load(face_cascade_name)){ std::cout << "Error loading face cascade" << std::endl; return -1; }

    Mat img = imread("/dev/shm/mjpeg/cam.jpg"), img_gray;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    VideoWriter outputVideo("testVid.avi", CV_FOURCC('M', 'J', 'P', 'G'), 2, Size(img.size().width, img.size().height));
    while (true){
		img = imread("/dev/shm/mjpeg/cam.jpg");
        if (!img.data){
            std::cout << "No image data. Continuing..." << std::endl;
            break;
        }
        auto start = std::chrono::system_clock::now();
        vector<Rect> found, found_filtered, faces;
        cvtColor(img, img_gray, CV_BGR2GRAY);
        //equalizeHist(img_gray, img_gray);
        hog.detectMultiScale(img_gray, found, 0,   Size(4, 4), Size(0,0), 1.08, 3);
        face_cascade.detectMultiScale(img_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));

        size_t i, j;
/*       
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
*/	
        for (i=0; i<found.size(); i++){
            Rect r = found[i];
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.06);
            r.height = cvRound(r.height*0.9);
            rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        }

        for(size_t k = 0; k < faces.size(); k++){
        	Point centre(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
        	ellipse(img, centre, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
        }

		outputVideo.write(img);
		if(found.size() > 0 || faces.size() > 0){
			std::cout << "Found something" << std::endl;
			nb_img++;
			/*stringstream ss("/var/www/bootstrap/");
			ss << "test";
			ss << i;
			ss << ".jpg";
			ss >> name;*/
			name = "/var/www/bootstrap/test" + std::to_string(nb_img) + ".jpg";			
			// place img in /var/www/bootstrap
			std::cout << name << std::endl;
			imwrite(name,img);
			name.erase(0,19);
			// add line 36 code for img
			ofstream file("/var/www/bootstrap/main.php",ios::app );
			if(file)  // if opened
			{
			//////////////////
			// place line36 //
			//////////////////
			file.seekp(1635,ios::beg);
			file << " <div class=\"carousel-item\">" << endl;
			file << "<img class=\"d-block w-100\" src=\"" << name  << "\">" << endl;
			file << "</div>" << endl;

			// instruction
			file.close();  // je referme le fichier		
			}
			//write in mode.txt "ON" and erase everything else
			ofstream f("/var/www/bootstrap/mode.txt", ios::out | ios::trunc);
			if(f){  // if opened			
				f << "ON" <<endl;
				// instructions
				f.close();  // je referme le fichier		
			}
			if(nb_img==5)
			{nb_img=0;}
			//sleep(5000);
		}		
		else
		{
			// write in mode.txt "OFF" and erase everything else
			ofstream f("/var/www/bootstrap/mode.txt", ios::out | ios::trunc);
			if(f){  // if opened
				f << "OFF" <<endl;
				// instructions
				f.close();  // je referme le fichier		
			}
		}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Time elapsed: " << elapsed.count() << "s\n" << std::endl;
    }
    outputVideo.release();
	return 0;
}
