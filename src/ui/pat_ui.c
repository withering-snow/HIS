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
static long long __input_date(const char* prompt) {
    printf("%s\n", prompt);
    int y, m, d;
    while (1) {
        y = (int)get_input_long_long("  年 (1900~2026)", 1900, 2026);
        if (y >= 1900 && y <= 2026) break;
        printf("  年份不合法，请重新输入\n");
    }
    while (1) {
        m = (int)get_input_long_long("  月 (1~12)", 1, 12);
        if (m >= 1 && m <= 12) break;
        printf("  月份不合法，请重新输入\n");
    }
    while (1) {
        d = (int)get_input_long_long("  日 (1~31)", 1, 31);
        if (d >= 1 && d <= 31) break;
        printf("  日期不合法，请重新输入\n");
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
        printf(" 0 - 退出登录\n");
        printf("=======================\n");

        PAT_MAIN_MENU_CHOICE main_choice = get_input_long_long("请选择功能", 0, 4);

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
                printf("\n选择预约日期（只允许预约3天内）：\n");
                int y = (int)get_input_long_long("  年", 2026, 2026);
                int m = (int)get_input_long_long("  月 (1~12)", 1, 12);
                int d = (int)get_input_long_long("  日 (1~31)", 1, 31);
                struct tm tm = {0};
                tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d;
                int target_date = Time_to_int_date((long long)mktime(&tm));

                // 检查是否在3天内
                if (target_date < today || target_date > today + 3) {
                    printf("只能预约今天起3天内！\n");
                    List_free(&available);
                    break;
                }

                // 5. 选择时间段
                printf("\n选择时间段：\n");
                printf(" 0 - 急诊\n");
                printf(" 1 - 8:00-8:30   2 - 8:30-9:00   3 - 9:00-9:30   4 - 9:30-10:00\n");
                printf(" 5 - 10:00-10:30 6 - 10:30-11:00 7 - 11:00-11:30 8 - 11:30-12:00\n");
                printf(" 9 - 13:00-13:30 10 - 13:30-14:00 11 - 14:00-14:30 12 - 14:30-15:00\n");
                printf(" 13 - 15:00-15:30 14 - 15:30-16:00 15 - 16:00-16:30 16 - 16:30-17:00\n");
                int time_frame = (int)get_input_long_long("请选择", 0, 16);

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
