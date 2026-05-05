#include <account.h>

struct Account_T{
    AccountClass    class;
    long long       actor_id;
    char            name[32];
    char            password[32];
};

Account_T Account_load(
    AccountClass class, long long actor_id,
    const char *name, const char *password)
{
    Account_T a=safe_malloc(sizeof(struct Account_T));
    a->class=class;
    a->actor_id=actor_id;
    strncpy(a->name,name,32);
    cipher_xor_cyclic(password, strlen(password));
    strncpy(a->password,password,32);
    return a;
}

Account_T Account_new(
    AccountClass class, long long actor_id,
    const char *name, const char *password_origin)
{
    Account_T a=safe_malloc(sizeof(struct Account_T));
    a->class=class;
    a->actor_id=actor_id;
    strncpy(a->name,name,32);
    cipher_xor_cyclic(password_origin, strlen(password_origin));
    strncpy(a->password,password_origin,32);
    return a;
}

void Account_free(Account_T* a)
{
    ASSERT(a !=NULL,"不合法");
    free(*a);
    *a=NULL;
}

Status Account_check_password(Account_T a, const char *password_origin)
{
    ASSERT(a !=NULL,"不合法");
    cipher_xor_cyclic(password_origin, strlen(password_origin));
    if (a->password[0]!=*password_origin)
        return HIS_ERR_PASSWORD_MISMATCH;
    else return HIS_OK;
}

AccountClass Account_class(Account_T a)
{
    ASSERT(a !=NULL,"不合法");
    return a->class;
}

long long Account_id(Account_T a)
{
    ASSERT(a !=NULL,"不合法");
    return a->actor_id;
}

const char *Account_name(Account_T a)
{
    ASSERT(a !=NULL,"不合法");
    return a->name;
}

const char *Account_password(Account_T a)
{
    ASSERT(a !=NULL,"不合法");
    return a->password;
}

Status Account_set_actor_id(Account_T a, long long new_id)
{
    ASSERT(a !=NULL,"不合法");
    a->actor_id=new_id;
    return HIS_OK;
}

Status Account_set_actor_name(Account_T a, char *new_name)
{
    ASSERT(a !=NULL,"不合法");
    strncpy(a->name,new_name,32);
    return HIS_OK;
}

Status Account_set_password(Account_T a, char *new_password_origin)
{
    ASSERT(a !=NULL,"不合法");
    cipher_xor_cyclic(new_password_origin, strlen(new_password_origin));
    strncpy(a->password,new_password_origin,32);
    return HIS_OK;
}

int Account_cmp_class(const void* a, const void* b)
{
    Account_T p=*(Account_T *)a;
    Account_T q=*(Account_T *)b;
    return (p->class>q->class)-(p->class<q->class);
}

int Account_cmp_actor_id(const void* a, const void* b)
{
    Account_T p=*(Account_T *)a;
    Account_T q=*(Account_T *)b;
    return (p->actor_id>q->actor_id)-(p->actor_id<q->actor_id);
}

int Account_cmp_name(const void* a, const void* b)
{
    Account_T p=*(Account_T *)a;
    Account_T q=*(Account_T *)b;
    return strncmp(p->name,q->name,32);
}

int Account_cmp_password(const void* a, const void* b)
{
    Account_T p=*(Account_T *)a;
    Account_T q=*(Account_T *)b;
    return strncmp(p->password,q->name,32);
}