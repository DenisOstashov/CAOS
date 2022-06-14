#include "pthread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

void fun0(int no);
void fun1();
int get_result_state();

_Atomic int flag = 1;

struct thread_info {
    pthread_t thread_id;
    int thread_num;
};

void *thread_start(void *arg) {
    int num;
    while (flag) {
        if (scanf("%d", &num) != 1) {
            flag = 0;
            break;
        }
        if (num == 1) {
            fun1();
        } else if (num != 0) {
            fun0(num);
        }
    }
    return NULL;
}

int main(int argc, char **argv)  {
    int n, k, id;
    sscanf(argv[1], "%d", &n);
    if (scanf("%d", &k) != 1) {
        return 1;
    }
    struct thread_info info[n];
    if (n > 1) {
        for (int i = 0; i < n; ++i) {
            info[i].thread_num = i;
            pthread_create(&info[i].thread_id, NULL, thread_start, NULL);
        }
        for (int i = 0; i < n; ++i) {
            pthread_join(info[i].thread_id, NULL);
        }
    } else {
        for (int i = 0; i < k; ++i) {
            if (scanf("%d", &id) != 1) {
                return 1;
            }
            if (id) {
                fun1();
            } else {
                if (scanf("%d", &id) != 1) {
                    return 1;
                }
                fun0(id);
            }
        }
    }
    printf("%d\n", get_result_state());
}
