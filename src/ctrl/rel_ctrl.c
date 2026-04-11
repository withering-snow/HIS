#include <relation_manager.h>

struct Rel_doc {
    int patient_id;
    int doctor_id;
};

struct Rel_ward {
    int patient_id;
    int ward_id;
    int bed_id;
    long long time_stamp;
};

static List_T Rel_doc_list;
static List_T Rel_ward_list;

// 生命周期：全部关系链表构建与释放
void Rel_init() {
    Rel_doc_list = List_new(sizeof(struct Rel_doc));
    Rel_ward_list = List_new(sizeof(struct Rel_ward));
}
void Rel_destroy() {
    List_free(& Rel_doc_list);
    List_free(& Rel_ward_list);
}

// 关系管理
int Rel_get_doctor_by_patient(int patient_id) {
    Rel_doc* tmp;
    if (List_first(Rel_doc_list) != NULL) {

        for(tmp=(Rel_doc*)List_first(Rel_doc_list);
            tmp != NULL;
            tmp = (Rel_doc*)List_next(Rel_doc_list))
        {
            if (tmp->patient_id == patient_id) {
                return tmp->doctor_id;
            }
        }
    }
    return HIS_ERR_NOT_FOUND;
}
List_T Rel_get_patient_by_doctor(int doctor_id) {
    Rel_doc* tmp; List_T result = List_new(sizeof(int));
    if (List_first(Rel_doc_list) != NULL) {

        for(tmp=(Rel_doc*)List_first(Rel_doc_list);
            tmp != NULL;
            tmp = (Rel_doc*)List_next(Rel_doc_list))
        {
            if (tmp->doctor_id == doctor_id) {
                List_push_back(result, &(tmp->patient_id));
            }
        }
    }
    return result;
}

int Rel_get_ward_by_patient(int patient_id) {
    Rel_ward* tmp;
    if (List_first(Rel_ward_list) != NULL) {

        for(tmp=(Rel_ward*)List_first(Rel_ward_list);
            tmp != NULL;
            tmp = (Rel_ward*)List_next(Rel_ward_list))
        {
            if (tmp->patient_id == patient_id) {
                return tmp->ward_id;
            }
        }
    }
    return HIS_ERR_NOT_FOUND;
}
int Rel_get_bed_by_patient(int patient_id) {
    Rel_ward* tmp;
    if (List_first(Rel_ward_list) != NULL) {

        for(tmp=(Rel_ward*)List_first(Rel_ward_list);
            tmp != NULL;
            tmp = (Rel_ward*)List_next(Rel_ward_list))
        {
            if (tmp->patient_id == patient_id) {
                return tmp->bed_id;
            }
        }
    }
    return HIS_ERR_NOT_FOUND;
}
long long Rel_get_time_stamp_by_patient(int patient_id) {
    Rel_ward* tmp;
    if (List_first(Rel_ward_list) != NULL) {

        for(tmp=(Rel_ward*)List_first(Rel_ward_list);
            tmp != NULL;
            tmp = (Rel_ward*)List_next(Rel_ward_list))
        {
            if (tmp->patient_id == patient_id) {
                return tmp->time_stamp;
            }
        }
    }
    return HIS_ERR_NOT_FOUND;
}

Status Rel_band_doc(int patient_id, int doctor_id) {
    List_push_back(Rel_doc_list, &(struct Rel_doc){patient_id, doctor_id});
    return HIS_OK;
}
Status Rel_band_ward(int patient_id, int ward_id, int bed_id, long long time_stamp) {
    List_push_back(Rel_ward_list, &(struct Rel_ward){patient_id, ward_id, bed_id, time_stamp});
    return HIS_OK;
}

Status Rel_remove_doc(int patient_id) {
    Rel_doc* tmp; bool has_this_patient = false;
    if (List_first(Rel_doc_list) != NULL) {

        for(tmp=(Rel_doc*)List_first(Rel_doc_list);
            tmp != NULL;
            tmp = (Rel_doc*)List_next(Rel_doc_list))
        {
            if (tmp->patient_id == patient_id) {
                has_this_patient = true;
                List_remove(Rel_doc_list, &tmp->patient_id);
            }
        }
    }
    return has_this_patient? HIS_OK: HIS_ERR_NOT_FOUND;
}
Status Rel_remove_ward(int patient_id) {
    Rel_ward* tmp; bool has_this_patient = false;
    if (List_first(Rel_ward_list) != NULL) {

        for(tmp=(Rel_ward*)List_first(Rel_ward_list);
            tmp != NULL;
            tmp = (Rel_ward*)List_next(Rel_ward_list))
        {
            if (tmp->patient_id == patient_id) {
                has_this_patient = true;
                List_remove(Rel_ward_list, &tmp->patient_id);
            }
        }
    }
    return has_this_patient? HIS_OK: HIS_ERR_NOT_FOUND;
}
