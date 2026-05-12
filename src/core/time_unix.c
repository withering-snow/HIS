#include <time_unix.h>

long long Time_now(void) {
    return (long long)time(NULL);
}

const char* Time_to_string(long long time_stamp) {
    static char buf[64];
    time_t t = (time_t)time_stamp;
    struct tm *tm_tar = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_tar);
    return buf;
}

const char * Time_to_string_date(long long time_stamp){
    static char buf[64];
    time_t t = (time_t)time_stamp;
    struct tm *tm_tar = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_tar);
    return buf;
}

int Time_to_int_date(long long ts) {
    time_t raw_time = (time_t)ts;
    struct tm *t = localtime(&raw_time);
    if (!t) return 0;
    return (t->tm_year + 1900) * 10000 + (t->tm_mon + 1) * 100 + t->tm_mday;
}

int Time_CalculateAge(long long birth_ts) {
    if (birth_ts <= 0) return 0;

    time_t now = time(NULL);
    struct tm *l_now = localtime(&now);

    int now_y = l_now->tm_year + 1900;
    int now_m = l_now->tm_mon + 1;
    int now_d = l_now->tm_mday;

    time_t b_time = (time_t)birth_ts;
    struct tm *l_birth = localtime(&b_time);

    int birth_y = l_birth->tm_year + 1900;
    int birth_m = l_birth->tm_mon + 1;
    int birth_d = l_birth->tm_mday;

    int age = now_y - birth_y;
    if (now_m < birth_m ||
        (now_m == birth_m && now_d < birth_d) )
        age--;

    return (age < 0) ? 0 : age;
}
