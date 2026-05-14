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
                get_input_str("请输入您的账号（身份证号）：\n", id_buf, 20);
                get_input_str("请输入密码（病人账户无需密码）：\n", pw_buf, 20);
                Status login_status = Serv_account_signin(id_buf, pw_buf);

                switch(login_status){
                case HIS_ERR_NOT_FOUND:
                    printf("账户不存在，是否创建病人账户？\n输入0确认，其他任意键取消\n");
                    long long choice;
                    scanf("%lld", &choice);
                    clear_space();
                    CLEAN();

                    if(choice == 0){
                        printf("请输入您的个人信息：\n");
                        gender g; int birth; char name[32]; char phone[20];
                        g = get_input_long_long("性别：\n0. 女\n1. 男\n", 0, 1);
                        birth = get_input_long_long("生日：（形如 20050101）\n", 19700101,Time_to_int_date(Time_now()) );
                        get_input_str("姓名：\n", name, 32);
                        get_input_str("联系电话：\n", phone, 20);

                        Patient_T pat = Patient_new(g, Int_date_to_time(birth), name, phone, id_buf);
                        List_push_back(Data_get_patient(), &pat);

                        Fund_T fund = Fund_new(Patient_id(pat));
                        List_push_back(Data_get_fund(), &fund);

                        Serv_account_signup(CLASS_PATIENT, Patient_id(pat), name, NULL);

                        Io_save();
                        printf("账户创建成功！按任意键返回……");
                        getchar();
                        CLEAN();
                    }
                    break;

                case HIS_ERR_PASSWORD_MISMATCH:
                    printf("密码错误！\n");
                    break;

                case HIS_OK:
                    printf("登录成功！");
                    has_logged = true;
                    break;

                }
            }
        }

        switch(Serv_account_cur_class()){
            case CLASS_PATIENT:{
                if(UI_pat_menu() != HIS_OK)
                    UI_main_should_continue = false;
                break;
            }
            case CLASS_DOCTOR:{
                if(UI_doc_menu() != HIS_OK)
                    UI_main_should_continue = false;
                break;
            }
            case CLASS_ROOT:{
                if(UI_root_menu() != HIS_OK)
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
    }
}
