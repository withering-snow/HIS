#include <fund.h>

struct Transaction {
    long long time_stamp;
    double amount;//开销金额
    char reason[64];//开销原因o
};

struct Fund_T {
    int patient_id;
    double balance;//余额
    List_T history;
};

// 生命周期
Fund_T Fund_new(int patient_id, double remain) {
    Fund_T fund;
    fund->patient_id=patient_id;
    fund->balance=remain;
    return fund;
}
void Fund_free(Fund_T *f) {
    ASSERT(f!=NULL, "资金指针不能为空");
    free(f);
    f=NULL;
}

Status Fund_deposit(Fund_T f, double amount) {
    ASSERT(f!=NULL, "资金指针不能为空");
    f->balance+=amount;
    return HIS_OK;
}
Status Fund_withdraw(Fund_T f, double amount, const char *reason) {
    ASSERT(f!=NULL, "资金指针不能为空");
    bool p=0;
    if(f->balance>=amount) p=1;
    else p=0;
    if(p) return HIS_OK;
    else return HIS_ERR_INSUFFICIENT_FUNDS;
}

double Fund_get_total_spending(Fund_T f) {
    ASSERT(f!=NULL, "资金指针不能为空");
    if(List_first(f->history)==NULL) return 0.0;
    struct Transaction *tmp;
    int n=0;
    for(tmp=(struct Transaction*)List_first(f->history);
        tmp!=NULL;
        tmp=(struct Transaction*)List_next(f->history)) {
        if(tmp->amount<0) {
            n+=tmp->amount;
        }
    }
    return n;
}
List_T Fund_get_history(Fund_T f) {
    ASSERT(f!=NULL, "资金指针不能为空");
    return f->history;
}
// 比较器
int Fund_cmp_patient_id(const void *a, const void *b) {
    Fund_T f1=*(Fund_T*)a;
    Fund_T f2=*(Fund_T*)b;
    return f1-f2;
}
