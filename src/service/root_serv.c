#include <root_serv.h>

// 增加医生
Status Serv_root_add_doctor(const Doctor_Update_Pack *pack) {
    ASSERT(pack != NULL, "Doctor_Update_Pack 不合法");

    // 检查身份证号是否已被使用（医生和病人共用身份证号池）
    List_T pat_list = Data_get_patient();
    void* ptr = List_first(pat_list);
    while (ptr != NULL) {
        Patient_T p = *(Patient_T*)ptr;
        if (strncmp(Patient_id_card(p), pack->id_card, 20) == 0) {
            return HIS_ERR_ALREADY_EXISTS;
        }
        ptr = List_next(pat_list);
    }
    List_T doc_list = Data_get_doctor();
    ptr = List_first(doc_list);
    while (ptr != NULL) {
        Doctor_T d = *(Doctor_T*)ptr;
        if (strncmp(Doctor_id_card(d), pack->id_card, 20) == 0) {
            return HIS_ERR_ALREADY_EXISTS;
        }
        ptr = List_next(doc_list);
    }

    Doctor_T doc = Doctor_new(
        pack->gender, pack->birth_ts, pack->is_active, pack->dept,
        pack->title, pack->name, pack->phone, pack->id_card, pack->reg_fee
    );

    if (doc == NULL) return HIS_ERR_NO_MEM;

    List_push_back(Data_get_doctor(), &doc);

    // 同步创建医生账号（默认密码为八个8）
    Serv_account_signup(CLASS_DOCTOR, Doctor_id(doc), Doctor_name(doc), "88888888");

    Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员新增医生[%lld][%s]", Doctor_id(doc), Doctor_name(doc));
    return HIS_OK;
}

Status Serv_root_remove_doctor(long long doc_id) {
    List_T list = Data_get_doctor();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Doctor_T d = *(Doctor_T*)ptr;
        if (Doctor_id(d) == doc_id) {
            Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员删除医生[%lld][%s]", doc_id, Doctor_name(d));
    List_remove(list, ptr);
    Doctor_free(&d);
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
    Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员新增药品[%lld][%s]", Medicine_id(med), Medicine_name(med));
    return HIS_OK;
}

// 增加药品批次
Status Serv_root_add_medicine_batch(long long med_id, long long buy_price, long long expire_ts, int remain, const char* batch_no) {
    List_T list = Data_get_medicine();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Medicine_T m = *(Medicine_T*)ptr;
        if (Medicine_id(m) == med_id) {
            long long batch_id = Medicine_batch_add(m, buy_price, expire_ts, remain, batch_no);
            if (batch_id > 0) {
                // 创建进货记录
                long long admin_id = Serv_account_cur_id();
                Record_T r = Rec_s_in_new(-(buy_price * remain), admin_id, med_id, batch_id, buy_price, expire_ts, remain, batch_no);

                List_push_back(Data_get_record(), &r);
                Log_printf(CLASS_ROOT, admin_id, "管理员药品[%lld][%s]进货 批次[%s] 数量[%d] 进价[%lld]", med_id, Medicine_name(m), batch_no, remain, buy_price);
                return HIS_OK;
            }
            return HIS_ERR_NOT_FOUND;

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
            Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员删除药品[%lld][%s]", med_id, Medicine_name(m));
            List_remove(list, ptr);
            Medicine_free(&m);
            return HIS_OK;
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}

// 增加病房
Status Serv_root_add_ward(Department dept, long long daily_cost, int start_bed_label, int bed_count, const char* ward_name) {
    Ward_T ward = Ward_new(dept, daily_cost, start_bed_label, bed_count, ward_name);
    if (ward == NULL) return HIS_ERR_NO_MEM;
    List_push_back(Data_get_ward(), &ward);
    Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员新增病房[%lld][%s]", Ward_id(ward), Ward_name(ward));
    return HIS_OK;
}

// 删除病房
Status Serv_root_remove_ward(long long ward_id) {
    List_T list = Data_get_ward();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Ward_T w = *(Ward_T*)ptr;
        if (Ward_id(w) == ward_id) {
            Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员删除病房[%lld][%s]", ward_id, Ward_name(w));
            List_remove(list, ptr);
            Ward_free(&w);
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
            List_remove(list, ptr);
            Account_free(&a);
            return HIS_OK;
        }
        ptr = List_next(list);
    }
    return HIS_ERR_NOT_FOUND;
}
