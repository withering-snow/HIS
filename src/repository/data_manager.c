#include <data_manager.h>

static List_T Doctor_list;
static List_T Fund_list;
static List_T Medicine_list;
static List_T Patient_list;
static List_T Ward_list;

// 生命周期：全部实体链表构建与释放
void Data_init() {
    // doctor
    Doctor_list = List_new(sizeof(Doctor_T));
    // fund
    Fund_list = List_new(sizeof(Fund_T));
    // medicine
    Medicine_list = List_new(sizeof(Medicine_T));
    // patient
    Patient_list = List_new(sizeof(Patient_T));
    // ward
    Ward_list = List_new(sizeof(Ward_T));
}
void Data_destroy() {
    List_free(& Doctor_list);
    List_free(& Fund_list);
    List_free(& Medicine_list);
    List_free(& Patient_list);
    List_free(& Ward_list);
}

// 获取对应链表
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
List_T Data_get_ward() {
    return Ward_list;
}
