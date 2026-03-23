#include <medicine.h>

struct Medicine_T {
    int id;
    int remain;
    double cost;
    char name[32];
};

// 生命周期
Medicine_T Medicine_new(int id, int remain, double cost) {}
void Medicine_free(Medicine_T *m) {}

// 访问器
int Medicine_get_id(Medicine_T m) {}
int Medicine_get_remain(Medicine_T m) {}
double Medicine_get_cost(Medicine_T m) {}
const char *Medicine_get_name(Medicine_T m) {}

// 修改器
Status Medicine_set_id(Medicine_T m, int new_id) {}
Status Medicine_set_remain(Medicine_T m, int new_remain) {}
Status Medicine_set_cost(Medicine_T m, double new_cost) {}
Status Medicine_set_name(Medicine_T m, const char *new_name) {}

// 比较器
int Medicine_cmp_id(const void *a, const void *b) {}

// 模糊药品名搜索比较器
int Medicine_cmp_fuzzy(const void *a, const void *b) {}
