#ifndef HIS_DOCTOR_SERV_H
#define HIS_DOCTOR_SERV_H


#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>
#include <HIS_serv_base.h>




Status Serv_doc_active();
Status Serv_doc_unactive();




Status Serv_doc_call_reg();
Status Serv_doc_end_reg();
Status Serv_doc_view_reg();


Status Serv_doc_consult(const char* diagnosis, const char* advice);
Status Serv_doc_exam(long long cost, const char* exam_name);
Status Serv_doc_prescribe(const char* med_name, int amount);
Status Serv_doc_admission(long long ward_id, int bed_id, long long deposit);

Status Serv_doc_discharge(long long pat_id );
Status Serv_doc_change_bed();
Status Serv_doc_change_doc();


#endif //HIS_DOCTOR_SERV_H
