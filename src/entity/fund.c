#include <fund.h>

struct Transaction {
    long long time_stamp;
    double amount;
    char reason[64];
};

struct Fund_T {
    int patient_id;
    double balance;
    List_T history;
};

// 生命周期
Fund_T Fund_new(int patient_id, double remain) {}
void Fund_free(Fund_T *f) {}

Status Fund_deposit(Fund_T f, double amount, const char *reason) {}
Status Fund_withdraw(Fund_T f, double amount, const char *reason) {}

double Fund_get_total_spending(Fund_T f) {}
List_T Fund_get_history(Fund_T f) {}
// 比较器
int Fund_cmp_patient_id(const void *a, const void *b) {}
