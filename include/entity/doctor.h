#ifndef HIS_DOCTOR_H
#define HIS_DOCTOR_H

#include <HIS_core.h>

#define T Doctor_T
typedef struct Doctor_T *T;

typedef enum {
    TITLE_RESIDENT,           // 住院医师
    TITLE_ATTENDING,          // 主院医师
    TITLE_ASSOC_CHIEF,        // 副主任医师
    TITLE_CHIEF               // 主任医师
} DoctorTitle;

// 生命周期
T Doctor_load(
    long long id,
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card)
;
T Doctor_new(
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card)
;
void Doctor_free(T* d);

// 访问器
long long       Doctor_id(T d);
gender          Doctor_gender(T d);
long long       Doctor_birth_ts(T d);
int             Doctor_age(T d);
bool            Doctor_is_active(T d);
Department      Doctor_dept(T d);
DoctorTitle     Doctor_title(T d);
const char*     Doctor_name(T d);
const char*     Doctor_phone(T d);
const char*     Doctor_id_card(T d);


// 公开的重装载的数据包
typedef struct {
    gender      gender;
    long long   birth_ts;
    Department  dept;
    DoctorTitle title;
    bool        is_active;
    char        name[32];
    char        phone[20];
    char        id_card[20];
} Doctor_Update_Pack;
// 重装载： 在校验数据合法性后，将所有数据进行覆盖
Status Doctor_update(T d, const Doctor_Update_Pack *pack);


// 比较器
int Doctor_cmp_id(const void *a, const void *b);
int Doctor_cmp_gender(const void *a, const void *b);
int Doctor_cmp_age(const void *a, const void *b);
int Doctor_cmp_is_active(const void *a, const void *b);
int Doctor_cmp_dept(const void *a, const void *b);
int Doctor_cmp_title(const void *a, const void *b);
int Doctor_cmp_name(const void *a, const void *b);
int Doctor_cmp_phone(const void *a, const void *b);
int Doctor_cmp_id_card(const void *a, const void *b);


/**
 * @brief 模糊姓名搜索器
 * @param a 被传入的结构体
 * @param b 搜索依据关键字
 * @return 只要关键字存在就返回 0
 */
int Doctor_cmp_fuzzy(const void *a, const void *b);

#undef T

#endif// HIS_DOCTOR_H
