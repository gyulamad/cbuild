#pragma once

#include "../Test.h"
#include "../../src/lib/Clock.h"

using namespace lib;

void test_lib_real_Clock() {
    Clock clock;
    unsigned long start_time = clock.now();
    clock.delay(100);
    unsigned long end_time = clock.now();
    ASSERT_TRUE(end_time - start_time >= 100 && end_time - start_time <= 110);
}

void test_lib_fake_Clock() {
    Clock clock(1);
    ASSERT_EQUALS(clock.now(), 1);

    clock.delay(99);
    ASSERT_EQUALS(clock.now(), 100);

    clock.set(50);
    ASSERT_EQUALS(clock.now(), 50);

    clock.delay(100);
    ASSERT_EQUALS(clock.now(), 150);
}

void test_lib_Clock() {
    TEST(test_lib_real_Clock);
    TEST(test_lib_fake_Clock);
}