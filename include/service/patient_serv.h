#ifndef HIS_PATIENT_SERV_H
#define HIS_PATIENT_SERV_H

#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>
#include <HIS_serv_base.h>


ServPatientDataPackage* Serv_patient_data(long long pat_id);
List_T Serv_patient_records(long long pat_id);

Status Serv_patient_register(long long doc_id, int target_date, int time_frame);


#endif //HIS_PATIENT_SERV_H
