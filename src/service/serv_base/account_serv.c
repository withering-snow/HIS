#include <account_serv.h>




// 当前账号信息
static struct{
    AccountClass class;
    long long actor_id;
    char actor_name[32];
}
_cur_account_info = {
    CLASS_NO_USER,
    INVALID_ID, // 未登录
    "NoUser"
};




Status Serv_account_init(){
    List_T list = Data_get_account();
    if(List_first(list) == NULL){
        Account_T acc = Account_new(CLASS_ROOT, 0LL, "root", "root");
        List_push_back(list, &acc);
        return HIS_OK;
    }
    else{
        return HIS_ERR_ALREADY_EXISTS;
    }
}


Status Serv_account_signup(
    AccountClass class, long long actor_id, const char *name, const char *password){

    List_T list = Data_get_account();
    void *find_ptr = List_first(list);
    while(find_ptr != NULL){
        Account_T tmp = *(Account_T*)find_ptr;
        if(Account_id(tmp) == actor_id){
            return HIS_ERR_ALREADY_EXISTS;
        }
        find_ptr = List_next(list);
    }

    if(class == CLASS_PATIENT){
        Account_T acc = Account_new(class, actor_id, name, "00000000");
        List_push_back(list, &acc);
    }
    else if(class == CLASS_DOCTOR){
        if(Serv_permission_check(CLASS_ROOT) != HIS_OK){
            return HIS_ERR_INSUFFICIENT_PERMISSION;
        }
        Account_T acc = Account_new(class, actor_id, name, password);
        List_push_back(list, &acc);
    }
    else if(class == CLASS_ROOT){
        return HIS_ERR_INSUFFICIENT_PERMISSION;
    }
    else{
        return HIS_ERR_INVALID_ARG;
    }

    return HIS_OK;
}


Status Serv_account_signin(
    const char* id_card, const char *password){
    AccountClass class = CLASS_NO_USER;

    long long actor_id = INVALID_ID;

    if(id_card[0] == '0' && id_card[1] == '\0'){
        class = CLASS_ROOT;
        actor_id = 0;
    }
    else{
        List_T pat_list = Data_get_patient(), doc_list = Data_get_doctor();

        void* find_ptr = List_first(pat_list);
        while(find_ptr != NULL){
            Patient_T tmp = *(Patient_T*)find_ptr;
            if(strncmp(Patient_id_card(tmp), id_card, 20) == 0){
                class = CLASS_PATIENT;
                actor_id = Patient_id(tmp);
                break;
            }
            find_ptr = List_next(pat_list);
        }

        if(actor_id == INVALID_ID){
            find_ptr = List_first(doc_list);
            while(find_ptr != NULL){
                Doctor_T tmp = *(Doctor_T*)find_ptr;
                if(strncmp(Doctor_id_card(tmp), id_card, 20) == 0){
                    class = CLASS_DOCTOR;
                    actor_id = Doctor_id(tmp);
                    break;
                }
                find_ptr = List_next(doc_list);
            }
        }
    }

    if(class == CLASS_NO_USER){
        return HIS_ERR_NOT_FOUND;
    }

    Account_T acc = Serv_helper_finder(actor_id, TYPE_ACCOUNT);
    Status s = Account_check_password(acc, password);
    if(s == HIS_OK){
        _cur_account_info.class = class;
        _cur_account_info.actor_id = actor_id;
        strncpy(_cur_account_info.actor_name, Account_name(acc), 32);
    }
    return s;
}

Status Serv_account_signout(){
    if(_cur_account_info.class == CLASS_NO_USER){
        return HIS_ERR_NO_USER;
    }

    _cur_account_info.class = CLASS_NO_USER;
    _cur_account_info.actor_id = INVALID_ID; // 登出
    strncpy(_cur_account_info.actor_name, "NoUser", 32);
    return HIS_OK;
}

Status Serv_permission_check(AccountClass required_class){
    if(_cur_account_info.class == CLASS_NO_USER)
        return HIS_ERR_NO_USER;

    if(_cur_account_info.class == CLASS_ROOT ||
        _cur_account_info.class == required_class) {
        return HIS_OK;
    }

    return HIS_ERR_INSUFFICIENT_PERMISSION;
}


long long Serv_account_cur_id(void){
    return _cur_account_info.actor_id;
}

const char * Serv_account_cur_name(void){
    return _cur_account_info.actor_name;
}

AccountClass Serv_account_cur_class(void){
    return _cur_account_info.class;
}

bool Serv_account_is_logged_in(void){
    return (_cur_account_info.actor_id != INVALID_ID) ? true : false; // 是否已登录
}
