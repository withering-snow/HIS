#ifndef HIS_MEDICINE_H
#define HIS_MEDICINE_H

#include <HIS_core.h>

#define T Medicine_T
typedef struct Medicine_T *T;

// 药品批次定义
typedef enum{
    AVAILABLE,  // 可用
    EXHAUSTED,  // 用尽
    DISCARD     // 弃置
}BatchStatus;
typedef struct {
    long long   id;
    long long   buy_price;      // 进价（单价）
    long long   expire_ts;      // 过期时间
    int         remain;         // 剩余量
    BatchStatus status;         // 当前状态
    char        batch_no[32];   // 批号
} MedicineBatch;

/* ---------------- 主表 ----------------- */
// 生命周期
T Medicine_load(
    long long id, long long cur_price,
    int total_remain, const char* name)
;
T Medicine_new(
    long long cur_price, const char* name)
;
void Medicine_free(T* m);

// 访问器
long long Medicine_id(T m);
long long Medicine_cur_price(T m);
int Medicine_total_remain(T m);
const char* Medicine_name(T m);
List_T* Medicine_batches(T m);

// 修改器
Status Medicine_set_price(T m, int new_price);
Status Medicine_update_remain(T m);
Status Medicine_set_name(T m, const char* new_name);

// 比较器
int Medicine_cmp_id(const void* a, const void* b);
int Medicine_cmp_remain(const void* a, const void* b);

// 模糊药品名搜索比较器
int Medicine_cmp_fuzzy(const void* a, const void* b);
/* ---------------- 主表 ----------------- */


/* ---------------- 从表 ----------------- */
// 添加/加载 药品批次
Status Medicine_batch_load(
    T m,
    long long id, long long buy_price, long long expire_ts,
    int remain, BatchStatus status, const char* batch_no)
;
Status Medicine_batch_add(
    T m,
    long long buy_price, long long expire_ts, int remain, const char* batch_no)
;

/**
 * @brief 扣减库存（核心业务：开药）
 * @param amount 扣减数量
 * @return Status 扣减成功或库存不足
 * 内部逻辑：遍历 batches，按 expire_ts 升序，
 * 自动处理跨批次扣减，并更新批次状态为 EXHAUSTED。
 */
Status Medicine_deduct(T m, int amount);

/**
 * @brief 报废特定批次
 * @param batch_id 批次内部 ID
 */
Status Medicine_discard_batch(T m, long long batch_id);

// 从表的比较器
// 按过期时间排序
int Medicine_batch_cmp_expire(const void* a, const void* b);
int Medicine_batch_cmp_id(const void* a, const void* b);
int Medicine_batch_cmp_remain(const void* a, const void* b);
/* ---------------- 从表 ----------------- */


#undef T

#endif// HIS_MEDICINE_H
