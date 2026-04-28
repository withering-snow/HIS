#ifndef HIS_IO_CTRL_H
#define HIS_IO_CTRL_H

#include <HIS_entity.h>
#include <HIS_core.h>
#include <HIS_db.h>

#ifdef __WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif


Status Io_save_patient();
Status Io_load_patient();
Status Io_save_doctor();
Status Io_load_doctor();
Status Io_save_fund();
Status Io_load_fund();
Status Io_load_medicine();
Status Io_save_medicine();
Status Io_save_account();
Status Io_load_account();
Status Io_save_ward();
Status Io_load_ward();
Status Io_save_record();
Status Io_load_record();
#endif// HIS_IO_CTRL_H
