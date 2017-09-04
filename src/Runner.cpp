#include "Runner.h"

void signalHandler(int sigNum) {
    std::cout << "Interrupt signal (" << sigNum << ") received. Exiting gracefully..." << std::endl;
    sensors.destroy();
    exit(sigNum);
}

int scp(std::string filename) {
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
}

void* PrintHello(void* threadid) {
    long tid;
    tid                                                           = (long) threadid;
    std::chrono::time_point<std::chrono::high_resolution_clock> t = std::chrono::high_resolution_clock::now();
    std::time_t timePt                                            = std::chrono::high_resolution_clock::to_time_t(t);
    std::cout << "Hello World! Thread ID, " << tid << " " << std::ctime(&timePt) << std::endl;
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t threads[NUM_THREADS];
    int rc, i;

    for (i = 0; i < NUM_THREADS; i++) {
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads[i], NULL, PrintHello, &i);

        if (rc) {
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}
