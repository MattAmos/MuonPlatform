#include "Runner.h"

int main(int argc, char** argv) {
    return 0;
}

int scp(std::string filename) {
    return system(("scp " + IMG_DIR + "/" + filename + " " + USER + "@" + HOST + ":/home/IMG").c_str());
}
