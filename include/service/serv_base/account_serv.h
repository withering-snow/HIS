#ifndef HIS_ACCOUNT_SERV_H
#define HIS_ACCOUNT_SERV_H


#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>
#include <HIS_serv_base.h>


/**
 * @brief       检查账户链表是否为空，若为空则注册 root 账户
 * @return      可能的异常：
 *              HIS_ERR_ALREADY_EXISTS
 * @note        若注册了 root 用户则返回 HIS_OK
 *              默认 root 用户 id=0, name="root", password="root"
 */
Status Serv_account_init();

/**
 * @brief           注册普通医患账号
 * @param class     账号类型
 * @param actor_id  对应的实体id
 * @param name      账号显示的名字
 * @param password  明文密码
 * @return          可能的异常：
 *                  HIS_ERR_ALREADY_EXISTS
 *                  HIS_ERR_INVALID_ARG
 *                  HIS_ERR_INSUFFICIENT_PERMISSION
 */
Status Serv_account_signup(
    AccountClass class, long long actor_id,
    const char* name, const char* password)
;

/**
 * @brief                   登录
 * @param class             账号权限
 * @param actor_id          账号关联主体id
 * @param password          密码
 * @return                  可能的异常：
 *                          HIS_ERR_NOT_FOUND
 *                          HIS_ERR_PASSWORD_MISMATCH
 */
Status Serv_account_signin(
    AccountClass class, long long actor_id, const char *password)
;

/**
 * @brief   注销账户
 * @return  可能的异常：
 *          HIS_ERR_NO_USER
 */
Status Serv_account_signout();

/**
 * @brief                   校验当前权限
 * @param required_class    需求权限
 * @return                  可能的异常：
 *                          HIS_ERR_NO_USER
 *                          HIS_ERR_INSUFFICIENT_PERMISSION
 * @note                    此函数会对root始终放行
 */
Status Serv_permission_check(AccountClass required_class);


// 对于当前账户的访问器
long long       Serv_account_cur_id(void);
const char*     Serv_account_cur_name(void);
AccountClass    Serv_account_cur_class(void);
bool            Serv_account_is_logged_in(void);


#endif //HIS_ACCOUNT_SERV_H
