#include "AGMBB.hpp"

#include <chrono>
#include <thread>

#define LOG_TAG "AndroidGreenMaskBlueBorderStressTest"


int main() {
    for (;;) {
        AGMBB::get().show();
        std::this_thread::sleep_for(std::chrono::seconds(3)); // keep it
        AGMBB::get().disMiss();
        std::this_thread::sleep_for(std::chrono::seconds(3)); // keep it

    }
    return 0;
}

