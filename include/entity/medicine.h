#ifndef HIS_MEDICINE_H
#define HIS_MEDICINE_H

#include <base.h>

#define T Medicine_T
typedef struct Medicine_T *T;

// 生命周期
T Medicine_new(int id, int remain, double cost);
void Medicine_free(T* m);

// 访问器
int Medicine_get_id(T m);
int Medicine_get_remain(T m);
double Medicine_get_cost(T m);
const char* Medicine_get_name(T m);

// 修改器
Status Medicine_set_id(T m, int new_id);
Status Medicine_set_remain(T m, int new_remain);
Status Medicine_set_cost(T m, double new_cost);
Status Medicine_set_name(T m, const char* new_name);

// 比较器
int Medicine_cmp_id(const void* a, const void* b);

// 模糊药品名搜索比较器
int Medicine_cmp_fuzzy(const void* a, const void* b);

#undef T

#endif// HIS_MEDICINE_H
