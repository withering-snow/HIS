#include <record.h>

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