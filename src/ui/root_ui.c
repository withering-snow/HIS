#include <root_ui.h>
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




static Status __patient() {
    CLEAN();
    printf("--- 病人信息总览 ---\n");
    printf("%-8s | %-12s | %-4s | %-15s | %-20s\n", "ID", "姓名", "性别", "电话", "身份证号");
    printf("----------------------------------------------------------------------\n");

    List_T list = Data_get_patient();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Patient_T p = *(Patient_T*)ptr;
        printf("%-8lld | %-12s | %-4s | %-15s | %-20s\n",
            Patient_id(p), Patient_name(p),
            Patient_gender(p) == MALE ? "男" : "女",
            Patient_phone(p), Patient_id_card(p));
        ptr = List_next(list);
    }
    printf("\n按任意键返回...");
    getchar();
    return HIS_OK;
}

static Status __doctor() {
    while (1) {
                        CLEAN();
        printf("--- 医生管理 ---\n");
        printf("%-8s | %-12s | %-10s | %-10s | %-10s\n", "ID", "姓名", "科室", "职称", "挂号费");
        printf("----------------------------------------------------------------------\n");

        List_T list = Data_get_doctor();
        void* ptr = List_first(list);
        while (ptr != NULL) {
            Doctor_T d = *(Doctor_T*)ptr;
            printf("%-8lld | %-12s | %-10s | %-10s | %lld.%02lld 元\n",
                Doctor_id(d), Doctor_name(d), department_name(Doctor_dept(d)),
                "医生", Doctor_reg_fee(d)/100, Doctor_reg_fee(d)%100);
            ptr = List_next(list);
        }

        printf("\n1. 新增医生  2. 删除医生  0. 返回\n");
        long long choice = get_input_long_long("选择操作", 0, 2);
        if (choice == 0) break;

        // ... existing code (新增医生) ...
        if (choice == 1) {
            Doctor_Update_Pack pack;
            get_input_str("姓名", pack.name, 32);
            pack.gender = (gender)get_input_long_long("性别 (0:女, 1:男)", 0, 1);
            pack.dept = (Department)get_input_long_long(
                "科室：\n1.内科\t2.外科\t3.儿科\t4.妇产科\n5.眼科\t6.口腔科\t7.皮肤科\t8.急诊科\n9.放射科\t10.检验科\t11.药剂科", 1, 11);
            pack.title = TITLE_RESIDENT; // 默认
            long long reg_fee = get_input_long_long("挂号费（分）", 0, 100000);
            pack.reg_fee = reg_fee;
            pack.birth_ts = 0;
            pack.is_active = true;
            get_input_str("电话", pack.phone, 20);
            get_input_str("身份证", pack.id_card, 20);
            Serv_root_add_doctor(&pack);
        }
        else if (choice == 2) {
            long long id = get_input_long_long("输入要删除的 ID", 0, 99999);
            Serv_root_remove_doctor(id);
        }
    }
    return HIS_OK;
}

static Status __medicine() {
    while (1) {
        CLEAN();
        printf("--- 药品及库存管理 ---\n");
        printf("%-8s | %-20s | %-10s | %-10s\n", "ID", "药品名称", "零售价", "总库存");
        printf("------------------------------------------------------------\n");

        List_T list = Data_get_medicine();
        void* ptr = List_first(list);
        while (ptr != NULL) {
            Medicine_T m = *(Medicine_T*)ptr;
            long long price = Medicine_cur_price(m);
            printf("%-8lld | %-20s | %lld.%02lld 元 | %-10d\n",
                Medicine_id(m), Medicine_name(m), price/100, price%100, Medicine_total_remain(m));
            ptr = List_next(list);
        }

        printf("\n1. 新增药品  2. 进货(加批次)  3. 删除药品  0. 返回\n");
        long long choice = get_input_long_long("操作", 0, 3);
        if (choice == 0) break;

        if (choice == 1) {
            char name[32];
            get_input_str("药品名称", name, 32);
            long long price = get_input_long_long("零售价（分）", 0, 100000);
            Serv_root_add_medicine(price, name);
        }
        else if (choice == 2) {
            long long mid = get_input_long_long("药品 ID", 0, 9999);
            long long bprice = get_input_long_long("进价", 0, 100000);
            int count = (int)get_input_long_long("数量", 1, 1000);
            char bno[32];
            get_input_str("批次号", bno, 32);
            Serv_root_add_medicine_batch(mid, bprice, time(NULL) + 31536000, count, bno);
        }
        else if (choice == 3) {
            long long mid = get_input_long_long("删除药品 ID", 0, 9999);
            Serv_root_remove_medicine(mid);
        }
    }
    return HIS_OK;
}

static Status __ward() {
    CLEAN();
    printf("--- 病房状态 ---\n");
    printf("%-8s | %-10s | %-12s | %-8s | %-8s\n", "ID", "科室", "名称", "总床位", "空床");
    printf("------------------------------------------------------------\n");
    List_T list = Data_get_ward();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Ward_T w = *(Ward_T*)ptr;
        printf("%-8lld | %-10d | %-12s | %-8d | %-8d\n",
            Ward_id(w), Ward_dept(w), Ward_name(w), Ward_bed_count(w), Ward_empty_count(w));
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无病房\n");
    printf("\n按任意键返回...");
    getchar();
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
    printf("\n按任意键返回...");
    getchar();
    return HIS_OK;
}

static Status __record() {
    CLEAN();
    printf("--- 全院诊疗记录 ---\n");
    printf("%-8s | %-12s | %-10s | %-12s | %-10s\n", "ID", "病人", "类型", "时间", "费用");
    printf("---------------------------------------------------------------\n");
    List_T list = Data_get_record();
    void* ptr = List_first(list);
    while (ptr != NULL) {
        Record_T r = *(Record_T*)ptr;
        long long cost = Rec_cost(r);
        printf("%-8lld | %-12s | %-10d | %-12lld | %lld.%02lld 元\n",
            Rec_actor_id(r), Serv_helper_id_to_name(Rec_actor_id(r), TYPE_PATIENT),
            Rec_type(r), Rec_time_stamp(r), cost/100, cost%100);
        ptr = List_next(list);
    }
    if(List_size(list) == 0) printf("暂无记录\n");
    printf("\n按任意键返回...");
    getchar();
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
            printf("%-8lld | %-12s | %-10d\n", Account_id(a), Account_name(a), Account_class(a));
            ptr = List_next(list);
        }

        printf("\n输入0返回\n");
        long long choice;
        scanf("%lld", &choice);
        clear_space();
        if (choice == 0) break;
    }
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
        printf("5. 财务流水总览\n");
        printf("6. 全院诊疗记录\n");
        printf("7. 系统账号管理\n");
        printf("0. 退出登录\n");
        printf("===============================\n");

        long long choice = get_input_long_long("请选择", 0, 7);
        switch (choice) {
        case 0: UI_root_should_continue = false; printf("退出登录...\n"); return HIS_OK;
        case 1: __patient(); break;
        case 2: __doctor(); break;
        case 3: __medicine(); break;
        case 4: __ward(); break;
        case 5: __fund(); break;
        case 6: __record(); break;
        case 7: __account(); break;
        default: break;
        }
    }
    return HIS_OK;
}