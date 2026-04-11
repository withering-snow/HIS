#ifndef HIS_PATIENT_H
#define HIS_PATIENT_H

#include <HIS_core.h>

// 匿名的主要实体
#define T Patient_T
typedef struct T *T;

// 医疗记录类型
typedef enum {
    REC_REGISTRATION, // 挂号
    REC_CONSULTATION, // 看诊
    REC_EXAMINATION,  // 检查
    REC_PRESCRIPTION, // 买药
    REC_ADMISSION,    // 住院
    REC_DISCHARGE,    // 出院
    REC_TRANSFER      // 变动
} RecordType;

// 1. 挂号详情
typedef struct {
    int dept_id;
    int doctor_id;
    int sequence_no; // 挂号序号
} DataRegistration;

// 2. 看诊详情
typedef struct {
    int doctor_id;
    char diagnosis[128]; // 诊断结果
    char advice[128];    // 医嘱
} DataConsultation;

// 3. 检查详情
typedef struct {
    char exam_name[64];  // 检查项名称
    double unit_cost;    // 单价
} DataExamination;

// 4. 开药详情
typedef struct {
    int medicine_id;
    int count;
} DataPrescription;

// 5. 入院详情
typedef struct {
    int ward_id;
    int bed_id;
    double deposit;      // 交纳的押金
} DataAdmission;

// 6. 床位变动
typedef struct {
    int from_ward_id;
    int to_ward_id;
    int from_bed_id;
    int to_bed_id;
} DataBedMove;

// 7. 医生变动
typedef struct {
    int old_doc_id;
    int new_doc_id;
} DataDocChange;

// 医疗记录实体
typedef struct MedicalRecord {
    long long time_stamp;
    RecordType type;
    double total_cost;   // 这一项操作产生的总费用 (挂号费、检查费等)
    union {
        DataRegistration reg;
        DataConsultation cons;
        DataExamination exam;
        DataPrescription drug;
        DataAdmission admit;
        DataBedMove move;
        DataDocChange change;
        char summary[256];       // 通用备注/出院小结
    } detail;
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
