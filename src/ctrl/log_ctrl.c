#include <log_ctrl.h>







static FILE* __the_log = NULL;

#ifdef __WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif




void Log_open(){
    if (__the_log==NULL)
    {
        char dir[32];
        snprintf(dir, sizeof(dir), "data%cHIS.log", SEP);
        __the_log = fopen(dir, "a");
    }
}

void Log_close(){
    if (__the_log != NULL)
    {
        fclose(__the_log);
        __the_log = NULL;}
}

void Log_printf(AccountClass class, long long id, const char* format, ...) {
    if (__the_log == NULL) return;

    // 1. 打印时间戳
    fprintf(__the_log, "[%s] ", Time_to_string(Time_now()));

    // 2. 打印账户类别
    switch (class) {
    case CLASS_NO_USER: fprintf(__the_log, "[PUBLIC] "); break;
    case CLASS_PATIENT: fprintf(__the_log, "[PATIENT] "); break;
    case CLASS_DOCTOR:  fprintf(__the_log, "[DOCTOR] ");  break;
    case CLASS_ROOT:    fprintf(__the_log, "[ROOT] ");    break;
    default:            fprintf(__the_log, "[UNKNOWN] "); break;
    }

    // 3. 打印 ID
    fprintf(__the_log, "[ID:%lld] ", id);

    // 4. 处理变长参数并直接写入文件 (无需中间 buffer)
    va_list args;
    va_start(args, format);
    vfprintf(__the_log, format, args); // 核心：直接转发到文件
    va_end(args);

    // 5. 打印换行并刷新
    fprintf(__the_log, "\n");
    fflush(__the_log);
}