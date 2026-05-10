#include <doctor.h>

static long long _id_counter=1;


struct Doctor_T {
    long long   id;           // 内部id
    gender      gender;       // 性别
    long long   birth_ts;     // 出生日期时间戳
    bool        is_active;    // 出诊状态
    Department  dept;         // 科室
    DoctorTitle title;        // 职称
    char        name[32];     // 姓名
    char        phone[20];    // 手机号
    char        id_card[20];  // 身份证号
    long long   reg_fee;      //挂号费用
};




// 生命周期
Doctor_T Doctor_load(
    long long id,
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card, long long reg_fee){
    Doctor_T d=safe_malloc(sizeof(struct Doctor_T));
    LOAD_ID(id);
    d->id=id;
    d->gender=gender;
    d->birth_ts=birth_ts;
    d->is_active=is_active;
    d->dept=dept;
    d->title=title;
    strncpy(d->name,name,32);
    strncpy(d->phone,phone,20);
    strncpy(d->id_card,id_card,20);
    d->reg_fee=reg_fee;
    return d;
}
Doctor_T Doctor_new(
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card, long long reg_fee){
    Doctor_T d=safe_malloc(sizeof(struct Doctor_T));
    d->id=NEW_ID();
    d->gender=gender;
    d->birth_ts=birth_ts;
    d->is_active=is_active;
    d->dept=dept;
    d->title=title;
    strncpy(d->name,name,32);
    strncpy(d->phone,phone,20);
    strncpy(d->id_card,id_card,20);
    d->reg_fee=reg_fee;
    return d;
}

void Doctor_free(Doctor_T* d){
    ASSERT(d !=NULL,"不合法");
    free(*d);
    *d=NULL;
}




long long Doctor_id(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->id;
}

gender Doctor_gender(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->gender;
}

long long Doctor_birth_ts(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->birth_ts;
}

int Doctor_age(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return  Time_CalculateAge(d-> birth_ts);
}

bool Doctor_is_active(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->is_active;
}

Department Doctor_dept(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->dept;
}

DoctorTitle Doctor_title(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->title;
}

const char * Doctor_name(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->name;
}

const char * Doctor_phone(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->phone;
}

const char * Doctor_id_card(Doctor_T d){
    ASSERT(d !=NULL,"不合法");
    return d->id_card;
}

long long Doctor_reg_fee(Doctor_T d)
{
    ASSERT(d !=NULL,"不合法");
    return d->reg_fee;
}


Status Doctor_update(Doctor_T d, const Doctor_Update_Pack *pack){
    ASSERT(d !=NULL,"不合法");
    ASSERT(pack !=NULL,"不合法");
    d->gender=pack->gender;
    d->birth_ts=pack->birth_ts;
    d->is_active=pack->is_active;
    d->dept=pack->dept;
    d->title=pack->title;
    strncpy(d->name,pack->name,32);
    strncpy(d->phone,pack->phone,20);
    strncpy(d->id_card,pack->id_card,20);
    d->reg_fee=pack->reg_fee;
    return HIS_OK;
}




int Doctor_cmp_id(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->id>q->id)-(p->id<q->id);
}

int Doctor_cmp_gender(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->gender>q->gender)-(p->gender<q->gender);
}

int Doctor_cmp_age(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->birth_ts>q->birth_ts)-(p->birth_ts<q->birth_ts);
}

int Doctor_cmp_is_active(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->is_active>q->is_active)-(p->is_active<q->is_active);
}

int Doctor_cmp_dept(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->dept>q->dept)-(p->dept<q->dept);
}

int Doctor_cmp_title(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->title>q->title)-(p->title<q->title);
}

int Doctor_cmp_name(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return strncmp(p->name,q->name,32);
}

int Doctor_cmp_phone(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return strncmp(p->phone,q->phone,20);
}

int Doctor_cmp_id_card(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return strncmp(p->id_card,q->id_card,20);
}

int Doctor_cmp_reg_fee(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    Doctor_T q=*(Doctor_T *)b;
    return (p->reg_fee>q->reg_fee)-(p->reg_fee<q->reg_fee);
}

int Doctor_cmp_fuzzy(const void *a, const void *b){
    Doctor_T p=*(Doctor_T *)a;
    if (strstr(p->name,b)!=NULL)
        return HIS_OK;
    return HIS_ERR_NOT_FOUND;
}
