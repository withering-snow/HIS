#include <root_serv.h>

// 增加医生
Status Serv_root_add_doctor(const Doctor_Update_Pack *pack) {
    ASSERT(pack != NULL, "Doctor_Update_Pack 不合法");

    Doctor_T doc = Doctor_new(
        pack->gender, pack->birth_ts, pack->is_active, pack->dept,
        pack->title, pack->name, pack->phone, pack->id_card, pack->reg_fee
    );

    if (doc == NULL) return HIS_ERR_NO_MEM;

    List_push_back(Data_get_doctor(), &doc);
    return HIS_OK;
}

// 删除医生 (同时需要考虑关联的账号吗？这里先做简单的实体删除)
Status Serv_root_remove_doctor(long long doc_id) {
    List_T list = Data_get_doctor();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Doctor_T d = *(Doctor_T*)ptr;
        if (Doctor_id(d) == doc_id) {
            Doctor_free(&d);
            List_remove(list, ptr);
            return HIS_OK;
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}

// 增加药品主表
Status Serv_root_add_medicine(long long cur_price, const char* name) {
    Medicine_T med = Medicine_new(cur_price, name);
    if (med == NULL) return HIS_ERR_NO_MEM;
    List_push_back(Data_get_medicine(), &med);
    return HIS_OK;
}

// 增加药品批次
Status Serv_root_add_medicine_batch(long long med_id, long long buy_price, long long expire_ts, int remain, const char* batch_no) {
    List_T list = Data_get_medicine();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Medicine_T m = *(Medicine_T*)ptr;
        if (Medicine_id(m) == med_id) {
            return Medicine_batch_add(m, buy_price, expire_ts, remain, batch_no);
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}

// 删除药品
Status Serv_root_remove_medicine(long long med_id) {
    List_T list = Data_get_medicine();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Medicine_T m = *(Medicine_T*)ptr;
        if (Medicine_id(m) == med_id) {
            Medicine_free(&m);
            List_remove(list, ptr);
            return HIS_OK;
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}

// 删除账号
Status Serv_root_remove_account(long long acc_id) {
    List_T list = Data_get_account();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Account_T a = *(Account_T*)ptr;
        if (Account_id(a) == acc_id) {
            Account_free(&a);
            List_remove(list, ptr);
            return HIS_OK;
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}
