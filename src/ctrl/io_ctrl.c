#include <io_ctrl.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif




static Status Io_save_patient()
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


static Status Io_load_patient()
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

    long long id; gender gender; long long birth_ts;
    char name[32]; char phone[20]; char id_card[20];

    while (
        fscanf(fp, "%lld|%d|%lld|%31[^|]|%19[^|]|%19[^|\r\n]\n",
            &id, &gender, &birth_ts, name, phone, id_card)
            == 6)
    {
        Patient_T pat = Patient_load(id, gender, birth_ts, name, phone, id_card);
        List_push_back(List_patient, &pat);
    }

    fclose(fp);
    return HIS_OK;
}




static Status Io_save_doctor()
{
    char dir[32];
    snprintf(dir,32, "data%cDoctor.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"ID|Gender|Birth_TS|Is_Active|Dept|Title|Name|Phone|ID_Card|Reg_Fee\n");
    List_T List_doctor = Data_get_doctor();
    void* doc_ptr = List_first(List_doctor);
    while (doc_ptr != NULL)
    {
        Doctor_T doc = *(Doctor_T *)doc_ptr;
        fprintf(fp, "%lld|%d|%lld|%d|%d|%d|%s|%s|%s|%lld\n",
            Doctor_id(doc), Doctor_gender(doc), Doctor_birth_ts(doc),
            Doctor_is_active(doc), Doctor_dept(doc), Doctor_title(doc), Doctor_name(doc),
            Doctor_phone(doc), Doctor_id_card(doc), Doctor_reg_fee(doc));
        doc_ptr = List_next(List_doctor);
    }
    fclose(fp);
    return HIS_OK;

}


static Status Io_load_doctor()
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


    long long   id;             gender      gender;           long long   birth_ts;
    int         is_active_int;  bool        is_active;        Department  dept;
    DoctorTitle title;          char        name[32];
    char        phone[20];      char        id_card[20];      long long   reg_fee;

    while (
        fscanf(fp, "%lld|%d|%lld|%d|%d|%d|%31[^|]|%19[^|]|%19[^|]|%lld\n",
            &id, &gender, &birth_ts, &is_active_int, &dept, &title, name, phone, id_card, &reg_fee)
            == 10)
    {
        is_active = (bool)is_active_int;
        Doctor_T doc = Doctor_load(id, gender, birth_ts, is_active, dept, title, name, phone, id_card, reg_fee);
        List_push_back(List_doctor, &doc);
    }

    fclose(fp);
    return HIS_OK;
}




static Status Io_save_fund()
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
}


static Status Io_load_fund()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cFund.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_fund = Data_get_fund();

    long long pat_id;  long long balance;

    while (fscanf(fp,"%lld|%lld\n", &pat_id, &balance)
        == 2)
    {
        Fund_T fund = Fund_load(pat_id, balance);
        List_push_back(List_fund, &fund);
    }

    fclose(fp);
    return HIS_OK;
}




static Status Io_save_medicine()
{
    char dir[32];
    snprintf(dir,32, "data%cMedicine.txt", SEP);
    FILE *fp=fopen(dir,"w");
    if (fp == NULL)
    {
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Med_id|Cur_price|Total_remain|Name\n");
    List_T List_med = Data_get_medicine();
    void* med_ptr = List_first(List_med);

    while (med_ptr != NULL)
    {
        Medicine_T med = *(Medicine_T* )med_ptr;

        fprintf(fp, "M|%lld|%lld|%d|%s\n",Medicine_id(med),Medicine_cur_price(med),
        Medicine_total_remain(med),Medicine_name(med));


        List_T List_batch = Medicine_batches(med);
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
}


static Status Io_load_medicine()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cMedicine.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_med = Data_get_medicine();

    Medicine_T recent_med = NULL;  char type = 'M';

    while (fscanf(fp, "%c|", &type) == 1)
    {
        if (type == 'M')
        {
            long long   id;           long long   cur_price;
            int         total_remain; char        name[32];
            if (fscanf(fp,"%lld|%lld|%d|%31[^|]\n", &id,&cur_price, &total_remain, name) != 4)
            {
                // 跳过损坏的行
                fscanf(fp, "%*[^\n]\n");
                continue;
            }
            Medicine_T medicine = Medicine_load(id, cur_price,total_remain,name);
            recent_med = medicine;
            List_push_back(List_med, &medicine);
        }
        else if (type == 'B')
        {
            if (recent_med == NULL) {
                // 没有对应的主表记录，跳过此行
                fscanf(fp, "%*[^\n]\n");
                continue;
            }
            MedicineBatch bat;
            memset(&bat, 0, sizeof(bat));
            if (fscanf(fp, "%lld|%lld|%lld|%d|%d|%31[^|]\n",
                &bat.id,
                &bat.buy_price,
                &bat.expire_ts,
                &bat.remain,
                &bat.status,
                bat.batch_no) != 6)
            {
                // 跳过损坏的行
                fscanf(fp, "%*[^\n]\n");
                continue;
            }
            List_push_back(Medicine_batches(recent_med), &bat);
        }
        else {
            // 未知类型，跳过整行
            fscanf(fp, "%*[^\n]\n");
        }
    }
    fclose(fp);
    return HIS_OK;
}




static Status Io_save_account()
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
        fprintf(fp,"%d|%lld|%s|",Account_class(acc),Account_id(acc),Account_name(acc));

        const unsigned char* password = (const unsigned char*)Account_password(acc);
        for (int i = 0; i < 32; i++) {
            fprintf(fp, "%02X", password[i]);
        }
        fprintf(fp, "\n");

        acc_ptr = List_next(List_account);
    }
    fclose(fp);
    return HIS_OK;
}


static Status Io_load_account()
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

    AccountClass    class;
    long long       actor_id;
    char            name[32];

    while (fscanf(fp, "%d|%lld|%31[^|]|",
            &class, &actor_id, name)
            == 3)
    {
        unsigned char password[32] = {0};
        for (int i = 0; i < 32; i++){
            fscanf(fp,"%02hhX", &password[i]);
        }
        fscanf(fp, "\n");

        Account_T acc = Account_load(class, actor_id, name, (char*)password);
        List_push_back(List_account, &acc);
    }

    fclose(fp);
    return HIS_OK;
}




static Status Io_save_ward()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cWard.txt", SEP);

    FILE *fp = fopen(dir, "w");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp, "Ward_id|Dept|Daily_Cost|Bed_Count|Empty_Count|Ward_Name\n");
    List_T List_ward = Data_get_ward();
    void* ward_ptr = List_first(List_ward);

    while (ward_ptr != NULL)
    {
        Ward_T ward = *(Ward_T* )ward_ptr;

        fprintf(fp, "W|%lld|%d|%lld|%d|%d|%s\n",Ward_id(ward),Ward_dept(ward),
        Ward_daily_cost(ward),Ward_bed_count(ward),Ward_empty_count(ward),Ward_name(ward));


        List_T List_bed = Ward_beds(ward);
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

}


static Status Io_load_ward()
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

    Ward_T recent_ward = NULL;char type = 'W';

    while (fscanf(fp,"%c|",&type) == 1)
    {
        if (type=='W')
        {
            long long   id;
            Department  dept;
            long long   daily_cost;
            int         bed_count;
            int         empty_count;
            char        ward_name[32];
            if (fscanf(fp,"%lld|%d|%lld|%d|%d|%31[^|\n]\n",&id,&dept,&daily_cost,&bed_count,&empty_count,ward_name)!=6)
            {
                return HIS_ERR_IO_FAILURE;
            }
            Ward_T ward = Ward_load(id,dept,daily_cost,ward_name);
            recent_ward = ward;
            List_push_back(List_ward, &ward);
        }
        else if (type == 'B')
        {
            Bed_T bed ;
            memset(&bed, 0, sizeof(bed));
            if (fscanf(fp, "%d|%lld|%d|%lld|\n",
                &bed.bed_label,
                &bed.pat_id,
                &bed.status,
                &bed.start_ts) != 4)
            {
                // 跳过损坏的行
                fscanf(fp, "%*[^\n]\n");
                continue;
            }
            if (recent_ward != NULL) {
                Ward_bed_load(recent_ward, bed.bed_label, bed.pat_id, (BedStatus)bed.status, bed.start_ts);
            }
        }
        else {
            // 未知类型，跳过整行
            fscanf(fp, "%*[^\n]\n");
        }
    }
    fclose(fp);
    return HIS_OK;
}




static Status Io_save_record()
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
        fprintf(fp,"%d|%d|%lld|%lld|%lld|",Rec_type(rec),Rec_is_invalid(rec),Rec_actor_id(rec),Rec_time_stamp(rec),Rec_cost(rec));
        switch (Rec_type(rec))
        {
        case REC_REGISTRATION:
            {
                DataRegistration* data = (DataRegistration*)Rec_detail(rec);
                fprintf(fp,"%lld|%d|%d|%d\n",data->doc_id,data->sequence_no,data->target_date,data->time_frame);
                break;
            }

        case REC_CONSULTATION:
            {
                DataConsultation* data = (DataConsultation*)Rec_detail(rec);
                fprintf(fp,"%lld|%s|%s\n",data->doc_id,data->diagnosis,data->advice);
                break;
            };
        case REC_EXAMINATION:
            {
                DataExamination* data = (DataExamination*)Rec_detail(rec);
                fprintf(fp,"%lld|%s\n",data->doc_id,data->exam_name);
                break;
            };
        case REC_PRESCRIPTION:
            {
                DataPrescription* data = (DataPrescription*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%d\n",data->doc_id,data->med_id,data->amount);
                break;
            };
        case REC_ADMISSION:
            {
                DataAdmission* data = (DataAdmission*)Rec_detail(rec);
                fprintf(fp,"%lld|%d|%lld\n",data->ward_id,data->bed_label,data->deposit);
                break;
            };
        case REC_DISCHARGE:
            {
                DataDischarge* data = (DataDischarge*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld\n",data->total_bill,data->paid);
                break;
            };
        case REC_CHANGE_BED:
            {
                DataChangeBed* data = (DataChangeBed*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%d|%d\n",data->from_ward_id,data->to_ward_id,data->from_bed_label,data->to_bed_label);
                break;
            };
        case REC_CHANGE_DOC:
            {
                DataChangeDoc* data = (DataChangeDoc*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld\n",data->old_doc_id,data->new_doc_id);//改成past和present更合适
                break;
            };
        case REC_STOCK_IN:
            {
                DataStockIn* data = (DataStockIn*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%lld|%lld|%d|%s\n",data->med_id,data->batch_id,data->buy_price,data->expire_ts,data->total,data->batch_no);
                break;
            };
        case REC_STOCK_OUT:
            {
                DataStockOut* data = (DataStockOut*)Rec_detail(rec);
                fprintf(fp,"%lld|%lld|%d|%s\n",data->med_id,data->batch_id,data->total,data->batch_no);
                break;
            };

        }
        rec_ptr = List_next(List_record);
    }
    fclose(fp);
    return HIS_OK;
}


static Status Io_load_record()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cRecord.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    List_T List_record = Data_get_record();

    RecordType      type;
    int             is_invalid_int;
    bool            is_invalid;
    long long       actor_id;
    long long       time_stamp;
    long long       cost;

    while (fscanf(fp,"%d|%d|%lld|%lld|%lld|",
        &type,&is_invalid_int, &actor_id,&time_stamp,&cost)
        == 5)
    {
        is_invalid = (bool)is_invalid_int;
        Record_T rec = NULL;
        switch (type)
        {
            case REC_REGISTRATION:
            {
                DataRegistration data;
                fscanf(fp,"%lld|%d|%d|%d\n", &data.doc_id, &data.sequence_no,&data.target_date,&data.time_frame);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_CONSULTATION:
            {
                DataConsultation data;
                fscanf(fp,"%lld|%127[^|]|%127[^|]\n", &data.doc_id, data.diagnosis,data.advice);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_EXAMINATION:
            {
                DataExamination data;
                fscanf(fp,"%lld|%63[^|]\n", &data.doc_id, data.exam_name);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_PRESCRIPTION:
            {
                DataPrescription data;
                fscanf(fp,"%lld|%lld|%d\n", &data.doc_id, &data.med_id,&data.amount);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_ADMISSION:
            {
                DataAdmission data;
                fscanf(fp,"%lld|%d|%lld\n", &data.ward_id, &data.bed_label, &data.deposit);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_DISCHARGE:
            {
                DataDischarge data;
                fscanf(fp,"%lld|%lld\n", &data.total_bill, &data.paid);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_CHANGE_BED:
            {
                DataChangeBed data;
                fscanf(fp,"%lld|%lld|%d|%d\n",  &data.from_ward_id,&data.to_ward_id,&data.from_bed_label, &data.to_bed_label);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_CHANGE_DOC:
            {
                DataChangeDoc data;
                fscanf(fp,"%lld|%lld\n", &data.old_doc_id, &data.new_doc_id);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_STOCK_IN:
            {
                DataStockIn data;
                fscanf(fp,"%lld|%lld|%lld|%lld|%d|%31[^|]\n",
                    &data.med_id, &data.batch_id,&data.buy_price,&data.expire_ts,&data.total,data.batch_no);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
            case REC_STOCK_OUT:
            {
                DataStockOut data;
                fscanf(fp,"%lld|%lld|%d|%31[^|]\n", &data.med_id, &data.batch_id,&data.total,data.batch_no);
                rec = Rec_load(type, is_invalid, actor_id, time_stamp, cost,
                    &data, sizeof(data));
                break;
            }
        }

        if(rec != NULL){
            List_push_back(List_record, &rec);
        }

    }

    fclose(fp);
    return HIS_OK;
}




static Status Io_save_doctor_relation()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cDoctor-relation.txt", SEP);

    FILE *fp = fopen(dir, "w");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Pat_id|Doc_id\n");
    List_T List_rel_doc = Rel_doc_get();
    Rel_doc* rel_doc_ptr = List_first(List_rel_doc);
    while (rel_doc_ptr != NULL)
    {
        fprintf(fp, "%lld|%lld\n",
            rel_doc_ptr->pat_id, rel_doc_ptr->doc_id);
        rel_doc_ptr = List_next(List_rel_doc);
    }
    fclose(fp);
    return HIS_OK;
}


static Status Io_load_doctor_relation()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cDoctor-relation.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    long long pat_id;  long long doc_id;

    while (fscanf(fp,"%lld|%lld\n", &pat_id, &doc_id)
        == 2)
    {
        Rel_bind_doctor(pat_id,doc_id);
    }
    fclose(fp);
    return HIS_OK;
}




static Status Io_save_ward_relation()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cWard-relation.txt", SEP);

    FILE *fp = fopen(dir, "w");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    fprintf(fp,"Pat_id|Ward_id\n");
    List_T List_rel_ward = Rel_ward_get();
    Rel_ward* rel_ward_ptr= List_first(List_rel_ward);
    while (rel_ward_ptr != NULL)
    {
        fprintf(fp, "%lld|%lld\n",
            rel_ward_ptr->pat_id, rel_ward_ptr->ward_id);
        rel_ward_ptr = List_next(List_rel_ward);
    }
    fclose(fp);
    return HIS_OK;
}


static Status Io_load_ward_relation()
{
    char dir[32];
    snprintf(dir, sizeof(dir), "data%cWard-relation.txt", SEP);

    FILE *fp = fopen(dir, "r");
    if (fp == NULL){
        return HIS_ERR_IO_FAILURE;
    }
    char dummy[256];
    fgets(dummy,sizeof(dummy),fp);
    long long pat_id;  long long ward_id;

    while (fscanf(fp,"%lld|%lld\n", &pat_id, &ward_id)
        == 2)
    {
        Rel_bind_ward(pat_id,ward_id);
    }
    fclose(fp);
    return HIS_OK;
}




static void Io_ensure_data_dir()
{
#ifdef _WIN32
    mkdir("data");
#else
    mkdir("data", 0755);
#endif
}

Status Io_save()
{
    Io_ensure_data_dir();
    Status s;
    bool any_fail = false;

    if((s = Io_save_patient()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_doctor()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_fund()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_medicine()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_account()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_ward()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_record()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_doctor_relation()) != HIS_OK){
        any_fail = true;
    }
    if((s = Io_save_ward_relation()) != HIS_OK){
        any_fail = true;
    }
    return any_fail ? HIS_ERR_IO_FAILURE : HIS_OK;
}


Status Io_load()
{
    Status s;
    bool any_load = false;

    s = Io_load_patient();
    if(s == HIS_OK) any_load = true;

    s = Io_load_doctor();
    if(s == HIS_OK) any_load = true;

    s = Io_load_fund();
    if(s == HIS_OK) any_load = true;

    s = Io_load_medicine();
    if(s == HIS_OK) any_load = true;

    s = Io_load_account();
    if(s == HIS_OK) any_load = true;

    s = Io_load_ward();
    if(s == HIS_OK) any_load = true;

    s = Io_load_record();
    if(s == HIS_OK) any_load = true;

    s = Io_load_doctor_relation();
    if(s == HIS_OK) any_load = true;

    s = Io_load_ward_relation();
    if(s == HIS_OK) any_load = true;

    return any_load ? HIS_OK : HIS_ERR_IO_FAILURE;
}