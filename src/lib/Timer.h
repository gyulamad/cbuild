#pragma once

#include "Clock.h"

#define TIMER_OFF 0ul

namespace lib {

    class Timer {
    private:
        Clock &clock;
        unsigned long ms;
        bool force;
        unsigned long next;

    public:
        Timer(Clock &clock, unsigned long ms = TIMER_OFF, bool force = true)
            : clock(clock), ms(ms), force(force), next(clock.now()) {}

        virtual ~Timer() {}

        Clock& getClock() const {
            return clock;
        }

        bool check() {
            if (clock.now() == CLK_NULL) return false;
            if (ms == TIMER_OFF) return false;
            unsigned long now = clock.now();
            if (next <= now)
            {
                if (force)
                    next += ms;
                else
                    next = now + ms;
                return true;
            }
            return false;
        }

        void setInterval(unsigned long ms = TIMER_OFF) {
            if (clock.now() == CLK_NULL) return;
            this->ms = ms;
        }

        void off() {
            setInterval(TIMER_OFF);
        }

        
        unsigned long getInterval() const {
            return ms;
        }

        void setForce(bool force) {
            this->force = force;
        }
        
        bool isForced() const {
            return force;
        }
    };
    
}
