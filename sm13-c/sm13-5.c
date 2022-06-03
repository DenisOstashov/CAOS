#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "stdlib.h"
#include "inttypes.h"
#include "string.h"
#include "sys/wait.h"
#include "limits.h"
#include "ctype.h"

int main(int argc, char** argv) {
    int first_channel[2], second_channel[2], wstatus, max, value = 1;
    pipe(first_channel);
    pipe(second_channel);
    pid_t first, second;
    sscanf(argv[1], "%d", &max);
    if ((first = fork()) == 0) {
        close(first_channel[1]);
        close(second_channel[0]);
        FILE *input = fdopen(first_channel[0], "r");
        FILE *output = fdopen(second_channel[1], "w");
        while (fscanf(input, "%d", &value) == 1 && value != max) {
            printf("1 %d\n", value);
            fflush(stdout);
            fprintf(output, "%d\n", value + 1);
            fflush(output);
        }
        fclose(input);
        fclose(output);
        close(first_channel[0]);
        close(second_channel[1]);
        return 0;
    }
    close(first_channel[0]);
    close(second_channel[1]);
    if ((second = fork()) == 0) {
        FILE *input = fdopen(second_channel[0], "r");
        FILE *output = fdopen(first_channel[1], "w");
        while (fscanf(input, "%d", &value) == 1 && value != max) {
            printf("2 %d\n", value);
            fflush(stdout);
            fprintf(output, "%d\n", value + 1);
            fflush(output);
        }
        fclose(input);
        fclose(output);
        close(first_channel[1]);
        close(second_channel[0]);
        return 0;
    }
    FILE *output = fdopen(first_channel[1], "w");
    fprintf(output, "%d\n", value);
    fflush(output);
    fclose(output);
    close(first_channel[1]);
    close(second_channel[0]);
    wait(NULL);
    wait(NULL);
    puts("Done");
    return 0;
}