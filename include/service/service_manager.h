#ifndef HIS_SERVICE_MANAGER_H
#define HIS_SERVICE_MANAGER_H

#include <HIS_entity.h>

/**
 * @brief 业务逻辑层 (Service Layer)
 * 职责：编排业务流程，处理跨实体的复杂逻辑。
 * 规范：所有函数接受 ID，返回 Status。UI 层通过返回的 Status 弹窗提示。
 */

/* ========================================================================== */
/*                          1. 患者自助服务 (Patient View)                      */
/* ========================================================================== */

/**
 * @brief 患者账户充值
 */
Status Service_patient_recharge(int patient_id, double amount);

/**
 * @brief 获取患者简报（用于显示余额、当前状态、最后一次就诊日期）
 * 此处可定义一个辅助结构体返回，或由 UI 层组合 DataMgr 接口
 */
Status Service_patient_get_data(int patient_id);


/* ========================================================================== */
/*                          2. 医护诊疗服务 (Doctor View)                       */
/* ========================================================================== */

/**
 * @brief 门诊挂号
 * 逻辑：检查医生是否出诊 -> 扣除挂号费 -> 产生一条挂号记录
 */
Status Service_clinic_register(int patient_id, int doctor_id);

/**
 * @brief 处方开药 (系统最核心逻辑)
 * 逻辑：校验库存 -> 校验余额 -> 扣款 -> 减库存 -> 记录病历 -> 增加医院营业额
 */
Status Service_clinic_prescribe(int patient_id, int medicine_id, int count);

/**
 * @brief 办理入院
 * 逻辑：校验病房空床 -> 校验押金(不低于1000) -> 修改床位状态 -> 记录关系
 */
Status Service_inpatient_admit(int patient_id, int ward_id, int bed_id);

/**
 * @brief 下达出院医嘱（医疗确认）
 * 逻辑：将病人状态改为“待结账”
 */
Status Service_inpatient_ready_discharge(int patient_id);


/* ========================================================================== */
/*                          3. 行政管理服务 (Admin View)                        */
/* ========================================================================== */

/**
 * @brief 最终结算出院 (财务审计)
 * 逻辑：计算住院天数 -> 计算总费用 -> 押金多退少补 -> 释放床位 -> 产生结算单
 */
Status Service_admin_settle(int patient_id);

/**
 * @brief 诊疗记录撤销与重录 (对应题签重点)
 * 逻辑：定位旧记录 -> 标记撤销 -> 插入新记录 -> 如果涉及金额需冲正流水
 */
Status Service_admin_change_record(int patient_id, int record_index, const MedicalRecord *new_data);

/**
 * @brief 医院资源管理
n  */
Status Service_admin_UpdateMedicinePrice(int med_id, double new_price);
Status Service_admin_SetDoctorDuty(int doc_id, bool is_on_duty);
Status Service_admin_purchase_medicine(int medicine_id, double price);


/* ========================================================================== */
/*                          4. 系统级服务 (System Service)                     */
/* ========================================================================== */

/**
 * @brief 一键保存系统状态
 * 逻辑：调用 IO_Manager 将 DataMgr 和 RelationMgr 的数据写入磁盘
 */
Status Service_System_SaveData(void);

/**
 * @brief 统计全院营业额报表
 */
double Service_System_GetTotalRevenue(void);

#endif // HIS_SERVICE_MANAGER_H