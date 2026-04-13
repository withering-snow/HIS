#include <ward.h>

struct Ward_T {
    long long   id;
    Department  dept;
    long long   daily_cost;
    int         bed_count;
    int         empty_count;
    List_T      beds;
};
