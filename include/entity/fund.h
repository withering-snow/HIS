#ifndef HIS_FUND_H
#define HIS_FUND_H

#include <HIS_core.h>

#define T Fund_T
typedef struct Fund_T *T;

// 生命周期
T Fund_new(int patient_id, double remain);
void Fund_free(T* f);

// 充值：记录时间戳
Status Fund_deposit(T f, double amount);
// 扣费：如果余额不足，返回错误码，且不记录流水
Status Fund_withdraw(T f, double amount, const char* reason);

// 统计：获取该病人的总支出（遍历链表累加负数）
double Fund_get_total_spending(T f);
// 导出：打印对账单（给 UI 用）
List_T Fund_get_history(T f);

// 比较器
int Fund_cmp_patient_id(const void* a, const void* b);

#undef T

#endif// HIS_FUND_H
