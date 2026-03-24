#ifndef HIS_RELATION_MANAGER_H
#define HIS_RELATION_MANAGER_H

#include <HIS_core.h>

/**
 * @brief 医患关系记录
 */
typedef struct {
    int patient_id;
    int doctor_id;
    long long assigned_time;
} Rel_DoctorPatient;

/**
 * @brief 住院关系记录
 */
typedef struct {
    int patient_id;
    int ward_id;
    int bed_no;
} Rel_PatientWard;

// 绑定操作
Status RelationMgr_assign_doctor(int pat_id, int doc_id);
Status RelationMgr_assign_ward(int pat_id, int ward_id, int bed_no);

// 查询操作
int    RelationMgr_get_doctor_by_patient(int pat_id);
List_T RelationMgr_get_patients_by_doctor(int doc_id);

#endif