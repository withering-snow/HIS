#ifndef HIS_WARD_H
#define HIS_WARD_H

#include <HIS_core.h>

#define T Ward_T
typedef struct Ward_T *T;

// 生命周期
T Ward_new(int id, int capacity, Department department, double cost);
void Ward_free(T *w);

// 访问器
int Ward_get_id(T w);
int Ward_get_capacity(T w);
Department Ward_get_department(T w);
double Ward_get_cost(T w);

// 修改器
Status Ward_set_id(T w, int new_id);
Status Ward_set_capacity(T w, int new_capacity);
Status Ward_set_department(T w, Department new_department);
Status Ward_set_cost(T w, double new_cost);

// 床位管理
int Ward_get_occupied_count(T w);
int Ward_get_occupied_id(T w, int index);
Status Ward_bed_add(T w, int patient_id);
Status Ward_bed_free(T w, int patient_id);

// 比较器
int Ward_cmp_id(const void *a, const void *b);
int Ward_cmp_department(const void *a, const void *b);
int Ward_cmp_cost(const void *a, const void *b);
int Ward_cmp_available(const void *a, const void *b);

#undef T

#endif// HIS_WARD_H
