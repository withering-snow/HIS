#include <doc_ui.h>
#include <log_ctrl.h>
load_ui_tools

bool UI_doc_should_continue = false;

typedef enum {
    DMC_QUIT       = 0,
    DMC_ACTIVATE   = 1,
    DMC_CALL_REG   = 2,
    DMC_CONSULT    = 3,
    DMC_EXAM       = 4,
    DMC_PRESCRIBE  = 5,
    DMC_ADMIT      = 6,
    DMC_DISCHARGE  = 7,
    DMC_VIEW_QUEUE = 8,
    DMC_CHANGE_PW  = 9,
    DMC_CHANGE_BED = 10,
    DMC_CHANGE_DOC = 11,
    DMC_END_REG    = 12,
    DMC_DEACTIVATE = 13,
} DOC_MENU_CHOICE;

Status UI_doc_menu() {
    UI_doc_should_continue = true;

    CLEAN();
    long long active = get_input_long_long("是否出诊？(1:是, 0:否)", 0, 1);
    if (active == 1) {
        Status s = Serv_doc_active();
        if (s == HIS_OK) printf("已开启出诊！\n");
        else if (s == HIS_ERR_ALREADY_EXISTS) printf("已在出诊状态\n");
    }
    press_enter();

    while (UI_doc_should_continue) {
        CLEAN();
        printf("=============== 医生工作站 ===============\n");
        long long cur_pat = Serv_doc_view_reg();
        if (cur_pat != INVALID_ID) {
            const char* name = Serv_helper_id_to_name(cur_pat, TYPE_PATIENT);
            printf("当前看诊病人: %s (ID: %lld)\n", name, cur_pat);
        } else {
            printf("当前无看诊病人\n");
        }
        printf(" 1.开始出诊   2.叫号       3.看诊       4.检查\n");
        printf(" 5.开药       6.入院       7.出院       8.队列\n");
        printf(" 9.修改密码  10.换床      11.换医生    12.结束看诊\n");
        printf("13.结束出诊   0.退出登录\n");

        long long choice = get_input_long_long("请选择", 0, 13);
        if (choice == 0) {
            CLEAN();
            printf("--- 退出 ---\n");
            printf("1. 退出登录（返回登录界面）\n");
            printf("2. 退出系统\n");
            printf("0. 取消\n");
            long long quit_choice = get_input_long_long("请选择", 0, 2);
            if (quit_choice == 1) {
                UI_doc_should_continue = false; Serv_doc_unactive(); Serv_account_signout();
                printf("已退出登录！\n");
            } else if (quit_choice == 2) {
                UI_doc_should_continue = false; Serv_doc_unactive(); Serv_account_signout();
                printf("正在退出系统……\n");
                return HIS_QUIT;
            }
        }
        else if (choice == 1) {

            Status s = Serv_doc_active();
            if (s == HIS_OK) printf("开始出诊！\n");
            else if (s == HIS_ERR_ALREADY_EXISTS) printf("已在出诊状态\n");
        }
        else if (choice == 2) {
            Status s = Serv_doc_call_reg();
            if (s == HIS_OK) {
                long long pat_id = Serv_doc_view_reg();
                const char* name = Serv_helper_id_to_name(pat_id, TYPE_PATIENT);
                printf("叫号成功！当前看诊病人: %s (ID: %lld)\n", name, pat_id);
            } else if (s == HIS_ERR_NOT_FOUND) {
                printf("队列为空，暂无候诊病人\n");
            } else {
                printf("叫号失败\n");
            }
        }
        else if (choice == 3) {
            if (cur_pat == INVALID_ID) {
                printf("当前无看诊病人，请先叫号\n");
            } else {
                char diagnosis[128] = "", advice[128] = "";
                get_input_str("请输入诊断", diagnosis, 128);
                get_input_str("请输入医嘱", advice, 128);
                Status s = Serv_doc_consult(diagnosis, advice);
                if (s == HIS_OK) printf("看诊完成！\n");
                else printf("看诊失败\n");
            }
        }
        else if (choice == 4) {
            if (cur_pat == INVALID_ID) {
                printf("当前无看诊病人，请先叫号\n");
            } else {
                long long cost = get_input_long_long("检查费用（分）", 0, 1000000);
                char exam_name[64] = "";
                get_input_str("检查项目名称", exam_name, 64);
                Status s = Serv_doc_exam(cost, exam_name);
                if (s == HIS_OK) printf("检查已开具！\n");
                else if (s == HIS_ERR_INSUFFICIENT_FUNDS) printf("余额不足！\n");
                else if (s == HIS_ERR_NO_FUNDS) printf("该病人没有资金账户\n");
                else printf("开具检查失败\n");

            }
        }
        else if (choice == 5) {
            if (cur_pat == INVALID_ID) {
                printf("当前无看诊病人，请先叫号\n");
            } else {
                // 先模糊搜索药品，再选ID
                char keyword[32] = "";
                get_input_str("搜索药品名称（直接回车显示全部）", keyword, 32);
                List_T med_list = Data_get_medicine();
                printf("\n--- 药品列表 ---\n");
                printf("%-8s | %-20s | %-10s | %-8s\n", "ID", "药品名称", "零售价", "库存");
                printf("----------------------------------------------------------\n");
                void* mp = List_first(med_list);
                while (mp != NULL) {
                    Medicine_T m = *(Medicine_T*)mp;
                    if (strlen(keyword) == 0 || strstr(Medicine_name(m), keyword) != NULL) {
                        printf("%-8lld | %-20s | %lld.%02lld 元 | %-8d\n",
                            Medicine_id(m), Medicine_name(m),
                            Medicine_cur_price(m)/100, Medicine_cur_price(m)%100,
                            Medicine_total_remain(m));
                    }
                    mp = List_next(med_list);
                }

                long long med_id = get_input_long_long("请输入药品ID", 1, 999999);
                int amount = (int)get_input_long_long("数量", 1, 1000);
                // 通过ID查找药品名
                Medicine_T target = (Medicine_T)Serv_helper_finder(med_id, TYPE_MEDICINE);
                if (target == NULL) {
                    printf("未找到该药品\n");
                } else {
                    Status s = Serv_doc_prescribe(Medicine_name(target), amount);
                    if (s == HIS_OK) {
                        printf("开药成功！\n");
                    } else if (s == HIS_ERR_NOT_FOUND) {
                        printf("未找到该药品\n");
                    } else if (s == HIS_ERR_OUT_OF_STOCK) {
                        printf("库存不足！\n");
                    } else if (s == HIS_ERR_INSUFFICIENT_FUNDS) {
                        printf("余额不足！\n");
                    } else if (s == HIS_ERR_NO_FUNDS) {
                        printf("该病人没有资金账户\n");
                    } else {
                        printf("开药失败\n");
                    }

                }
            }
        }
        else if (choice == 6) {
            if (cur_pat == INVALID_ID) {
                printf("当前无看诊病人，请先叫号\n");
            } else {
                // 展示病房列表
                printf("\n--- 病房列表 ---\n");
                List_T ward_list = Data_get_ward();
                printf("%-8s | %-10s | %-12s | %-8s | %-8s\n", "ID", "科室", "名称", "总床位", "空床");
                printf("------------------------------------------------------------\n");
                void* wp = List_first(ward_list);
                while (wp != NULL) {
                    Ward_T w = *(Ward_T*)wp;
                    printf("%-8lld | %-10s | %-12s | %-8d | %-8d\n",
                        Ward_id(w), department_name(Ward_dept(w)),
                        Ward_name(w), Ward_bed_count(w), Ward_empty_count(w));
                    wp = List_next(ward_list);
                }

                long long ward_id = get_input_long_long("病房ID", 1, 99999);
                // 展示该病房所有床位状态
                Ward_T ward = (Ward_T)Serv_helper_finder(ward_id, TYPE_WARD);
                if (ward == NULL) {
                    printf("未找到该病房\n");
                } else {
                    printf("\n--- %s 床位状态 ---\n", Ward_name(ward));
                    printf("%-6s | %-8s | %s\n", "床号", "状态", "病人");
                    printf("-----------------------------\n");
                    List_T beds = Ward_beds(ward);
                    void* bp = List_first(beds);
                    while (bp != NULL) {
                        Bed_T* bed = (Bed_T*)bp;
                        const char* status_str = "";
                        switch(bed->status) {
                            case BED_EMPTY:     status_str = "空闲"; break;
                            case BED_OCCUPIED:  status_str = "有人"; break;
                            case BED_MAINTAIN:  status_str = "维修"; break;
                        }
                        const char* pat_name = "";
                        if (bed->pat_id > 0) {
                            pat_name = Serv_helper_id_to_name(bed->pat_id, TYPE_PATIENT);
                        }
                        printf("%-6d | %-8s | %s\n",
                            bed->bed_label, status_str,
                            (bed->status == BED_OCCUPIED && pat_name) ? pat_name : "---");
                        bp = List_next(beds);
                    }

                    int bed_label = (int)get_input_long_long("床号", 1, 100);
                    long long deposit = get_input_long_long("押金（分）", 0, 100000000);
                    Status s = Serv_doc_admission(ward_id, bed_label, deposit);
                    if (s == HIS_OK) printf("入院办理成功！\n");
                    else if (s == HIS_ERR_NOT_FOUND) printf("未找到该病房\n");
                    else if (s == HIS_ERR_BED_OCCUPIED) printf("该床位已被占用\n");
                    else printf("入院办理失败\n");
                }
            }
        }
        else if (choice == 7) {
            // 先展示当前住院病人列表
            printf("\n--- 当前住院病人 ---\n");
            List_T ward_list = Data_get_ward();
            void* wp = List_first(ward_list);
            bool has_inpatient = false;
            while (wp != NULL) {
                Ward_T w = *(Ward_T*)wp;
                List_T beds = Ward_beds(w);
                void* bp = List_first(beds);
                while (bp != NULL) {
                    Bed_T* bed = (Bed_T*)bp;
                    if (bed->status == BED_OCCUPIED && bed->pat_id > 0) {
                        const char* pat_name = Serv_helper_id_to_name(bed->pat_id, TYPE_PATIENT);
                        printf("  病人ID: %-8lld | 姓名: %-12s | 病房: %s | 床号: %d\n",
                            bed->pat_id, pat_name ? pat_name : "未知",
                            Ward_name(w), bed->bed_label);
                        has_inpatient = true;
                    }
                    bp = List_next(beds);
                }
                wp = List_next(ward_list);
            }
            if (!has_inpatient) printf("  暂无住院病人\n");

            long long id = get_input_long_long("请输入要办理出院的病人ID", 1, 99999);
            Status s = Serv_doc_discharge(id);
            if (s == HIS_OK) printf("出院办理成功！\n");
            else if (s == HIS_ERR_NOT_FOUND) printf("未找到该病人的住院记录\n");
            else printf("出院办理失败\n");
        }
        else if (choice == 8) {
            List_T w = Rel_queue_get_waiting(Serv_account_cur_id());
            void* p = List_first(w); int i=1;
            printf("%-4s | %-8s | %-12s | %-8s | %s\n", "序号", "病人ID", "姓名", "时段", "排队号");
            printf("--------------------------------------------------\n");
            while(p) {
                RelQueueDataPackage* q=(RelQueueDataPackage*)p;
                const char* pat_name = Serv_helper_id_to_name(q->pat_id, TYPE_PATIENT);
                const char* tf_str = "";
                if (q->time_frame == 0) tf_str = "急诊";
                else if (q->time_frame <= 4) tf_str = "上午";
                else if (q->time_frame <= 8) tf_str = "下午";
                else tf_str = "晚上";
                printf("%-4d | %-8lld | %-12s | %-8s | %d\n",
                    i++, q->pat_id, pat_name ? pat_name : "未知", tf_str, q->sequence_no);
                p=List_next(w);
            }
            if(i==1) printf("队列为空\n");
            List_free(&w);
        }
        else if (choice == 9) {
            char old_pw[20] = "", new_pw[20] = "";
            get_input_str("请输入当前密码", old_pw, 20);
            long long actor_id = Serv_account_cur_id();
            Account_T acc = (Account_T)Serv_helper_finder(actor_id, TYPE_ACCOUNT);
            if (acc != NULL && Account_check_password(acc, old_pw) == HIS_OK) {
                get_input_str("请输入新密码", new_pw, 20);
                Account_set_password(acc, new_pw);
                printf("密码修改成功！\n");
                Log_printf(CLASS_DOCTOR, actor_id, "医生修改密码");
            } else {
                printf("当前密码错误！\n");
            }
        }
        else if (choice == 10) {
            // 换床 - 先展示住院病人列表
            printf("\n--- 当前住院病人 ---\n");
            List_T ward_list_all = Data_get_ward();
            void* wp_all = List_first(ward_list_all);
            bool has_inpatient = false;
            while (wp_all != NULL) {
                Ward_T w = *(Ward_T*)wp_all;
                List_T beds = Ward_beds(w);
                void* bp = List_first(beds);
                while (bp != NULL) {
                    Bed_T* bed = (Bed_T*)bp;
                    if (bed->status == BED_OCCUPIED && bed->pat_id > 0) {
                        const char* pat_name = Serv_helper_id_to_name(bed->pat_id, TYPE_PATIENT);
                        printf("  病人ID: %-8lld | 姓名: %-12s | 病房: %s | 床号: %d\n",
                            bed->pat_id, pat_name ? pat_name : "未知",
                            Ward_name(w), bed->bed_label);
                        has_inpatient = true;
                    }
                    bp = List_next(beds);
                }
                wp_all = List_next(ward_list_all);
            }
            if (!has_inpatient) printf("  暂无住院病人\n");

            long long pat_id = get_input_long_long("请输入要换床的病人ID", 1, 99999);
            // 展示病房列表
            printf("\n--- 病房列表 ---\n");
            List_T ward_list = Data_get_ward();
            printf("%-8s | %-10s | %-12s | %-8s | %-8s\n", "ID", "科室", "名称", "总床位", "空床");
            printf("------------------------------------------------------------\n");
            void* wp = List_first(ward_list);
            while (wp != NULL) {
                Ward_T w = *(Ward_T*)wp;
                printf("%-8lld | %-10s | %-12s | %-8d | %-8d\n",
                    Ward_id(w), department_name(Ward_dept(w)),
                    Ward_name(w), Ward_bed_count(w), Ward_empty_count(w));
                wp = List_next(ward_list);
            }
            long long to_ward_id = get_input_long_long("目标病房ID", 1, 99999);
            // 展示目标病房所有床位状态
            Ward_T to_ward = (Ward_T)Serv_helper_finder(to_ward_id, TYPE_WARD);
            if (to_ward == NULL) {
                printf("未找到该病房\n");
            } else {
                printf("\n--- %s 床位状态 ---\n", Ward_name(to_ward));
                printf("%-6s | %-8s | %s\n", "床号", "状态", "病人");
                printf("-----------------------------\n");
                List_T beds = Ward_beds(to_ward);
                void* bp = List_first(beds);
                while (bp != NULL) {
                    Bed_T* bed = (Bed_T*)bp;
                    const char* status_str = "";
                    switch(bed->status) {
                        case BED_EMPTY:     status_str = "空闲"; break;
                        case BED_OCCUPIED:  status_str = "有人"; break;
                        case BED_MAINTAIN:  status_str = "维修"; break;
                    }
                    const char* pat_name = "";
                    if (bed->pat_id > 0) {
                        pat_name = Serv_helper_id_to_name(bed->pat_id, TYPE_PATIENT);
                    }
                    printf("%-6d | %-8s | %s\n",
                        bed->bed_label, status_str,
                        (bed->status == BED_OCCUPIED && pat_name) ? pat_name : "---");
                    bp = List_next(beds);
                }

                int bed_label = (int)get_input_long_long("目标床号", 1, 100);
                // 通过病房名调用换床（Serv_doc_change_bed 需要病房名）
                Status s = Serv_doc_change_bed(pat_id, Ward_name(to_ward), bed_label);
                if (s == HIS_OK) printf("换床成功！\n");
                else if (s == HIS_ERR_NOT_FOUND) printf("未找到目标病房或病人\n");
                else if (s == HIS_ERR_BED_OCCUPIED) printf("该床位已被占用\n");
                else printf("换床失败\n");
            }
        }
        else if (choice == 12) {
            // 结束看诊（释放当前病人）
            if (cur_pat == INVALID_ID) {
                printf("当前无看诊病人\n");
            } else {
                Status s = Serv_doc_end_reg();
                if (s == HIS_OK) {
                    printf("已结束当前看诊，病人已释放\n");
                } else {
                    printf("结束看诊失败\n");
                }
            }
        }
        else if (choice == 13) {
            Serv_doc_unactive();
            printf("已结束出诊\n");
        }
        else if (choice == 11) {
            // 换医生 - 先搜索病人
            char pat_keyword[32] = "";
            get_input_str("搜索病人姓名（直接回车显示全部）", pat_keyword, 32);
            List_T pat_list = Data_get_patient();
            printf("\n--- 病人列表 ---\n");
            printf("%-8s | %-12s | %-4s | %-4s | %-15s\n", "ID", "姓名", "性别", "年龄", "电话");
            printf("------------------------------------------------------------\n");
            void* pp = List_first(pat_list);
            while (pp != NULL) {
                Patient_T p = *(Patient_T*)pp;
                if (strlen(pat_keyword) == 0 || strstr(Patient_name(p), pat_keyword) != NULL) {
                    printf("%-8lld | %-12s | %-4s | %-4d | %-15s\n",
                        Patient_id(p), Patient_name(p),
                        Patient_gender(p) == MALE ? "男" : "女",
                        Patient_age(p), Patient_phone(p));
                }
                pp = List_next(pat_list);
            }

            long long pat_id = get_input_long_long("请输入病人ID", 1, 99999);
            // 展示医生列表
            printf("\n--- 医生列表 ---\n");
            List_T doc_list = Data_get_doctor();
            printf("%-8s | %-12s | %-10s | %-10s\n", "ID", "姓名", "科室", "职称");
            printf("----------------------------------------------------------\n");
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
                printf("%-8lld | %-12s | %-10s | %-10s\n",
                    Doctor_id(d), Doctor_name(d), department_name(Doctor_dept(d)), title_str);
                dp = List_next(doc_list);
            }
            long long to_doc_id = get_input_long_long("目标医生ID", 1, 999999);
            Status s = Serv_doc_change_doc(pat_id, to_doc_id);
            if (s == HIS_OK) printf("换医生成功！\n");
            else if (s == HIS_ERR_NOT_FOUND) printf("未找到该病人或医生\n");
            else printf("换医生失败\n");
        }
        press_enter();
    }
    return HIS_OK;
}
