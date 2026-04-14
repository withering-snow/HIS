#include <account.h>

struct Account_T{
    AccountClass    class;
    long long       actor_id;
    char            name[32];
    char            password[32];
};
