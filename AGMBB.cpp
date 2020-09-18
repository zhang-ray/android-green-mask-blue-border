#include "AGMBB.hpp"

#include "AndroidGreenMaskBlueBorder.hpp"
#include <memory>

AGMBB::AGMBB() {
    if (!instance_) {
        instance_ = std::make_shared<AndroidGreenMaskBlueBorder>();
        instance_->init();
        instance_->drawOnce();
    }
}

void AGMBB::show(){
    instance_->show();
}

void AGMBB::disMiss() {
    instance_->disMiss();
}
