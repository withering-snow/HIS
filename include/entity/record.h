#ifndef HIS_RECORD_H
#define HIS_RECORD_H

#include <HIS_core.h>

#define T Record_T
typedef struct T *T;




// 医疗记录类型
typedef enum {
    REC_REGISTRATION, // 挂号
    REC_CONSULTATION, // 看诊
    REC_EXAMINATION,  // 检查
    REC_PRESCRIPTION, // 买药
    REC_ADMISSION,    // 住院
    REC_DISCHARGE,    // 出院
    REC_CHANGE_BED,   // 床位变动
    REC_CHANGE_DOC    // 医生变动
} RecordType;

// 挂号详情
typedef enum{
    APPOINTMENT,      // 预约
    WAITING,          // 候诊
    COMPLETED         // 诊毕
}RegistrationStatus;  // 挂号状态
typedef struct {
    long long   doc_id;
    int         sequence_no;    // 挂号序号
    int         target_date;    // 挂号目标日期（注意是形如“20260412”的int类型，还请注意转换）
    int         time_frame;     // 挂号时段（后续以索引形式分割不同挂号时段）
    RegistrationStatus
                status;         // 当前挂号状态
} DataRegistration;
// 看诊详情
typedef struct {
    long long   doc_id;
    char        diagnosis[128]; // 诊断结果
    char        advice[128];    // 医嘱
} DataConsultation;
// 检查详情
typedef struct {
    long long   doc_id;
    char        exam_name[64];  // 检查项名称
} DataExamination;
// 开药详情
typedef struct {
    long long   doc_id;
    long long   med_id;
    int         amount;         // 开药数量
} DataPrescription;
// 入院详情
typedef struct {
    long long   ward_id;
    long long   bed_id;
    long long   deposit;        // 交纳的押金
} DataAdmission;
// 出院详情
typedef struct{
    long long   total_bill;     // 总支出
    long long   paid;           // 已支付的押金
} DataDischarge;
// 床位变动
typedef struct {
    long long   from_ward_id;
    long long   to_ward_id;
    long long   from_bed_id;
    long long   to_bed_id;
} DataChangeBed;
// 医生变动
typedef struct {
    long long   old_doc_id;
    long long   new_doc_id;
} DataChangeDoc;




// 生命周期
T Rec_load(
    RecordType type, bool is_invalid, long long pat_id, long long time_stamp,
    long long cost, void* specific_data, size_t data_size)
;
T Rec_reg_new(
    long long cost, long long pat_id,
    long long doc_id, int sequence_no, int target_date, int time_frame, RegistrationStatus status);
;
T Rec_cons_new(
    long long cost, long long pat_id,
    long long doc_id, const char* diagnosis, const char* advice)
;
T Rec_exam_new(
    long long cost, long long pat_id,
    long long doc_id, const char* exam_name)
;
T Rec_pres_new(
    long long cost, long long pat_id,
    long long doc_id, long long med_id, int amount)
;
T Rec_admit_new(
    long long cost, long long pat_id,
    long long ward_id, long long bed_id, long long deposit)
;
T Rec_disc_new(
    long long cost, long long pat_id,
    long long total_bill, long long paid)
;
T Rec_c_bed_new(
    long long cost, long long pat_id,
    long long from_ward_id, long long to_ward_id,
    long long from_bed_id, long long to_bed_id)
;
T Rec_c_doc_new(
    long long cost, long long pat_id,
    long long old_doc_id, long long new_doc_id)
;
void Rec_free(T* r);


// 获取公共属性
RecordType Rec_get_type(T r);
bool       Rec_get_is_invalid(T r);
long long  Rec_get_pat_id(T r);
long long  Rec_get_time_stamp(T r);
long long  Rec_get_cost(T r);
// 返回详情区域的指针，外部根据 Rec_type(r) 强转为 DataRegistration* 等
void* Rec_detail(T r);


// 逻辑作废该条记录
void Rec_set_invalid(T r);


// 将记录转换为字符串描述（比如显示 "患者A 挂号 医生B 费用10元"）
void Rec_to_string(T r, char *buf, size_t len);


#undef T

#endif //HIS_RECORD_H
