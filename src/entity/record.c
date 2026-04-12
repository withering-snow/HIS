#include <record.h>


// 医疗记录实体
struct Record_T {
    RecordType      type;
    bool            is_invalid;
    long long       pat_id;
    long long       time_stamp;
    long long       cost;
    union {
        DataRegistration    reg;
        DataConsultation    cons;
        DataExamination     exam;
        DataPrescription    pres;
        DataAdmission       admit;
        DataDischarge       disc;
        DataChangeBed       c_bed;
        DataChangeDoc       c_doc;
    } detail;
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

    // 填充公共头部
    r->type = type;
    r->is_invalid = is_invalid;
    r->pat_id = pat_id;
    r->time_stamp = time_stamp;
    r->cost = cost;

    // 填充 Union 详情部分
    memcpy(&(r->detail), specific_data, data_size);
    return r;
}
Record_T Rec_reg_new(
    long long cost, long long pat_id,
    long long doc_id, int sequence_no, int target_date, int time_frame, RegistrationStatus status){

}
Record_T Rec_cons_new(
    long long cost, long long pat_id,
    long long doc_id, const char* diagnosis, const char* advice){

}
Record_T Rec_exam_new(
    long long cost, long long pat_id,
    long long doc_id, const char* exam_name){

}
Record_T Rec_pres_new(
    long long cost, long long pat_id,
    long long doc_id, long long med_id, int amount){

}
Record_T Rec_admit_new(
    long long cost, long long pat_id,
    long long ward_id, long long bed_id, long long deposit){

}
Record_T Rec_disc_new(
    long long cost, long long pat_id,
    long long total_bill, long long paid){

}
Record_T Rec_c_bed_new(
    long long cost, long long pat_id,
    long long from_ward_id, long long to_ward_id,
    long long from_bed_id, long long to_bed_id){

}
Record_T Rec_c_doc_new(
    long long cost, long long pat_id,
    long long old_doc_id, long long new_doc_id){

}
void Rec_free(Record_T* r){

}




// 获取公共属性
RecordType Rec_type(Record_T r){

}

bool Rec_is_invalid(Record_T r){

}

long long Rec_pat_id(Record_T r){

}

long long Rec_time_stamp(Record_T r){

}

long long Rec_cost(Record_T r){

}




// 返回详情区域的指针，外部根据 Rec_type(r) 强转为 DataRegistration* 等
void * Rec_detail(Record_T r){

}




// 逻辑作废该条记录
void Rec_set_invalid(Record_T r){

}




// 将记录转换为字符串描述（比如显示 "患者A 挂号 医生B 费用10元"）
void Rec_to_string(Record_T r, char *buf, size_t len){
    ASSERT(r && buf, "指针或缓冲区为空");
    // TODO: 外部提供 buffer 空间，内部填充
    // 先格式化公共头部：时间、ID、类型
    // 然后根据 switch(r->type) 格式化不同的 detail 部分
    // 建议使用 snprintf 以防止 buffer 溢出
}
