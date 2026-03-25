#ifndef HIS_DATA_MANAGER_H
#define HIS_DATA_MANAGER_H

#include <HIS_core.h>
#include <HIS_entity.h>

// 生命周期：全部实体链表构建与释放
void Data_init();
void Data_destroy();

// 获取对应链表
List_T Data_get_doctor();
List_T Data_get_fund();
List_T Data_get_medicine();
List_T Data_get_patient();
List_T Data_get_ward();

#endif// HIS_DATA_MANAGER_H
