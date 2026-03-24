#ifndef HIS_DATA_MANAGER_H
#define HIS_DATA_MANAGER_H

#include <base.h>
#include <list.h>

#include <doctor.h>
#include <fund.h>
#include <medicine.h>
#include <patient.h>
#include <ward.h>

// 生命周期：全部实体链表构建与释放
void Data_init();
void Data_destroy();

// 获取对应链表
List_T Data_get_doctors();
List_T Data_get_funds();
List_T Data_get_medicines();
List_T Data_get_patients();
List_T Data_get_wards();

#endif// HIS_DATA_MANAGER_H
