#include <ctime>
#include <string>
#include <cstdio>

#include "Util.h"

namespace management {

char* readbleTimeDifference(char *result, time_t start, time_t finish) {
	/*
	 * If the difference is greater than one day do not show it.
	 */
	if(difftime(finish,start) < (24*60*60)) {
		finish -= start;
	} else {
		start = finish;
	}
	struct tm *timeInfo = gmtime(&finish);
	strftime(result, 20, "%H:%M:%S", timeInfo);
	return( result );
}

char* readbleDataTime(char *result, const time_t uts) {
	struct tm *time = localtime(&uts);
	strftime(result, 20, "%d.%m.%Y %H:%M:%S", time);
	return( result );
}

}
