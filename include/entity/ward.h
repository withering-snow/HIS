#ifndef HIS_WARD_H
#define HIS_WARD_H

#include <HIS_core.h>

#define T Ward_T
typedef struct Ward_T *T;

// 床位状态
typedef enum {
    BED_EMPTY,      // 空闲
    BED_OCCUPIED,   // 有人
    BED_MAINTAIN    // 维修/消毒
} BedStatus;

// 床位明细（从表）
typedef struct {
    int         bed_label;   // 床号
    long long   pat_id;      // 住在这儿的病人 ID (0 表示没人)
    BedStatus   status;
    long long   start_ts;    // 入住时间（用于算钱）
} Bed_T;


/* ---------------- 主表 ---------------- */

// 生命周期
T Ward_load(
    long long id, Department dept, long long daily_cost)
    // TODO: 这里没有提供bed_count,请置零，在后续床位加载进行累加
;
// TODO :初始化病房：要按照起始病床号和床数顺序生成床
T Ward_new(
    Department dept, long long daily_cost,
    int start_bed_label, int bed_count)
;
void Ward_free(T* w);


// 访问器
long long   Ward_id(T w);
Department  Ward_dept(T w);
long long   Ward_daily_cost(T w);
int         Ward_bed_count(T w); // 返回该病房总床位数
int         Ward_empty_count(T w); // 返回可用病床数

// 公开的重装载的数据包
typedef struct {
    Department  dept;
    long long   daily_cost;
} Ward_Update_Pack;
// 重装载： 在校验数据合法性后，将所有数据进行覆盖
Status Ward_update(T w, const Ward_Update_Pack* pack);

// 比较器
int Ward_cmp_id(const void* a, const void* b);
int Ward_cmp_dept(const void* a, const void* b);
int Ward_cmp_cost(const void* a, const void* b);
int Ward_cmp_empty(const void* a, const void* b); // 找哪间房空位多

/* ---------------- 主表 ---------------- */


/* ---------------- 从表 ---------------- */

// 床位管理
Status Ward_occupy_bed(T w, int bed_label, long long pat_id);
Status Ward_free_bed(T w, int bed_label);



// 腾空床位：根据病人 ID 自动找到床并释放
Status Ward_release_bed(T w, long long pat_id);

// 获取某个床位的具体信息
Bed_T* Ward_get_bed(T w, int bed_label);

/* ---------------- 从表 ---------------- */


#undef T

#endif// HIS_WARD_H
