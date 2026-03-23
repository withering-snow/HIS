#ifndef HIS_FUND_H
#define HIS_FUND_H

#include <base.h>

#define T Fund_T
typedef struct Fund_T *T;

// 生命周期
T Fund_new(int patient_id, double remain);
void Fund_free(T* f);

// 访问器
int Fund_get_patient_id(T* f);
double Fund_get_remain(T* f);

// 修改器
Status Fund_set_patient_id(T* f, int new_patient_id);
Status Fund_set_remain(T* f, int new_remain);

// 比较器
int Fund_cmp_patient_id(const void* a, const void* b);

#undef T

#endif// HIS_FUND_H
