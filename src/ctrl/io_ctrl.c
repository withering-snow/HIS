#include <io_ctrl.h>

// TODO： 临时函数，解决从表返回值后删除
static List_T __tmp_list();

Status Io_save_patient()
{
    char dir[32];
    snprintf(dir,32, "data%cPatient.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"ID|Gender|Birth_TS|Name|Phone|ID_Card\n");
    List_T List_patient = Data_get_patient();
    void* pat_ptr = List_first(List_patient);
    while (pat_ptr != NULL)
    {
        Patient_T pat = *(Patient_T *)pat_ptr;
        fprintf(fp, "%lld|%d|%lld|%s|%s|%s\n",
            Patient_id(pat), Patient_gender(pat), Patient_birth_ts(pat),
            Patient_name(pat), Patient_phone(pat), Patient_id_card(pat));
        pat_ptr = List_next(List_patient);
    }
    fclose(fp);
    return HIS_OK;
}



Status Io_load_patient()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cPatient.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_patient = Data_get_patient();

    while (feof(fp) != 0)
    {
        long long id; gender gender; long long birth_ts;
        char name[32]; char phone[20]; char id_card[20];
        if (fscanf(fp, "%lld|%d|%lld|%s|%s|%s\n",
                id, gender, birth_ts, name, phone, id_card)!=6)
        {
            return HIS_ERR_IO_FAILURE;
        }
        Patient_T pat = Patient_load(id, gender, birth_ts, name, phone, id_card);
        List_push_back(List_patient, &pat);
    }
    fclose(fp);
    return HIS_OK;
}

Status  Io_save_doctor()
{
    char dir[32];
    snprintf(dir,32, "data%cDoctor.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"ID|Gender|Birth_TS|Is_Active|Debt|Title|Name|Phone|ID_Card\n");
    List_T List_doctor = Data_get_doctor();
    void* doc_ptr = List_first(List_doctor);
    while (doc_ptr != NULL)
    {
        Doctor_T doc = *(Doctor_T *)doc_ptr;
        fprintf(fp, "%lld|%d|%lld|%d|%d|%d|%s|%s|%s\n",
            Doctor_id(doc), Doctor_gender(doc), Doctor_birth_ts(doc),
            Doctor_is_active(doc), Doctor_dept(doc), Doctor_title(doc), Doctor_name(doc),
            Doctor_phone(doc), Doctor_id_card(doc));
        doc_ptr = List_next(List_doctor);
    }
    fclose(fp);
    return HIS_OK;

};
Status Io_load_doctor()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cDoctor.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_doctor = Data_get_doctor();
    while (feof(fp)!= 0)
    {
        long long   id;           gender gender;   long long   birth_ts;
        bool        is_active;    Department  dept;
        DoctorTitle title;        char        name[32];
        char        phone[20];    char        id_card[20];
        if (fscanf(fp, "%lld|%d|%lld|%d|%d|%d|%s|%s|%s\n",
            id, gender, birth_ts,
            is_active, dept,title, name,
            phone, id_card)!= 9)
        {
            return HIS_ERR_IO_FAILURE;
        }
        Doctor_T doc = Doctor_load(id, gender, birth_ts, is_active, dept, title, name, phone, id_card);
        List_push_back(List_doctor, &doc);
    }
    fclose(fp);
    return HIS_OK;
    };

Status Io_save_fund()
{
    char dir[32];
    snprintf(dir,32, "data%cFund.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Pat_id|Balance\n");

    List_T List_fund = Data_get_fund();
    void* fund_ptr = List_first(List_fund);
    while (fund_ptr != NULL)
    {
        Fund_T fund = *(Fund_T *)fund_ptr;
        fprintf(fp,"%lld|%lld\n",Fund_pat_id(fund),Fund_balance(fund));
        fund_ptr = List_next(List_fund);
    }
    fclose(fp);
    return HIS_OK;
};
Status Io_load_fund()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cFund.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_fund = Data_get_fund();
    while (feof(fp)!= 0)
    {
        long long pat_id;  long long balance;
        if (fscanf(fp,"%lld|%lld\n",pat_id,balance)!=2)
        {
                return HIS_ERR_IO_FAILURE;
        }
        Fund_T fund = Fund_load(pat_id, balance);
        List_push_back(List_fund, &fund);
    }
    fclose(fp);
    return HIS_OK;
    };
Status Io_save_medicine()
{
    char dir[32];
    snprintf(dir,32, "data%cMedicine.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    List_T List_med = Data_get_medicine();
    void* med_ptr = List_first(List_med);

    while (med_ptr != NULL)
    {
        Medicine_T med = *(Medicine_T* )med_ptr;

        fprintf(fp, "M|%lld|%lld|%d|%s\n",Medicine_id(med),Medicine_cur_price(med),
        Medicine_total_remain(med),Medicine_name(med));


        List_T List_batch = __tmp_list(); // TODO: med
        void* bat_ptr = List_first(List_batch);

        while (bat_ptr != NULL)
        {
            MedicineBatch* bat = (MedicineBatch*)bat_ptr;

            // 写入从表数据（以 B 标识）
            fprintf(fp, "B|%lld|%lld|%lld|%d|%d|%s\n",
                    bat->id,
                    bat->buy_price,
                    bat->expire_ts,
                    bat->remain,
                    bat->status,
                    bat->batch_no);

            bat_ptr = List_next(List_batch);
        }

        med_ptr = List_next(List_med);
    }

    fclose(fp);
    return HIS_OK;
};
Status Io_load_medicine()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cMedicine.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_med = Data_get_medicine();
    void* med_ptr = List_first(List_med);
    while (feof(fp)!= 0)
    {
        long long   id;           long long   cur_price;
        int         total_remain; char        name[32];
        if (fscanf(fp,"M|%lld|%lld|%d|%s\n",id,cur_price,
        total_remain,name)!=4)
        {
            return HIS_ERR_IO_FAILURE;
        }
        Medicine_T medicine = Medicine_load(id, cur_price,total_remain,name);
        List_push_back(List_med, &medicine);
        List_T List_batch = __tmp_list(); //TODO:med
        void* bat_ptr = List_first(List_batch);
        while (bat_ptr != NULL)
        {
        MedicineBatch* bat = (MedicineBatch*)bat_ptr;


        fscanf(fp, "B|%lld|%lld|%lld|%d|%d|%s\n",
                bat->id,
                bat->buy_price,
                bat->expire_ts,
                bat->remain,
                bat->status,
                bat->batch_no);

        bat_ptr = List_next(List_batch);
    }

    med_ptr = List_next(List_med);
    }
    fclose(fp);
    return HIS_OK;
};

Status Io_save_account()
{
    char dir[32];
    snprintf(dir,32, "data%cAccount.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Class|Actor_id|Name|Password\n");
    List_T List_account = Data_get_account();
    void* acc_ptr = List_first(List_account);
    while (acc_ptr != NULL)
    {
        Account_T acc = *(Account_T *)acc_ptr;
        fprintf(fp,"%d|%lld|%s|%s\n",Account_class(acc),Account_id(acc),Account_name(acc),Account_password(acc));
        acc_ptr = List_next(List_account);
    }
    fclose(fp);
    return HIS_OK;
};
Status Io_load_account()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cAccount.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_account = Data_get_account();
    while (feof(fp)!= 0)
    {
        AccountClass    class;
        long long       actor_id;
        char            name[32];
        char            password[32];
        if (fscanf(fp, "%d|%lld|%s|%s\n",
            class,actor_id,name,password)!= 4)
        {
            return HIS_ERR_IO_FAILURE;
        }
        Account_T acc = Account_load(class,actor_id,name,password);
        List_push_back(List_account, &acc);
    }
    fclose(fp);
    return HIS_OK;
};
Status Io_save_ward()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cWard.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    List_T List_ward = Data_get_ward();
    void* ward_ptr = List_first(List_ward);

    while (ward_ptr != NULL)
    {
        Ward_T ward = *(Ward_T* )ward_ptr;

        fprintf(fp, "W|%lld|%d|%lld|%d|%d\n",Ward_id(ward),Ward_dept(ward),
        Ward_daily_cost(ward),Ward_bed_count(ward),Ward_empty_count(ward));


        List_T List_bed = __tmp_list(); // TODO: ward
        void* bed_ptr = List_first(List_bed);

        while (bed_ptr != NULL)
        {
            Bed_T* bed = (Bed_T*)bed_ptr;

            // 写入从表数据（以 B 标识）
            fprintf(fp, "B|%d|%lld|%d|%lld|\n",
                    bed->bed_label,
                    bed->pat_id,
                    bed->status,
                    bed->start_ts);

            bed_ptr = List_next(List_bed);
        }

        ward_ptr = List_next(List_ward);
    }

    fclose(fp);
    return HIS_OK;

};
Status Io_load_ward()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cWard.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    };
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_ward = Data_get_ward();
    void* ward_ptr = List_first(List_ward);
    while (feof(fp)!= 0)
    {
        long long   id;
        Department  dept;
        long long   daily_cost;
        int         bed_count;
        int         empty_count;
        if (fscanf(fp,"W|%lld|%d|%lld|%s|%s\n",id,dept,
        daily_cost,bed_count,empty_count)!=5)
        {
            return HIS_ERR_IO_FAILURE;
        }
        Ward_T ward = Ward_load(id,dept,daily_cost);
        List_push_back(List_ward, &ward);
        List_T List_bed = __tmp_list(); //TODO:ward
        void* bed_ptr = List_first(List_bed);
        while (bed_ptr != NULL)
        {
            Bed_T* bed = (Bed_T*)bed_ptr;


            fprintf(fp, "B|%d|%lld|%d|%lld|\n",
                    bed->bed_label,
                    bed->pat_id,
                    bed->status,
                    bed->start_ts);

            bed_ptr = List_next(List_bed);
        }

        ward_ptr = List_next(List_ward);
    }
    fclose(fp);
    return HIS_OK;
};
Status Io_save_record()
{
    char dir[32];
    snprintf(dir,32, "data%cRecord.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Type|Is_Invalid|Actor_Id|Time_Stamp|Cost|Detail...\n");
    List_T List_record = Data_get_record();
    void* rec_ptr = List_first(List_record);
    while (rec_ptr != NULL)
    {
        Record_T rec = *(Record_T *)rec_ptr;
        fprintf(fp,"%d|%d|%lld|%lld|%lld",Rec_type(rec),Rec_is_invalid(rec),Rec_actor_id(rec),Rec_time_stamp(rec),Rec_cost(rec));
        switch (Rec_type(rec))
        {
        case REC_REGISTRATION:
            {
                DataRegistration* data = (DataRegistration*)Rec_detail(rec);
                fprintf(fp,"%lld|%d|%d|%d",data->doc_id,data->sequence_no,data->target_date,data->time_frame);
                break;
            }

        case REC_CONSULTATION:
            {
                DataConsultation* data = (DataConsultation*)Rec_detail(rec);
                fprintf(fp,"%lld|%s|%s",data->doc_id,data->diagnosis,data->advice);
                break;
            };
        case REC_EXAMINATION:
            {
                DataRegistration* data = (DataRegistration*)Rec_detail(rec);
                fprintf(fp,"%lld|%s",data->doc_id,data->exam_name);
                break;
            };
        case REC_PRESCRIPTION:
            {
                DataPrescription* data = (DataPrescription*)Rec_detail(rec);
                fprintf(fp,"%lld|%LLd|%d",data->doc_id,data->med_id,data->amount);
                break;
            };
        case REC_ADMISSION:
            {
                DataAdmission* data = (DataAdmission*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%lld",data->ward_id,data->bed_id,data->deposit);
                break;
            };
        case REC_DISCHARGE:
            {
                DataDischarge* data = (DataDischarge*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld",data->total_bill,data->paid);
                break;
            };
        case REC_CHANGE_BED:
            {
                DataChangeBed* data = (DataChangeBed*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%lld|%lld",data->from_ward_id,data->to_ward_id,data->from_bed_id,data->to_bed_id);
                break;
            };
        case REC_CHANGE_DOC:
            {
                DataChangeDoc* data = (DataChangeDoc*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld",data->old_doc_id,data->new_doc_id);//改成past和present更合适
                break;
            };
        case REC_STOCK_IN:
            {
                DataStackIn* data = (DataStackIn*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%lld|%lld|%d|%s",data->med_id,data->batch_id,data->buy_price,data->expire_ts,data->total,data->batch_no);
                break;
            };
        case REC_STOCK_OUT:
            {
                DataStackOut* data = (DataStackOut*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%d|%s",data->med_id,data->batch_id,data->total,data->batch_no);
                break;
            };

        rec_ptr = List_next(List_record);
    }
    fclose(fp);
    return HIS_OK;
}

Status Io_load_record()
    {
        char dir[32];
        snprintf(dir, sizeof(dir), "data%cRecord.txt", SEP);

        FILE *fp = fopen(dir, "r");
        if (fp == NULL){
            return HIS_ERR_IO_FAILURE;
        };
        char dummy[256];
        fgets(dummy,sizeof(dummy),fp);
        List_T List_record = Data_get_record();
        void* record_ptr = List_first(List_record);
        while (feof(fp)!= 0)
        {

            RecordType      type;
            bool            is_invalid;
            long long       actor_id;
            long long       time_stamp;
            long long       cost;
            if (fscanf(fp,"%d|%d|%lld|%lld|%lld\n",type,is_invalid,
            actor_id,time_stamp,cost)!=5)
            {
                return HIS_ERR_IO_FAILURE;
            }


            switch (type)
            {
            case REC_REGISTRATION:
                {
                    DataRegistration data;
                    fscanf(fp,"%lld|%lld|%lld|%d|%d", data.doc_id, data.sequence_no,data.target_date,data.time_frame);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_CONSULTATION:
                {
                    DataConsultation data;
                    fscanf(fp,"%lld|%s|%s", data.doc_id, data.diagnosis,data.advice);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_EXAMINATION:
                {
                    DataExamination data;
                    fscanf(fp,"%lld|%s", data.doc_id, data.exam_name);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_PRESCRIPTION:
                {
                    DataPrescription data;
                    fscanf(fp,"%lld|%lld|%d", data.doc_id, data.med_id,data.amount);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_ADMISSION:
                {
                    DataAdmission data;
                    fscanf(fp,"%lld|%lld|%lld", data.ward_id, data.bed_id,data.deposit);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_DISCHARGE:
                {
                    DataDischarge data;
                    fscanf(fp,"%lld|%lld", data.total_bill, data.paid);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_CHANGE_BED:
                {
                    DataChangeBed data;
                    fscanf(fp,"%lld|%lld|%lld|%lld",  data.from_ward_id,data.to_ward_id,data.from_bed_id,data.to_bed_id);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_CHANGE_DOC:
                {
                    DataChangeDoc data;
                    fscanf(fp,"%lld|%lld", data.old_doc_id, data.new_doc_id);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_STOCK_IN:
                {
                    DataStackIn data;
                    fscanf(fp,"%lld|%lld|%lld|%lld|%d|%s", data.med_id, data.batch_id,data.buy_price,data.expire_ts,data.total,data.batch_no);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            case REC_STOCK_OUT:
                {
                    DataStackOut data;
                    fscanf(fp,"%lld|%lld|%d|%s", data.med_id, data.batch_id,data.total,data.batch_no);
                    Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                            &data, sizeof(data));
                    break;
                }
            }
            record_ptr = List_next(List_record);
        }
        fclose(fp);
        return HIS_OK;
    }