#ifndef HIS_BASE_H
#define HIS_BASE_H




// ---------------- 可能使用的库 ---------------- //
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <stdarg.h>
// ---------------- 可能使用的库 ---------------- //




// ---------------- 状态码 ---------------- //
typedef enum {
    // 基础状态
    HIS_OK = 0,                     // 正常运行
    HIS_ERR_NO_MEM = -1,            // 内存耗尽
    HIS_ERR_INVALID_ARG = -2,       // 传入参数非法

    // 通用搜索错误
    HIS_ERR_NOT_FOUND = -10,        // 成员不存在
    HIS_ERR_ALREADY_EXISTS = -11,   // 成员已存在（防止反复创建）

    // Patient/Doctor 业务相关
    HIS_ERR_INVALID_ID = -20,       // ID 非法

    // Ward (病房) 业务相关
    HIS_ERR_WARD_FULL = -30,            // 病房已满
    HIS_ERR_BED_OCCUPIED = -31,         // 病床被占用
    HIS_ERR_PATIENT_NOT_IN_WARD = -32,  // 病人不在此病房

    // Medicine (药品) 业务相关
    HIS_ERR_OUT_OF_STOCK = -40,     // 药品无库存
    HIS_ERR_INVALID_DOSAGE = -41,   // 剂量非法

    // Fund (财务) 业务相关
    HIS_ERR_INSUFFICIENT_FUNDS = -50,   // 余额不足
    HIS_ERR_PAYMENT_REQUIRED = -51,     // 未付款
    HIS_ERR_INVALID_PAYMENT = -52,
    HIS_ERR_NO_FUNDS = -53,             // 没有账户

    // 系统级/IO 错误
    HIS_ERR_IO_FAILURE = -60,               // io时发生错误
    HIS_ERR_FILE_CORRUPT = -61,             // 文件损坏或格式不正确

    // 服务层级错误
    HIS_ERR_INSUFFICIENT_PERMISSION = -70,  // 当前账户权限不足
    HIS_ERR_NO_USER = -71,

    //Account（账号）错误
    HIS_ERR_PASSWORD_MISMATCH = -72,
    HIS_ERR_QUEUE_FULL = -73,               // 挂号队列已满

    //Record（记录）错误
    HIS_ERR_STATUS_ERROR = -80,
    HIS_ERR_INVALID_STATUS = -81,

} Status;
// ---------------- 状态码 ---------------- //




// ---------------- 科室相关 ---------------- //
// 科室枚举定义
typedef enum {
    DEP_NONE = 0,         // 不属于任何科室
    DEP_INTERNAL,         // 内科
    DEP_SURGERY,          // 外科
    DEP_PEDIATRICS,       // 儿科
    DEP_GYNECOLOGY,       // 妇产科
    DEP_OPHTHALMOLOGY,    // 眼科
    DEP_DENTAL,           // 口腔科
    DEP_DERMATOLOGY,      // 皮肤科
    DEP_EMERGENCY,        // 急诊科
    DEP_RADIOLOGY,        // 放射科
    DEP_LABORATORY,       // 检验科
    DEP_PHARMACY,         // 药剂科
    DEP_COUNT             // 枚举计数器
}Department;

// 科室与文字的转换
static const char* department_names[12] = {
    "内科", "外科", "儿科", "妇产科", "眼科",
    "口腔科", "皮肤科", "急诊科", "放射科", "检验科",
    "药剂科", "未知科室"
};

/**
 * @brief 将枚举转为对应的字符串，用于io显示
 * @param department 输入科室枚举
 * @return 返回对应的字符串名称，若不合法则返回“未知科室”
 */
static inline const char* department_name(Department department) {
    if (department < 0 || department >= DEP_COUNT)
        return department_names[DEP_COUNT];
    return department_names[department];
}
// ---------------- 科室相关 ---------------- //




// ---------------- 错误处理 ---------------- //
// 硬断言：抓Bug
/**
 * @brief 硬断言，用于针对预期外的错误
 * @param condition 应当出现的情况
 * @param message 未按预期时的报错消息
 */
#define ASSERT(condition, message)                                                                                     \
    do {                                                                                                               \
        if(! (condition)) {                                                                                            \
            fprintf(stderr, "\n\033[1;31m[Runtime Error]\033[0m\n");                                                   \
            fprintf(stderr, "Condition: %s\n", #condition);                                                            \
            fprintf(stderr, "Message:   %s\n", message);                                                               \
            fprintf(stderr, "Location:  %s (%s:%d)\n", __func__, __FILE__, __LINE__);                                  \
            abort();                                                                                                   \
        }                                                                                                              \
    } while(0)

// 安全内存分配
/**
 * @brief 对空间检查的malloc
 * @param size 字节大小
 * @return 返回内存空间
 */
static inline void *safe_malloc(size_t size) {
    void *p = malloc(size);
    ASSERT(p != NULL, "内存耗尽，程序强制停止");
    return p;
}
// ---------------- 错误处理 ---------------- //




// ---------------- 工具 ---------------- //


// 性别类型
typedef enum{
    FEMALE = 0,
    MALE = 1
}gender;


// 通用比较器声明
/**
 * @note 对于此比较器：
 * 若a应当排列在b的后面，则返回一个正数；
 * 若二者相等，则返回0；
 * 否则，返回一个负数
 */
typedef int (*compare)(const void *a, const void *b);


// 以下为对于实体的通用内部id工具
#define INVALID_ID (-1LL)
/**
 * @brief id生成器，基于当前静态变量 id_counter
 * 若已经达到上限，会返回-1
 */
#define NEW_ID() \
    ((_id_counter < LLONG_MAX)? (_id_counter++): INVALID_ID)

/**
 * @brief 加载实体时，需要使用此函数用于记录当前最大id
 * @param id 当前加载的id
 */
#define LOAD_ID(id) \
    do{ \
        if(id > _id_counter) _id_counter = id; \
    }while(0)


/**
 * @brief 密码加密与解密，原理为循环 XOR 混淆
 * @param data 需要混淆的数据（明文密码）
 * @param data_len 数据长度 (通常是 strlen(data))
 */
static inline void cipher_xor_cyclic(char* data, size_t data_len) {
    const char* key = "HIS_XOR";
    size_t key_len = strlen(key);
    for (size_t i = 0; i < data_len; i++) {
        data[i] ^= key[i % key_len];
    }
}


// 终端清空指令
#ifdef __unix__
#define CLEAN() system("clear")
#elif defined(_WIN32) || defined(_WIN64)
#define CLEAN() system("cls")
#endif


// 暂停指令，单位统一为 ms
#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif


// ui需要用到的交互函数包
#define load_ui_tools                                               \
static void clear_space() {                                         \
    int c;                                                          \
    while ((c = getchar()) != '\n' && c != EOF) ;                   \
}                                                                   \
static void get_input_str(const char* prompt, char* dest, int max_len) { \
    while (1) {                                                     \
        printf("%s (限%d字符): ", prompt, max_len - 1);              \
        if (fgets(dest, max_len, stdin) == NULL) continue;          \
                                                                    \
        size_t len = strlen(dest);                                  \
        if (len > 0 && dest[len - 1] != '\n') {                     \
            printf("错误：输入过长！请重新输入（最多%d字符）。\n", max_len - 1); \
            clear_space();                                          \
            continue;                                               \
        }                                                           \
        if (len > 0 && dest[len - 1] == '\n') dest[len - 1] = '\0'; \
                                                                    \
        if (strchr(dest, '|')) {                                    \
            printf("错误：输入包含非法字符 '|'，请重新输入。\n");      \
            continue;                                               \
        }                                                           \
        if (strlen(dest) == 0) {                                    \
            printf("错误：内容不能为空，请重新输入。\n");              \
            continue;                                               \
        }                                                           \
                                                                    \
        break;                                                      \
    }                                                               \
}                                                                   \
static long long get_input_long_long(const char* prompt,            \
    long long min, long long max) {                                 \
    long long val;                                                  \
    while (1) {                                                     \
        printf("%s (%lld-%lld): ", prompt, min, max);               \
        if (scanf("%lld", &val) != 1 || val < min || val > max) {   \
            printf("请输入位于区间[%lld, %lld]的合法数字\n", min, max);  \
            clear_space();                                          \
            continue;                                               \
        }                                                           \
        clear_space();                                              \
        return val;                                                 \
    }                                                               \
}
// ---------------- 工具 ---------------- //




#endif
