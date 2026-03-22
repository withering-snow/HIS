#include <patient.h>

struct Patient_T {
    int id;
    int age;
    int sex;
    char name[32];
    List_T record;
};

// 生命周期
Patient_T Patient_new(int id, int age, int sex, const char *name) {}
void Patient_free(Patient_T *p) {}

// 访问成员
int Patient_get_id(Patient_T p) {}
int Patient_get_age(Patient_T p) {}
int Patient_get_sex(Patient_T p) {}
char *Patient_get_name(Patient_T p) {}

// 修改成员
Status Patient_set_id(Patient_T p, int new_id) {}
Status Patient_set_age(Patient_T p, int new_age) {}
Status Patient_set_sex(Patient_T p, int new_sex) {}
Status Patient_set_name(Patient_T p, const char *new_name) {}

// 对医疗记录的操作
int Patient_get_record_count(Patient_T p) {}
const struct MedicalRecord *Patient_get_record(Patient_T p, int index) {}
Status Patient_add_record(Patient_T p, const struct MedicalRecord *new_record) {}
Status Patient_set_record(Patient_T p, const struct MedicalRecord *new_record) {}

// 比较器
int Patient_cmp_id(const void *a, const void *b){}
int Patient_cmp_age(const void *a, const void *b){}
int Patient_cmp_name(const void *a, const void *b){}
int Patient_cmp_record(const void *a, const void *b){}

// 模糊姓名搜索器
int Patient_cmp_fuzzy(const void *a, const void *b){}
