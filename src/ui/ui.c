#include <stdio.h>
#define Data  void*
#define User  void*
void ui_patient_menu(Data data, User user);
void ui_doctor_menu(Data data, User user);
void ui_manager_menu(Data data, User user);
//患者
void patient_query_records(Data data, User user);
void patient_register(Data data, User user);
//医生
void doctor_query_records(Data data, User user);
//管理员
void manager_add_record(Data data, User user);
void manager_del_record(Data data, User user);
void manager_update_record(Data data, User user);
void manager_query_by_dept(Data data, User user);
void manager_query_doctor(Data data, User user);
void manager_query_patient(Data data, User user);
void manager_stat_income(Data data, User user);
void manager_query_by_time(Data data, User user);
void manager_save_to_file(Data data, User user);
//登录
void ui_login(Data data, User user)
{
    long long id;
    int role;

    printf("=========================================\n");
    printf("        医院诊疗管理系统（登录）\n");
    printf("=========================================\n");
    printf("请输入账号ID：");
    scanf("%20d", &id);
    printf("请选择身份：\n");
    printf("1 管理员\n");
    printf("2 医生\n");
    printf("3 患者\n");
    printf("选择：");
    scanf("%d", &role);

    if (role == 1)
        ui_manager_menu(data, user);
    else if (role == 2)
        ui_doctor_menu(data, user);
    else if (role == 3)
        ui_patient_menu(data, user);
    else
        printf("输入错误！\n");
}
//患者
void ui_patient_menu(Data data, User user)
{
    int op;
    while (1)
    {
        printf("\n=========================================\n");
        printf("               患者菜单\n");
        printf("=========================================\n");
        printf("1 查询本人诊疗记录\n");
        printf("2 挂号\n");
        printf("0 返回登录\n");
        printf("=========================================\n");
        printf("请选择：");
        scanf("%d", &op);

        if (op == 1)
            patient_query_records(data, user);
        else if (op == 2)
            patient_register(data, user);
        else if (op == 0)
            break;
    }
}
//医生
void ui_doctor_menu(Data data, User user)
{
    int op;
    while (1)
    {
        printf("\n=========================================\n");
        printf("               医生菜单\n");
        printf("=========================================\n");
        printf("1 查询本人诊疗记录\n");
        printf("0 返回登录\n");
        printf("=========================================\n");
        printf("请选择：");
        scanf("%d", &op);

        if (op == 1)
            doctor_query_records(data, user);
        else if (op == 0)
            break;
    }
}
//管理员
void ui_manager_menu(Data data, User user)
{
    int op;
    while (1)
    {
        printf("\n=========================================\n");
        printf("              管理员菜单\n");
        printf("=========================================\n");
        printf("1 增加诊疗记录\n");
        printf("2 删除诊疗记录\n");
        printf("3 修改诊疗记录\n");
        printf("4 按科室查询诊疗信息\n");
        printf("5 按医生工号查询诊疗信息\n");
        printf("6 按患者信息查询诊疗信息\n");
        printf("7 统计医院营业额 + 住院患者报表\n");
        printf("8 按时间段查询所有诊疗信息\n");
        printf("9 保存所有信息到文件\n");
        printf("0 返回登录\n");
        printf("=========================================\n");
        printf("请选择：");
        scanf("%d", &op);

        if (op == 1)  manager_add_record(data, user);
        else if (op == 2)  manager_del_record(data, user);
        else if (op == 3)  manager_update_record(data, user);
        else if (op == 4)  manager_query_by_dept(data, user);
        else if (op == 5)  manager_query_doctor(data, user);
        else if (op == 6)  manager_query_patient(data, user);
        else if (op == 7)  manager_stat_income(data, user);
        else if (op == 8)  manager_query_by_time(data, user);
        else if (op == 9) manager_save_to_file(data, user);
        else if (op == 0)  break;
    }
}
void patient_query_records(Data data, User user)
{
    printf("\n→ 查询本人全部诊疗记录\n");
}
void patient_register(Data data, User user)
{
    printf("\n→ 挂号：选择科室 → 选择医生\n");
}
void doctor_query_records(Data data, User user)
{
    printf("\n→ 查询本人诊疗记录\n");
}
void manager_add_record(Data data, User user)
{ printf("\n→ 增加诊疗记录\n"); }
void manager_del_record(Data data, User user)
{ printf("\n→ 删除诊疗记录\n"); }
void manager_update_record(Data data, User user)
{ printf("\n→ 修改诊疗记录\n"); }
void manager_query_by_dept(Data data, User user)
{ printf("\n→ 按科室查询诊疗信息\n");}
void manager_query_doctor(Data data, User user)
{ printf("\n→ 按医生工号查询诊疗信息\n");}
void manager_query_patient(Data data, User user)
{ printf("\n→ 按患者信息查询诊疗信息\n");}
void manager_stat_income(Data data, User user)
{ printf("\n→ 统计医院营业额 + 住院患者报表\n");}
void manager_query_by_time(Data data, User user)
{ printf("\n→ 按时间段查询所有诊疗信息\n");}
void manager_save_to_file(Data data, User user)
{ printf("\n→ 保存所有信息到文件\n");}