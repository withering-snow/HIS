#include <fund.h>

struct Fund_T {
    long long pat_id;
    long long balance;
};


Fund_T Fund_load(long long pat_id, long long balance)
{
    Fund_T f=safe_malloc(sizeof(Fund_T));
    f->pat_id=pat_id;
    f->balance=balance;
    return f;
}

Fund_T Fund_new(long long pat_id)
{
    Fund_T f=safe_malloc(sizeof(struct Fund_T));
    f->pat_id=pat_id;
    return f;
}

void Fund_free(Fund_T* f)
{
    ASSERT(f !=NULL,"不合法");
    free(*f);
    *f=NULL;
}

long long Fund_pat_id(Fund_T f)
{
    ASSERT(f !=NULL,"不合法");
    return f->pat_id;
}

long long Fund_balance(Fund_T f)
{
    ASSERT(f !=NULL,"不合法");
    return f->balance;
}

Status Fund_deposit(Fund_T f, long long amount)
{
    ASSERT(f !=NULL,"不合法");
    if (amount<=0)
    return HIS_ERR_INVALID_PAYMENT;
    else
    {
        f->balance+=amount;
        return HIS_OK ;
    }
}

Status Fund_can_afford(Fund_T f, long long amount)
{
    ASSERT(f !=NULL,"不合法");
    if(f->balance < amount)
    return HIS_ERR_INSUFFICIENT_FUNDS ;
    return  HIS_OK ;
}

void Fund_withdraw(Fund_T f, long long amount)
{
    ASSERT(f !=NULL,"不合法");
    f->balance-=amount;
}

int Fund_cmp_pat_id(const void* a, const void* b)
{
    Fund_T p=*(Fund_T *)a;
    Fund_T q=*(Fund_T *)b;
    return (p->pat_id>q->pat_id)-(p->pat_id<q->pat_id);
}

int Fund_cmp_balance(const void* a, const void* b)
{
    Fund_T p=*(Fund_T *)a;
    Fund_T q=*(Fund_T *)b;
    return (p->balance>q->balance)-(p->balance<q->balance);
}

