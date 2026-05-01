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


List_T Serv_patient_records(long long pat_id){
}

Status Serv_patient_register(long long doc_id, int target_date, int time_frame){
}

#undef T
