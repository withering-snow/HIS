#include <root_ui.h>
#include <log_ctrl.h>
load_ui_tools


bool UI_root_should_continue = false;



typedef enum{
    RMC_QUIT      = 0,
    RMC_PATIENT   = 1,
    RMC_DOCTOR    = 2,
    RMC_MEDICINE  = 3,
    RMC_WARD      = 4,
    RMC_FUND      = 5,
    RMC_RECORD    = 6,
    RMC_ACCOUNT   = 7
}ROOT_MENU_CHOICE;

typedef enum{
    RQC_SIGN_OUT = 1,
    RQC_QUIT     = 2
}ROOT_QUIT_CHOICE;




// 三级输入日期：年、月、日分别输入，每级校验，不合法则重新输入
// 返回时间戳
static int __is_leap_year(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}
static int __month_days(int y, int m) {
    static int days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && __is_leap_year(y)) return 29;
    return days[m];
}
static long long __input_date(const char* prompt) {
    printf("%s\n", prompt);
    char buf[16];
    int y, m, d;
    while (1) {
        printf("  请输入8位数字日期 (如20061121): ");
        if (fgets(buf, 16, stdin) == NULL) continue;
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        if (strlen(buf) != 8) {
            printf("  必须输入8位数字！\n");
            continue;
        }
        int ok = 1;
        for (int i = 0; i < 8; i++) {
            if (buf[i] < '0' || buf[i] > '9') { ok = 0; break; }
        }
        if (!ok) { printf("  只能包含数字！\n"); continue; }
        sscanf(buf, "%4d%2d%2d", &y, &m, &d);
        if (y < 1900 || y > 2026) { printf("  年份不合法 (1900~2026)！\n"); continue; }
        if (m < 1 || m > 12) { printf("  月份不合法 (1~12)！\n"); continue; }
        if (d < 1 || d > __month_days(y, m)) { printf("  日期不合法！\n"); continue; }
        break;
    }
    struct tm tm = {0};
    tm.tm_year = y - 1900;
    tm.tm_mon  = m - 1;
    tm.tm_mday = d;
    return (long long)mktime(&tm);
}

static void __print_patient_list(List_T list) {
    printf("%-8s | %-12s | %-4s | %-4s | %-15s | %-20s\n", "ID", "姓名", "性别", "年龄", "电话", "身份证号");
    printf("------------------------------------------------------------------------------------\n");
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Patient_T p = *(Patient_T*)ptr;
        printf("%-8lld | %-12s | %-4s | %-4d | %-15s | %-20s\n",
            Patient_id(p), Patient_name(p),
            Patient_gender(p) == MALE ? "男" : "女",
            Patient_age(p),
            Patient_phone(p), Patient_id_card(p));
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无病人\n");
}

static Status __patient() {
    while (1) {
        CLEAN();
        printf("--- 病人信息管理 ---\n");
        printf("1. 查看全部病人  2. 搜索病人  3. 修改病人信息  0. 返回\n");
        long long choice = get_input_long_long("请选择", 0, 3);
        if (choice == 0) break;

        if (choice == 1) {
            CLEAN();
            printf("--- 病人信息总览 ---\n");
            __print_patient_list(Data_get_patient());
            press_enter();
        }
        else if (choice == 2) {
            CLEAN();
            printf("--- 搜索病人 ---\n");
            char keyword[32];
            get_input_str("请输入姓名关键字", keyword, 32);
            List_T list = Data_get_patient();
            List_T result = List_new(sizeof(Patient_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Patient_T p = *(Patient_T*)ptr;
                if (Patient_cmp_fuzzy(&p, keyword) == 0) {
                    List_push_back(result, &p);
                }
                ptr = List_next(list);
            }
            if (List_size(result) > 0) {
                __print_patient_list(result);
            } else {
                printf("未找到匹配的病人\n");
            }
            List_free(&result);
            press_enter();
        }
        else if (choice == 3) {
            CLEAN();
            printf("--- 修改病人信息 ---\n");
            long long pat_id = get_input_long_long("请输入要修改的病人ID", 0, 999999);
            List_T list = Data_get_patient();
            void* ptr = List_first(list);
            Patient_T target = NULL;
            while (ptr != NULL) {
                Patient_T p = *(Patient_T*)ptr;
                if (Patient_id(p) == pat_id) {
                    target = p;
                    break;
                }
                ptr = List_next(list);
            }
            if (target == NULL) {
                printf("未找到该病人\n");
                press_enter();
                continue;
            }

            CLEAN();
            printf("--- 修改病人信息 (ID: %lld) ---\n", pat_id);
            printf("当前信息：\n");
            printf("  姓名：%s\n", Patient_name(target));
            printf("  性别：%s\n", Patient_gender(target) == MALE ? "男" : "女");
            printf("  出生日期：%s\n", Time_to_string_date(Patient_birth_ts(target)));
            printf("  电话：%s\n", Patient_phone(target));
            printf("  身份证号：%s\n", Patient_id_card(target));
            printf("\n请输入新信息：\n");

            Patient_Update_Pack pack;
            pack.gender = (gender)get_input_long_long(
                "性别 (0:女, 1:男)", 0, 1);
            pack.birth_ts = __input_date("出生日期");
            get_input_str("姓名", pack.name, 32);
            get_input_str("电话", pack.phone, 20);
            get_input_str("身份证号", pack.id_card, 20);
            Status s = Patient_update(target, &pack);
            if (s == HIS_OK) {
                printf("修改成功！\n");
            } else {
                printf("修改失败！\n");
            }
            press_enter();
        }
    }
    return HIS_OK;
}

static void __print_doctor_list(List_T list) {
    printf("%-8s | %-12s | %-4s | %-4s | %-10s | %-10s | %-8s | %-10s\n",
        "ID", "姓名", "性别", "年龄", "科室", "职称", "出诊", "挂号费");
    printf("--------------------------------------------------------------------------------------------\n");
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Doctor_T d = *(Doctor_T*)ptr;
        const char* title_str = "";
        switch(Doctor_title(d)){
            case TITLE_RESIDENT:    title_str = "住院医师"; break;
            case TITLE_ATTENDING:   title_str = "主治医师"; break;
            case TITLE_ASSOC_CHIEF: title_str = "副主任医师"; break;
            case TITLE_CHIEF:       title_str = "主任医师"; break;
            default:                title_str = "医生"; break;
        }
        printf("%-8lld | %-12s | %-4s | %-4d | %-10s | %-10s | %-8s | %lld.%02lld 元\n",
            Doctor_id(d), Doctor_name(d),
            Doctor_gender(d) == MALE ? "男" : "女",
            Doctor_age(d),
            department_name(Doctor_dept(d)),
            title_str,
            Doctor_is_active(d) ? "出诊中" : "未出诊",
            Doctor_reg_fee(d)/100, Doctor_reg_fee(d)%100);
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无医生\n");
}

static Status __doctor() {
    while (1) {
        CLEAN();
        printf("--- 医生管理 ---\n");
        __print_doctor_list(Data_get_doctor());

        printf("\n1. 新增医生  2. 删除医生  3. 搜索医生  4. 按科室筛选  5. 修改医生信息  0. 返回\n");
        long long choice = get_input_long_long("选择操作", 0, 5);
        if (choice == 0) break;

        if (choice == 1) {
            Doctor_Update_Pack pack;
            get_input_str("姓名", pack.name, 32);
            pack.gender = (gender)get_input_long_long("性别 (0:女, 1:男)", 0, 1);
            pack.birth_ts = __input_date("出生日期");
            pack.dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            pack.title = (DoctorTitle)get_input_long_long(
                "职称：\n1.住院医师\t2.主治医师\t3.副主任医师\t4.主任医师", 1, 4);
            long long reg_fee = get_input_long_long("挂号费（分）", 0, 100000);
            pack.reg_fee = reg_fee;
            pack.is_active = true;
            get_input_str("电话", pack.phone, 20);
            get_input_str("身份证", pack.id_card, 20);
            Status s = Serv_root_add_doctor(&pack);
            if (s == HIS_OK) {
                printf("医生创建成功！默认密码为 88888888，请提醒医生及时修改密码。\n");
            } else if (s == HIS_ERR_ALREADY_EXISTS) {
                printf("创建失败：该身份证号已被使用！\n");
            } else {
                printf("创建失败！\n");
            }
        }
        else if (choice == 2) {
            long long id = get_input_long_long("输入要删除的 ID", 0, 99999);
            Status s = Serv_root_remove_doctor(id);
            if (s == HIS_OK) {
                printf("删除成功！\n");
            } else {
                printf("未找到该医生\n");
            }
        }
        else if (choice == 3) {
            CLEAN();
            printf("--- 搜索医生 ---\n");
            char keyword[32];
            get_input_str("请输入姓名关键字", keyword, 32);
            List_T list = Data_get_doctor();
            List_T result = List_new(sizeof(Doctor_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Doctor_T d = *(Doctor_T*)ptr;
                if (Doctor_cmp_fuzzy(&d, keyword) == 0) {
                    List_push_back(result, &d);
                }
                ptr = List_next(list);
            }
            if (List_size(result) > 0) {
                __print_doctor_list(result);
            } else {
                printf("未找到匹配的医生\n");
            }
            List_free(&result);
            press_enter();
        }
        else if (choice == 4) {
            CLEAN();
            printf("--- 按科室筛选 ---\n");
            Department dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            List_T list = Data_get_doctor();
            List_T result = List_new(sizeof(Doctor_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Doctor_T d = *(Doctor_T*)ptr;
                if (Doctor_dept(d) == dept) {
                    List_push_back(result, &d);
                }
                ptr = List_next(list);
            }
            if (List_size(result) > 0) {
                printf("--- %s 医生列表 ---\n", department_name(dept));
                __print_doctor_list(result);
            } else {
                printf("该科室暂无医生\n");
            }
            List_free(&result);
            press_enter();
        }
        else if (choice == 5) {
            CLEAN();
            printf("--- 修改医生信息 ---\n");
            long long doc_id = get_input_long_long("请输入要修改的医生ID", 0, 999999);
            List_T list = Data_get_doctor();
            void* ptr = List_first(list);
            Doctor_T target = NULL;
            while (ptr != NULL) {
                Doctor_T d = *(Doctor_T*)ptr;
                if (Doctor_id(d) == doc_id) {
                    target = d;
                    break;
                }
                ptr = List_next(list);
            }
            if (target == NULL) {
                printf("未找到该医生\n");
                press_enter();
                continue;
            }

            CLEAN();
            printf("--- 修改医生信息 (ID: %lld) ---\n", doc_id);
            printf("当前信息：\n");
            printf("  姓名：%s\n", Doctor_name(target));
            printf("  性别：%s\n", Doctor_gender(target) == MALE ? "男" : "女");
            printf("  出生日期：%s\n", Time_to_string_date(Doctor_birth_ts(target)));
            printf("  科室：%s\n", department_name(Doctor_dept(target)));
            const char* cur_title_str = "";
            switch(Doctor_title(target)){
                case TITLE_RESIDENT:    cur_title_str = "住院医师"; break;
                case TITLE_ATTENDING:   cur_title_str = "主治医师"; break;
                case TITLE_ASSOC_CHIEF: cur_title_str = "副主任医师"; break;
                case TITLE_CHIEF:       cur_title_str = "主任医师"; break;
                default:                cur_title_str = "医生"; break;
            }
            printf("  职称：%s\n", cur_title_str);
            printf("  挂号费：%lld.%02lld 元\n", Doctor_reg_fee(target)/100, Doctor_reg_fee(target)%100);
            printf("  出诊状态：%s\n", Doctor_is_active(target) ? "出诊中" : "未出诊");
            printf("  电话：%s\n", Doctor_phone(target));
            printf("  身份证号：%s\n", Doctor_id_card(target));
            printf("\n请输入新信息：\n");

            Doctor_Update_Pack pack;
            pack.gender = (gender)get_input_long_long(
                "性别 (0:女, 1:男)", 0, 1);
            pack.birth_ts = __input_date("出生日期");
            pack.dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            pack.title = (DoctorTitle)get_input_long_long(
                "职称：\n1.住院医师\t2.主治医师\t3.副主任医师\t4.主任医师", 1, 4);
            pack.reg_fee = get_input_long_long("挂号费（分）", 0, 100000);
            pack.is_active = (bool)get_input_long_long("出诊状态 (0:未出诊, 1:出诊中)", 0, 1);
            get_input_str("姓名", pack.name, 32);
            get_input_str("电话", pack.phone, 20);
            get_input_str("身份证号", pack.id_card, 20);
            Status s = Doctor_update(target, &pack);
            if (s == HIS_OK) {
                printf("修改成功！\n");
            } else {
                printf("修改失败！\n");
            }
            press_enter();
        }
    }
    return HIS_OK;
}

static void __print_medicine_list(List_T list) {
    printf("%-8s | %-20s | %-10s | %-10s\n", "ID", "药品名称", "零售价", "总库存");
    printf("------------------------------------------------------------\n");
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Medicine_T m = *(Medicine_T*)ptr;
        long long price = Medicine_cur_price(m);
        printf("%-8lld | %-20s | %lld.%02lld 元 | %-10d\n",
            Medicine_id(m), Medicine_name(m), price/100, price%100, Medicine_total_remain(m));
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无药品\n");
}

static Status __medicine() {
    while (1) {
        CLEAN();
        printf("--- 药品及库存管理 ---\n");
        __print_medicine_list(Data_get_medicine());

        printf("\n1. 新增药品  2. 进货(加批次)  3. 删除药品  4. 搜索药品  5. 修改药品信息  6. 查看批次  7. 报废批次  0. 返回\n");
        long long choice = get_input_long_long("操作", 0, 7);
        if (choice == 0) break;

        if (choice == 1) {
            char name[32];
            get_input_str("药品名称", name, 32);
            long long price = get_input_long_long("零售价（分）", 0, 100000);
            Status s = Serv_root_add_medicine(price, name);
            if (s == HIS_OK) {
                printf("新增成功！\n");
            } else {
                printf("新增失败！\n");
            }
        }
        else if (choice == 2) {
            long long mid = get_input_long_long("药品 ID", 0, 9999);
            long long bprice = get_input_long_long("进价（分）", 0, 100000);
            int count = (int)get_input_long_long("数量", 1, 1000);
            long long expire_ts = __input_date("过期日期");
            char bno[32];
            get_input_str("批次号", bno, 32);
            Status s = Serv_root_add_medicine_batch(mid, bprice, expire_ts, count, bno);
            if (s == HIS_OK) {
                printf("进货成功！\n");
            } else {
                printf("未找到该药品\n");
            }
        }
        else if (choice == 3) {
            long long mid = get_input_long_long("删除药品 ID", 0, 9999);
            Status s = Serv_root_remove_medicine(mid);
            if (s == HIS_OK) {
                printf("删除成功！\n");
            } else {
                printf("未找到该药品\n");
            }
        }
        else if (choice == 4) {
            CLEAN();
            printf("--- 搜索药品 ---\n");
            char keyword[32];
            get_input_str("请输入药品名称关键字", keyword, 32);
            List_T list = Data_get_medicine();
            List_T result = List_new(sizeof(Medicine_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Medicine_T m = *(Medicine_T*)ptr;
                if (Medicine_cmp_fuzzy(&m, keyword) == 0) {
                    List_push_back(result, &m);
                }
                ptr = List_next(list);
            }
            if (List_size(result) > 0) {
                __print_medicine_list(result);
            } else {
                printf("未找到匹配的药品\n");
            }
            List_free(&result);
            press_enter();
        }
        else if (choice == 5) {
            CLEAN();
            printf("--- 修改药品信息 ---\n");
            long long mid = get_input_long_long("请输入要修改的药品ID", 0, 9999);
            List_T list = Data_get_medicine();
            void* ptr = List_first(list);
            Medicine_T target = NULL;
            while (ptr != NULL) {
                Medicine_T m = *(Medicine_T*)ptr;
                if (Medicine_id(m) == mid) {
                    target = m;
                    break;
                }
                ptr = List_next(list);
            }
            if (target == NULL) {
                printf("未找到该药品\n");
                press_enter();
                continue;
            }

            CLEAN();
            printf("--- 修改药品信息 (ID: %lld) ---\n", mid);
            printf("当前信息：\n");
            printf("  名称：%s\n", Medicine_name(target));
            printf("  零售价：%lld.%02lld 元\n", Medicine_cur_price(target)/100, Medicine_cur_price(target)%100);
            printf("  总库存：%d\n", Medicine_total_remain(target));
            printf("\n请输入新信息：\n");

            char name[32];
            get_input_str("药品名称", name, 32);
            if (strlen(name) > 0) {
                Medicine_set_name(target, name);
            }
            long long new_price = get_input_long_long("零售价（分，输入-1跳过）", -1, 100000);
            if (new_price >= 0) {
                Medicine_set_price(target, (int)new_price);
            }
            printf("修改成功！\n");
            press_enter();
        }
        else if (choice == 6) {
            CLEAN();
            printf("--- 查看批次 ---\n");
            long long mid = get_input_long_long("药品 ID", 0, 9999);
            List_T med_list = Data_get_medicine();
            void* mp = List_first(med_list);
            Medicine_T target = NULL;
            while (mp != NULL) {
                Medicine_T m = *(Medicine_T*)mp;
                if (Medicine_id(m) == mid) {
                    target = m;
                    break;
                }
                mp = List_next(med_list);
            }
            if (target == NULL) {
                printf("未找到该药品\n");
                press_enter();
                continue;
            }

            printf("\n药品：%s\n", Medicine_name(target));
            printf("%-8s | %-12s | %-12s | %-8s | %-10s | %s\n",
                "批次ID", "进价", "过期时间", "剩余", "状态", "批号");
            printf("----------------------------------------------------------------------\n");
            List_T batches = Medicine_batches(target);
            void* bp = List_first(batches);
            while (bp != NULL) {
                MedicineBatch* bat = (MedicineBatch*)bp;
                const char* status_str = "";
                switch(bat->status) {
                    case AVAILABLE: status_str = "可用"; break;
                    case EXHAUSTED: status_str = "用尽"; break;
                    case DISCARD:   status_str = "弃置"; break;
                }
                printf("%-8lld | %lld.%02lld 元 | %-12s | %-8d | %-10s | %s\n",
                    bat->id, bat->buy_price/100, bat->buy_price%100,
                    Time_to_string_date(bat->expire_ts),
                    bat->remain, status_str, bat->batch_no);
                bp = List_next(batches);
            }
            press_enter();
        }
        else if (choice == 7) {
            CLEAN();
            printf("--- 报废批次 ---\n");
            long long mid = get_input_long_long("药品 ID", 0, 9999);
            List_T med_list = Data_get_medicine();
            void* mp = List_first(med_list);
            Medicine_T target = NULL;
            while (mp != NULL) {
                Medicine_T m = *(Medicine_T*)mp;
                if (Medicine_id(m) == mid) {
                    target = m;
                    break;
                }
                mp = List_next(med_list);
            }
            if (target == NULL) {
                printf("未找到该药品\n");
                press_enter();
                continue;
            }

            // 先展示该药品的所有批次
            printf("\n药品：%s\n", Medicine_name(target));
            printf("%-8s | %-12s | %-12s | %-8s | %-10s | %s\n",
                "批次ID", "进价", "过期时间", "剩余", "状态", "批号");
            printf("----------------------------------------------------------------------\n");
            List_T batches = Medicine_batches(target);
            void* bp = List_first(batches);
            while (bp != NULL) {
                MedicineBatch* bat = (MedicineBatch*)bp;
                const char* status_str = "";
                switch(bat->status) {
                    case AVAILABLE: status_str = "可用"; break;
                    case EXHAUSTED: status_str = "用尽"; break;
                    case DISCARD:   status_str = "弃置"; break;
                }
                printf("%-8lld | %lld.%02lld 元 | %-12s | %-8d | %-10s | %s\n",
                    bat->id, bat->buy_price/100, bat->buy_price%100,
                    Time_to_string_date(bat->expire_ts),
                    bat->remain, status_str, bat->batch_no);
                bp = List_next(batches);
            }

            long long batch_id = get_input_long_long("请输入要报废的批次ID", 0, 999999);
            Status s = Medicine_discard_batch(target, batch_id);
            if (s == HIS_OK) {
                printf("报废成功！\n");
            } else {
                printf("报废失败（批次不存在或已报废）\n");
            }
            press_enter();
        }
    }
    return HIS_OK;
}

static void __print_ward_list(List_T list) {
    printf("%-8s | %-10s | %-12s | %-8s | %-8s\n", "ID", "科室", "名称", "总床位", "空床");
    printf("------------------------------------------------------------\n");
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Ward_T w = *(Ward_T*)ptr;
        printf("%-8lld | %-10s | %-12s | %-8d | %-8d\n",
            Ward_id(w), department_name(Ward_dept(w)), Ward_name(w), Ward_bed_count(w), Ward_empty_count(w));
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无病房\n");
}

static void __print_bed_detail(Ward_T w) {
    printf("\n病房：%s（%s）\n", Ward_name(w), department_name(Ward_dept(w)));
    printf("%-6s | %-10s | %-10s | %s\n", "床号", "状态", "病人ID", "入住时间");
    printf("--------------------------------------------------\n");
    List_T beds = Ward_beds(w);
    void* bp = List_first(beds);
    while (bp != NULL) {
        Bed_T* bed = (Bed_T*)bp;
        const char* status_str = "";
        switch(bed->status) {
            case BED_EMPTY:     status_str = "空闲"; break;
            case BED_OCCUPIED:  status_str = "有人"; break;
            case BED_MAINTAIN:  status_str = "维修"; break;
        }
        char time_str[32] = "---";
        if (bed->start_ts > 0) {
            strncpy(time_str, Time_to_string_date(bed->start_ts), 32);
        }
        printf("%-6d | %-10s | %-10lld | %s\n",
            bed->bed_label, status_str, bed->pat_id, time_str);
        bp = List_next(beds);
    }
}

static Status __ward() {
    while (1) {
        CLEAN();
        printf("--- 病房管理 ---\n");
        __print_ward_list(Data_get_ward());

        printf("\n1. 新增病房  2. 删除病房  3. 修改病房信息  4. 查看床位明细  5. 按科室筛选  0. 返回\n");
        long long choice = get_input_long_long("请选择", 0, 5);
        if (choice == 0) break;

        if (choice == 1) {
            CLEAN();
            printf("--- 新增病房 ---\n");
            Department dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            long long daily_cost = get_input_long_long("每日费用（分）", 0, 1000000);
            int start_bed = (int)get_input_long_long("起始床号", 1, 100);
            int bed_count = (int)get_input_long_long("床位数", 1, 100);
            char ward_name[32];
            get_input_str("病房名称", ward_name, 32);
            Status s = Serv_root_add_ward(dept, daily_cost, start_bed, bed_count, ward_name);
            if (s == HIS_OK) {
                printf("新增病房成功！\n");
            } else {
                printf("新增病房失败！\n");
            }
            press_enter();
        }
        else if (choice == 2) {
            CLEAN();
            printf("--- 删除病房 ---\n");
            long long ward_id = get_input_long_long("请输入要删除的病房ID", 0, 99999);
            Status s = Serv_root_remove_ward(ward_id);
            if (s == HIS_OK) {
                printf("删除成功！\n");
            } else {
                printf("未找到该病房\n");
            }
            press_enter();
        }
        else if (choice == 3) {
            CLEAN();
            printf("--- 修改病房信息 ---\n");
            long long ward_id = get_input_long_long("请输入要修改的病房ID", 0, 99999);
            List_T list = Data_get_ward();
            void* ptr = List_first(list);
            Ward_T target = NULL;
            while (ptr != NULL) {
                Ward_T w = *(Ward_T*)ptr;
                if (Ward_id(w) == ward_id) {
                    target = w;
                    break;
                }
                ptr = List_next(list);
            }
            if (target == NULL) {
                printf("未找到该病房\n");
                press_enter();
                continue;
            }

            CLEAN();
            printf("--- 修改病房信息 (ID: %lld) ---\n", ward_id);
            printf("当前信息：\n");
            printf("  名称：%s\n", Ward_name(target));
            printf("  科室：%s\n", department_name(Ward_dept(target)));
            printf("  每日费用：%lld.%02lld 元\n", Ward_daily_cost(target)/100, Ward_daily_cost(target)%100);
            printf("\n请输入新信息：\n");

            Ward_Update_Pack pack;
            pack.dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            pack.daily_cost = get_input_long_long("每日费用（分）", 0, 1000000);
            get_input_str("病房名称", pack.ward_name, 32);
            Status s = Ward_update(target, &pack);
            if (s == HIS_OK) {
                printf("修改成功！\n");
            } else {
                printf("修改失败！\n");
            }
            press_enter();
        }
        else if (choice == 4) {
            CLEAN();
            printf("--- 查看床位明细 ---\n");
            long long ward_id = get_input_long_long("请输入病房ID", 0, 99999);
            List_T list = Data_get_ward();
            void* ptr = List_first(list);
            Ward_T target = NULL;
            while (ptr != NULL) {
                Ward_T w = *(Ward_T*)ptr;
                if (Ward_id(w) == ward_id) {
                    target = w;
                    break;
                }
                ptr = List_next(list);
            }
            if (target == NULL) {
                printf("未找到该病房\n");
            } else {
                __print_bed_detail(target);
            }
            press_enter();
        }
        else if (choice == 5) {
            CLEAN();
            printf("--- 按科室筛选 ---\n");
            Department dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            List_T list = Data_get_ward();
            List_T result = List_new(sizeof(Ward_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Ward_T w = *(Ward_T*)ptr;
                if (Ward_dept(w) == dept) {
                    List_push_back(result, &w);
                }
                ptr = List_next(list);
            }
            if (List_size(result) > 0) {
                printf("--- %s 病房列表 ---\n", department_name(dept));
                __print_ward_list(result);
            } else {
                printf("该科室暂无病房\n");
            }
            List_free(&result);
            press_enter();
        }
    }
    return HIS_OK;
}

static Status __fund() {
    CLEAN();
    printf("--- 资金账户总览 ---\n");
    printf("%-8s | %-12s\n", "病人ID", "余额");
    printf("------------------------\n");
    List_T list = Data_get_fund();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Fund_T f = *(Fund_T*)ptr;
        printf("%-8lld | %-12lld.%02lld 元\n",

            Fund_pat_id(f), Fund_balance(f)/100, Fund_balance(f)%100);
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无资金账户\n");
    press_enter();
    return HIS_OK;
}

static void __print_records(List_T list) {
    printf("%-8s | %-12s | %-10s | %-12s | %-10s\n", "ID", "病人", "类型", "时间", "费用");
    printf("---------------------------------------------------------------\n");
    long long total_income = 0;
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Record_T r = *(Record_T*)ptr;
        long long cost = Rec_cost(r);
        total_income += cost;
        const char* rec_type_str = "";
        switch(Rec_type(r)) {
            case REC_REGISTRATION:  rec_type_str = "挂号"; break;
            case REC_CONSULTATION:  rec_type_str = "看诊"; break;
            case REC_EXAMINATION:   rec_type_str = "检查"; break;
            case REC_PRESCRIPTION:  rec_type_str = "开药"; break;
            case REC_ADMISSION:     rec_type_str = "入院"; break;
            case REC_DISCHARGE:     rec_type_str = "出院"; break;
            case REC_CHANGE_BED:    rec_type_str = "换床"; break;
            case REC_CHANGE_DOC:    rec_type_str = "换医生"; break;
            case REC_STOCK_IN:      rec_type_str = "入库"; break;
            case REC_STOCK_OUT:     rec_type_str = "出库"; break;
            default:                rec_type_str = "未知"; break;
        }
        printf("%-8lld | %-12s | %-10s | %-12s | %lld.%02lld 元\n",
            Rec_actor_id(r), Serv_helper_id_to_name(Rec_actor_id(r), TYPE_PATIENT),
            rec_type_str, Time_to_string_date(Rec_time_stamp(r)), cost/100, cost%100);
        ptr = List_next(list);
    }
    if(List_size(list) == 0) {
        printf("暂无记录\n");
    } else {
        printf("---------------------------------------------------------------\n");
        printf("总收入：%lld.%02lld 元\n", total_income/100, total_income%100);
    }
}

static Status __record() {
    while (1) {
        CLEAN();
        printf("--- 全院诊疗记录 ---\n");
        printf("1. 查看全部记录  2. 按时间筛选  0. 返回\n");
        long long choice = get_input_long_long("请选择", 0, 2);
        if (choice == 0) break;

        if (choice == 1) {
            CLEAN();
            printf("--- 全院诊疗记录 ---\n");
            __print_records(Data_get_record());
            press_enter();
        }
        else if (choice == 2) {
            CLEAN();
            printf("--- 按时间筛选 ---\n");
            printf("起始时间：\n");
            int y1 = (int)get_input_long_long("  年 (1900~2026)", 1900, 2026);
            int m1 = (int)get_input_long_long("  月 (1~12)", 1, 12);
            int d1 = (int)get_input_long_long("  日 (1~31)", 1, 31);
            struct tm tm1 = {0};
            tm1.tm_year = y1 - 1900; tm1.tm_mon = m1 - 1; tm1.tm_mday = d1;
            long long start_ts = (long long)mktime(&tm1);

            printf("结束时间：\n");
            int y2 = (int)get_input_long_long("  年 (1900~2026)", 1900, 2026);
            int m2 = (int)get_input_long_long("  月 (1~12)", 1, 12);
            int d2 = (int)get_input_long_long("  日 (1~31)", 1, 31);
            struct tm tm2 = {0};
            tm2.tm_year = y2 - 1900; tm2.tm_mon = m2 - 1; tm2.tm_mday = d2;
            long long end_ts = (long long)mktime(&tm2);

            List_T list = Data_get_record();
            List_T result = List_new(sizeof(Record_T));
            void* ptr = List_first(list);
            while (ptr != NULL) {
                Record_T r = *(Record_T*)ptr;
                if (Rec_time_stamp(r) >= start_ts && Rec_time_stamp(r) <= end_ts) {
                    List_push_back(result, &r);
                }
                ptr = List_next(list);
            }
            printf("--- 筛选结果 ---\n");
            __print_records(result);
            List_free(&result);
            press_enter();
        }
    }
    return HIS_OK;
}

static Status __account(){
    while (1){
        CLEAN();
        printf("--- 系统账号管理 ---\n");
        printf("%-8s | %-12s | %-10s\n", "ID", "用户名", "类型");
        printf("--------------------------------------\n");

        List_T list = Data_get_account();
        void* ptr = List_first(list);
        while (ptr != NULL) {
            Account_T a = *(Account_T*)ptr;
            const char* class_str = "";
            switch(Account_class(a)) {
                case CLASS_PATIENT: class_str = "病人"; break;
                case CLASS_DOCTOR:  class_str = "医生"; break;
                case CLASS_ROOT:    class_str = "管理员"; break;
                default:            class_str = "未知"; break;
            }
            printf("%-8lld | %-12s | %-10s\n", Account_id(a), Account_name(a), class_str);
            ptr = List_next(list);
        }

        printf("\n输入0返回\n");
        long long choice = get_input_long_long("", 0, 0);
        if (choice == 0) break;
    }
    return HIS_OK;
}

Status UI_root_menu() {
    UI_root_should_continue = true;
    while (UI_root_should_continue) {
        CLEAN();
        printf("========== 管理员菜单 ==========\n");
        printf("1. 病人信息总览\n");
        printf("2. 医生管理\n");
        printf("3. 药品管理\n");
        printf("4. 病房状态查询\n");
        printf("5. 资金账户总览\n");
        printf("6. 全院诊疗记录\n");
        printf("7. 系统账号管理\n");
        printf("8. 修改密码\n");
        printf("0. 退出登录\n");
        printf("===============================\n");

        long long choice = get_input_long_long("请选择", 0, 8);
        switch (choice) {
        case 0: {
            CLEAN();
            printf("--- 退出 ---\n");
            printf("1. 退出登录（返回登录界面）\n");
            printf("2. 退出系统\n");
            printf("0. 取消\n");
            long long quit_choice = get_input_long_long("请选择", 0, 2);
            if (quit_choice == 1) {
                UI_root_should_continue = false;
                printf("退出登录...\n");
                return HIS_OK;
            } else if (quit_choice == 2) {
                UI_root_should_continue = false;
                printf("正在退出系统……\n");
                return HIS_QUIT;
            }
            break;
        }
        case 1: __patient(); break;
        case 2: __doctor(); break;
        case 3: __medicine(); break;
        case 4: __ward(); break;
        case 5: __fund(); break;
        case 6: __record(); break;
        case 7: __account(); break;
        case 8: {
            CLEAN();
            printf("--- 密码管理 ---\n");
            printf("1. 修改我的密码\n");
            printf("2. 重置医生密码\n");
            printf("0. 返回\n");
            long long pw_choice = get_input_long_long("请选择", 0, 2);
            if (pw_choice == 0) break;

            if (pw_choice == 1) {
                char old_pw[20] = "", new_pw[20] = "";
                get_input_str("请输入当前密码", old_pw, 20);
                long long actor_id = Serv_account_cur_id();
                Account_T acc = (Account_T)Serv_helper_finder(actor_id, TYPE_ACCOUNT);
                if (acc != NULL && Account_check_password(acc, old_pw) == HIS_OK) {
                    get_input_str("请输入新密码", new_pw, 20);
                    Account_set_password(acc, new_pw);
                    printf("密码修改成功！\n");
                    Log_printf(CLASS_ROOT, actor_id, "管理员修改密码");
                } else {
                    printf("当前密码错误！\n");
                }
            }
            else if (pw_choice == 2) {
                long long doc_id = get_input_long_long("请输入要重置密码的医生ID", 0, 999999);
                Account_T acc = (Account_T)Serv_helper_finder(doc_id, TYPE_ACCOUNT);
                if (acc == NULL) {
                    printf("未找到该医生账号\n");
                } else {
                    Account_set_password(acc, "88888888");
                    printf("医生 [%lld] 密码已重置为 88888888，请提醒医生及时修改密码。\n", doc_id);
                    Log_printf(CLASS_ROOT, Serv_account_cur_id(), "管理员重置医生[%lld]密码", doc_id);
                }
            }
            press_enter();
            break;
        }
        default: break;
        }
    }
    return HIS_OK;
}
