#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <libgen.h> // for dirname()
#include <limits.h>

using namespace std;

namespace lib {

    #define LIB_VERSION_MAJOR 0
    #define LIB_VERSION_MINOR 0
    #define LIB_VERSION_PATCH 1

    #define MS_PER_SECOND 1000ul
    #define MS_PER_MINUTE (60ul * MS_PER_SECOND)
    #define MS_PER_HOUR (60ul * MS_PER_MINUTE)
    #define MS_PER_DAY (24ul * MS_PER_HOUR)
    #define MS_PER_WEEK (7ul * MS_PER_DAY)

    #define COLOR_DEFAULT "\033[0;0;0m"
    #define COLOR_ERROR "\033[31m"
    #define COLOR_ALERT "\033[31m"
    #define COLOR_WARNING "\033[33m"
    #define COLOR_INFO "\033[36m"
    #define COLOR_SUCCESS "\033[32m"
    #define COLOR_DEBUG "\033[35m"
    #define COLOR_FILENAME COLOR_INFO
    #define COLOR_DATETIME COLOR_INFO

    #define QUOTEME_1(x) #x
    #define QUOTEME(x) QUOTEME_1(x)

    // pseudo-random numbers using a linear congruential generator (LCG)
    #define RANDOM_SEED(seed) unsigned long _rnd_seed = (seed)
    #define RAND(min, max) ((min) + (_rnd_seed = _rnd_seed * 1664525UL + 1013904223UL) % ((max) - (min) + 1))
    #define RANDD(min, max) ((min) + ((double)(_rnd_seed = _rnd_seed * 16807UL)) / ((double)ULONG_MAX) * ((max) - (min)))

    #define RANDS(min, max) ((short)(RAND((min), (max))))
    #define RANDC(min, max) ((char)(RAND((min), (max))))
    #define RANDB() ((bool)(RAND(0, 1)))


    #define ERROR_MESSAGE(...) concat(COLOR_ERROR, "[ERROR] ", __VA_ARGS__, " " COLOR_FILENAME __FILE__, ":", __LINE__, COLOR_DEFAULT)
    #define ERROR(...) runtime_error(ERROR_MESSAGE(__VA_ARGS__))

    #define PRINT(...) cout << concat(COLOR_DEFAULT, __VA_ARGS__) << endl;

    #define DBG(...) cout << concat(COLOR_DEBUG, "[DEBUG] (", join(", ", __VA_ARGS__), ") " COLOR_FILENAME __FILE__, ":", __LINE__, COLOR_DEFAULT) << endl

    #define __DIR__ str_dirname(__FILE__)

    // ------------------- version -----------------------

    bool verion_check_min(int major, int minor, int patch) {
        if (LIB_VERSION_MAJOR < major) return false;
        if (LIB_VERSION_MINOR < minor) return false;
        if (LIB_VERSION_PATCH < patch) return false;
        return true;
    }

    bool verion_check_min(int major, int minor) {
        if (LIB_VERSION_MAJOR < major) return false;
        if (LIB_VERSION_MINOR < minor) return false;
        return true;
    }

    bool verion_check_min(int major) {
        if (LIB_VERSION_MAJOR < major) return false;
        return true;
    }

    bool verion_check_max(int major, int minor, int patch) {
        if (LIB_VERSION_MAJOR > major) return false;
        if (LIB_VERSION_MINOR > minor) return false;
        if (LIB_VERSION_PATCH > patch) return false;
        return true;
    }

    bool verion_check_max(int major, int minor) {
        if (LIB_VERSION_MAJOR > major) return false;
        if (LIB_VERSION_MINOR > minor) return false;
        return true;
    }

    bool verion_check_max(int major) {
        if (LIB_VERSION_MAJOR > major) return false;
        return true;
    }

    bool verion_check(int major, int minor, int patch) {
        if (LIB_VERSION_MAJOR != major) return false;
        if (LIB_VERSION_MINOR != minor) return false;
        if (LIB_VERSION_PATCH != patch) return false;
        return true;
    }

    bool verion_check(int major, int minor) {
        if (LIB_VERSION_MAJOR != major) return false;
        if (LIB_VERSION_MINOR != minor) return false;
        return true;
    }

    bool verion_check(int major) {
        if (LIB_VERSION_MAJOR != major) return false;
        return true;
    }

    // ---------------------------------------------


    inline string str_dirname(const char* filename) {
        string path(filename);
        return dirname(&path[0]);
    }

    inline string join_remove_last_glue(ostringstream& oss, const string& glue) {
        int n = glue.length();
        string str = oss.str();
        str.erase(str.length() - n, n);
        return str;
    }

    template <typename... T>
    inline string join(const string& glue, T... args) {
        ostringstream oss;
        ((oss << args << glue), ...);
        return join_remove_last_glue(oss, glue);
    }

    template <typename T>
    inline string join(const string& glue, vector<T> vec) {
        ostringstream oss;
        for(const T& v: vec) oss << v << glue;
        return join_remove_last_glue(oss, glue);
    }

    template <typename... T>
    inline string concat(T... args) {
        return join("", args...);
    }

    inline string quote(const string& str, const string& quote = "\"") {
        return quote + str + quote;
    }

    inline int reg_match(const string& pattern, const string& str, vector<string>* matches = nullptr) {
        regex r(pattern);
        smatch m;
        if (regex_search(str, m, r)) {
            if (matches != nullptr) {
                // Clear the vector before adding new matches
                matches->clear();
                for (unsigned int i = 0; i < m.size(); i++) {
                    matches->push_back(m[i].str());
                }
            }
            return 1;
        }
        return 0;
    }

    inline unsigned long date_parse(const string& date_string) {
        if (date_string.empty()) return 0;

        struct tm time_info = {};
        int milliseconds = 0;
        
        int size = date_string.size();
        time_info.tm_year = (size > 3 ? stoi(date_string.substr(0, 4)) : 1970) - 1900;
        time_info.tm_mon = (size > 6 ? stoi(date_string.substr(5, 2)) : 1) - 1;
        time_info.tm_mday = size > 9 ? stoi(date_string.substr(8, 2)) : 1;
        time_info.tm_hour = size > 12 ? stoi(date_string.substr(11, 2)) : 0;
        time_info.tm_min = size > 15 ? stoi(date_string.substr(14, 2)) : 0;
        time_info.tm_sec = size > 18 ? stoi(date_string.substr(17, 2)) : 0;
        milliseconds = size > 22 ? stoi(date_string.substr(20, 3)) : 0;

        // Convert the struct tm to milliseconds
        unsigned long seconds = mktime(&time_info);
        return seconds * 1000 + milliseconds;
    }


    inline int exec(const string& command, string &output, bool throws = true)
    {
        array<char, 128> buffer;
        string result;
        FILE *pipe = popen((command + " 2>&1").c_str(), "r");
        // LCOV_EXCL_START
        if (!pipe) return -1;
        // LCOV_EXCL_STOP
        while (!feof(pipe))
        {
            if (fgets(buffer.data(), 128, pipe) != nullptr)
                result += buffer.data();
        }

        output = result;
        int status = pclose(pipe);
        if (throws && status) {
            // LCOV_EXCL_START
            throw ERROR("command execution failed:\n$ ", command,"\n", output);
            // LCOV_EXCL_STOP
        }
        return status;
    }

    inline vector<string> explode(char delimiter, const string& str) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
}