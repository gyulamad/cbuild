#pragma once

#include "../../src/lib/utils.h"
#include "test_Clock.h"

const int MAJOR = LIB_VERSION_MAJOR;
const int MINOR = LIB_VERSION_MINOR;
const int PATCH = LIB_VERSION_PATCH;

void test_lib_version_check_min() {
    ASSERT_TRUE(verion_check_min(MAJOR, MINOR, PATCH));
    ASSERT_TRUE(verion_check_min(MAJOR, MINOR));
    ASSERT_TRUE(verion_check_min(MAJOR));
    ASSERT_FALSE(verion_check_min(MAJOR + 1, MINOR, PATCH));
    ASSERT_FALSE(verion_check_min(MAJOR, MINOR + 1, PATCH));
    ASSERT_FALSE(verion_check_min(MAJOR, MINOR + 1));
    ASSERT_FALSE(verion_check_min(MAJOR + 1));
}


void test_lib_version_check_max() {
    ASSERT_TRUE(verion_check_max(MAJOR, MINOR, PATCH));
    ASSERT_TRUE(verion_check_max(MAJOR, MINOR));
    ASSERT_TRUE(verion_check_max(MAJOR));
    ASSERT_FALSE(verion_check_max(MAJOR - 1, MINOR, PATCH));
    ASSERT_FALSE(verion_check_max(MAJOR, MINOR - 1, PATCH));
    ASSERT_FALSE(verion_check_max(MAJOR, MINOR - 1));
    ASSERT_FALSE(verion_check_max(MAJOR - 1));
}

void test_lib_version_check() {
    ASSERT_TRUE(verion_check(MAJOR, MINOR, PATCH));
    ASSERT_TRUE(verion_check(MAJOR, MINOR));
    ASSERT_TRUE(verion_check(MAJOR));
    ASSERT_FALSE(verion_check(MAJOR + 1, MINOR, PATCH));
    ASSERT_FALSE(verion_check(MAJOR, MINOR + 1, PATCH));
    ASSERT_FALSE(verion_check(MAJOR, MINOR, PATCH + 1));
    ASSERT_FALSE(verion_check(MAJOR, MINOR + 1));
    ASSERT_FALSE(verion_check(MAJOR + 1));
}

void test_lib_str_dirname() {
    const char* filename = "/path/to/file.txt";
    std::string expected_dirname = "/path/to";
    std::string dirname = str_dirname(filename);
    ASSERT_STRING_EQUALS(dirname, expected_dirname);
}

void test_lib_join_remove_last_glue() {
    ostringstream oss;
    oss << "apple" << "," << "banana" << "," << "pear" << ",";
    string expected_str = "apple,banana,pear";
    string result = join_remove_last_glue(oss, ",");
    ASSERT_STRING_EQUALS(result, expected_str);
}

void test_lib_join() {
    std::vector<int> vec = {1, 2, 3};
    std::string result = join("-", "a", "b", "c");
    ASSERT_STRING_EQUALS(result, "a-b-c");

    result = join(":", vec);
    ASSERT_STRING_EQUALS(result, "1:2:3");
}

void test_lib_concat() {
    std::string result = concat("Hello", " world", "!");
    std::string expected = "Hello world!";
    ASSERT_STRING_EQUALS(result, expected);
}

void test_lib_quote() {
    std::string str = "hello";
    std::string expected = "\"hello\"";

    std::string quoted = quote(str);

    ASSERT_STRING_EQUALS(quoted, expected);
}
void test_lib_reg_match() {
    string str = "This is a test string with some numbers 123 and some special characters !@#$%^&*";
    vector<string> matches;
    
    // Test for matching pattern that exists in the string
    ASSERT_EQUALS(reg_match("[0-9]+", str, &matches), 1);
    ASSERT_EQUALS(matches.size(), 1);
    ASSERT_STRING_EQUALS(matches[0], "123");
    
    // Test for matching pattern that does not exist in the string
    ASSERT_EQUALS(reg_match("[a-z]+", str, &matches), 1);
    ASSERT_EQUALS(matches.size(), 1);
    
    // Test for matching pattern that captures multiple groups
    ASSERT_EQUALS(reg_match("([a-zA-Z]+)\\s([a-z]+)", str, &matches), 1);
    ASSERT_EQUALS(matches.size(), 3);
    ASSERT_STRING_EQUALS(matches[0], "This is");
    ASSERT_STRING_EQUALS(matches[1], "This");
    ASSERT_STRING_EQUALS(matches[2], "is");
}

void test_lib_reg_match_alphabets() {
    std::string str = "Thisisateststringwithsomenumbersandallsorts of special characters!@#$%^&*()_+";
    std::vector<std::string> matches;

    // Test for matching pattern that does not exist in the string
    ASSERT_EQUALS(reg_match("[0-9]+", str, &matches), 0);
    ASSERT_EQUALS(matches.size(), 0);

    // Test for matching pattern that exists in the string
    ASSERT_EQUALS(reg_match("test", str, &matches), 1);
    ASSERT_EQUALS(matches.size(), 1);
    ASSERT_STRING_EQUALS(matches[0], "test");

    // Test for matching pattern that appears multiple times in the string
    ASSERT_EQUALS(reg_match("[a-z]+", str, &matches), 1);
    ASSERT_EQUALS(matches.size(), 1);
}

void test_lib_date_parse() {
    string date_string = "2022-04-28 15:45:00.123";
    unsigned long expected_timestamp = 1651160700123;

    unsigned long timestamp = date_parse(date_string);

    ASSERT_LONG_EQUALS(timestamp, expected_timestamp);
}

void test_lib_exec() {
    string output;
    int status = exec("echo 'Hello, world!'", output);
    ASSERT_EQUALS(status, 0);
    ASSERT_MATCH("Hello, world!", output);
}

void test_lib_explode() {
    string str = "apple,banana,orange";
    vector<string> tokens = explode(',', str);
    vector<string> expected_tokens = {"apple", "banana", "orange"};
    ASSERT_TRUE(tokens == expected_tokens);

    str = "one|two|three|four|five";
    tokens = explode('|', str);
    expected_tokens = {"one", "two", "three", "four", "five"};
    ASSERT_TRUE(tokens == expected_tokens);

    str = "a,b,c,d,e,";
    tokens = explode(',', str);
    expected_tokens = {"a", "b", "c", "d", "e"};
    ASSERT_TRUE(tokens == expected_tokens);

    str = "";
    tokens = explode(',', str);
    expected_tokens = {};
    ASSERT_TRUE(tokens == expected_tokens);
}

void test_lib_random_macros() {
    // Set random seed to a fixed value for reproducibility
    RANDOM_SEED(42);

    // Test RAND macro
    int rand_val = RAND(1, 10);
    ASSERT_TRUE(rand_val >= 1 && rand_val <= 10);

    // Test RANDD macro
    double randd_val = RANDD(0.0, 1.0);
    ASSERT_TRUE(randd_val >= 0.0 && randd_val <= 1.0);


    // Test RANDS macro
    short rands_val = RANDS(-10, 10);
    ASSERT_TRUE(rands_val >= -10 && rands_val <= 10);

    // Test RANDC macro
    char randc_val = RANDC('a', 'z');
    ASSERT_TRUE(randc_val >= 'a' && randc_val <= 'z');

    // Test RANDB macro
    bool randb_val = RANDB();
    ASSERT_TRUE(randb_val == 0 || randb_val == 1);
}



void test_lib() {
    TEST(test_lib_version_check_min);
    TEST(test_lib_version_check_max);
    TEST(test_lib_version_check);
    TEST(test_lib_Clock);
    TEST(test_lib_str_dirname);
    TEST(test_lib_join_remove_last_glue);
    TEST(test_lib_join);
    TEST(test_lib_concat);
    TEST(test_lib_quote);
    TEST(test_lib_reg_match);
    TEST(test_lib_reg_match_alphabets);
    TEST(test_lib_date_parse);
    TEST(test_lib_exec);
    TEST(test_lib_explode);
    TEST(test_lib_random_macros);
}