#include "time.h"
#include "inttypes.h"
#include "stdio.h"

enum {
    YEAR_COUNTDOWN = 1900,
};

int main(int argc, char **argv) {
    FILE *file = fopen(argv[1], "r");
    struct tm last_time;
    if (fscanf(file, "%04d/%02d/%02d %02d:%02d:%02d", &(last_time.tm_year), &(last_time.tm_mon),
           &(last_time.tm_mday), &(last_time.tm_hour), &(last_time.tm_min), &(last_time.tm_sec)) < 6) {
        return 0;
    }
    --last_time.tm_mon;
    last_time.tm_year -= YEAR_COUNTDOWN;
    last_time.tm_isdst = -1;
    time_t last_countdown = mktime(&last_time);
    while (fscanf(file, "%04d/%02d/%02d %02d:%02d:%02d", &(last_time.tm_year), &(last_time.tm_mon),
                  &(last_time.tm_mday), &(last_time.tm_hour), &(last_time.tm_min), &(last_time.tm_sec)) == 6) {
        --last_time.tm_mon;
        last_time.tm_year -= YEAR_COUNTDOWN;
        last_time.tm_isdst = -1;
        time_t cur_countdown = mktime(&last_time);
        printf("%ld\n", cur_countdown - last_countdown);
        last_countdown = cur_countdown;
    }
    fclose(file);
}