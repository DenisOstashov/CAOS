#include "time.h"
#include "inttypes.h"
#include "stdio.h"

enum {
    SEC_IN_DAY = 86400,
    YEAR_COUNTDOWN = 1900,
};

int main() {
    time_t timep;
    time(&timep);
    int32_t value;
    while (scanf("%" SCNd32 "", &value) == 1) {
        int64_t diff = SEC_IN_DAY * (int64_t) value;
        int64_t seconds = (int64_t)timep + diff;
        if (diff > INT32_MAX || diff < INT32_MIN || seconds > INT32_MAX || seconds < INT32_MIN) {
            puts("OVERFLOW");
        } else {
            time_t tp = (time_t) seconds;
            struct tm *cur_time = localtime(&tp);
            printf("%04d-%02d-%02d\n", cur_time->tm_year + YEAR_COUNTDOWN, cur_time->tm_mon + 1, cur_time->tm_mday);
        }
    }
}