#include <doctor.h>

struct Doctor_T {
    int id;
    int age;
    bool sex;
    bool is_active;
    Department department;
    char name[32];
};

// 生命周期
Doctor_T Doctor_new(int id, int age, int sex, Department department, const char* name){}
void Doctor_free(Doctor_T* d){}

// 访问器
int Doctor_get_id(Doctor_T d){}
int Doctor_get_age(Doctor_T d){}
bool Doctor_get_sex(Doctor_T d){}
bool Doctor_is_active(Doctor_T d){}
Department Doctor_get_department(Doctor_T d){}
const char* Doctor_get_name(Doctor_T d){}

// 修改器
Status Doctor_set_id(Doctor_T d, int new_id){}
Status Doctor_set_age(Doctor_T d, int new_age){}
Status Doctor_set_sex(Doctor_T d, bool new_sex) {}
Status Doctor_set_is_active(Doctor_T d, bool new_is_active) {}
Status Doctor_set_department(Doctor_T d, Department new_department){}
Status Doctor_set_name(Doctor_T d, const char* new_name){}

// 比较器
int Doctor_cmp_id(const void *a, const void *b){}
int Doctor_cmp_age(const void *a, const void *b){}
int Doctor_cmp_department(const void *a, const void *b){}
int Doctor_cmp_name(const void *a, const void *b){}

// 模糊姓名搜索比较器
int Doctor_cmp_fuzzy(const void *a, const void *b){}
