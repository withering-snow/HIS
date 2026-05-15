#include <doctor_serv.h>




static bool _cur_is_active = false;
static Rel_doc _cur_doc_pat = {
    INVALID_ID, INVALID_ID
};




Status Serv_doc_active(){
    if(_cur_is_active){
        return HIS_ERR_ALREADY_EXISTS;
    }
    _cur_is_active = true;
    _cur_doc_pat.doc_id = Serv_account_cur_id();
    return HIS_OK;
}


Status Serv_doc_unactive(){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    _cur_is_active = false;
    _cur_doc_pat.doc_id = INVALID_ID;
    return HIS_OK;
}




Status Serv_doc_call_reg(){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    Status s = Rel_queue_call_reg(_cur_doc_pat.doc_id);
    _cur_doc_pat.pat_id = Rel_queue_cur_pat(_cur_doc_pat.doc_id);
    return s;
}


Status Serv_doc_end_reg(){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    if(_cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NOT_FOUND;
    }
    Status s = Rel_queue_end_reg(_cur_doc_pat.doc_id);
    _cur_doc_pat.pat_id = INVALID_ID;
    return s;
}


long long Serv_doc_cur_reg_id(){
    if(!_cur_is_active){
        return INVALID_ID;
    }
    return _cur_doc_pat.pat_id;
}




Status Serv_doc_consult(const char *diagnosis, const char *advice){

}

Status Serv_doc_exam(long long cost, const char *exam_name){
}

Status Serv_doc_prescribe(const char *med_name, int amount){
}

Status Serv_doc_admission(long long ward_id, int bed_label, long long deposit){
}

Status Serv_doc_discharge(long long pat_id){
}

Status Serv_doc_change_bed(long long pat_id, const char *to_ward_name, int to_bed_label){
}

Status Serv_doc_change_doc(long long pat_id, long long to_doc_id){
}

