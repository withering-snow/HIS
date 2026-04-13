#include <medicine.h>

struct Medicine_T {
    long long   id;
    long long   cur_price;
    int         total_remain;
    char        name[32];
    List_T      batches;
};
