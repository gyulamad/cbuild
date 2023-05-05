#include <iostream>

#include "Test.h"
#include "lib/test_lib.h"
// NOTE: include more tests here...

using namespace std;

int main() {
    try {
	TEST(test_lib);
        // TODO: call more tests here...
    } 
    // LCOV_EXCL_START
    catch (exception &e) {
        cerr << e.what() << endl;
        return -1;
    }
    // LCOV_EXCL_STOP
}