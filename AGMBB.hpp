#pragma once

#include "Singleton.hpp"
#include <memory>


class AndroidGreenMaskBlueBorder;

class AGMBB : public Singleton<AGMBB> {
    friend class Singleton<AGMBB>;
    std::shared_ptr<AndroidGreenMaskBlueBorder> instance_ = nullptr;
    AGMBB();
    ~AGMBB(){}
public:
    void show();
    void disMiss();
};