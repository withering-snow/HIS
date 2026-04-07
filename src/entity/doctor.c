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
    Doctor_T doctor;
    doctor->id=id;
    doctor->age=age;
    doctor->sex=sex;
    doctor->department=department;
    strcpy(doctor->name, name);
    return doctor;
}
void Doctor_free(Doctor_T* d) {
    free(d);
    d=NULL;
}

// 访问器
int Doctor_get_id(Doctor_T d) {
    printf("Doctor's id is");
    printf("%d\n", d->id);
    return d->id;
}
int Doctor_get_age(Doctor_T d) {
    printf("Doctor's age is");
    printf("%d\n", d->age);
    return d->age;
}
bool Doctor_get_sex(Doctor_T d) {
     if(d->sex!=0) {
         printf("Doctor's sex is man");
     }
     else printf("Doctor's sex is woman");
}
bool Doctor_is_active(Doctor_T d) {
    if(d->is_active!=0) {
        printf("Doctor is active");
    }
    else printf("Doctor isn't active");
}
Department Doctor_get_department(Doctor_T d) {
    printf("Doctor's department is");
    printf("%d\n", d->department);
    return d->department;
}
const char* Doctor_get_name(Doctor_T d) {
    printf("Doctor's name is");
    printf("%s\n", d->name);
    return d->name;
}

// 修改器
Status Doctor_set_id(Doctor_T d, int new_id) {
    d->id=new_id;
    printf("Doctor's new id is");
    printf("%d\n", new_id);
    return new_id;
}
Status Doctor_set_age(Doctor_T d, int new_age) {
    d->age=new_age;
    printf("Doctor's new age is");
    printf("%d\n", new_age);
    return new_age;
}
Status Doctor_set_sex(Doctor_T d, bool new_sex) {
    d->sex=new_sex;
    printf("Doctor's new sex is ");
    if(new_sex!=0) printf("man");
    else printf("woman");
}
Status Doctor_set_is_active(Doctor_T d, bool new_is_active) {
    d->is_active=new_is_active;
    if(new_is_active!=0) printf("Doctor is active");
    else printf("Doctor isn't active");
}
Status Doctor_set_department(Doctor_T d, Department new_department) {
    d->department=new_department;
    printf("Doctor's new department is");
    printf("%d\n", new_department);
    return new_department;
}
Status Doctor_set_name(Doctor_T d, const char* new_name) {
    strcpy(d->name, new_name);
    printf("Doctor's new name is");
    printf("%s\n", new_name);
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
