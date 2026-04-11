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
