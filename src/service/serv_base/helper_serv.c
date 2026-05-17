#include <helper_serv.h>




// =============================================================================
// 静态辅助数据
static const char* record_type_names[] =
{"挂号", "看诊", "检查", "买药", "住院", "出院", "床位变动", "医生变动", "进货", "废弃"};
static const char* time_frame_names[] =
{
    "急诊",
    "8:00-8:30", "8:30-9:00", "9:00-9:30", "9:30-10:00", "10:00-10:30", "10:30-11:00", "11:00-11:30", "11:30-12:00",
    "13:00-13:30", "13:30-14:00", "14:00-14:30", "14:30-15:00", "15:00-15:30", "15:30-16:00", "16:00-16:30", "16:30-17:00",
    "非法时段"
    };
static const char* registration_type_names[] =
{"预约", "候诊", "看诊中", "诊毕"};


const void * Serv_helper_finder(long long entity_id, ServEntityType type){
    List_T list = NULL;
    switch(type){
        case TYPE_ACCOUNT:  list = Data_get_account();  break;
        case TYPE_DOCTOR:   list = Data_get_doctor();   break;
        case TYPE_FUND:     list = Data_get_fund();     break;
        case TYPE_MEDICINE: list = Data_get_medicine(); break;
        case TYPE_PATIENT:  list = Data_get_patient();  break;
        case TYPE_RECORD:   list = Data_get_record();   break;
        case TYPE_WARD:     list = Data_get_ward();     break;
    }

    void* find_ptr = List_first(list);
    long long find_id = INVALID_ID;
    while(find_ptr != NULL){
        switch(type){
            case TYPE_ACCOUNT:  find_id = Account_id(*(Account_T*)find_ptr); break;
            case TYPE_DOCTOR:   find_id = Doctor_id(*(Doctor_T*)find_ptr);  break;
            case TYPE_FUND:     find_id = Fund_pat_id(*(Fund_T*)find_ptr); break;
            case TYPE_MEDICINE: find_id = Medicine_id(*(Medicine_T*)find_ptr); break;
            case TYPE_PATIENT:  find_id = Patient_id(*(Patient_T*)find_ptr); break;
            case TYPE_RECORD:   find_id = Rec_actor_id(*(Record_T*)find_ptr); break;
            case TYPE_WARD:     find_id = Ward_id(*(Ward_T*)find_ptr); break;
        }
        if(find_id == entity_id){
            return *(void**)find_ptr;
        }
        find_ptr = List_next(list);
    }
    return NULL;
}


const char* Serv_helper_id_to_name(long long id, ServEntityType type){
    if(id == 0){
        return "root";
    }
    switch(type){
        case TYPE_PATIENT:{
            List_T list = Data_get_patient();
            void* find_ptr = List_first(list);
            while(find_ptr != NULL){
                Patient_T pat = *(Patient_T *)find_ptr;
                if(id == Patient_id(pat)){
                    return Patient_name(pat);
                }
                find_ptr = List_next(list);
            }
            break;
        }
        case TYPE_DOCTOR:{
            List_T list = Data_get_doctor();
            void* find_ptr = List_first(list);
            while(find_ptr != NULL){
                Doctor_T doc = *(Doctor_T *)find_ptr;
                if(id == Doctor_id(doc)){
                    return Doctor_name(doc);
                }
                find_ptr = List_next(list);
            }
            break;
        }
        case TYPE_MEDICINE:{
            List_T list = Data_get_medicine();
            void* find_ptr = List_first(list);
            while(find_ptr != NULL){
                Medicine_T med = *(Medicine_T *)find_ptr;
                if(id == Medicine_id(med)){
                    return Medicine_name(med);
                }
                find_ptr = List_next(list);
            }
            break;
        }
        case TYPE_WARD:{
            List_T list = Data_get_ward();
            void* find_ptr = List_first(list);
            while(find_ptr != NULL){
                Ward_T ward = *(Ward_T *)find_ptr;
                if(id == Ward_id(ward)){
                    return Ward_name(ward);
                }
                find_ptr = List_next(list);
            }
            break;
        }
        default:
            break;
    }
    return "未知对象";
}


const char * Serv_helper_time_frame_to_name(long long time_frame){
    if(time_frame>=0 && time_frame<17){
        return time_frame_names[time_frame];
    }
    return time_frame_names[17];
}


#define T ServRecordDataPackage
T* Serv_helper_record_to_pkg(Record_T r){
    T* pkg = (T*)safe_malloc(sizeof(T));
    strncpy(pkg->type_name, record_type_names[Rec_type(r)], 16);
    pkg->is_invalid = Rec_is_invalid(r);
    pkg->actor_id = Rec_actor_id(r);
    strncpy(pkg->actor_name,
        Serv_helper_id_to_name(Rec_actor_id(r),
            (Rec_type(r)<REC_STOCK_IN) ? TYPE_PATIENT : TYPE_DOCTOR), 32);
    pkg->time_stamp = Rec_time_stamp(r);
    pkg->cost = Rec_cost(r);

    char buffer[512] = "";
    switch(Rec_type(r)){
        case REC_REGISTRATION:{
            DataRegistration* data = (DataRegistration*)Rec_detail(r);
            snprintf(buffer, 512, "[%s] [%s]医生 看诊日期[%d] 时段[%s] 挂号序号[%d]",
                registration_type_names[data->status],
                Serv_helper_id_to_name(data->doc_id, TYPE_DOCTOR),
                data->target_date, time_frame_names[data->time_frame], data->sequence_no);
            break;
        }

        case REC_CONSULTATION:{
            DataConsultation* data = (DataConsultation*)Rec_detail(r);
            snprintf(buffer, 512, "[%s]医生 诊断：%s 医嘱：%s",
                Serv_helper_id_to_name(data->doc_id, TYPE_DOCTOR),
                data->diagnosis, data->advice);
            break;
        }

        case REC_EXAMINATION:{
            DataExamination* data = (DataExamination*)Rec_detail(r);
            snprintf(buffer, 512, "[%s]医生 进行了[%s]检查",
                Serv_helper_id_to_name(data->doc_id, TYPE_DOCTOR),
                data->exam_name);
            break;
        }

        case REC_PRESCRIPTION:{
            DataPrescription* data = (DataPrescription*)Rec_detail(r);
            snprintf(buffer, 512, "[%s]医生 开具药品[%s][%d]件",
                Serv_helper_id_to_name(data->doc_id, TYPE_DOCTOR),
                Serv_helper_id_to_name(data->med_id, TYPE_MEDICINE), data->amount);
                break;
        }

        case REC_ADMISSION:{
            DataAdmission* data = (DataAdmission*)Rec_detail(r);
            snprintf(buffer, 512, "病房[%s] 病床号[%d] 共缴纳押金[%lld.%02lld]元",
                Serv_helper_id_to_name(data->ward_id, TYPE_WARD),
                data->bed_label, data->deposit/100, data->deposit%100);
            break;
        }

        case REC_DISCHARGE:{
            DataDischarge* data = (DataDischarge*)Rec_detail(r);
            long long refund = data->paid >= data->total_bill ? data->paid - data->total_bill : 0;
            snprintf(buffer, 512, "总住院开销[%lld.%02lld]元 实付[%lld.%02lld]元 退还押金[%lld.%02lld]元",
                data->total_bill/100, data->total_bill%100,
                data->paid/100, data->paid%100,
                refund/100, refund%100);
            break;
        }


        case REC_CHANGE_BED:{
            DataChangeBed* data = (DataChangeBed*)Rec_detail(r);
            snprintf(buffer, 512, "从[%s][%d]床 转至 [%s][%d]床",
                Serv_helper_id_to_name(data->from_ward_id, TYPE_WARD), data->from_bed_label,
                Serv_helper_id_to_name(data->to_ward_id, TYPE_WARD), data->to_bed_label);
            break;
        }

        case REC_CHANGE_DOC:{
            DataChangeDoc* data = (DataChangeDoc*)Rec_detail(r);
            snprintf(buffer, 512, "从[%s]医生 转至 [%s]医生",
                Serv_helper_id_to_name(data->old_doc_id, TYPE_DOCTOR),
                Serv_helper_id_to_name(data->new_doc_id, TYPE_DOCTOR));
            break;
        }

        case REC_STOCK_IN:{
            DataStockIn* data = (DataStockIn*)Rec_detail(r);
            snprintf(buffer, 512,
                "入库[%s]共[%d]件 批号[%s]\n于[%s]过期 购入单价[%lld.%02lld]元",
                Serv_helper_id_to_name(data->med_id, TYPE_MEDICINE),
                data->total, data->batch_no,
                Time_to_string_date(data->expire_ts), data->buy_price/100, data->buy_price%100);
            break;
        }

        case REC_STOCK_OUT:{
            DataStockOut* data = (DataStockOut*)Rec_detail(r);
            snprintf(buffer, 512, "报废[%s]共[%d]件 批号[%s]",
                Serv_helper_id_to_name(data->med_id, TYPE_MEDICINE),
                data->total, data->batch_no);
            break;
        }
    }

    snprintf(pkg->content, 512, "%s", buffer);
    return pkg;
}
#undef T
