#ifndef HIS_RELATION_MANAGER_H
#define HIS_RELATION_MANAGER_H

#include <HIS_core.h>

typedef struct Rel_doc Rel_doc;
typedef struct Rel_ward Rel_ward;


// 生命周期
/**
 * @brief 初始化所有关系链表
 */
void Rel_init();

/**
 * @brief 销毁所有关系链表
 */
void Rel_destroy();


// 关系查询
/**
 * @brief 查询一个病人的主治医生
 * @param patient_id 查询依据：病人id
 * @return 返回对应的医生id,若不存在返回-10
 */
int Rel_get_doctor_by_patient(int patient_id);

/**
 * @brief 查询一个医生正在治疗的病人
 * @param doctor_id 查询依据：医生id
 * @return 返回对应的病人id链表，注意需要释放
 */
List_T Rel_get_patient_by_doctor(int doctor_id);

/**
 * @brief 查询一个病人所在的病房
 * @param patient_id 查询依据：病人id
 * @return 返回对应的病房号，若不存在返回-10
 */
int Rel_get_ward_by_patient(int patient_id);

/**
 * @brief 查询一个病人的病床号
 * @param patient_id 查询依据：病人id
 * @return 返回对应的病床号，若不存在返回-10
 */
int Rel_get_bed_by_patient(int patient_id);

/**
 * @brief 查询一个病人的入院时间戳
 * @param patient_id 查询依据：病人id
 * @return 返回对应的入院时间戳，若不存在返回-10
 */
long long Rel_get_time_stamp_by_patient(int patient_id);


// 关系绑定与移除
/**
 * @brief 绑定医患关系
 * @param patient_id 病人id
 * @param doctor_id 医生id
 * @return 状态码
 */
Status Rel_band_doc(int patient_id, int doctor_id);

/**
 * @brief 绑定住院关系
 * @param patient_id 病人id
 * @param ward_id 病房id
 * @param bed_id 床位id
 * @param time_stamp 入院时间戳
 * @return 状态码
 */
Status Rel_band_ward(int patient_id, int ward_id, int bed_id, long long time_stamp);

/**
 * @brief 删除医患关系
 * @param patient_id 病人id
 * @return 状态码
 */
Status Rel_remove_doc(int patient_id);

/**
 * @brief 删除住院关系
 * @param patient_id 病人id
 * @return 状态码
 */
Status Rel_remove_ward(int patient_id);

#endif