#ifndef HIS_ACCOUNT_H
#define HIS_ACCOUNT_H


#include <HIS_core.h>




#define T Account_T
typedef struct T *T;


// 账号类型枚举
typedef enum{
    CLASS_NO_USER,
    CLASS_PATIENT,
    CLASS_DOCTOR,
    CLASS_ROOT
}AccountClass;


// 生命周期
T Account_load(
    AccountClass class, long long actor_id,
    const char *name, const char *password)
;
T Account_new(
    AccountClass class, long long actor_id,
    const char *name, const char *password_origin)
;
void Account_free(T* a);


// 密码验证
Status Account_check_password(T a, const char *password_origin);


// 访问器
long long Account_id(T a);
AccountClass Account_class(T a);
const char *Account_name(T a);
const char *Account_password(T a);


// 修改器
Status Account_set_actor_id(T a, long long new_id);
Status Account_set_actor_name(T a, const char *new_name);
Status Account_set_password(T a, const char *new_password_origin);


// 比较器
int Account_cmp_actor_id(const void* a, const void* b);
int Account_cmp_class(const void* a, const void* b);
int Account_cmp_name(const void* a, const void* b);
int Account_cmp_password(const void* a, const void* b);




#undef T


#endif //HIS_ACCOUNT_H
