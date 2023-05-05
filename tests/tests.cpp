#include <iostream>

#include "Test.h"
// TODO: include tests: #include "mytests/mytest1.h"

using namespace std;

int main() {
    try {
        // TODO: call your tests: TEST(test_mytest1);
    } 
    // LCOV_EXCL_START
    catch (exception &e) {
        cerr << e.what() << endl;
        return -1;
    }
    // LCOV_EXCL_STOP
}