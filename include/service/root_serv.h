#ifndef HIS_ROOT_SERV_H
#define HIS_ROOT_SERV_H

#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>
#include <HIS_serv_base.h>
#include <log_ctrl.h>

/* 医生管理业务 */
Status Serv_root_add_doctor(const Doctor_Update_Pack *pack);
Status Serv_root_remove_doctor(long long doc_id);

/* 药品管理业务 */
Status Serv_root_add_medicine(long long cur_price, const char* name);
Status Serv_root_add_medicine_batch(long long med_id, long long buy_price, long long expire_ts, int remain, const char* batch_no);
Status Serv_root_remove_medicine(long long med_id);

/* 病房管理业务 */
Status Serv_root_add_ward(Department dept, long long daily_cost, int start_bed_label, int bed_count, const char* ward_name);
Status Serv_root_remove_ward(long long ward_id);

/* 账户管理业务 */
Status Serv_root_remove_account(long long acc_id);

#endif //HIS_ROOT_SERV_H
