#ifndef HIS_PATIENT_H
#define HIS_PATIENT_H

#include <HIS_core.h>

// 匿名的主要实体
#define T Patient_T
typedef struct T *T;

// 医疗记录类型
typedef enum {
    REC_REGISTRATION,// 挂号
    REC_CONSULTATION,// 看诊
    REC_EXAMINATION,// 检查
    REC_HOSPITAL// 住院
} RecordType;

// 医疗记录实体
typedef struct MedicalRecord {
    long long time_stamp;
    RecordType type;
    int doctor_id;
    double cost;
    char detail[256];
} MedicalRecord;

// 生命周期
T Patient_new(int id, int age, int sex, const char *name);
void Patient_free(T *p);

// 访问器
int Patient_get_id(T p);
int Patient_get_age(T p);
int Patient_get_sex(T p);
char *Patient_get_name(T p);

// 修改器
Status Patient_set_id(T p, int new_id);
Status Patient_set_age(T p, int new_age);
Status Patient_set_sex(T p, int new_sex);
Status Patient_set_name(T p, const char *new_name);

// 医疗记录管理
int Patient_get_record_count(T p);
const struct MedicalRecord *Patient_get_record(T p, int index);
Status Patient_add_record(T p, const struct MedicalRecord *new_record);
Status Patient_set_record(T p, const struct MedicalRecord *new_record);

// 比较器
int Patient_cmp_id(const void *a, const void *b);
int Patient_cmp_age(const void *a, const void *b);
int Patient_cmp_name(const void *a, const void *b);
int Patient_cmp_record(const void *a, const void *b);

// 模糊姓名搜索比较器
int Patient_cmp_fuzzy(const void *a, const void *b);

#undef T

#endif// HIS_PATIENT_H
