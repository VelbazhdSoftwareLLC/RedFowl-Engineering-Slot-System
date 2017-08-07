#ifndef MANAGEMENT_UTIL_H_
#define MANAGEMENT_UTIL_H_

namespace management {

extern char* readbleTimeDifference(char *result, time_t start, time_t finish);
extern char* readbleDataTime(char *result, const time_t uts);

}

#endif
