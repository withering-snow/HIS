#ifndef HIS_RELATION_MANAGER_H
#define HIS_RELATION_MANAGER_H

#include <HIS_core.h>

typedef struct Rel_doc Rel_doc;
typedef struct Rel_ward Rel_ward;

void Rel_init();
void Rel_destroy();

int Rel_get_doctor_by_patient(int patient_id);
List_T Rel_get_patient_by_doctor(int doctor_id);

int Rel_get_ward_by_patient(int patient_id);
int Rel_get_bed_by_patient(int patient_id);
long long Rel_get_time_stamp_by_patient(int patient_id);

Status Rel_band_doc(int patient_id, int doctor_id);
Status Rel_band_ward(int patient_id, int ward_id, int bed_id, long long time_stamp);

Status Rel_remove_doc(int patient_id);
Status Rel_remove_ward(int patient_id);

#endif