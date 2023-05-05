#pragma once

#include <string>
#include "Clock.h"

using namespace std;

namespace lib {

	inline string datef(long ms = 0, const char* fmt = "%Y-%m-%d %H:%M:%S", bool millis = true, bool local = false) {
		if (!ms) {
		    Clock clock;
		    ms = clock.now();
		}
		
		long sec = ms / 1000;
		long mil = ms % 1000;

		struct tm converted_time;
		if (local)
		{
		    localtime_r(&sec, &converted_time);
		}
		else
		{
		    gmtime_r(&sec, &converted_time);
		}

		char time_sbuff[26];
		strftime(time_sbuff, 26, fmt, &converted_time);

		if (millis) {
		    char mil_sbuff[10];
		    sprintf(mil_sbuff, "%03ld", mil);

		    char out_sbuff[42];
		    sprintf(out_sbuff, "%s.%s", time_sbuff, mil_sbuff);
		    return string(out_sbuff);
		}

		return string(time_sbuff);
	}

	inline string datefYMD(long ms = 0, bool local = false) {
		return datef(ms, "%Y-%m-%d", false, local);
	}

}

