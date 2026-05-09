#include <main_ui.h>


// =============================
// ====临时函数
extern Status IO_load();
extern Status IO_save();
extern Status UI_pat_menu();
extern Status UI_doc_menu();
extern Status UI_root_menu();


bool UI_main_should_continue = true;


Status UI_main(){

    while(UI_main_should_continue){

        CLEAN();
        printf("欢迎使用医疗信息管理系统！\n");
        printf("正在读取已有数据\n");
        SLEEP_MS(500);

        if(IO_load() != HIS_OK ||
            Serv_account_init() == HIS_OK){ //TODO:状态码对应修改
            printf("系统暂无数据，将创建初始管理员账户\n");
            printf("初始管理员账户信息如下，请牢记\n");
            printf("账户id    0\n");
            printf("用户名    root\n");
            printf("密码      root\n");
            printf("管理员用户已生成，即将重启系统……");
            sleep(3000);
        }

        else{
            // TODO:登录界面
        }

        switch(Serv_account_cur_class()){
            case CLASS_PATIENT:{
                UI_pat_menu();
                break;
            }
            case CLASS_DOCTOR:{
                UI_doc_menu();
                break;
            }
            case CLASS_ROOT:{
                UI_root_menu();
                break;
            }
            default:
                break;
        }

        Serv_account_signout();
    }
}
