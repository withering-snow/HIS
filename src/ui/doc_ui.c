#include <doc_ui.h>
load_ui_tools

bool UI_doc_should_continue = false;

typedef enum {
    DMC_DEACTIVATE = 0,
    DMC_ACTIVATE   = 1,
    DMC_CALL_REG   = 2,
    DMC_CONSULT    = 3,
    DMC_EXAM       = 4,
    DMC_PRESCRIBE  = 5,
    DMC_ADMIT      = 6,
    DMC_DISCHARGE  = 7,
    DMC_VIEW_QUEUE = 8,
    DMC_QUIT       = 99
} DOC_MENU_CHOICE;

Status UI_doc_menu() {
    UI_doc_should_continue = true;

    CLEAN();
    long long active = get_input_long_long("是否出诊？(1:是, 0:否)", 0, 1);
    if (active == 1) Serv_doc_active();
    getchar();

    while (UI_doc_should_continue) {
        CLEAN();
        printf("=============== 医生工作站 ===============\n");
        long long cur_pat = Serv_doc_view_reg();
        if (cur_pat != INVALID_ID) {
            const char* name = Serv_helper_id_to_name(cur_pat, TYPE_PATIENT);
            printf("当前看诊病人: %s\n", name);
        } else {
            printf("当前无看诊病人\n");
        }
        printf("1.开始出诊  2.叫号  3.看诊  4.检查\n");
        printf("5.开药     6.入院  7.出院  8.队列\n");
        printf("0.结束出诊  99.退出登录\n");

        long long choice = get_input_long_long("请选择", 0, 99);
        if (choice == 99) { UI_doc_should_continue = false; Serv_doc_unactive(); Serv_account_signout(); return HIS_QUIT; }
        else if (choice == 1) Serv_doc_active();
        else if (choice == 2) Serv_doc_call_reg();
        else if (choice == 3) { char d[]="诊断ok", a[]="医嘱ok"; Serv_doc_consult(d,a); }
        else if (choice == 4) Serv_doc_exam(100, "血常规");
        else if (choice == 5) Serv_doc_prescribe("头孢",1);
        else if (choice == 6) Serv_doc_admission(1,1,500);
        else if (choice == 7) { long long id = get_input_long_long("病人ID",0,99999); Serv_doc_discharge(id); }
        else if (choice == 8) {
            List_T w = Rel_queue_get_waiting(Serv_account_cur_id());
            void* p = List_first(w); int i=1;
            while(p) { RelQueueDataPackage* q=(RelQueueDataPackage*)p;
                printf("%d. 病人%lld\n",i++,q->pat_id); p=List_next(w); }
            if(i==1) printf("队列为空\n");
            List_free(&w);
        }
        else if (choice == 0) Serv_doc_unactive();
        else if (choice == 99) { UI_doc_should_continue=false; Serv_doc_unactive(); Serv_account_signout(); }
        printf("\n按回车继续..."); getchar();
    }
    return HIS_OK;
}