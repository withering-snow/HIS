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
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生开始出诊");
    return HIS_OK;
}


Status Serv_doc_unactive(){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    // 检查是否还有正在看诊的病人
    if(_cur_doc_pat.pat_id != INVALID_ID){
        Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生结束出诊时仍有病人[%lld]在看诊，自动结束看诊", _cur_doc_pat.pat_id);
        // 自动结束当前看诊
        Rel_queue_end_reg(_cur_doc_pat.doc_id);
        // 将挂号状态改为 COMPLETED
        List_T records = Data_get_record();
        void* ptr = List_first(records);
        while(ptr != NULL){
            Record_T rec = *(Record_T*)ptr;
            if(!Rec_is_invalid(rec) && Rec_type(rec) == REC_REGISTRATION &&
                Rec_actor_id(rec) == _cur_doc_pat.pat_id)
            {
                DataRegistration* data = (DataRegistration*)Rec_detail(rec);
                if(data->doc_id == _cur_doc_pat.doc_id && data->status == IN_PROGRESS){
                    Rec_set_reg_status(rec, COMPLETED);
                    break;
                }
            }
            ptr = List_next(records);
        }
        _cur_doc_pat.pat_id = INVALID_ID;
    }
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生结束出诊");
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
    if (s == HIS_OK) {
        Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生叫号 当前看诊病人[%lld]", _cur_doc_pat.pat_id);
    }
    return s;
}


Status Serv_doc_end_reg(){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    if(_cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NOT_FOUND;
    }
    // 将挂号状态从 IN_PROGRESS 改为 COMPLETED
    List_T records = Data_get_record();
    void* ptr = List_first(records);
    while(ptr != NULL){
        Record_T rec = *(Record_T*)ptr;
        if(!Rec_is_invalid(rec) && Rec_type(rec) == REC_REGISTRATION &&
            Rec_actor_id(rec) == _cur_doc_pat.pat_id)
        {
            DataRegistration* data = (DataRegistration*)Rec_detail(rec);
            if(data->doc_id == _cur_doc_pat.doc_id && data->status == IN_PROGRESS){
                Rec_set_reg_status(rec, COMPLETED);
                break;
            }
        }
        ptr = List_next(records);
    }

    Status s = Rel_queue_end_reg(_cur_doc_pat.doc_id);
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生结束看诊 病人[%lld] 挂号状态已设为COMPLETED", _cur_doc_pat.pat_id);
    _cur_doc_pat.pat_id = INVALID_ID;
    return s;
}


Status Serv_doc_consult(const char *diagnosis, const char *advice){
    if(!_cur_is_active || _cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NO_USER;
    }
    // 创建看诊记录
    Record_T r = Rec_cons_new(0, _cur_doc_pat.pat_id, _cur_doc_pat.doc_id, diagnosis, advice);
    List_push_back(Data_get_record(), &r);

    // 结束当前看诊（病人状态改为 COMPLETED）
    List_T records = Data_get_record();
    void* ptr = List_first(records);
    while(ptr != NULL){
        Record_T rec = *(Record_T*)ptr;
        if(!Rec_is_invalid(rec) && Rec_type(rec) == REC_REGISTRATION &&
            Rec_actor_id(rec) == _cur_doc_pat.pat_id)
        {
            DataRegistration* data = (DataRegistration*)Rec_detail(rec);
            if(data->doc_id == _cur_doc_pat.doc_id && data->status == IN_PROGRESS){
                Rec_set_reg_status(rec, COMPLETED);
                break;
            }
        }
        ptr = List_next(records);
    }

    // 注意：不在这里结束看诊，医生可继续开药/检查等操作
    // 医生需要手动选择"结束看诊"来释放当前病人
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生对病人[%lld]完成看诊", _cur_doc_pat.pat_id);
    return HIS_OK;
}

Status Serv_doc_exam(long long cost, const char *exam_name){
    if(!_cur_is_active || _cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NO_USER;
    }

    // 先检查余额是否足够
    Fund_T fund = (Fund_T)Serv_helper_finder(_cur_doc_pat.pat_id, TYPE_FUND);
    if(fund == NULL){
        return HIS_ERR_NO_FUNDS;
    }
    Status can_afford = Fund_can_afford(fund, cost);
    if(can_afford != HIS_OK){
        return can_afford;
    }

    Record_T r = Rec_exam_new(cost, _cur_doc_pat.pat_id, _cur_doc_pat.doc_id, exam_name);
    List_push_back(Data_get_record(), &r);

    // 扣费
    Fund_withdraw(fund, cost);
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生为病人[%lld]开具检查[%s] 费用[%lld]", _cur_doc_pat.pat_id, exam_name, cost);
    return HIS_OK;
}


Status Serv_doc_prescribe(const char *med_name, int amount){
    if(!_cur_is_active || _cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NO_USER;
    }
    // 模糊查找药品
    List_T med_list = Data_get_medicine();
    void* ptr = List_find(med_list, (void*)med_name, Medicine_cmp_fuzzy);
    if(ptr == NULL){
        return HIS_ERR_NOT_FOUND;
    }
    Medicine_T med = *(Medicine_T*)ptr;
    long long med_id = Medicine_id(med);
    long long price = Medicine_cur_price(med);
    long long total_cost = price * amount;

    // 先检查余额是否足够（在扣库存之前检查，避免库存扣了但钱不够）
    Fund_T fund = (Fund_T)Serv_helper_finder(_cur_doc_pat.pat_id, TYPE_FUND);
    if(fund == NULL){
        return HIS_ERR_NO_FUNDS;
    }
    Status can_afford = Fund_can_afford(fund, total_cost);
    if(can_afford != HIS_OK){
        return can_afford;
    }

    // 扣库存
    Status s = Medicine_deduct(med, amount);
    if(s != HIS_OK){
        return s;
    }

    // 创建记录
    Record_T r = Rec_pres_new(total_cost, _cur_doc_pat.pat_id, _cur_doc_pat.doc_id, med_id, amount);
    List_push_back(Data_get_record(), &r);

    // 扣费
    Fund_withdraw(fund, total_cost);
    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生为病人[%lld]开药[%s]x[%d] 费用[%lld]", _cur_doc_pat.pat_id, med_name, amount, total_cost);
    return HIS_OK;

}

Status Serv_doc_admission(long long ward_id, int bed_label, long long deposit){
    if(!_cur_is_active || _cur_doc_pat.pat_id == INVALID_ID){
        return HIS_ERR_NO_USER;
    }
    // 找病房
    Ward_T ward = (Ward_T)Serv_helper_finder(ward_id, TYPE_WARD);
    if(ward == NULL){
        return HIS_ERR_NOT_FOUND;
    }

    // 占用床位
    Status s = Ward_occupy_bed(ward, bed_label, _cur_doc_pat.pat_id);
    if(s != HIS_OK){
        return s;
    }

    // 充押金到资金账户
    Fund_T fund = (Fund_T)Serv_helper_finder(_cur_doc_pat.pat_id, TYPE_FUND);
    if(fund == NULL){
        // 创建资金账户
        fund = Fund_new(_cur_doc_pat.pat_id);
        List_push_back(Data_get_fund(), &fund);
    }
    Fund_deposit(fund, deposit);

    // 绑定病房关系
    Rel_bind_ward(_cur_doc_pat.pat_id, ward_id);

    // 创建入院记录
    Record_T r = Rec_admit_new(deposit, _cur_doc_pat.pat_id, ward_id, bed_label, deposit);

    List_push_back(Data_get_record(), &r);

    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生为病人[%lld]办理入院 病房[%lld]床号[%d]押金[%lld]", _cur_doc_pat.pat_id, ward_id, bed_label, deposit);
    return HIS_OK;
}

Status Serv_doc_discharge(long long pat_id){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    // 找病房关系
    long long ward_id = Rel_get_ward_by_patient(pat_id);
    if(ward_id == INVALID_ID || ward_id == HIS_ERR_NOT_FOUND){
        return HIS_ERR_NOT_FOUND;
    }

    Ward_T ward = (Ward_T)Serv_helper_finder(ward_id, TYPE_WARD);

    // 获取入住时间，腾空床位
    Bed_T* bed = (ward != NULL) ? Ward_get_bed_by_patient(ward, pat_id) : NULL;
    long long start_ts = (bed != NULL) ? bed->start_ts : INVALID_TIME;

    // 腾空床位
    if(ward != NULL){
        Ward_vacate_by_patient(ward, pat_id);
    }

    // 计算费用
    long long duration = (start_ts != INVALID_TIME) ?
        (Time_now() - start_ts) / 86400 : 1;
    if(duration < 1) duration = 1;
    long long daily_cost = (ward != NULL) ? Ward_daily_cost(ward) : 0;
    long long total_bill = daily_cost * duration;

    // 扣费并退还剩余押金
    Fund_T fund = (Fund_T)Serv_helper_finder(pat_id, TYPE_FUND);
    long long paid = 0;
    long long refund = 0;
    if(fund != NULL){
        long long balance = Fund_balance(fund);
        if(balance >= total_bill){
            paid = total_bill;
            refund = balance - total_bill;
        } else {
            paid = balance;
            refund = 0;
        }
        Fund_withdraw(fund, balance);  // 全部取出（住院费+退还剩余押金）
    }

    // 解除绑定
    Rel_unbind_ward(pat_id);

    // 创建出院记录
    Record_T r = Rec_disc_new(paid, pat_id, total_bill, paid);

    List_push_back(Data_get_record(), &r);

    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "医生办理病人[%lld]出院 账单[%lld]实付[%lld] 退还押金[%lld]", pat_id, total_bill, paid, refund);
    return HIS_OK;

}

Status Serv_doc_change_bed(long long pat_id, const char *to_ward_name, int to_bed_label){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    long long from_ward_id = Rel_get_ward_by_patient(pat_id);
    if(from_ward_id == INVALID_ID || from_ward_id == HIS_ERR_NOT_FOUND){
        return HIS_ERR_NOT_FOUND;
    }

    // 找目标病房
    long long to_ward_id = INVALID_ID;
    List_T ward_list = Data_get_ward();
    void* ptr = List_find(ward_list, (void*)to_ward_name, Ward_cmp_fuzzy);
    if(ptr != NULL){
        Ward_T w = *(Ward_T*)ptr;
        to_ward_id = Ward_id(w);
    }
    if(to_ward_id == INVALID_ID){
        return HIS_ERR_NOT_FOUND;
    }

    // 腾出旧床位
    Ward_T from_ward = (Ward_T)Serv_helper_finder(from_ward_id, TYPE_WARD);
    if(from_ward != NULL){
        Ward_vacate_by_patient(from_ward, pat_id);
    }

    // 占用新床位
    Ward_T to_ward = (Ward_T)Serv_helper_finder(to_ward_id, TYPE_WARD);
    if(to_ward == NULL){
        return HIS_ERR_NOT_FOUND;
    }
    Status s = Ward_occupy_bed(to_ward, to_bed_label, pat_id);
    if(s != HIS_OK){
        return s;
    }

    // 更新绑定
    Rel_bind_ward(pat_id, to_ward_id);

    // 创建记录
    Bed_T* old_bed = (from_ward != NULL) ? Ward_get_bed_by_patient(from_ward, pat_id) : NULL;
    int from_label = old_bed ? old_bed->bed_label : 0;
    Record_T r = Rec_c_bed_new(0, pat_id, from_ward_id, to_ward_id, from_label, to_bed_label);
    List_push_back(Data_get_record(), &r);

    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "病人[%lld]从病房[%lld]转至病房[%lld]床号[%d]", pat_id, from_ward_id, to_ward_id, to_bed_label);
    return HIS_OK;
}

Status Serv_doc_change_doc(long long pat_id, long long to_doc_id){
    if(!_cur_is_active){
        return HIS_ERR_NO_USER;
    }
    long long old_doc_id = Rel_get_doctor_by_patient(pat_id);
    if(old_doc_id == HIS_ERR_NOT_FOUND){
        return HIS_ERR_NOT_FOUND;
    }

    // 检查目标医生是否存在
    Doctor_T to_doc = (Doctor_T)Serv_helper_finder(to_doc_id, TYPE_DOCTOR);
    if(to_doc == NULL){
        return HIS_ERR_NOT_FOUND;
    }

    // 更新绑定
    Rel_unbind_doctor(pat_id);
    Rel_bind_doctor(pat_id, to_doc_id);

    // 创建记录
    Record_T r = Rec_c_doc_new(0, pat_id, old_doc_id, to_doc_id);
    List_push_back(Data_get_record(), &r);

    Log_printf(CLASS_DOCTOR, _cur_doc_pat.doc_id, "病人[%lld]从医生[%lld]转至医生[%lld]", pat_id, old_doc_id, to_doc_id);
    return HIS_OK;
}
long long Serv_doc_view_reg(){
    if(!_cur_is_active){
        return INVALID_ID;
    }
    return _cur_doc_pat.pat_id;
}
