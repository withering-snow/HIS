#include <ward.h>

struct Ward_T {
    int id;
    int capacity;
    Department department;
    double cost;
    List_T beds;
};

// 生命周期
Ward_T Ward_new(int id, int capacity, Department department, double cost){}
void Ward_free(Ward_T *w){}

// 访问器
int Ward_get_id(Ward_T w){}
int Ward_get_capacity(Ward_T w){}
Department Ward_get_department(Ward_T w){}
double Ward_get_cost(Ward_T w){}

// 修改器
Status Ward_set_id(Ward_T w, int new_id){}
Status Ward_set_capacity(Ward_T w, int new_capacity){}
Status Ward_set_available(Ward_T w, int new_occupied){}
Status Ward_set_department(Ward_T w, Department new_department){}
Status Ward_set_cost(Ward_T w, double new_cost){}

// 床位管理
int Ward_get_occupied_count(Ward_T w){}
int Ward_get_occupied_id(Ward_T w, int index){}
Status Ward_bed_add(Ward_T w, int patient_id){}
Status Ward_bed_free(Ward_T w, int patient_id){}

// 比较器
int Ward_cmp_id(const void *a, const void *b){}
int Ward_cmp_department(const void *a, const void *b){}
int Ward_cmp_cost(const void *a, const void *b){}
int Ward_cmp_available(const void *a, const void *b){}
