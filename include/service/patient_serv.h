#ifndef HIS_PATIENT_SERV_H
#define HIS_PATIENT_SERV_H

#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>
#include <HIS_serv_base.h>


/**
 * @brief           获取病人基础数据包
 * @param pat_id    病人id
 * @return          病人数据包，若病人不存在则返回 NULL
 * @note            非 NULL 的情况下注意释放
 */
ServPatientDataPackage* Serv_patient_data(long long pat_id);

/**
 * @brief           获取病人诊疗记录包
 * @param pat_id    病人id
 * @return          与该病人有关的诊疗记录信息链表（可能是空表）
 * @note            返回链表需要释放
 */
List_T Serv_patient_records(long long pat_id);


/**
 * @brief   获取指定医生时间段的挂号状态
 * @return  返回 ServSlotDataPackage ，注意释放
 */
ServSlotDataPackage* Serv_patient_get_slot_status(long long doc_id, int target_date, int time_frame);

/**
 * @brief 获取某医生某天的全时段挂号状态列表
 * @return 包含 ServSlotDataPackage 的 List_T， 注意释放
 */
List_T Serv_patient_day_slots(long long doc_id, int target_date);


/**
 * @brief 病人预约挂号
 * @param doc_id 医生id
 * @param target_date 挂号目标日期
 * @param time_frame 时间段索引
 * @return
 */
Status Serv_patient_register(long long doc_id, int target_date, int time_frame);
Status Serv_patient_checkin(long long doc_id);
List_T Serv_patient_queue_status();


#endif //HIS_PATIENT_SERV_H
