#ifndef HIS_ACCOUNT_SERV_H
#define HIS_ACCOUNT_SERV_H


#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>


/**
 * @brief                   登录
 * @param class             账号权限
 * @param actor_id          账号关联主体id
 * @param name              账号用户名
 * @param password          密码
 * @return                  状态码
 */
Status Serv_account_login(
    AccountClass class, long long actor_id, const char *password)
;
/**
 * @brief   注销账户
 * @return  状态码
 */
Status Serv_account_logout();

/**
 * @brief                   校验当前权限
 * @param required_class    需求权限
 * @return                  状态码
 * @note                    此函数会对root始终放行
 */
Status Serv_permission_check(AccountClass required_class);


// 对于当前账户的访问器
long long       Serv_account_cur_id(void);
const char*     Serv_account_cur_name(void);
AccountClass    Serv_account_cur_class(void);
bool            Serv_account_is_logged_in(void);


#endif //HIS_ACCOUNT_SERV_H
