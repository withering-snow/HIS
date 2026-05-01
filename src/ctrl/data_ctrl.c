#include <data_ctrl.h>




static List_T Account_list;
static List_T Doctor_list;
static List_T Fund_list;
static List_T Medicine_list;
static List_T Patient_list;
static List_T Record_list;
static List_T Ward_list;




// 生命周期：全部实体链表构建与释放
void Data_init() {
    Account_list    = List_new(sizeof(Account_T));
    Doctor_list     = List_new(sizeof(Doctor_T));
    Fund_list       = List_new(sizeof(Fund_T));
    Medicine_list   = List_new(sizeof(Medicine_T));
    Patient_list    = List_new(sizeof(Patient_T));
    Record_list     = List_new(sizeof(Record_T));
    Ward_list       = List_new(sizeof(Ward_T));
}




void Data_destroy() {
    void* acc_ptr = List_first(Account_list);
    while (acc_ptr != NULL) {
        Account_T acc = *(Account_T *)acc_ptr;
        Account_free(&acc);
        acc_ptr = List_next(Account_list);
    }
    List_free(& Account_list);

    void* doc_ptr = List_first(Doctor_list);
    while (doc_ptr != NULL){
        Doctor_T doc = *(Doctor_T *)doc_ptr;
        Doctor_free(&doc);
        doc_ptr = List_next(Doctor_list);
    }
    List_free(& Doctor_list);

    void* fun_ptr = List_first(Fund_list);
    while (fun_ptr != NULL){
        Fund_T fun = *(Fund_T *)fun_ptr;
        Fund_free(&fun);
        fun_ptr = List_next(Fund_list);
    }
    List_free(& Fund_list);

    void* med_ptr = List_first(Medicine_list);
    while (med_ptr != NULL){
        Medicine_T med = *(Medicine_T *)med_ptr;
        Medicine_free(&med);
        med_ptr = List_next(Medicine_list);
    }
    List_free(& Medicine_list);

    void* pat_ptr = List_first(Patient_list);
    while (pat_ptr != NULL){
        Patient_T pat = *(Patient_T *)pat_ptr;
        Patient_free(&pat);
        pat_ptr = List_next(Patient_list);
    }
    List_free(& Patient_list);

    void* rec_ptr = List_first(Record_list);
    while (rec_ptr != NULL){
        Record_T rec = *(Record_T *)rec_ptr;
        Rec_free(&rec);
        rec_ptr = List_next(Record_list);
    }
    List_free(& Record_list);

    void* war_ptr = List_first(Ward_list);
    while (war_ptr != NULL){
        Ward_T war = *(Ward_T *)war_ptr;
        Ward_free(&war);
        war_ptr = List_next(Ward_list);
    }
    List_free(& Ward_list);
}




// 获取对应链表
List_T Data_get_account(){
    return Account_list;
}
List_T Data_get_doctor() {
    return Doctor_list;
}
List_T Data_get_fund() {
    return Fund_list;
}
List_T Data_get_medicine() {
    return Medicine_list;
}
List_T Data_get_patient() {
    return Patient_list;
}
List_T Data_get_record(){
    return Record_list;
}
List_T Data_get_ward() {
    return Ward_list;
}
