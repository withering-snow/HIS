#include <main_ui.h>
load_ui_tools


// =============================
// ====临时函数
extern Status UI_pat_menu();
extern Status UI_doc_menu();
extern Status UI_root_menu();


bool UI_main_should_continue = true;






Status UI_main(){



    while(UI_main_should_continue){


        Data_init();
        Rel_init();
        Io_load();
        Rel_queue_update();
        Log_open();
        CLEAN();

        printf("欢迎使用医疗信息管理系统！\n");
        printf("正在读取已有数据\n");
        SLEEP_MS(500);




        if(Serv_account_init() == HIS_OK){
            printf("系统暂无数据，将创建初始管理员账户\n");
            printf("初始管理员账户信息如下，请牢记\n");
            printf("账户id    0\n");
            printf("用户名    root\n");
            printf("密码      root\n");
            printf("管理员用户已生成，按任意键重启系统……\n");
            getchar();
            CLEAN();
        }




        else{

            bool has_logged = false;
            while(!has_logged){


                CLEAN();
                printf("============= 欢迎使用医疗信息管理系统 =============\n");
                char id_buf[20] = {0}, pw_buf[20] = {0};
                printf("请输入您的账号（输入 q 退出程序）：\n");
                printf("  - 管理员：输入 0\n");
                printf("  - 病人/医生：输入身份证号\n");
                get_input_str("", id_buf, 20);
                if (id_buf[0] == 'q' || id_buf[0] == 'Q') {
                    if (id_buf[1] == '\0') {
                        UI_main_should_continue = false;
                        break;
                    }
                }
                printf("请输入密码（病人账户无需密码，直接回车即可）：\n");
                get_input_str_allow_empty("", pw_buf, 20);
                Status login_status = Serv_account_signin(id_buf, pw_buf);


                switch(login_status){


                case HIS_ERR_NOT_FOUND:
                    printf("账户不存在，是否创建病人账户？\n输入0确认，其他任意键取消\n");
                    long long choice = 1;
                    scanf("%lld", &choice);
                    clear_space();
                    CLEAN();

                    if(choice == 0){
                        printf("请输入您的个人信息：\n");

                        gender g; char name[32]; char phone[20];
                        g = get_input_long_long("性别：\n0. 女\n1. 男\n", 0, 1);

                        // 一次性输入8位日期
                        printf("出生日期：\n");
                        char date_buf[16];
                        int y, m, d;
                        while (1) {
                            printf("  请输入8位数字日期 (如20061121): ");
                            if (fgets(date_buf, 16, stdin) == NULL) continue;
                            size_t len = strlen(date_buf);
                            if (len > 0 && date_buf[len-1] == '\n') date_buf[len-1] = '\0';
                            if (strlen(date_buf) != 8) {
                                printf("  必须输入8位数字！\n");
                                continue;
                            }
                            int ok = 1;
                            for (int i = 0; i < 8; i++) {
                                if (date_buf[i] < '0' || date_buf[i] > '9') { ok = 0; break; }
                            }
                            if (!ok) { printf("  只能包含数字！\n"); continue; }
                            sscanf(date_buf, "%4d%2d%2d", &y, &m, &d);
                            if (y < 1900 || y > 2026) { printf("  年份不合法 (1900~2026)！\n"); continue; }
                            if (m < 1 || m > 12) { printf("  月份不合法 (1~12)！\n"); continue; }
                            if (d < 1 || d > 31) { printf("  日期不合法！\n"); continue; }
                            break;
                        }
                        struct tm btm = {0};
                        btm.tm_year = y - 1900; btm.tm_mon = m - 1; btm.tm_mday = d;
                        long long birth_ts = (long long)mktime(&btm);
                        int birth = Time_to_int_date(birth_ts);

                        get_input_str("姓名：\n", name, 32);
                        get_input_str("联系电话：\n", phone, 20);

                        Status signup_status = Serv_patient_signup(g, birth, name, phone, id_buf);
                        if (signup_status == HIS_OK) {
                            Io_save();
                            printf("账户创建成功！按回车键返回……\n");
                        } else {
                            printf("账户创建失败（可能身份证号已存在），请重试。按回车键返回……\n");
                        }
                        clear_space();
                        CLEAN();
                    }
                    break;


                case HIS_ERR_PASSWORD_MISMATCH:
                    printf("密码错误！\n");
                    break;


                case HIS_OK:
                    printf("登录成功！\n");
                    has_logged = true;
                    break;

                }
                SLEEP_MS(1500);
            }
        }



        switch(Serv_account_cur_class()){


            case CLASS_PATIENT:{
                if(UI_pat_menu() == HIS_QUIT)
                    UI_main_should_continue = false;
                break;
            }


            case CLASS_DOCTOR:{
                if(UI_doc_menu() == HIS_QUIT)
                    UI_main_should_continue = false;
                break;
            }


            case CLASS_ROOT:{
                if(UI_root_menu() == HIS_QUIT)
                    UI_main_should_continue = false;
                break;
            }


            default:
                break;
        }




        Serv_account_signout();
        Io_save();
        Rel_destroy();
        Data_destroy();
        Log_close();
    }




    return HIS_OK;
}
