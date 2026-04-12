#include <patient.h>




// 病人实体使用的id分配计数器，必须定义
static long long _id_counter = 0;




// Patient实体具体定义
struct Patient_T {
    long long   id;         // 内部id
    gender      gender;     // 性别
    long long   birth_ts;   // 出生日期时间戳
    char        name[32];   // 姓名
    char        phone[20];  // 电话号码
    char        id_card[20];// 身份证号
};





// 生命周期
Patient_T Patient_new(
    gender gender, long long birth_ts, const char *name,
    const char *phone_num, const char *identity_id){
}

Patient_T Patient_load(
    long long id,
    gender gender, long long birth_ts, const char *name,
    const char *phone, const char *id_card){
}

void Patient_free(Patient_T *p){

}




// 访问器
long long Patient_id(Patient_T p){
}

gender Patient_gender(Patient_T p){
}

long long Patient_birth_ts(Patient_T p){
}

int Patient_age(Patient_T p){
}

char * Patient_name(Patient_T p){
}

char * Patient_phone(Patient_T p){
}

char * Patient_id_card(Patient_T p){
}




// 重装载： 在校验数据合法性后，将所有数据进行覆盖
Status Patient_update(Patient_T p, const Patient_Update_Pack *pack){
}




// 比较器详细定义见 base.h
int Patient_cmp_id(const void *a, const void *b){
}

int Patient_cmp_gender(const void *a, const void *b){
}

int Patient_cmp_age(const void *a, const void *b){
}

int Patient_cmp_name(const void *a, const void *b){
}

int Patient_cmp_phone(const void *a, const void *b){
}

int Patient_cmp_id_card(const void *a, const void *b){
}




// 模糊姓名搜索器
int Patient_cmp_fuzzy(const void *a, const void *b){
}
