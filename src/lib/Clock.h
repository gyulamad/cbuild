#pragma once

#include <ctime>
#include <chrono>
#include <thread>
#include "utils.h"

using namespace std;

namespace lib {

    #define CLK_REAL 0ul
    #define CLK_NULL -1ul

    class Clock
    {
    protected:
        unsigned long ts = CLK_NULL;
    public:
        // ts = 0 means it's a real clock
        Clock(unsigned long ts = CLK_REAL): ts(ts) {}

        unsigned long now() const {
            if (ts == CLK_REAL) {
                unsigned long _now = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now())
                    .time_since_epoch()
                    .count();
                return _now;
            }
            return ts;
        }

        void delay(unsigned long ms) {
            if (ts == CLK_REAL) {
                this_thread::sleep_for(chrono::milliseconds(ms));
                return;
            }
            ts += ms;
        }

        void set(unsigned long _ts) {
            if (ts == CLK_REAL) throw ERROR("Can not set time on real clock");
            ts = _ts;
        }
    };

}