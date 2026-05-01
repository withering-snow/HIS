#include <account_serv.h>




// 当前账号信息
static struct{
    AccountClass class;
    long long actor_id;
    char actor_name[32];
}
_cur_account_info = {
    CLASS_NO_USER,
    -1LL, //TODO: INVALID_ID
    "NoUser"
};


Status Serv_account_signup(
    AccountClass class, long long actor_id,
    const char *name, const char *password){
    switch(class)
}



Status Serv_account_signin(AccountClass class, long long actor_id, const char *password){
    List_T list = Data_get_account();
    Account_T tmp = Account_load(class, actor_id, "tmp", "tmp");
    Status status = HIS_ERR_NOT_FOUND;


    void* find_ptr = List_first(list);
    while(find_ptr != NULL){

        if( Account_cmp_actor_id(&tmp, find_ptr) == 0 &&
            Account_cmp_class(&tmp, find_ptr) == 0){

            status = Account_check_password(*(Account_T*)find_ptr, password);
            if(status == HIS_OK){
                _cur_account_info.class = class;
                _cur_account_info.actor_id = actor_id;
                strncpy(_cur_account_info.actor_name, Account_name(*(Account_T*)find_ptr), 32);
            }
            break;
        }

        find_ptr = List_next(list);
    }

    Account_free(&tmp);
    return status;
}

Status Serv_account_signout(){
    if(_cur_account_info.class == CLASS_NO_USER){
        return HIS_ERR_NO_USER;
    }

    _cur_account_info.class = CLASS_NO_USER;
    _cur_account_info.actor_id = -1LL; //TODO: INVALID_ID
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
    return (_cur_account_info.actor_id != -1LL) ? true : false; // TODO: INVALID_ID
}
