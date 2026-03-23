#include <fund.h>

struct Fund_T {
    int patient_id;
    double remain;
};

// 生命周期
Fund_T Fund_new(int patient_id, double remain) {}
void Fund_free(Fund_T *f) {}

// 访问器
int Fund_get_patient_id(Fund_T *f) {}
double Fund_get_remain(Fund_T *f) {}

// 修改器
Status Fund_set_patient_id(Fund_T *f, int new_patient_id) {}
Status Fund_set_remain(Fund_T *f, int new_remain) {}

// 比较器
int Fund_cmp_patient_id(const void *a, const void *b) {}
