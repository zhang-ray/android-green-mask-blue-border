#include "AndroidGreenMaskBlueBorder.hpp"


#define LOG_TAG "AndroidGreenMaskBlueBorder"


int main() {
    for (;;) {
        AndroidGreenMaskBlueBorder androidGreenMaskBlueBorder;

        if (NO_ERROR != androidGreenMaskBlueBorder.init()) {
            ALOGI("What the hell?!");
            return -1;
        }

        androidGreenMaskBlueBorder.drawOnce();

        usleep(1000000 * 3/*s*/); // keep it

        androidGreenMaskBlueBorder.deInit();
    }
    return 0;
}

