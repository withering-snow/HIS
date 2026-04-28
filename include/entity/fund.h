#ifndef HIS_FUND_H
#define HIS_FUND_H

#include <HIS_core.h>

#define T Fund_T
typedef struct Fund_T *T;

// 生命周期
T Fund_load(long long pat_id, long long balance);
T Fund_new(long long pat_id);
void Fund_free(T* f);

// 访问器
long long Fund_pat_id(T f);
long long Fund_balance(T f);

// 充值与扣费

// 充值
Status Fund_deposit(T f, long long amount);

// 扣费（预检）
Status Fund_can_afford(T f, long long amount);

//执行扣费
void Fund_withdraw(T f, long long amount);


// 比较器
int Fund_cmp_pat_id(const void* a, const void* b);
int Fund_cmp_balance(const void* a, const void* b);


#undef T

#endif// HIS_FUND_H
