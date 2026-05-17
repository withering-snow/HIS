#include <pat_ui.h>
#include <log_ctrl.h>
load_ui_tools

bool UI_pat_should_continue = true;

typedef enum {
    PAT_QUIT       = 0,
    PAT_PATIENT    = 1,
    PAT_RECORD     = 2,
    PAT_DOCTOR     = 3,
    PAT_REGISTER   = 4,
    PAT_CHECKIN    = 5,
} PAT_MAIN_MENU_CHOICE;

typedef enum {
    PAT_SUBORD_MENU_CHOICE_INVALID1 = -1,
    PAT_SUBORD_MENU_CHOICE_PATIENT = 1,
    PAT_SUBORD_MENU_CHANGE_PHONE = 2,
} PAT_SUBORD_MENU_PATIENT;

typedef enum {
    PAT_SUBORD_MENU_CHOICE_INVALID2 = -1,
    PAT_SUBORD_MENU_CHOICE_RECORD = 1,
    PAT_SUBORD_MENU_FILTER_TIME = 2,
    PAT_SUBORD_MENU_CHECK_FUND = 3,
    PAT_SUBORD_MENU_DEPOSIT = 4,
} PAT_SUBORD_MENU_RECORD;

// 辅助打印函数：显示患者基本信息
static void UI_show_patient_info(ServPatientDataPackage* pkg) {
    if (!pkg) {
        printf("未找到患者信息！\n");
        return;
    }
    printf("\n====== 患者信息 ======\n");
    printf("姓名：%s\n", pkg->name);
    printf("性别：%s\n", pkg->gender == MALE ? "男" : "女");
    printf("年龄：%d 岁\n", pkg->age);
    printf("生日：%s\n", pkg->birthday);
    printf("电话：%s\n", pkg->phone);
    printf("身份证：%s\n", pkg->id_card);
    printf("======================\n");
}

// 三级输入日期
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

// 主菜单
Status UI_pat_menu() {
    while (UI_pat_should_continue) {
        CLEAN();
        printf("=======================\n");
        printf("      患者主菜单       \n");
        printf("=======================\n");
        printf(" 1 - 患者信息管理\n");
        printf(" 2 - 诊疗记录查询\n");
        printf(" 3 - 医生信息查询\n");
        printf(" 4 - 预约挂号\n");
        printf(" 5 - 签到与排队状态\n");
        printf(" 0 - 退出登录\n");
        printf("=======================\n");

        PAT_MAIN_MENU_CHOICE main_choice = get_input_long_long("请选择功能", 0, 5);

        switch (main_choice) {
            case PAT_PATIENT: {
                CLEAN();
                long long pat_id = Serv_account_cur_id();

                printf("\n====== 患者子菜单 ======\n");
                printf("1 - 查看个人信息\n");
                printf("2 - 修改手机号码\n");
                printf("========================\n");
                PAT_SUBORD_MENU_PATIENT choice1 = get_input_long_long("请选择", 1, 2);

                if (choice1 == PAT_SUBORD_MENU_CHOICE_PATIENT) {
                    // 1. 查询患者信息（自动使用当前登录患者）
                    ServPatientDataPackage* pkg = Serv_patient_data(pat_id);
                    UI_show_patient_info(pkg);
                    Serv_helper_free_value(pkg);
                }
                else if (choice1 == PAT_SUBORD_MENU_CHANGE_PHONE) {
                    // 2. 修改电话
                    char new_phone[20];
                    get_input_str("请输入新手机号", new_phone, 20);
                    List_T list = Data_get_patient();
                    void* ptr = List_first(list);
                    while (ptr != NULL) {
                        Patient_T p = *(Patient_T*)ptr;
                        if (Patient_id(p) == pat_id) {
                            Patient_Update_Pack pack;
                            pack.gender = Patient_gender(p);
                            pack.birth_ts = Patient_birth_ts(p);
                            strncpy(pack.name, Patient_name(p), 32);
                            strncpy(pack.phone, new_phone, 20);
                            strncpy(pack.id_card, Patient_id_card(p), 20);
                            Patient_update(p, &pack);
                            printf("手机号修改成功！\n");
                            Log_printf(CLASS_PATIENT, pat_id, "病人修改手机号[%s]->[%s]", Patient_phone(p), new_phone);
                            break;
                        }
                        ptr = List_next(list);
                    }
                }
                break;
            }

            case PAT_RECORD: {
                CLEAN();
                long long pat_id = Serv_account_cur_id(); // 自动使用当前登录患者

                printf("\n====== 诊疗记录菜单 ======\n");
                printf("1 - 查看全部记录\n");
                printf("2 - 按时间筛选\n");
                printf("3 - 查询账户余额\n");
                printf("4 - 账户充值\n");
                printf("==========================\n");
                PAT_SUBORD_MENU_RECORD choice2 = get_input_long_long("请选择", 1, 4);

                if (choice2 == PAT_SUBORD_MENU_CHOICE_RECORD) {
                    // 查询诊疗记录
                    List_T records = Serv_patient_records(pat_id);
                    printf("共 %zu 条记录\n", List_size(records));
                    void* rp = List_first(records);
                    while (rp != NULL) {
                        ServRecordDataPackage* pkg = (ServRecordDataPackage*)rp;
                        printf("---\n");
                        printf("类型: %s\n", pkg->type_name);
                        printf("时间: %s\n", Time_to_string_date(pkg->time_stamp));
                        printf("费用: %lld.%02lld 元\n", pkg->cost/100, pkg->cost%100);
                        printf("详情: %s\n", pkg->content);
                        rp = List_next(records);
                    }
                    Serv_helper_free_list(records);
                }
                else if (choice2 == PAT_SUBORD_MENU_FILTER_TIME) {
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

                    List_T records = Serv_patient_records(pat_id);
                    printf("共 %zu 条记录\n", List_size(records));
                    void* rp = List_first(records);
                    while (rp != NULL) {
                        ServRecordDataPackage* pkg = (ServRecordDataPackage*)rp;
                        if (pkg->time_stamp >= start_ts && pkg->time_stamp <= end_ts) {
                            printf("---\n");
                            printf("类型: %s\n", pkg->type_name);
                            printf("时间: %s\n", Time_to_string_date(pkg->time_stamp));
                            printf("费用: %lld.%02lld 元\n", pkg->cost/100, pkg->cost%100);
                            printf("详情: %s\n", pkg->content);
                        }
                        rp = List_next(records);
                    }
                    Serv_helper_free_list(records);
                }
                else if (choice2 == PAT_SUBORD_MENU_CHECK_FUND) {
                    // 查询余额
                    long long fund = Serv_patient_get_fund();
                    if (fund < 0)
                        printf("未找到患者账户！\n");
                    else
                        printf("当前账户余额：%lld.%02lld 元\n", fund/100, fund%100);
                }
                else if (choice2 == PAT_SUBORD_MENU_DEPOSIT) {
                    // 充值
                    long long amount = get_input_long_long("请输入充值金额（分）", 1, 100000000);
                    Fund_T fund = (Fund_T)Serv_helper_finder(pat_id, TYPE_FUND);
                    if (fund == NULL) {
                        printf("未找到患者账户！\n");
                    } else {
                        Fund_deposit(fund, amount);
                        printf("充值成功！当前余额：%lld.%02lld 元\n",
                            Fund_balance(fund)/100, Fund_balance(fund)%100);
                        Log_printf(CLASS_PATIENT, pat_id, "病人充值[%lld]分 余额[%lld]", amount, Fund_balance(fund));
                    }
                }
                break;
            }

            case PAT_DOCTOR:
                CLEAN();
                printf("--- 医生信息查询 ---\n");
                printf("%-8s | %-12s | %-4s | %-10s | %-10s | %-8s | %-10s\n",
                    "ID", "姓名", "性别", "科室", "职称", "出诊", "挂号费");
                printf("--------------------------------------------------------------------------------\n");
                {
                    List_T doc_list = Data_get_doctor();
                    void* dp = List_first(doc_list);
                    while (dp != NULL) {
                        Doctor_T d = *(Doctor_T*)dp;
                        const char* title_str = "";
                        switch(Doctor_title(d)){
                            case TITLE_RESIDENT:    title_str = "住院医师"; break;
                            case TITLE_ATTENDING:   title_str = "主治医师"; break;
                            case TITLE_ASSOC_CHIEF: title_str = "副主任医师"; break;
                            case TITLE_CHIEF:       title_str = "主任医师"; break;
                            default:                title_str = "医生"; break;
                        }
                        printf("%-8lld | %-12s | %-4s | %-10s | %-10s | %-8s | %lld.%02lld 元\n",
                            Doctor_id(d), Doctor_name(d),
                            Doctor_gender(d) == MALE ? "男" : "女",
                            department_name(Doctor_dept(d)),
                            title_str,
                            Doctor_is_active(d) ? "出诊中" : "未出诊",
                            Doctor_reg_fee(d)/100, Doctor_reg_fee(d)%100);
                        dp = List_next(doc_list);
                    }
                    if (List_size(doc_list) == 0) printf("暂无医生信息\n");
                }
                break;

            case PAT_REGISTER: {
                CLEAN();
                printf("--- 预约挂号 ---\n");

                // 1. 选择科室
                Department dept = (Department)get_input_long_long(
                    "选择科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);

                // 2. 展示该科室在诊医生
                List_T doc_list = Data_get_doctor();
                List_T available = List_new(sizeof(Doctor_T));
                void* dp = List_first(doc_list);
                while (dp != NULL) {
                    Doctor_T d = *(Doctor_T*)dp;
                    if (Doctor_dept(d) == dept && Doctor_is_active(d)) {
                        List_push_back(available, &d);
                    }
                    dp = List_next(doc_list);
                }

                if (List_size(available) == 0) {
                    printf("该科室暂无出诊医生\n");
                    List_free(&available);
                    break;
                }

                printf("\n--- %s 出诊医生 ---\n", department_name(dept));
                printf("%-8s | %-12s | %-10s | %-10s\n", "ID", "姓名", "职称", "挂号费");
                printf("----------------------------------------------------------\n");
                void* ap = List_first(available);
                while (ap != NULL) {
                    Doctor_T d = *(Doctor_T*)ap;
                    const char* title_str = "";
                    switch(Doctor_title(d)){
                        case TITLE_RESIDENT:    title_str = "住院医师"; break;
                        case TITLE_ATTENDING:   title_str = "主治医师"; break;
                        case TITLE_ASSOC_CHIEF: title_str = "副主任医师"; break;
                        case TITLE_CHIEF:       title_str = "主任医师"; break;
                        default:                title_str = "医生"; break;
                    }
                    printf("%-8lld | %-12s | %-10s | %lld.%02lld 元\n",
                        Doctor_id(d), Doctor_name(d), title_str,
                        Doctor_reg_fee(d)/100, Doctor_reg_fee(d)%100);
                    ap = List_next(available);
                }

                // 3. 选择医生
                long long doc_id = get_input_long_long("请选择医生ID", 0, 999999);

                // 4. 选择日期（只允许预约3天内）
                int today = Time_to_int_date(Time_now());
                // 计算今天、明天、后天的具体日期字符串
                char date_str[3][20];
                for (int i = 0; i < 3; i++) {
                    long long ts = Int_date_to_time(today + i);
                    strncpy(date_str[i], Time_to_string_date(ts), 20);
                }
                printf("\n选择预约日期：\n");
                printf(" 1 - %s (今天)\n", date_str[0]);
                printf(" 2 - %s (明天)\n", date_str[1]);
                printf(" 3 - %s (后天)\n", date_str[2]);
                long long date_choice = get_input_long_long("请选择", 1, 3);
                int target_date = today + (int)(date_choice - 1);

                // 5. 选择时间段（如果是今天，过滤已过时段）
                // 每个时段的结束小时：急诊0点结束，1~4时段结束于12点，5~8结束于12点，9~16结束于17点
                // 简化判断：时段0(急诊)全天可约；时段1~8(上午)结束于12点；时段9~16(下午)结束于17点
                int min_tf = 0, max_tf = 16;
                if (target_date == today) {
                    // 获取当前小时
                    time_t now_ts = Time_now();
                    struct tm* now_tm = localtime(&now_ts);
                    int cur_hour = now_tm->tm_hour;
                    int cur_min = now_tm->tm_min;

                    // 根据当前时间计算最小可选时段
                    // 时段0(急诊)始终可选
                    // 时段1(8:00-8:30): 如果当前>=8:30则不可选
                    // 时段2(8:30-9:00): 如果当前>=9:00则不可选 ... 以此类推
                    // 上午时段1~8: 开始时间 = 8:00 + (i-1)*30分钟
                    // 下午时段9~16: 开始时间 = 13:00 + (i-9)*30分钟
                    int cur_total_min = cur_hour * 60 + cur_min;
                    min_tf = 0; // 急诊始终可选
                    for (int i = 1; i <= 16; i++) {
                        int start_hour, start_min;
                        if (i <= 8) {
                            start_hour = 8 + (i - 1) / 2;
                            start_min = (i - 1) % 2 * 30;
                        } else {
                            start_hour = 13 + (i - 9) / 2;
                            start_min = (i - 9) % 2 * 30;
                        }
                        int start_total_min = start_hour * 60 + start_min;
                        // 如果该时段开始时间在当前时间之后，则可用
                        if (start_total_min > cur_total_min) {
                            min_tf = i;
                            break;
                        }
                    }
                    if (min_tf == 0 && cur_total_min >= 0) {
                        // 所有时段都过了，但急诊还是可以选
                        min_tf = 0;
                    }
                }

                printf("\n选择时间段：\n");
                printf(" 0 - 急诊");
                if (target_date == today && min_tf > 0) printf(" (已过时段已隐藏)");
                printf("\n");
                for (int i = 1; i <= 16; i++) {
                    if (target_date == today && i < min_tf) continue; // 跳过已过时段
                    int start_hour, start_min, end_hour, end_min;
                    if (i <= 8) {
                        start_hour = 8 + (i - 1) / 2;
                        start_min = (i - 1) % 2 * 30;
                        end_hour = 8 + i / 2;
                        end_min = i % 2 * 30;
                    } else {
                        start_hour = 13 + (i - 9) / 2;
                        start_min = (i - 9) % 2 * 30;
                        end_hour = 13 + (i - 8) / 2;
                        end_min = (i - 8) % 2 * 30;
                    }
                    printf(" %2d - %02d:%02d-%02d:%02d", i, start_hour, start_min, end_hour, end_min);
                    if (i % 4 == 0) printf("\n");
                }
                if (target_date != today || 16 >= min_tf) {
                    if (16 % 4 != 0) printf("\n");
                }
                int time_frame = (int)get_input_long_long("请选择", min_tf, 16);

                // 6. 检查号源并预约
                Status s = Serv_patient_register(doc_id, target_date, time_frame);
                if (s == HIS_OK) {
                    printf("预约成功！\n");
                } else if (s == HIS_ERR_QUEUE_FULL) {
                    printf("该时段已满，请选择其他时段\n");
                } else {
                    printf("预约失败！\n");
                }

                List_free(&available);
                break;
            }

            case PAT_CHECKIN: {
                CLEAN();
                printf("--- 签到与排队状态 ---\n");
                long long pat_id = Serv_account_cur_id();

                // 先查看当前排队状态（只显示已签到的候诊中状态）
                List_T queue_status = Serv_patient_queue_status();
                if (queue_status != NULL && List_size(queue_status) > 0) {
                    printf("当前候诊状态：\n");
                    printf("%-8s | %-12s | %-8s | %-8s | %s\n", "医生ID", "医生姓名", "排队号", "位置", "前面人数");
                    printf("------------------------------------------------------------\n");
                    void* qp = List_first(queue_status);
                    while (qp != NULL) {
                        ServQueueStatusPackage* pkg = (ServQueueStatusPackage*)qp;
                        const char* doc_name = Serv_helper_id_to_name(pkg->doc_id, TYPE_DOCTOR);
                        printf("%-8lld | %-12s | %-8d | %-8d | %d\n",
                            pkg->doc_id, doc_name ? doc_name : "未知",
                            pkg->sequence_no, pkg->position, pkg->people_ahead);
                        qp = List_next(queue_status);
                    }
                } else {
                    printf("暂无候诊信息（请先预约挂号，然后签到）\n");
                }
                Serv_helper_free_list(queue_status);

                // 提供签到选项
                printf("\n1. 签到\n");
                printf("0. 返回\n");
                long long checkin_choice = get_input_long_long("请选择", 0, 1);
                if (checkin_choice == 1) {
                    // 先展示该病人的预约记录
                    printf("\n--- 我的预约记录 ---\n");
                    List_T records = Data_get_record();
                    int today = Time_to_int_date(Time_now());
                    void* rp = List_first(records);
                    bool has_appoint = false;
                    while (rp != NULL) {
                        Record_T r = *(Record_T*)rp;
                        if (!Rec_is_invalid(r) && Rec_type(r) == REC_REGISTRATION &&
                            Rec_actor_id(r) == pat_id)
                        {
                            DataRegistration* data = (DataRegistration*)Rec_detail(r);
                            if (data->target_date >= today &&
                                (data->status == APPOINTMENT || data->status == WAITING))
                            {
                                const char* doc_name = Serv_helper_id_to_name(data->doc_id, TYPE_DOCTOR);
                                const char* tf_str = "";
                                if (data->time_frame == 0) tf_str = "急诊";
                                else if (data->time_frame <= 8) tf_str = "上午";
                                else tf_str = "下午";
                                printf("  医生ID: %-8lld | 医生: %-12s | 日期: %d | %s | 状态: %s\n",
                                    data->doc_id, doc_name ? doc_name : "未知",
                                    data->target_date, tf_str,
                                    data->status == APPOINTMENT ? "待签到" : "候诊中");
                                has_appoint = true;
                            }
                        }
                        rp = List_next(records);
                    }
                    if (!has_appoint) printf("  暂无待签到的预约记录\n");

                    long long doc_id = get_input_long_long("请输入要签到的医生ID", 0, 999999);
                    Status s = Serv_patient_checkin(doc_id);
                    if (s == HIS_OK) {
                        printf("签到成功！请等待医生叫号。\n");
                    } else if (s == HIS_ERR_NOT_FOUND) {
                        printf("未找到该医生的预约记录，请确认已预约且日期正确\n");
                    } else if (s == HIS_ERR_ALREADY_EXISTS) {
                        printf("已签到，请耐心等待叫号\n");
                    } else if (s == HIS_ERR_NO_FUNDS || s == HIS_ERR_INSUFFICIENT_FUNDS) {
                        printf("余额不足，无法支付挂号费，请先充值\n");
                    } else {
                        printf("签到失败！\n");
                    }
                }
                break;
            }

            case PAT_QUIT:
                CLEAN();
                printf("--- 退出 ---\n");
                printf("1. 退出登录（返回登录界面）\n");
                printf("2. 退出系统\n");
                printf("0. 取消\n");
                long long quit_choice = get_input_long_long("请选择", 0, 2);
                if (quit_choice == 1) {
                    UI_pat_should_continue = false;
                    Serv_account_signout();
                    printf("已退出登录！\n");
                } else if (quit_choice == 2) {
                    UI_pat_should_continue = false;
                    Serv_account_signout();
                    printf("正在退出系统……\n");
                    return HIS_QUIT;
                }
                break;

            default:
                printf("输入无效！\n");
                break;
        }
        press_enter();
    }
    return HIS_OK;
}
