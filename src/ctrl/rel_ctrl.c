#include <rel_ctrl.h>

static List_T Rel_doc_list;
static List_T Rel_ward_list;




void Rel_init(){
    Rel_doc_list  = List_new(sizeof(Rel_doc));
    Rel_ward_list = List_new(sizeof(Rel_ward));
}

void Rel_destroy(){
    List_free(& Rel_doc_list);
    List_free(& Rel_ward_list);
}




Status Rel_bind_doctor(long long patient_id, long long doctor_id){
    Rel_doc tmp = {patient_id, doctor_id};
    List_push_back(Rel_doc_list, &tmp);
    return HIS_OK;
}

Status Rel_unbind_doctor(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            List_remove(Rel_doc_list, tmp);
            return HIS_OK;
        }
        tmp = List_next(Rel_doc_list);
    }
    return HIS_ERR_NOT_FOUND;
}

long long Rel_get_doctor_by_patient(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return tmp->doc_id;
        }
        tmp = List_next(Rel_doc_list);
    }
    return HIS_ERR_NOT_FOUND;
}

List_T Rel_get_patients_by_doctor(long long doctor_id){
    List_T patients = List_new(sizeof(long long));
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->doc_id == doctor_id){
            List_push_back(patients, &(tmp->pat_id));
        }
        tmp = List_next(Rel_doc_list);
    }
    return patients;
}




Status Rel_bind_ward(long long patient_id, long long ward_id){
    Rel_ward tmp = {patient_id, ward_id};
    List_push_back(Rel_ward_list, &tmp);
    return HIS_OK;
}

Status Rel_unbind_ward(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            List_remove(Rel_ward_list, tmp);
            return HIS_OK;
        }
        tmp = List_next(Rel_ward_list);
    }
    return HIS_ERR_NOT_FOUND;
}

long long Rel_get_ward_by_patient(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return tmp->ward_id;
        }
        tmp = List_next(Rel_ward_list);
    }
    return HIS_ERR_NOT_FOUND;
}

List_T Rel_get_patients_by_ward(long long ward_id){
    List_T patients = List_new(sizeof(long long));
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->ward_id == ward_id){
            List_push_back(patients, &(tmp->pat_id));
        }
        tmp = List_next(Rel_ward_list);
    }
    return patients;
}




bool Rel_is_patient_admitted(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return true;
        }
        tmp = List_next(Rel_ward_list);
    }
    return false;
}

bool Rel_has_doctor(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return true;
        }
        tmp = List_next(Rel_doc_list);
    }
    return false;
}
