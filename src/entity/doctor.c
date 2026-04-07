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
Doctor_T Doctor_new(int id, int age, int sex, Department department, const char* name) {
    Doctor_T doctor = (Doctor_T)safe_malloc(sizeof(struct Doctor_T));
    doctor->id=id;
    doctor->age=age;
    doctor->sex=sex;
    doctor->department=department;
    strcpy(doctor->name, name);
    return doctor;
}
void Doctor_free(Doctor_T* d) {
    ASSERT(d!=NULL, "医生指针不能为空");
    free(*d);
    *d=NULL;
}

// 访问器
int Doctor_get_id(Doctor_T d) {
    return d->id;
}
int Doctor_get_age(Doctor_T d) {
    return d->age;
}
bool Doctor_get_sex(Doctor_T d) {
    return d->sex;
}
bool Doctor_is_active(Doctor_T d) {
    return d->is_active;
}
Department Doctor_get_department(Doctor_T d) {
    return d->department;
}
const char* Doctor_get_name(Doctor_T d) {
    return d->name;
}

// 修改器
Status Doctor_set_id(Doctor_T d, int new_id) {
    d->id=new_id;
    return HIS_OK;
}
Status Doctor_set_age(Doctor_T d, int new_age) {
    d->age=new_age;
    return HIS_OK;
}
Status Doctor_set_sex(Doctor_T d, bool new_sex) {
    d->sex=new_sex;
    return HIS_OK;
}
Status Doctor_set_is_active(Doctor_T d, bool new_is_active) {
    d->is_active=new_is_active;
    return HIS_OK;
}
Status Doctor_set_department(Doctor_T d, Department new_department) {
    d->department=new_department;
    return HIS_OK;
}
Status Doctor_set_name(Doctor_T d, const char* new_name) {
    strcpy(d->name, new_name);
    return HIS_OK;
}

// 比较器
int Doctor_cmp_id(const void *a, const void *b) {
    const Doctor_T d1=*(Doctor_T *)a;
    const Doctor_T d2=*(Doctor_T *)b;
    return d1->id-d2->id;
}
int Doctor_cmp_age(const void *a, const void *b) {
    const Doctor_T d1=*(Doctor_T *)a;
    const Doctor_T d2=*(Doctor_T *)b;
    return d1->age-d2->age;
}
int Doctor_cmp_department(const void *a, const void *b) {
    const Doctor_T d1=*(Doctor_T *)a;
    const Doctor_T d2=*(Doctor_T *)b;
    return d1->department-d2->department;
}
int Doctor_cmp_name(const void *a, const void *b) {
    const Doctor_T d1=*(Doctor_T *)a;
    const Doctor_T d2=*(Doctor_T *)b;
    return strcmp(d1->name, d2->name);
}

// 模糊姓名搜索比较器
int Doctor_cmp_fuzzy(const void *a, const void *b) {
    const Doctor_T d1=*(Doctor_T *)a;
    const char q=*(char*)b;
    return strstr(a,b) ? 0:1;
}
