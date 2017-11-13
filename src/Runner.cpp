#include "Runner.h"

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    sensors.destroy();
    endwin();
    exit(sigNum);
}

int scp(std::string filename) {
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
}

bool kbhit() {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return true;
    }
    else {
        return false;
    }
}

void* servo_pwm_thread(void* threadid) {
    while (true) {
    	 pthread_mutex_lock(&mutex);
        sensors.servo.setValGPIO("1");
        usleep(sensors.servo.getPwmTime());
        sensors.servo.setValGPIO("0");
        usleep(20000 - sensors.servo.getPwmTime());
         pthread_mutex_unlock(&mutex);
     usleep(1000);
    }
}

void* dc_pwm_thread(void* threadid){
    while(true) {
        sensors.dc_move(DC_FRWD);
        //usleep(10000);
        //sensors.dc_move(DC_STOP);
        //usleep(20000);
    }
}

void* inp_thread(void* threadid) {
    while (true) {
        if (joystick.isFound()) {
            if (joystick.sample(&event) && event.isAxis()) {
                if (event.number == 0) {  // L3 HORIZONTAL
                    sensors.servo.setPwmTime(mid_t + (event.value / 65535.0) * (SERVO_PWM_MAX - SERVO_PWM_MIN));
                }
                else {
                    std::cout << "Controller event: " << event.value << std::endl;
                }
            }
        }
        else {
            if (kbhit()) {
                contFlag = true;
                int ch   = getch();
                switch (ch) {
                    case 'a': sensors.servo.incPwmTime(100); break;
                    case 'd': sensors.servo.incPwmTime(-100); break;
                    case 'A': sensors.servo.setPwmTime(SERVO_PWM_MAX); break;
                    case 'D': sensors.servo.setPwmTime(SERVO_PWM_MIN); break;
                    case 'w': sensors.move = DC_FRWD; break;
                    case 's': sensors.move = DC_BACK; break;
                    case 32: sensors.move  = DC_STOP; break;
                    case 27: contFlag      = false; break;
                }
            }
        }
    }
}

void* move_thread(void* threadid) {
    // Ranger rFinder = Ranger();
    int frontDist, leftDist, rightDist, backDist;
    
    while (true) {
        //if (!kbhit()) {
            // Get sensors
            frontDist = sensors.sonic_front.getCM();
            leftDist  = sensors.sonic_left.getCM();
            rightDist = sensors.sonic_right.getCM();
            backDist  = sensors.sonic_back.getCM();
            
	    pthread_mutex_lock(&mutex);
            //If something is immediately in front of car
            std::cout << "Front: " << frontDist << std::endl;
            if(frontDist > 0 && frontDist < 56)
            {
                while(frontDist < 56 && backDist > 15)
                {  //backup until a turn can be made
                    frontDist = sensors.sonic_front.getCM();
                    backDist  = sensors.sonic_back.getCM();

                    sensors.move = DC_BACK;
                }
           	
      /*          frontDist = sensors.sonic_front.getCM();

                //Finished backing up
                if(frontDist < 56){   //if distance is still too small for a successful simple turn
                    //find which side has most room, go that way
                    //If we have room on the left
                    if(leftDist > rightDist)        { sensors.servo.setPwmTime(SERVO_PWM_MAX); }
                    else if(rightDist > leftDist)   { sensors.servo.setPwmTime(SERVO_PWM_MIN); }
                    else                            { sensors.servo.setPwmTime(SERVO_PWM_MID); }
                }
                sensors.move = DC_FRWD;
            }
            else{
                if(leftDist > rightDist + 5){
                    sensors.servo.setPwmTime(SERVO_PWM_MAX);
                }
                else if(rightDist > leftDist + 5){
                    sensors.servo.setPwmTime(SERVO_PWM_MIN);
                }
                sensors.move = DC_FRWD;
        */    }
            pthread_mutex_unlock(&mutex);
            usleep(1000);
      //  }
    }
}

void* cv_thread(void* threadid) {
    int nb_img                    = 0;
    std::string name              = "";
    std::string line              = "";
    std::string face_cascade_name = "/home/pi/MuonPlatform/test/haarcascade_frontalface_alt.xml";
    cv::CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_name)) {
        std::cout << "Error loading face cascade" << std::endl;
    }

    cv::Mat img = cv::imread("/dev/shm/mjpeg/cam.jpg"), img_gray;
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    while (true) {
        img = cv::imread("/dev/shm/mjpeg/cam.jpg");
        if (!img.data) {
            std::cout << "No image data. Continuing..." << std::endl;
            break;
        }
        auto start = std::chrono::system_clock::now();
        std::vector<cv::Rect> found, found_filtered, faces;
        cv::cvtColor(img, img_gray, CV_BGR2GRAY);
        // equalizeHist(img_gray, img_gray);
        hog.detectMultiScale(img_gray, found, 0, cv::Size(4, 4), cv::Size(0, 0), 1.08, 3);
        face_cascade.detectMultiScale(img_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));

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
        for (i = 0; i < found.size(); i++) {
            cv::Rect r = found[i];
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.06);
            r.height = cvRound(r.height * 0.9);
            cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }

        for (size_t k = 0; k < faces.size(); k++) {
            cv::Point centre(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
            cv::ellipse(img,
                        centre,
                        cv::Size(faces[i].width * 0.5, faces[i].height * 0.5),
                        0,
                        0,
                        360,
                        cv::Scalar(255, 0, 255),
                        4,
                        8,
                        0);
        }

        if (found.size() > 0 || faces.size() > 0) {
            std::cout << "Found " << found.size() << " people, " << faces.size() << " faces." <<  std::endl;
            nb_img++;
            /*stringstream ss("/var/www/bootstrap/");
            ss << "test";
            ss << i;
            ss << ".jpg";
            ss >> name;*/
            name = "/var/www/bootstrap/test" + std::to_string(nb_img) + ".jpg";
            // place img in /var/www/bootstrap
            std::cout << name << std::endl;
            cv::imwrite(name, img);
            name.erase(0, 19);
            // add line 36 code for img
            std::ofstream file("/var/www/bootstrap/main.php", std::ios::app);
            if (file)  // if opened
            {
                //////////////////
                // place line36 //
                //////////////////
                file.seekp(1635, std::ios::beg);
                file << " <div class=\"carousel-item\">" << std::endl;
                file << "<img class=\"d-block w-100\" src=\"" << name << "\">" << std::endl;
                file << "</div>" << std::endl;

                // instruction
                file.close();  // je referme le fichier
            }
            // write in mode.txt "ON" and erase everything else
            std::ofstream f("/var/www/bootstrap/mode.txt", std::ios::out | std::ios::trunc);
            if (f) {  // if opened
                f << "ON" << std::endl;
                // instructions
                f.close();  // je referme le fichier
            }
            if (nb_img == 5) {
                nb_img = 0;
            }
            // sleep(5000);
        }
        else {
            // write in mode.txt "OFF" and erase everything else
            std::ofstream f("/var/www/bootstrap/mode.txt", std::ios::out | std::ios::trunc);
            if (f) {  // if opened
                f << "OFF" << std::endl;
                // instructions
                f.close();  // je referme le fichier
            }
        }
        auto end                              = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time elapsed: " << elapsed.count() << "s\n" << std::endl;
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    pthread_t threads[NUM_THREADS];
    int rc[NUM_THREADS], i[NUM_THREADS];
    sensors.init();
    wiringPiSetupGpio();

    // Setup curses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    sensors.dc_1.setValGPIO("1");
    sensors.dc_2.setValGPIO("0");

    // Create our threads
      rc[0] = pthread_create(&threads[0], NULL, servo_pwm_thread, &i[0]);
      rc[1] = pthread_create(&threads[1], NULL, inp_thread, &i[1]);
      rc[2] = pthread_create(&threads[2], NULL, move_thread, &i[2]);
 //   rc[3] = pthread_create(&threads[3], NULL, dc_pwm_thread, &i[3]);
      rc[4] = pthread_create(&threads[4], NULL, cv_thread, &i[4]);
    pthread_exit(NULL);

    return 0;
}
