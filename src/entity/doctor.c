#include <doctor.h>




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
};




// 生命周期
Doctor_T Doctor_load(
    long long id,
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card){

}
Doctor_T Doctor_new(
    gender gender, long long birth_ts, bool is_active, Department dept, DoctorTitle title,
    const char* name, const char* phone, const char* id_card){

}

void Doctor_free(Doctor_T* d){

}




long long Doctor_id(Doctor_T d){

}

gender Doctor_gender(Doctor_T d){

}

long long Doctor_birth_ts(Doctor_T d){

}

int Doctor_age(Doctor_T d){

}

bool Doctor_is_active(Doctor_T d){

}

Department Doctor_dept(Doctor_T d){

}

DoctorTitle Doctor_title(Doctor_T d){

}

const char * Doctor_name(Doctor_T d){

}

const char * Doctor_phone(Doctor_T d){

}

const char * Doctor_id_card(Doctor_T d){

}




Status Doctor_update(Doctor_T d, const Doctor_Update_Pack *pack){

}




int Doctor_cmp_id(const void *a, const void *b){

}

int Doctor_cmp_gender(const void *a, const void *b){

}

int Doctor_cmp_age(const void *a, const void *b){

}

int Doctor_cmp_is_active(const void *a, const void *b){

}

int Doctor_cmp_dept(const void *a, const void *b){

}

int Doctor_cmp_title(const void *a, const void *b){

}

int Doctor_cmp_name(const void *a, const void *b){

}

int Doctor_cmp_phone(const void *a, const void *b){

}

int Doctor_cmp_id_card(const void *a, const void *b){

}




int Doctor_cmp_fuzzy(const void *a, const void *b){

}
