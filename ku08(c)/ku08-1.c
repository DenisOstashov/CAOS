#include "pthread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

bool activate_service(size_t user_id, void* service);

bool place_order(_Atomic unsigned accounts[], size_t user_id, unsigned price, void* service) {
    if (price == 0) {
        return activate_service(user_id, service);
    }
    int success = 0;
    unsigned expected = accounts[user_id], delta;
    while (expected >= price) {
        delta = expected - price;
        if (atomic_compare_exchange_strong(accounts + user_id, &expected, delta)) {
            success = 1;
            break;
        }
    }
    if (!success) {
        return false;
    }
    if (activate_service(user_id, service)) {
        return true;
    }
    while (!atomic_compare_exchange_strong(accounts + user_id, &expected, expected + delta)) {}
    return false;
}