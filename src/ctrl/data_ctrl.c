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
    Account_T acc = (Account_T)List_first(Account_list);
    while (acc != NULL) {
        Account_free(&acc);
        acc = (Account_T)List_next(Account_list);
    }
    List_free(&Account_list);

    Doctor_T doc = (Doctor_T)List_first(Doctor_list);
    while (doc != NULL){
        Doctor_free(&doc);
        doc = (Doctor_T)List_next(Doctor_list);
    }
    List_free(& Doctor_list);

    Fund_T fun = (Fund_T)List_first(Fund_list);
    while (fun != NULL){
        Fund_free(&fun);
        fun = (Fund_T)List_next(Fund_list);
    }
    List_free(& Fund_list);

    Medicine_T med = (Medicine_T)List_first(Medicine_list);
    while (med != NULL){
        Medicine_free(&med);
        med = (Medicine_T)List_next(Medicine_list);
    }
    List_free(& Medicine_list);

    Patient_T pat = (Patient_T)List_first(Patient_list);
    while (pat != NULL){
        Patient_free(&pat);
        pat = (Patient_T)List_next(Patient_list);
    }
    List_free(& Patient_list);

    Record_T rec = (Record_T)List_first(Record_list);
    while (rec != NULL){
        Rec_free(&rec);
        rec = (Record_T)List_next(Record_list);
    }
    List_free(& Record_list);

    Ward_T war = (Ward_T)List_first(Ward_list);
    while (war != NULL){
        Ward_free(&war);
        war = (Ward_T)List_next(Ward_list);
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
