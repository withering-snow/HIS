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
// ---------------- 可能使用的库 ---------------- //




// ---------------- 状态码 ---------------- //
typedef enum {
    // 基础状态
    HIS_OK = 0,
    HIS_ERR_NO_MEM = -1,
    HIS_ERR_INVALID_ARG = -2,

    // 通用搜索错误
    HIS_ERR_NOT_FOUND = -10,
    HIS_ERR_ALREADY_EXISTS = -11,

    // Patient/Doctor 业务相关
    HIS_ERR_INVALID_ID = -20,

    // Ward (病房) 业务相关
    HIS_ERR_WARD_FULL = -30,
    HIS_ERR_BED_OCCUPIED = -31,
    HIS_ERR_PATIENT_NOT_IN_WARD = -32,

    // Medicine (药品) 业务相关
    HIS_ERR_OUT_OF_STOCK = -40,
    HIS_ERR_INVALID_DOSAGE = -41,

    // Fund (财务) 业务相关
    HIS_ERR_INSUFFICIENT_FUNDS = -50,
    HIS_ERR_PAYMENT_REQUIRED = -51,

    // 系统级/IO 错误
    HIS_ERR_IO_FAILURE = -60,
    HIS_ERR_FILE_CORRUPT = -61
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
/**
 * @brief id生成器，基于当前静态变量 id_counter
 * 若已经达到上限，会返回-1
 */
#define NEW_ID() \
    ((_id_counter < LLONG_MAX)? (_id_counter++): -1)

/**
 * @brief 加载实体时，需要使用此函数用于记录当前最大id
 * @param id 当前加载的id
 */
#define LOAD_ID(id) \
    do{ \
        if(id > _id_counter) _id_counter = id; \
    while(0);
// ---------------- 工具 ---------------- //




#endif
