#ifndef HIS_LIST_H
#define HIS_LIST_H

#include <base.h>

#define T List_T
typedef struct T *T;

// 生命周期
/**
 * @brief 创建一个空链表
 * @param element_size 元素大小
 * @return 返回创建空链表的指针
 */
T List_new(size_t element_size);

/**
 * @brief 释放链表内存
 * @param l 链表指针的地址
 * @note 传入的参数应为链表指针的 **地址**
 */
void List_free(T *l);

// 基础操作
/**
 * @brief 获取元素个数
 * @param l 链表指针
 * @return 元素个数
 */
size_t List_size(T l);

/**
 * @brief 添加元素（尾插）
 * @param l 链表指针
 * @param data 存入的数据
 */
void List_push_back(T l, const void *data);

/**
 * @brief 查找数据
 * @param l 链表指针
 * @param data_ptr 数据指针
 * @param cmp 比较器
 * @return 返回搜索到的首个数据地址，若不存在，返回NULL
 */
void *List_find(T l, const void *data_ptr, compare cmp);

/**
 * @brief 删除指定元素
 * @param l 链表指针
 * @param data_ptr 数据指针
 */
void List_remove(T l, void *data_ptr);

// 遍历
/**
 * @brief 使游标归零并返回数据，若链表为空返回NULL
 * @param l 链表指针
 * @return 返回当前游标数据
 */
void *List_first(T l);

/**
 * @brief 使游标向下移动并返回数据，若移动前后为dummy（即链表遍历结束）返回NULL
 * @param l 链表指针
 * @return 返回当前游标数据
 */
void *List_next(T l);

// 排序
/**
 * @brief 对链表进行归并排序
 * @param l 链表指针
 * @param cmp 比较器
 */
void List_sort(T l, compare cmp);

#undef T

#endif
