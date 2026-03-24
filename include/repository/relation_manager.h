#ifndef HIS_RELATION_MANAGER_H
#define HIS_RELATION_MANAGER_H

#include <base.h>
#include <list.h>

typedef struct Rel_doc Rel_doc;
typedef struct Rel_ward Rel_ward;

// 生命周期：全部关系链表构建与释放
void Rel_init();
void Rel_destroy();

// 关系管理
Status Rel_doc_band(int patient_id, int doctor_id);
Status Rel_ward_band(int patient_id, int ward_id);
Status Rel_doc_remove(int patient_id, int doctor_id);
Status Rel_ward_remove(int patient_id, int ward_id);
int Rel_doc_get(int patient_id);
int Rel_ward_get(int patient_id);


#endif// HIS_RELATION_MANAGER_H
