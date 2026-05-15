#include <record.h>


// 医疗记录实体
struct Record_T {
    // 8 types
    long long       actor_id;   // 行为主体id
    long long       time_stamp;
    long long       cost;

    // union
    union {
        DataRegistration    reg;
        DataConsultation    cons;
        DataExamination     exam;
        DataPrescription    pres;
        DataAdmission       admit;
        DataDischarge       disc;
        DataChangeBed       c_bed;
        DataChangeDoc       c_doc;//前面的actor_id为pat_id
        DataStockIn         s_in;//后两个actor_id为doc_id
        DataStockOut        s_out;
    }               detail;

    // 4 bytes
    RecordType      type;

    // 1 byte
    bool            is_invalid; // 是否被废弃

};




// 生命周期
Record_T Rec_load(
    RecordType type, bool is_invalid, long long pat_id, long long time_stamp,
    long long cost, void* specific_data, size_t data_size){

    // 确保传入的数据大小不会撑破 Union
    ASSERT(data_size <= sizeof(((Record_T)0)->detail), "加载的医疗记录大小溢出");
    ASSERT(specific_data != NULL, "加载的医疗记录指针为空");

    // 分配内存
    Record_T r = safe_malloc(sizeof(struct Record_T));
    memset(r, 0, sizeof(struct Record_T));
    // 填充公共头部
    r->type = type;
    r->is_invalid = is_invalid;
    r->actor_id = pat_id;
    r->time_stamp = time_stamp;
    r->cost = cost;

    // 填充 Union 详情部分
    memcpy(&(r->detail), specific_data, data_size);
    return r;
}
Record_T Rec_reg_new(long long cost, long long pat_id, long long doc_id,
                     int sequence_no, int target_date, int time_frame, RegistrationStatus status) {
    DataRegistration data = {
        .doc_id = doc_id,
        .sequence_no = sequence_no,
        .target_date = target_date,
        .time_frame = time_frame,
        .status = status
    };
    return Rec_load(REC_REGISTRATION, false, pat_id,
        Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_cons_new(long long cost, long long pat_id, long long doc_id,
                      const char* diagnosis, const char* advice) {
    DataConsultation data = {.doc_id = doc_id};
    strncpy(data.diagnosis, diagnosis, 127);
    strncpy(data.advice, advice, 127);
    data.diagnosis[127] = '\0';
    data.advice[127] = '\0';
    return Rec_load(REC_CONSULTATION, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_exam_new(long long cost, long long pat_id, long long doc_id, const char* exam_name) {
    DataExamination data = {.doc_id = doc_id};
    strncpy(data.exam_name, exam_name, 63);
    data.exam_name[63] = '\0';
    return Rec_load(REC_EXAMINATION, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_pres_new(long long cost, long long pat_id, long long doc_id, long long med_id, int amount) {
    DataPrescription data = {.doc_id = doc_id, .med_id = med_id, .amount = amount};
    return Rec_load(REC_PRESCRIPTION, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_admit_new(long long cost, long long pat_id, long long ward_id,
    int bed_label, long long deposit) {
    DataAdmission data = {.ward_id = ward_id, .bed_label = bed_label, .deposit = deposit};
    return Rec_load(REC_ADMISSION, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_disc_new(long long cost, long long pat_id, long long total_bill, long long paid) {
    DataDischarge data = {.total_bill = total_bill, .paid = paid};
    return Rec_load(REC_DISCHARGE, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_c_bed_new(long long cost, long long pat_id,
                       long long from_ward_id, long long to_ward_id,
                       int from_bed_label, int to_bed_label) {
    DataChangeBed data = {
        .from_ward_id = from_ward_id,
        .to_ward_id = to_ward_id,
        .from_bed_label = from_bed_label,
        .to_bed_label = to_bed_label
    };
    return Rec_load(REC_CHANGE_BED, false,
        pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_c_doc_new(long long cost, long long pat_id, long long old_doc_id, long long new_doc_id) {
    DataChangeDoc data = {.old_doc_id = old_doc_id, .new_doc_id = new_doc_id};
    return Rec_load(REC_CHANGE_DOC, false, pat_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_s_in_new(long long cost, long long admin_id, long long med_id, long long batch_id, long long buy_price,
    long long expire_ts, int total, const char *batch_no){
    DataStockIn data = {
        .batch_id = batch_id,
        .buy_price = buy_price,
        .expire_ts = expire_ts,
        .med_id = med_id,
        .total = total
    };
    strncpy(data.batch_no, batch_no, 31);
    data.batch_no[31] = '\0';
    return Rec_load(REC_STOCK_IN, false,
        admin_id, Time_now(), cost, &data, sizeof(data));
}

Record_T Rec_s_out_new(long long cost, long long admin_id, long long med_id, long long batch_id, int total,
    const char *batch_no){
    DataStockOut data = {
        .batch_id = batch_id,
        .med_id = med_id,
        .total = total
    };
    strncpy(data.batch_no, batch_no, 31);
    data.batch_no[31] = '\0';
    return Rec_load(REC_STOCK_OUT, false,
        admin_id, Time_now(), cost, &data, sizeof(data));
}

void Rec_free(Record_T* r){
    ASSERT((r !=NULL),"不合法");
    free(*r);
    *r = NULL;
}

// 获取公共属性
RecordType Rec_type(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return r->type;
}

bool Rec_is_invalid(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return r->is_invalid;
}

long long Rec_actor_id(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return r->actor_id;
}

long long Rec_time_stamp(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return r->time_stamp;
}

long long Rec_cost(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return r->cost;
}




// 返回详情区域的指针，外部根据 Rec_type(r) 强转为 DataRegistration* 等
void * Rec_detail(Record_T r){
    ASSERT((r !=NULL),"不合法");
    return &(r->detail);
}




// 逻辑作废该条记录
void Rec_set_invalid(Record_T r){
    ASSERT((r !=NULL),"不合法");
    r->is_invalid = true;
}

Status Rec_set_reg_status(Record_T r, RegistrationStatus new_status)
{
    ASSERT((r !=NULL),"不合法");
    if (r->type != REC_REGISTRATION) {
        return HIS_ERR_STATUS_ERROR;
    }
    if (new_status < APPOINTMENT || new_status > COMPLETED) {
        return HIS_ERR_INVALID_STATUS;
    }
    r->detail.reg.status = (RegistrationStatus)new_status;
    return HIS_OK;
}

//TODO：以下作废

// 将记录转换为字符串描述（比如显示 "患者A 挂号 医生B 费用10元"）
void Rec_to_string(Record_T r, char *buf, size_t len){
    ASSERT(r && buf, "指针或缓冲区为空");
    // TODO: 外部提供 buffer 空间，内部填充
    // 先格式化公共头部：时间、ID、类型
    // 然后根据 switch(r->type) 格式化不同的 detail 部分
    // 建议使用 snprintf 以防止 buffer 溢出
}
