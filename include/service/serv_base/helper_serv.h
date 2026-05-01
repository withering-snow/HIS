#ifndef HIS_HELPER_SERV_H
#define HIS_HELPER_SERV_H

#include <HIS_core.h>
#include <HIS_entity.h>
#include <HIS_db.h>


/**
 * @brief           对返回的临时链表进行释放
 * @param list      临时链表变量
 */
#define Serv_helper_free_list(list) List_free(&list);
/**
 * @brief           对返回的临时值进行释放（一般为结构体）
 * @param value     临时返回值
 */
#define Serv_helper_free_value(value)          \
    do{                                        \
        free(value);                           \
        value = NULL;                          \
    }while(0)


typedef struct{
    gender gender;
    int age;
    char birthday[20];
    char name[32];
    char phone[20];
    char id_card[20];
} ServPatientDataPackage;


typedef enum{
    TYPE_PATIENT,
    TYPE_DOCTOR,
    TYPE_MEDICINE
}ServEntityType;
/**
 * @brief       将id转换成指定实体的名字字符串，便于显示
 * @param id    传入id
 * @param type  id所指实体类型
 * @return      id所指实体的名字
 */
const char* Serv_helper_id_to_name(long long id, ServEntityType type);

typedef struct{
    char        type_name[16]; // 记录类型名
    bool        is_invalid;    // 是否废弃
    long long   actor_id;      // 行为主体id
    char        actor_name[32];// 行为主体名字
    long long   time_stamp;    // 时间
    long long   cost;          // 花费
    char        content[512];  // 核心内容
}ServRecordDataPackage;
ServRecordDataPackage* Serv_helper_record_to_pkg(Record_T r);






static const char* record_type_names[] =
    {"挂号", "看诊", "检查", "买药", "住院", "出院", "床位变动", "医生变动", "进货", "废弃"};
static const char* time_frame_names[] =
    {
    "急诊",
    "8:00-8:30", "8:30-9:00", "9:00-9:30", "9:30-10:00", "10:00-10:30", "10:30-11:00", "11:00-11:30", "11:30-12:00",
    "13:00-13:30", "13:30-14:00", "14:00-14:30", "14:30-15:00", "15:00-15:30", "15:30-16:00", "16:00-16:30", "16:30-17:00",
    };
static const char* registration_type_names[] =
    {"预约", "候诊", "诊毕"};


#endif //HIS_HELPER_SERV_H
