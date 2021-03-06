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

void forward() {
    sensors.dc_2.setValGPIO("0");
    sensors.dc_1.setValGPIO("1");
}

void backward() {
    sensors.dc_1.setValGPIO("0");
    sensors.dc_2.setValGPIO("1");
}

void stop() {
    sensors.dc_2.setValGPIO("0");
    sensors.dc_1.setValGPIO("0");
}

void* servo_pwm_thread(void* threadid) {
    while (true) {
        pthread_mutex_lock(&mutex);
        sensors.servo.setValGPIO("1");
        usleep(sensors.servo.getPwmTime());
        sensors.servo.setValGPIO("0");
        usleep(20000 - sensors.servo.getPwmTime());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}

void* dc_pwm_thread(void* threadid) {
    while (true) {
        pthread_mutex_lock(&dc_mut);
        sensors.dc_move(sensors.move);
        pthread_mutex_unlock(&dc_mut);
        usleep(5);
        sensors.dc_move(DC_STOP);
        usleep(5000);
    }
}

void* inp_thread(void* threadid) {
    std::ifstream file;
    std::string first, second;
    int ch;
    while (true) {
        // Check file for network control
        first = second = "";
        file.open("/var/www/bootstrap/run.txt");
        file >> first;
        // If we have two commands (<left/right> <front/back>)
        if (file >> second) {
            if (first == "left") {
                sensors.servo.setPwmTime(SERVO_PWM_MAX);
            }
            else if (first == "right") {
                sensors.servo.setPwmTime(SERVO_PWM_MIN);
            }
            if (second == "front") {
                pthread_mutex_lock(&dc_mut);
                sensors.move = DC_FRWD;
                pthread_mutex_unlock(&dc_mut);
            }
            else if (second == "back") {
                pthread_mutex_lock(&dc_mut);
                sensors.move = DC_BACK;
                pthread_mutex_unlock(&dc_mut);
            }
        }
        // If we have one command (<front/back/auto>)
        else {
            if (first != "auto") {
                sensors.servo.setPwmTime(SERVO_PWM_MID);
                if (first == "front") {
                    pthread_mutex_lock(&dc_mut);
                    sensors.move = DC_FRWD;
                    pthread_mutex_unlock(&dc_mut);
                }
                else if (first == "back") {
                    pthread_mutex_lock(&dc_mut);
                    sensors.move = DC_BACK;
                    pthread_mutex_unlock(&dc_mut);
                }
            }
            else {
                usleep(50000);
            }
        }
        file.close();
        // Check controller for PS4 control
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
            // Check keyboard control
            if (kbhit()) {
                contFlag = true;
                ch       = getch();
                switch (ch) {
                    case 'a': sensors.servo.incPwmTime(100); break;
                    case 'd': sensors.servo.incPwmTime(-100); break;
                    case 'A': sensors.servo.setPwmTime(SERVO_PWM_MAX); break;
                    case 'D': sensors.servo.setPwmTime(SERVO_PWM_MIN); break;
                    case 'w':
                        pthread_mutex_unlock(&dc_mut);
                        sensors.move = DC_FRWD;
                        pthread_mutex_unlock(&dc_mut);
                        break;
                    case 's':
                        pthread_mutex_unlock(&dc_mut);
                        sensors.move = DC_BACK;
                        pthread_mutex_unlock(&dc_mut);
                        break;
                    case 32:
                        pthread_mutex_unlock(&dc_mut);
                        sensors.move = DC_STOP;
                        pthread_mutex_unlock(&dc_mut);
                        break;
                    case 27: contFlag = false; break;
                }
            }
        }
    }
}

int getAvg(int d[NUM_SAMP]) {
    int result = 0;
    for (int i = 0; i < NUM_SAMP; ++i) {
        result += d[i];
    }
    result *= INV_NUM_SAMP;
    return result;
}

void turn(int ld, int rd) {
    if (ld > 2000 && rd > 2000) {
        if (rand() < 0.5) {
            //std::cout << "RMAX " << ld << " " << rd << std::endl;
            sensors.servo.setPwmTime(SERVO_PWM_MAX);
        }
        else {
            //std::cout << "RMIN " << ld << " " << rd << std::endl;
            sensors.servo.setPwmTime(SERVO_PWM_MIN);
        }
    }
    else if (ld > 2000) {
        //std::cout << "MAX " << ld << " " << rd << std::endl;
        sensors.servo.setPwmTime(SERVO_PWM_MAX);
    }
    else if (rd > 2000) {
        //std::cout << "MIN " << ld << " " << rd << std::endl;
        sensors.servo.setPwmTime(SERVO_PWM_MIN);
    }
    else {
        if (abs(ld - rd) < 5) {
            //           sensors.servo.setPwmTime(SERVO_PWM_MID);

            if (rand() < 0.5) {
                //std::cout << "RMAX " << ld << " " << rd << std::endl;
                sensors.servo.setPwmTime(SERVO_PWM_MAX);
            }
            else {
                //std::cout << "RMIN " << ld << " " << rd << std::endl;
                sensors.servo.setPwmTime(SERVO_PWM_MIN);
            }
        }
        else if (ld > rd) {
            //std::cout << "MAX " << ld << " " << rd << std::endl;
            sensors.servo.setPwmTime(SERVO_PWM_MAX);
        }
        else if (rd > ld) {
            //std::cout << "MIN " << ld << " " << rd << std::endl;
            sensors.servo.setPwmTime(SERVO_PWM_MIN);
        }
    }
}

void* move_thread(void* threadid) {
    // Ranger rFinder = Ranger();
    int frontDist[NUM_SAMP], leftDist[NUM_SAMP], rightDist[NUM_SAMP], backDist[NUM_SAMP];
    int fd, ld, rd, bd;
    srand(time(NULL));
    // so far, the logic is:
    // 1. It goes forward
    // 2. If something's within 56 cm, reverse -> if something's behind it within 15cm, or it reverses outside of the
    // 56cm mark leave the loop
    // 2a. After loop, it checks which direction it goes
    int count = 1;
    pthread_mutex_lock(&dc_mut);
    sensors.move = DC_FRWD;  // go forward
    pthread_mutex_unlock(&dc_mut);
    usleep(50);
    int fdStop = 56, stuckCount = 0;

    std::chrono::time_point<std::chrono::system_clock> timePt = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed;

    while (true) {
        count++;
        // if (count % 3000 == 0) {
        //    sensors.sonicInit();
        //}
        // if (!kbhit()) {
        // Get sensors
        frontDist[count % NUM_SAMP] = sensors.sonic_front.getCM();
        leftDist[count % NUM_SAMP]  = sensors.sonic_left.getCM();
        rightDist[count % NUM_SAMP] = sensors.sonic_right.getCM();
        backDist[count % NUM_SAMP]  = sensors.sonic_back.getCM();

        fd = getAvg(frontDist);
        ld = getAvg(leftDist);
        rd = getAvg(rightDist);
        bd = getAvg(backDist);

        printf("[MOTION] F %4d, L %4d, R %4d, B %4d\n", fd, ld, rd, bd);

        pthread_mutex_lock(&mutex);
        if (fd < fdStop || (fd < fdStop + 20 && ld < 60) || (fdStop + 20 < 100 && rd < 60) || (ld < 10 && rd > 10)
            || (rd < 15 && ld > 15))  // 2. check if <56 cm front
        {
            fdStop = 30;
            while (((fd < fdStop || (fd + 20 < fdStop && ld < 60) || (fd + 20 < fdStop && rd < 60) || (ld < 10 && rd > 10)
                     || (rd < 15 && ld > 15)))
                   && bd > 10)  // 2a. reverse until out of range
            {
                count++;
                //  if(count % 3000 == 0){ sensors.sonicInit(); }
                frontDist[count % NUM_SAMP] = sensors.sonic_front.getCM();
                leftDist[count % NUM_SAMP]  = sensors.sonic_left.getCM();
                rightDist[count % NUM_SAMP] = sensors.sonic_right.getCM();
                backDist[count % NUM_SAMP]  = sensors.sonic_back.getCM();

                fd = getAvg(frontDist);
                ld = getAvg(leftDist);
                rd = getAvg(rightDist);
                bd = getAvg(backDist);

                printf("[MOTION] F %4d, L %4d, R %4d, B %4d\n", fd, ld, rd, bd);
                turn(rd, ld);
                pthread_mutex_lock(&dc_mut);
                sensors.move = DC_BACK;
                pthread_mutex_unlock(&dc_mut);
		usleep(50);
                // Now we check if we're stuck in a loop, try to reverse out of it!
                elapsed = std::chrono::system_clock::now() - timePt;
                timePt  = std::chrono::system_clock::now();

                if (elapsed.count() < 1) { //If we're trying to reverse after a second of movement
                    stuckCount++;
                }
                if (stuckCount > 20) {
		    std::cout << "[MOTION] Stuck!" << std::endl;
                    while (bd > 20 && (std::chrono::system_clock::now() - timePt).count() < 15) {
			//turn(rd, ld);
			std::cout << "[MOTION] Unsticking!" << std::endl;
                        backDist[count % NUM_SAMP] = sensors.sonic_back.getCM();
                        bd                         = getAvg(backDist);
                    }
                    stuckCount = 0;
                }
                //                backward();
            }

            ld = sensors.sonic_left.getCM();
            rd = sensors.sonic_right.getCM();

            // 2b. choose direction
            //  stop();
            pthread_mutex_lock(&dc_mut);
            sensors.move = DC_STOP;
            pthread_mutex_unlock(&dc_mut);
            usleep(1000);

            turn(ld, rd);
            pthread_mutex_unlock(&mutex);
            usleep(100);
            pthread_mutex_lock(&mutex);
            pthread_mutex_lock(&dc_mut);
            sensors.move = DC_FRWD;  // go forward
            pthread_mutex_unlock(&dc_mut);
            usleep(50);
        }
        else  // 3. reset to forward position when all fine
        {
            turn(ld, rd);
            pthread_mutex_lock(&dc_mut);
            sensors.move = DC_FRWD;
            pthread_mutex_unlock(&dc_mut);
            usleep(50);
            pthread_mutex_unlock(&mutex);
            usleep(100);
            pthread_mutex_lock(&mutex);
            fdStop = 120;
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
}

void* cv_thread(void* threadid) {
    int nb_img                    = 0;
    std::string name              = "";
    std::string line              = "";
    std::string face_cascade_name = "/home/pi/MuonPlatform/test/haarcascade_frontalface_alt.xml";
    cv::CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_name)) {
        std::cout << "[VISION] Error loading face cascade" << std::endl;
    }

    cv::Mat img = cv::imread("/dev/shm/mjpeg/cam.jpg"), img_gray;
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    std::vector<cv::Rect> found, found_filtered, faces;
    size_t i, j;

    cv::Rect r;
    cv::Point centre;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed;

    while (true) {
        img = cv::imread("/dev/shm/mjpeg/cam.jpg");
        if (!img.data) {
            std::cout << "[VISION] No image data. Continuing..." << std::endl;
            break;
        }
        start = std::chrono::system_clock::now();
        cv::cvtColor(img, img_gray, CV_BGR2GRAY);
        // equalizeHist(img_gray, img_gray);
        hog.detectMultiScale(img_gray, found, 0, cv::Size(4, 4), cv::Size(0, 0), 1.08, 3);
        face_cascade.detectMultiScale(img_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));

	/*
        for (i = 0; i < found.size(); i++) {
            cv::Rect r = found[i];
            for (j = 0; j < found.size(); j++) {
                if (j != i && (r & found[j]) == r) {
                    break;
                }
            }
            if (j == found.size()) {
                found_filtered.push_back(r);
            }
        }
	*/

        for (i = 0; i < found.size(); i++) {
            r = found[i];
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.06);
            r.height = cvRound(r.height * 0.9);
            cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }

        for (size_t k = 0; k < faces.size(); k++) {
            centre = cv::Point(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
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
            std::cout << "[VISION] Found " << found.size() << " people, " << faces.size() << " faces." << std::endl;
            nb_img++;
            /*stringstream ss("/var/www/bootstrap/");
            ss << "test";
            ss << i;
            ss << ".jpg";
            ss >> name;*/
            name = "/var/www/bootstrap/test" + std::to_string(nb_img) + ".jpg";
            // place img in /var/www/bootstrap
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
            }
            file.close();  // je referme le fichier
            // write in mode.txt "ON" and erase everything else
            std::ofstream f("/var/www/bootstrap/mode.txt", std::ios::out | std::ios::trunc);
            if (f) {  // if opened
                f << "ON" << std::endl;
                // instructions
            }
            f.close();  // je referme le fichier
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
            }
            f.close();  // je referme le fichier
        }
        end     = std::chrono::system_clock::now();
        elapsed = end - start;
        std::cout << "[VISION] Time elapsed: " << elapsed.count() << "s\n" << std::endl;
    }
    usleep(100);
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    pthread_t threads[NUM_THREADS];
    int rc[NUM_THREADS], i;
    sensors.init();
    wiringPiSetupGpio();

    // Setup curses
    /*    initscr();
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
    */
    sensors.move = DC_FRWD;

    // Create our threads
    rc[0] = pthread_create(&threads[0], NULL, servo_pwm_thread, &i);
    rc[1] = pthread_create(&threads[1], NULL, inp_thread, &(++i));
    rc[2] = pthread_create(&threads[2], NULL, move_thread, &(++i));
    rc[3] = pthread_create(&threads[3], NULL, dc_pwm_thread, &(++i));
    rc[4] = pthread_create(&threads[4], NULL, cv_thread, &(++i));
    pthread_exit(NULL);

    return 0;
}
