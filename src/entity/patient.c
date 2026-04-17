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
    Patient_T p=safe_malloc(sizeof(struct Patient_T));
    p->id=NEW_ID();
    ASSERT(p !=NULL,"不合法");
    p->gender=gender;
    p->birth_ts=birth_ts;
    strncpy(p->name,name,32);
    strncpy(p->phone,phone_num,20);
    strncpy(p->id_card,identity_id,20);
    return p;
}

Patient_T Patient_load(
    long long id,
    gender gender, long long birth_ts, const char *name,
    const char *phone, const char *id_card){
    Patient_T p=safe_malloc(sizeof(struct Patient_T));
    ASSERT(p !=NULL,"不合法");
    LOAD_ID(id);
    p->id=id;
    p->gender=gender;
    p->birth_ts=birth_ts;
    strncpy(p->name,name,32);
    strncpy(p->phone,phone,20);
    strncpy(p->id_card,id_card,20);
    return p;
}

void Patient_free(Patient_T *p){
    ASSERT((p!=NULL&&*p!=NULL),"不合法");
    free(p);
    *p=NULL;
}




// 访问器
long long Patient_id(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->id;
}

gender Patient_gender(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->gender;
}

long long Patient_birth_ts(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->birth_ts;
}

int Patient_age(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return  Time_CalculateAge(p-> birth_ts);
}

char * Patient_name(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->name;
}

char * Patient_phone(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->phone;
}

char * Patient_id_card(Patient_T p){
    ASSERT(p !=NULL,"不合法");
    return p->id_card;
}




// 重装载： 在校验数据合法性后，将所有数据进行覆盖
Status Patient_update(Patient_T p, const Patient_Update_Pack *pack){
    ASSERT(p !=NULL,"不合法");
    ASSERT(pack !=NULL,"不合法");
    p->gender=pack->gender;
    p->birth_ts=pack->birth_ts;
    strncpy(p->name,pack->name,32);
    strncpy(p->phone,pack->phone,20);
    strncpy(p->id_card,pack->id_card,20);
    return HIS_OK;
}




// 比较器详细定义见 base.h
int Patient_cmp_id(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return (p->id > q->id) - (p->id < q->id);
}

int Patient_cmp_gender(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return (p->gender > q->gender) - (p->gender < q->gender);
}

int Patient_cmp_age(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return (p->birth_ts > q->birth_ts) - (p->birth_ts < q->birth_ts);
}

int Patient_cmp_name(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return strncmp(p->name,q->name,32);
}

int Patient_cmp_phone(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return strncmp(p->phone,q->phone,20);
}

int Patient_cmp_id_card(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    const struct Patient_T *q=(struct Patient_T *)b;
    return strncmp(p->id_card,q->id_card,20);
}




// 模糊姓名搜索器
int Patient_cmp_fuzzy(const void *a, const void *b){
    const struct Patient_T *p=(struct Patient_T *)a;
    if (strstr(p->name,b)!=NULL)
        return 0;
    else  return HIS_ERR_NOT_FOUND;
}
