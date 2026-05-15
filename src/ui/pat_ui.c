#include <pat_ui.h>
load_ui_tools

bool UI_pat_should_continue = true;

typedef enum {
    PAT_MAIN_MENU_CHOICE_INVALID = -1,
    PAT_MAIN_MENU_CHOICE_PATIENT = 0,
    PAT_MAIN_MENU_CHOICE_RECORD = 1,
    PAT_MAIN_MENU_CHOICE_DOCTOR = 2,
    PAT_QUIT = 100,
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

// 主菜单
Status UI_pat_menu() {
    while (UI_pat_should_continue) {
        CLEAN();
        printf("=======================\n");
        printf("      患者主菜单       \n");
        printf("=======================\n");
        printf(" 0 - 患者信息管理\n");
        printf(" 1 - 诊疗记录查询\n");
        printf(" 2 - 医生信息查询\n");
        printf("100 - 退出登录\n");
        printf("=======================\n");

        PAT_MAIN_MENU_CHOICE main_choice = get_input_long_long("请选择功能", 0, 100);

        switch (main_choice) {
            case PAT_MAIN_MENU_CHOICE_PATIENT: {
                CLEAN();
                long long pat_id = get_input_long_long("请输入患者ID", 1, 999999999);

                printf("\n====== 患者子菜单 ======\n");
                printf("1 - 查看个人信息\n");
                printf("2 - 修改手机号码\n");
                printf("========================\n");
                PAT_SUBORD_MENU_PATIENT choice1 = get_input_long_long("请选择", 1, 2);

                if (choice1 == PAT_SUBORD_MENU_CHOICE_PATIENT) {
                    // 1. 查询患者信息
                    ServPatientDataPackage* pkg = Serv_patient_data(pat_id);
                    UI_show_patient_info(pkg);
                    Serv_helper_free_value(pkg);
                }
                else if (choice1 == PAT_SUBORD_MENU_CHANGE_PHONE) {
                    // 2. 修改电话
                    char new_phone[20];
                    get_input_str("请输入新手机号", new_phone, 20);
                    printf("手机号修改功能待实现！\n");
                }
                break;
            }

            case PAT_MAIN_MENU_CHOICE_RECORD: {
                CLEAN();
                long long pat_id = get_input_long_long("请输入患者ID", 1, 999999999);

                printf("\n====== 诊疗记录菜单 ======\n");
                printf("1 - 查看全部记录\n");
                printf("2 - 按时间筛选\n");
                printf("3 - 查询账户余额\n");
                printf("==========================\n");
                PAT_SUBORD_MENU_RECORD choice2 = get_input_long_long("请选择", 1, 3);

                if (choice2 == PAT_SUBORD_MENU_CHOICE_RECORD) {
                    // 3. 查询诊疗记录
                    List_T records = Serv_patient_records(pat_id);
                    printf("共 %d 条记录\n", List_size(records));
                    Serv_helper_free_list(records);
                }
                else if (choice2 == PAT_SUBORD_MENU_FILTER_TIME) {
                    printf("时间筛选功能待实现！\n");
                }
                else if (choice2 == PAT_SUBORD_MENU_CHECK_FUND) {
                    // 5. 查询余额
                    long long fund = Serv_patient_get_fund();
                    if (fund < 0)
                        printf("未找到患者账户！\n");
                    else
                        printf("当前账户余额：%lld 元\n", fund);
                }
                break;
            }

            case PAT_MAIN_MENU_CHOICE_DOCTOR:
                CLEAN();
                printf("医生查询功能待实现！\n");
                break;

            case PAT_QUIT:
                UI_pat_should_continue = false;
                Serv_account_signout();
                printf("已安全退出！\n");
                break;

            default:
                printf("输入无效！\n");
                break;
        }
        printf("\n按回车继续...");
        getchar();
    }
    return HIS_OK;
}

