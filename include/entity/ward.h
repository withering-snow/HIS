#ifndef HIS_WARD_H
#define HIS_WARD_H

#include <HIS_core.h>

/* * * * * * * * * * * * * * *
 * 再次声明，实体层不向外暴露结构体*
 *    所有Xxxx_T均为对应指针    *
 * * * * * * * * * * * * * * */
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
/**
 * @brief               加载文件中的病房结构体
 * @param id            病床id
 * @param dept          部门枚举
 * @param daily_cost    每日花费
 * @return              返回加载完成的 Ward_T
 */
T Ward_load(
    long long id, Department dept, long long daily_cost)
;
/**
 * @brief                   新建病房结构体，并创建好病房从表，按顺序排号
 * @param dept              部门枚举
 * @param daily_cost        每日花费
 * @param start_bed_lable   起始的床号
 * @param bed_count         病床数
 * @return                  返回新建的 Ward_T
 */
T Ward_new(
    Department dept, long long daily_cost,
    int start_bed_lable, int bed_count)
;
/**
 * @brief       释放病房结构体并置空指针
 * @param w     Ward_T 指针
 * @note        注意是对 Ward_T 再次取地址
 */
void Ward_free(T* w);


// 访问器
long long   Ward_id(T w);
Department  Ward_dept(T w);
long long   Ward_daily_cost(T w);
int         Ward_bed_count(T w); // 返回该病房总床位数
int         Ward_empty_count(T w); // 返回可用病床数
List_T      Ward_beds(T w);

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

Status Ward_bed_load(T w, int bed_label, long long pat_id, BedStatus status, long long start_ts);

// 占用绑定
Status Ward_occupy_bed(T w, int bed_label, long long pat_id);

// 根据床号腾空
Status Ward_vacate_by_bed(T w, int bed_label);
// 根据病人腾空
Status Ward_vacate_by_patient(T w, long long pat_id);

// 根据床号获取明细
Bed_T* Ward_get_bed(T w, int bed_label);
// 根据病人获取明细
Bed_T* Ward_get_bed_by_patient(T w, long long pat_id);

/* ---------------- 从表 ---------------- */


#undef T

#endif// HIS_WARD_H
