#include "time.h"
#include "inttypes.h"
#include "stdio.h"

enum {
    YEAR_COUNTDOWN = 1900,
    SEC_IN_WEAK = 604800,
    FRIDAY = 4,
};

int main() {
    struct tm last_time;
    while (scanf("%04d/%02d/%02d", &(last_time.tm_year),
                 &(last_time.tm_mon), &(last_time.tm_mday)) == 3) {
        --last_time.tm_mon;
        last_time.tm_year -= YEAR_COUNTDOWN;
        last_time.tm_isdst = -1;
        last_time.tm_hour = 0;
        last_time.tm_min = 0;
        last_time.tm_sec = 0;
        time_t last_countdown = mktime(&last_time);
        last_countdown += 2 * SEC_IN_WEAK;
        struct tm *deadline_time = localtime(&last_countdown);
        if (deadline_time->tm_wday - 1 != FRIDAY) {
            puts("ERROR");
        } else {
            printf("%04d/%02d/%02d 23:59:59\n", deadline_time->tm_year + YEAR_COUNTDOWN,
                   deadline_time->tm_mon + 1, deadline_time->tm_mday - 1);
        }
    }
}
