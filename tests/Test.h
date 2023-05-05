#pragma once

// LCOV_EXCL_START

#include <stdio.h>
#include <string.h>
#include <json/json.h>

#include "../src/madlib/utils.h"
#include "../src/madlib/Clock.h"

namespace madlib {

    #define ERR_TEST_FAILED_MSG "Test failed: "
    #define ERR_TEST_FAILED -1

    class Test {
    protected:

        static void tick(const char* outp = ".") {
            printf("%s", outp);
            fflush(stdout);
        }

        static void fail(const char* outp = "E") {
            printf("%s", outp);
            fflush(stdout);
        }

    public:
        static void assertTrue(bool expr, const char* file, int line) {
            if (!expr) {
                fail();
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expression is not TRUE");
            }
            tick();
        }

        static void assertFalse(bool expr, const char* file, int line) {
            if (expr) {
                fail();
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expression is not FALSE");
            }
            tick();
        }

        static void assertLongEquals(long long expected, long long actual, const char* file, int line) {
            if (expected != actual) {
                fail();
                printf("\nExpected..: %lld", expected);
                printf("\nActual....: %lld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected number is not equal to actual");
            }
            tick();
        }

        static void assertDoublesEquals(double expected, double actual, const char* file, int line) {
            if (expected != actual) {
                fail();
                printf("\nExpected..: %lf", expected);
                printf("\nActual....: %lf\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected double is not equal to actual");
            }
            tick();
        }

        static void assertDoublesEquals(double expected, double actual, double tolerance, const char* file, int line) {
            if (!(expected - tolerance <= actual && expected + tolerance >= actual)) {
                fail();
                printf("\nExpected..: %lf", expected);
                printf("\nActual....: %lf\n", actual);
                printf("\n(delta): %lf\n", tolerance);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected double is not equal to actual");
            }
            tick();
        }

        static void assertSizeEquals(size_t expected, size_t actual, const char* file, int line) {
            if (expected != actual) {
                fail();
                printf("\nExpected..: %lu", expected);
                printf("\nActual....: %lu\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected size is not equal to actual");
            }
            tick();
        }

        static void assertStringEquals(string expected, string actual, const char* file, int line) {
            if (expected.compare(actual) != 0) {
                fail();
                printf("\nExpected..: \"%s\"", expected.c_str());
                printf("\nActual....: \"%s\"\n", actual.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected string is not equal to actual");
            }
            tick();
        }

        static void assertPointersEquals(void* expected, void* actual, const char* file, int line) {
            if (expected != actual) {
                fail();
                printf("\nExpected..: \"%lld\"", (long long)&expected);
                printf("\nActual....: \"%lld\"\n", (long long)&actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected pointer is not equal to actual");
            }
            tick();
        }
        
        template<typename T>
        static void assertVectorEquals(const vector<T>& expected, const vector<T>& actual, const char* file, int line) {
            assertSizeEquals(expected.size(), actual.size());
            for (size_t i = 0; i < expected.size(); i++) {
                if (expected[i] != actual[i]) {
                    fail();                
                    printf("\nfailed at vector[%ld]\n", i);
                    throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected vector is not equal to actual");
                }
                tick();
            }
        }

        static void assertLongNotEquals(long long expected, long long actual, const char* file, int line) {
            if (expected == actual) {
                fail();
                printf("\nExpected..: %lld", expected);
                printf("\nActual....: %lld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected number is equal to actual");
            }
            tick();
        }

        static void assertDoublesNotEquals(double expected, double actual, const char* file, int line) {
            if (expected == actual) {
                fail();
                printf("\nExpected..: %lf", expected);
                printf("\nActual....: %lf\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected double is equal to actual");
            }
            tick();
        }

        static void assertDoublesNotEquals(double expected, double actual, double tolerance, const char* file, int line) {
            if (expected - tolerance <= actual && expected + tolerance >= actual) {
                fail();
                printf("\nExpected..: %lf", expected);
                printf("\nActual....: %lf\n", actual);
                printf("\n(delta): %lf\n", tolerance);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected double is equal to actual");
            }
            tick();
        }

        static void assertSizeNotEquals(size_t expected, size_t actual, const char* file, int line) {
            if (expected == actual) {
                fail();
                printf("\nExpected..: %lu", expected);
                printf("\nActual....: %lu\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected size is equal to actual");
            }
            tick();
        }

        static void assertStringNotEquals(string expected, string actual, const char* file, int line) {
            if (expected.compare(actual) == 0) {
                fail();
                printf("\nExpected..: \"%s\"", expected.c_str());
                printf("\nActual....: \"%s\"\n", actual.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected string is equal to actual");
            }
            tick();
        }
        
        template<typename T>
        static void assertVectorNotEquals(const vector<T>& expected, const vector<T>& actual, const char* file, int line) {          
            if (expected.size() != actual.size()) {
                tick();
                return;
            }   
            for (size_t i = 0; i < expected.size(); i++) {                
                if (expected[i] != actual[i]) {
                    tick();
                    return;
                }     
            }
            fail();                
            printf("\nfailed at vector comparison\n");
            throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected vector is equal to actual");   
        }

        static void assertLess(long expected, long actual, const char* file, int line) {
            if (expected >= actual) {
                fail();
                printf("\nExpected..: %ld", expected);
                printf("\nActual....: %ld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected is not less than actual");
            }
            tick();
        }

        static void assertGreater(long expected, long actual, const char* file, int line) {
            if (expected <= actual) {
                fail();
                printf("\nExpected..: %ld", expected);
                printf("\nActual....: %ld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected is not greather than actual");
            }
            tick();
        }

        static void assertLessOrEquals(long expected, long actual, const char* file, int line) {
            if (expected > actual) {
                fail();
                printf("\nExpected..: %ld", expected);
                printf("\nActual....: %ld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected is not less than or equal to actual");
            }
            tick();
        }

        static void assertGreaterOrEquals(long expected, long actual, const char* file, int line) {
            if (expected < actual) {
                fail();
                printf("\nExpected..: %ld", expected);
                printf("\nActual....: %ld\n", actual);
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected is not greather than or equal to actual");
            }
            tick();
        }

        static void assertMatch(string exp, string act, const char* file, int line) {
            if (!reg_match(exp, act)) {
                fail();
                printf("\nRegexp....: %s", exp.c_str());
                printf("\nActual....: %s\n", act.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "actual is not match to the regular expression");
            }
        }

        static void assertNotMatch(string exp, string act, const char* file, int line) {
            if (reg_match(exp, act)) {
                fail();
                printf("\nRegexp....: %s", exp.c_str());
                printf("\nActual....: %s\n", act.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "actual is match to the regular expression");
            }
        }

        static void assertContains(string exp, string act, const char* file, int line) {
            if (act.find(exp) == string::npos) {
                fail();
                printf("\nExpected..: %s", exp.c_str());
                printf("\nActual....: %s\n", act.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "actual is not containing the expected substring");
            }
        }

        static void assertNotContains(string exp, string act, const char* file, int line) {
            if (act.find(exp) != string::npos) {
                fail();
                printf("\nExpected..: %s", exp.c_str());
                printf("\nActual....: %s\n", act.c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "actual is containing the expected substring");
            }
        }

        template<typename T>
        static void assertThrowsContains(T func, const string& exptyp, const string& expmsg, const char* file, int line) {
            bool thrown = false;
            try {
                func();
                fail();            
            } catch(const exception& e) {
                thrown = true;
                Test::assertContains(expmsg, e.what(), file, line);
                Test::assertStringEquals(exptyp, typeid(e).name(), file, line);
            }
            if (!thrown) {
                throw ERROR("Fail at ", file, ":", line, " - ", concat(ERR_TEST_FAILED_MSG "expected exception was not thrown: ", exptyp));
            }
        }


        static void assertJsonEquals(Json::Value expected, Json::Value actual, const char* file, int line) {
            if (expected != actual) {
                fail();
                printf("\nExpected..: %s", expected.toStyledString().c_str());
                printf("\nActual....: %s\n", actual.toStyledString().c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected Json is not equal to actual");
            }
            tick();
        }

        static void assertJsonNotEquals(Json::Value expected, Json::Value actual, const char* file, int line) {
            if (expected == actual) {
                fail();
                printf("\nExpected..: %s", expected.toStyledString().c_str());
                printf("\nActual....: %s\n", actual.toStyledString().c_str());
                throw ERROR("Fail at ", file, ":", line, " - ", ERR_TEST_FAILED_MSG "expected Json is equal to actual");
            }
            tick();
        }

        // -------------
        static int deepness;

        static void call(void (*func)(void), const char* name, const char* tick = COLOR_SUCCESS "✓\n" COLOR_DEFAULT) {
            if (deepness) printf("%s", tick);
            printf("%s", concat("Test running: ", name, "() ").c_str());
            fflush(stdout);
            deepness++;
            Clock clock;
            unsigned long before = clock.now();
            func();
            unsigned long passed = clock.now() - before;
            printf(" (%ld ms) ", passed);
            fflush(stdout);
            deepness--;
            if (!deepness) printf("%s", tick);
            fflush(stdout);
        }
    };

    int Test::deepness = 0;

    #define ASSERT_TRUE(exp) Test::assertTrue(exp, __FILE__, __LINE__)
    #define ASSERT_FALSE(exp) Test::assertFalse(exp, __FILE__, __LINE__)
    #define ASSERT_EQUALS(exp, act) Test::assertLongEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_LONG_EQUALS(exp, act) Test::assertLongEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_STRING_EQUALS(exp, act) Test::assertStringEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_POINTERS_EQUALS(exp, act) Test::assertPointersEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_DOUBLES_EQUALS(exp, act) Test::assertDoublesEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_DOUBLES_EQUALS_TOLERANT(exp, act, tolerance) Test::assertDoublesEquals(exp, act, tolerance, __FILE__, __LINE__)
    #define ASSERT_SIZE_EQUALS(exp, act) Test::assertSizeEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_VECTOR_EQUALS(exp, act) Test::assertVectorEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_LONG_NOT_EQUALS(exp, act) Test::assertLongNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_STIRNG_NOT_EQUALS(exp, act) Test::assertStringNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_POINTERS_NOT_EQUALS(exp, act) Test::assertPointersNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_DOUBLES_NOT_EQUALS(exp, act) Test::assertDoublesNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_DOUBLES_NOT_EQUALS_TOLERANT(exp, act, tolerance) Test::assertDoublesNotEquals(exp, act, tolerance, __FILE__, __LINE__)
    #define ASSERT_JSON_EQUALS(exp, act) Test::assertJsonEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_JSON_NOT_EQUALS(exp, act) Test::assertJsonNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_SIZE_NOT_EQUALS(exp, act) Test::assertSizeNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_VECTOR_NOT_EQUALS(exp, act) Test::assertVectorNotEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_LESS(exp, act) Test::assertLess(exp, act, __FILE__, __LINE__)
    #define ASSERT_GREATER(exp, act) Test::assertGreater(exp, act, __FILE__, __LINE__)
    #define ASSERT_LESS_OR_EQUALS(exp, act) Test::assertLessOrEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_GREATER_OR_EQUALS(exp, act) Test::assertGreaterOrEquals(exp, act, __FILE__, __LINE__)
    #define ASSERT_MATCH(exp, act) Test::assertMatch(exp, act, __FILE__, __LINE__)
    #define ASSERT_NOT_MATCH(exp, act) Test::assertNotMatch(exp, act, __FILE__, __LINE__)
    #define ASSERT_CONTAINS(exp, act) Test::assertContains(exp, act, __FILE__, __LINE__)
    #define ASSERT_NOT_CONTAINS(exp, act) Test::assertNotContains(exp, act, __FILE__, __LINE__)
    #define ASSERT_THROWS_CONTAINS(func, exctyp, expmsg) Test::assertThrowsContains([&](){ func; }, typeid(exctyp).name(), expmsg, __FILE__, __LINE__)

    #define TEST(func) Test::call(func, concat(COLOR_INFO __FILE__, ":", __LINE__, COLOR_DEFAULT, " ", QUOTEME(func)).c_str())
    
}

// LCOV_EXCL_STOP