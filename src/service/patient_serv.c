#include <patient_serv.h>




#define T ServPatientDataPackage
T* Serv_patient_data(long long pat_id){

    List_T list = Data_get_patient();
    Patient_T tar = Patient_load(pat_id, MALE, -1LL, "", "", "");

    void* find_ptr = List_first(list);
    T* pkg = NULL;
    while(find_ptr != NULL){
        Patient_T pat = *(Patient_T*)find_ptr;

        if(Patient_cmp_id(&tar, find_ptr) == 0){
            pkg = (T*)safe_malloc(sizeof(T));
            pkg->gender = Patient_gender(pat); pkg->age = Patient_age(pat);
            strncpy(pkg->birthday, Time_to_string_date(Patient_birth_ts(pat)), 20);
            strncpy(pkg->name, Patient_name(pat), 32);
            strncpy(pkg->phone, Patient_phone(pat), 20);
            strncpy(pkg->id_card, Patient_id_card(pat), 20);
            break;
        }

        find_ptr = List_next(list);
    }
    Patient_free(&tar);
    return pkg;
}
#undef T


#define T ServRecordDataPackage
List_T Serv_patient_records(long long pat_id){
    List_T pat_records = List_new(sizeof(T));

    List_T list = Data_get_record();
    void* find_ptr = List_first(list);
    while(find_ptr != NULL){

        Record_T r = *(Record_T*)find_ptr;
        if(Rec_actor_id(r) == pat_id && Rec_type(r) <= REC_CHANGE_DOC){
            T* tmp = Serv_helper_record_to_pkg(r);
            List_push_back(pat_records, tmp);
            free(tmp);
        }
        find_ptr = List_next(list);
    }
    return pat_records;
}
#undef T


#define T ServSlotDataPackage
T Serv_patient_get_slot_status(long long doc_id, int target_date, int time_frame) {
    T pkg;
    pkg.time_frame = time_frame;
    pkg.registered = 0;

    if(time_frame == 0){
        pkg.capacity = INT_MAX; // 急诊不限制
    }
    else{
        pkg.capacity = 3; // 默认每个时段 3
    }

    List_T records = Data_get_record();
    void* find_ptr = List_first(records);
    while (find_ptr != NULL) {
        Record_T r = *(Record_T*)find_ptr;

        if (!Rec_is_invalid(r) && Rec_type(r) == REC_REGISTRATION){

            DataRegistration* data = (DataRegistration*)Rec_detail(r);
            if (data->doc_id == doc_id &&
                data->target_date == target_date &&
                data->time_frame == time_frame &&
                data->status != COMPLETED) {
                    pkg.registered++;
                }
        }
        find_ptr = List_next(records);
    }

    pkg.is_full = (pkg.registered >= pkg.capacity);
    return pkg;
}
List_T Serv_patient_day_slots(long long doc_id, int target_date) {
    List_T slot_list = List_new(sizeof(T));

    for (int i = 0; i < 17; i++){
        T tmp = Serv_patient_get_slot_status(doc_id, target_date, i);
        List_push_back(slot_list, &tmp);
    }
    return slot_list;
}


Status Serv_patient_register(long long doc_id, int target_date, int time_frame){

    T check = Serv_patient_get_slot_status(doc_id, target_date, time_frame);
    if (check.is_full) {
        return HIS_ERR_QUEUE_FULL;
    }

    int new_seq = check.registered + 1;

    Record_T r = Rec_reg_new(0, Serv_account_cur_id(), doc_id,
        new_seq, target_date, time_frame, APPOINTMENT);
    List_T records = Data_get_record();
    List_push_back(records, &r);

    return HIS_OK;
}
#undef T


#define T ServQueueStatusPackage

List_T Serv_patient_queue_status() {
    List_T result_list = List_new(sizeof(T));
    long long pat_id = Serv_account_cur_id();

    List_T all_doctors = Data_get_doctor();
    void* doc_ptr = List_first(all_doctors);
    while (doc_ptr != NULL) {

        Doctor_T doc = *(Doctor_T*)doc_ptr;
        long long doc_id = Doctor_id(doc);
        List_T queue = Rel_queue_get_all(doc_id);
        int pos = 1;
        int found_pos = -1;
        int total = List_size(queue);

        void* pkg_ptr = List_first(queue);
        while (pkg_ptr != NULL) {
            if (((RelQueueDataPackage*)pkg_ptr)->pat_id == pat_id) {
                found_pos = pos;
                break;
            }
            pos++;
            pkg_ptr = List_next(queue);
        }

        if (found_pos != -1) {
            T pkg;
            pkg.doc_id = doc_id;
            strncpy(pkg.doc_name, Doctor_name(doc), 32);
            pkg.position = found_pos;
            pkg.people_ahead = found_pos - 1;
            pkg.total_waiting = total;
            pkg.time_frame = ((RelQueueDataPackage*)pkg_ptr)->time_frame;
            pkg.sequence_no = ((RelQueueDataPackage*)pkg_ptr)->sequence_no;

            List_push_back(result_list, &pkg);
        }

        List_free(&queue);
        doc_ptr = List_next(all_doctors);
    }

    return result_list;
}
#undef T