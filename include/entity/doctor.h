#ifndef HIS_DOCTOR_H
#define HIS_DOCTOR_H

#include <base.h>

#define T Doctor_T
typedef struct Doctor_T *T;

// 生命周期
T Doctor_new(int id, int age, int sex, Department department, const char* name);
void Doctor_free(T* d);

// 访问器
int Doctor_get_id(T d);
int Doctor_get_age(T d);
int Doctor_get_sex(T d);
Department Doctor_get_department(T d);
const char* Doctor_get_name(T d);

// 修改器
Status Doctor_set_id(T d, int new_id);
Status Doctor_set_age(T d, int new_age);
Status Doctor_set_sex(T d, int new_sex);
Status Doctor_set_department(T d, Department new_department);
Status Doctor_set_name(T d, const char* new_name);

// 比较器
int Doctor_cmp_id(const void *a, const void *b);
int Doctor_cmp_age(const void *a, const void *b);
int Doctor_cmp_department(const void *a, const void *b);
int Doctor_cmp_name(const void *a, const void *b);

// 模糊姓名搜索比较器
int Doctor_cmp_fuzzy(const void *a, const void *b);

#undef T

#endif// HIS_DOCTOR_H
