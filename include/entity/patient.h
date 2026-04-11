#ifndef HIS_PATIENT_H
#define HIS_PATIENT_H

#include <HIS_core.h>

// 匿名的主要实体
#define T Patient_T
typedef struct T *T;


// 生命周期
T Patient_new(
    gender gender, long long birth_ts, const char *name,
    const char *phone_num, const char *identity_id)
;
T Patient_load(
    int id,
    gender gender, long long birth_ts, const char *name,
    const char *phone, const char *id_card)
;
/* TODO: 你不会问我为什么这里有两个函数吧……
 * new  负责对新增的成员进行分配，这包括id的分配，详见位于base.h中的NEW_ID
 * load 负责在加载文件的时候进行分配，也就是说id已知，传入即可，不过需要更新id的最大值，详见位于base.h中的LOAD_ID
 */
void Patient_free(T *p);


// 访问器
int          Patient_get_id(T p);
gender       Patient_get_gender(T p);
long long    Patient_get_birth_ts(T p);
int          Patient_get_age(T p);      // TODO: 这里年龄计算记得调用 time_unix 中的计算函数
char *       Patient_get_name(T p);
char *       Patient_get_phone(T p);
char *       Patient_get_id_card(T p);


// 公开的重装载的数据包
typedef struct {
    gender      gender;     // 性别
    long long   birth_ts;   // 出生日期时间戳
    char        name[32];   // 姓名
    char        phone[12];  // 电话号码
    char        id_card[20];// 身份证号
} Patient_Update_Pack;
// 重装载： 在校验数据合法性后，将所有数据进行覆盖
Status Patient_update(T p, const Patient_Update_Pack *pack);


// 比较器详细定义见 base.h
int Patient_cmp_id(const void *a, const void *b);
int Patient_cmp_gender(const void *a, const void *b);
int Patient_cmp_age(const void *a, const void *b);
int Patient_cmp_name(const void *a, const void *b);
int Patient_cmp_phone(const void *a, const void *b);
int Patient_cmp_id_card(const void *a, const void *b);


/**
 * @brief 模糊姓名搜索器
 * @param a 被传入的字符串
 * @param b 搜索依据关键字
 * @return 只要关键字存在就返回 0
 */
int Patient_cmp_fuzzy(const void *a, const void *b);

#undef T

#endif// HIS_PATIENT_H
