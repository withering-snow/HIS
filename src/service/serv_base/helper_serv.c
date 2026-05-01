#include <helper_serv.h>




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
        default:
            break;
    }
    return "未知对象";
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
            snprintf(buffer, 512, "[%s]医生\n诊断： %s\n医嘱： %s",
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
            // TODO: ward_name required
            // TODO: bed_id in record should be type int
            snprintf(buffer, 512, "病房号[%s] 病床号[%d] 共缴纳押金[%lld.%02lld]元",
                "ward_name", data->bed_id, data->deposit/100, data->deposit%100);
            break;
        }

        case REC_DISCHARGE:{
            DataDischarge* data = (DataDischarge*)Rec_detail(r);
            snprintf(buffer, 512, "总住院开销[%lld.%02lld]元 已支付押金[%lld.%02lld]元",
                data->total_bill/100, data->total_bill%100, data->paid/100, data->paid%100);
            break;
        }

        // TODO: ward_name required
        // TODO: bed_id in record should be type int
        case REC_CHANGE_BED:{
            DataChangeBed* data = (DataChangeBed*)Rec_detail(r);
            snprintf(buffer, 512, "从[%s][%d]床 转至 [%s][%d]床",
                "from_ward_name", data->from_bed_id,
                "to_ward_name", data->to_bed_id);
            break;
        }

        case REC_CHANGE_DOC:{
            DataChangeDoc* data = (DataChangeDoc*)Rec_detail(r);
            snprintf(buffer, 512, "从[%s]医生 转至 [%s]医生",
                Serv_helper_id_to_name(data->old_doc_id, TYPE_DOCTOR),
                Serv_helper_id_to_name(data->new_doc_id, TYPE_DOCTOR));
            break;
        }

        // TODO: 谁tm教的你英语，回头改掉，是 stock in
        case REC_STOCK_IN:{
            DataStackIn* data = (DataStackIn*)Rec_detail(r);
            snprintf(buffer, 512,
                "入库[%s]共[%d]件 批号[%s]\n于[%s]过期 购入单价[%lld.%02lld]元",
                Serv_helper_id_to_name(data->med_id, TYPE_MEDICINE),
                data->total, data->batch_no,
                Time_to_string_date(data->expire_ts), data->buy_price/100, data->buy_price%100);
            break;
        }

        case REC_STOCK_OUT:{
            DataStackOut* data = (DataStackOut*)Rec_detail(r);
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
