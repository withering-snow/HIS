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
    REC_CHANGE_DOC,   // 医生变动
    REC_STOCK_IN,     // 进货
    REC_STOCK_OUT     // 废弃
} RecordType;

// 挂号详情
typedef enum{
    APPOINTMENT,      // 预约
    WAITING,          // 候诊
    IN_PROGRESS,      // 就诊
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
    long long   deposit;        // 交纳的押金
    int         bed_label;
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
    int         from_bed_label;
    int         to_bed_label;
} DataChangeBed;
// 医生变动
typedef struct {
    long long   old_doc_id;
    long long   new_doc_id;
} DataChangeDoc;
// 进货详情
typedef struct{
    long long   med_id;
    long long   batch_id;
    long long   buy_price;      // 进价（单价）
    long long   expire_ts;      // 过期时间
    int         total;          // 总购入量
    char        batch_no[32];   // 批号
}DataStockIn;
// 报废详情
typedef struct{
    long long   med_id;
    long long   batch_id;
    int         total;          // 总报废量
    char        batch_no[32];   // 批号
}DataStockOut;




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
    long long ward_id, int bed_label, long long deposit)
;
T Rec_disc_new(
    long long cost, long long pat_id,
    long long total_bill, long long paid)
;
T Rec_c_bed_new(
    long long cost, long long pat_id,
    long long from_ward_id, long long to_ward_id,
    int from_bed_label, int to_bed_label)
;
T Rec_c_doc_new(
    long long cost, long long pat_id,
    long long old_doc_id, long long new_doc_id)
;
T Rec_s_in_new(
    long long cost, long long admin_id,
    long long med_id, long long batch_id, long long buy_price,
    long long expire_ts, int total, const char* batch_no)
;
T Rec_s_out_new(
    long long cost, long long admin_id,
    long long med_id, long long batch_id, int total, const char* batch_no)
;
void Rec_free(T* r);


// 获取公共属性
RecordType Rec_type(T r);
bool       Rec_is_invalid(T r);
long long  Rec_actor_id(T r);
long long  Rec_time_stamp(T r);
long long  Rec_cost(T r);
// 返回详情区域的指针，外部根据 Rec_type(r) 强转为 DataRegistration* 等
void* Rec_detail(T r);


// 逻辑作废该条记录
void Rec_set_invalid(T r);

Status Rec_set_reg_status(Record_T r, RegistrationStatus new_status);



#undef T

#endif //HIS_RECORD_H
