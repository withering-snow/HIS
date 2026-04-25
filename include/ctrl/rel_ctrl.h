#ifndef HIS_REL_CTRL_H
#define HIS_REL_CTRL_H

#include <HIS_core.h>
#include <data_ctrl.h>


typedef struct Rel_doc {
    long long pat_id;
    long long doc_id;
}Rel_doc;


typedef struct Rel_ward {
    long long pat_id;
    long long ward_id;
}Rel_ward;


typedef struct {
    long long doc_id;
    List_T queue;
}Rel_queue;
typedef struct{
    long long pat_id;
    long long priority_score;
}Rel_queue_node;


/*
 * Rel_Ctrl
 * 管理“病人-医生”逻辑绑定,“病人-病房”的高位路由索引
 * 不持有物理状态。
 * 床位与时间戳等物理数据请通过本模块返回的 ID 进一步访问 Ward 模块获取。
 */

// =============================================================================
// 生命周期管理

/**
 * @brief 初始化全局关系
 */
void Rel_init();

/**
 * @brief 销毁关系链表
 */
void Rel_destroy();


// =============================================================================
// 医患关系逻辑

/**
 * @brief 绑定医患关系
 * @return 状态码
 */
Status Rel_bind_doctor(long long patient_id, long long doctor_id);

/**
 * @brief 解除病人的医生分配
 * @return 状态码
 */
Status Rel_unbind_doctor(long long patient_id);

/**
 * @brief 查询病人的主治医生 ID
 * @return 医生 ID，若未分配返回 INVALID_ID
 */
long long Rel_get_doctor_by_patient(long long patient_id);

/**
 * @brief 反向查询：获取某位医生负责的所有病人 ID 集合
 * @return 包含 patient_id 的 List_T
 * @note ============ [内存警示] 调用者必须使用 List_free 释放返回的容器 ============
 */
List_T Rel_get_patients_by_doctor(long long doctor_id);


// =============================================================================
// 住院路由导航

/**
 * @brief 建立住院路由索引
 * @note 仅记录病人所在的病房，不涉及具体床位 (Bed_T 由 Ward 模块管理)
 */
Status Rel_bind_ward(long long patient_id, long long ward_id);

/**
 * @brief 注销住院
 */
Status Rel_unbind_ward(long long patient_id);

/**
 * @brief 快速定位病人所在的病房 ID
 * @return 病房 ID 或错误码
 * @note 拿到此 ID 后，可配合 Data_Ctrl 获取 Ward 对象，进而读取 Bed_T 中的 start_ts
 */
long long Rel_get_ward_by_patient(long long patient_id);

/**
 * @brief 反向查询：获取某个病房内的所有病人 ID 集合
 * @return 包含 patient_id 的 List_T
 * @note ============ [内存警示] 调用者必须使用 List_free 释放返回的容器 ============
 */
List_T Rel_get_patients_by_ward(long long ward_id);


// =============================================================================
// 快捷状态检索

/**
 * @brief 检查病人当前是否处于在院状态 (是否有住院路由)
 */
bool Rel_is_patient_admitted(long long patient_id);

/**
 * @brief 检查病人当前是否已分配医生
 */
bool Rel_has_doctor(long long patient_id);


// =============================================================================
// 实时叫号队列
/*
 * 职责：管理医生今日的活跃接诊队列。
 * 逻辑：基于“准时”假设，队列内部按 time_slot 物理排序。
 */

/**
 * @brief  将挂号/签到病人推入医生的今日队列
 * @param  sequence_no  时段序号 (用于内部排序)
 */
Status Rel_queue_push(long long doc_id, long long pat_id, int sequence_no, int time_frame);

/**
 * @brief  退号/取消挂号：从队列中移除特定病人
 */
Status Rel_queue_remove(long long doc_id, long long pat_id);

/**
 * @brief  医生叫号：弹出当前最该就诊的病人
 */
long long Rel_queue_pop(long long doc_id);

/**
 * @brief  获取某医生当前的排队副本
 * @return 包含 patient_id 的新链表。调用者负责 List_free
 */
List_T Rel_queue_get_all(long long doc_id);

/**
 * @brief  系统启动/每日零点同步：解析 Record 链表，重建所有医生的 Rel_Queue
 */
void Rel_queue_update();

// =============================================================================
// 对返回的链表提供释放工具
/**
 * @brief 辅助工具：清空并释放由本模块生成的临时 ID 列表
 */
#define Rel_list_free(l) List_free(&(l))

#endif // HIS_REL_CTRL_H