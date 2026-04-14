#ifndef HIS_DATA_CTRL_H
#define HIS_DATA_CTRL_H

#include <HIS_core.h>
#include <HIS_entity.h>

// 生命周期：全部实体链表构建与释放
/**
 * @brief 初始化所有数据链表
 */
void Data_init();

/**
 * @brief 销毁所有数据链表
 */
void Data_destroy();


// 获取对应链表
/**
 * @brief 获取account链表
 * @return 返回account数据链表
 */
List_T Data_get_account();

/**
 * @brief 获取doctor链表
 * @return 返回doctor数据链表
 */
List_T Data_get_doctor();

/**
 * @brief 获取fund链表
 * @return 返回fund数据链表
 */
List_T Data_get_fund();

/**
 * @brief 获取medicine链表
 * @return 返回medicine数据链表
 */
List_T Data_get_medicine();

/**
 * @brief 获取patient链表
 * @return 返回patient数据链表
 */
List_T Data_get_patient();

/**
 * @brief 获取record链表
 * @return 返回record数据链表
 */
List_T Data_get_record();

/**
 * @brief 获取ward链表
 * @return 返回ward数据链表
 */
List_T Data_get_ward();

#endif// HIS_DATA_CTRL_H
