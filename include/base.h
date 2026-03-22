#ifndef HIS_BASE_H
#define HIS_BASE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// 1. 状态码定义
typedef enum { HIS_OK = 0, HIS_ERR_NO_MEM = -1, HIS_ERR_INVALID_ARG = -2 } Status;

// 2. 硬断言：抓程序员的 Bug
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

// 3. 安全内存分配
static inline void *safe_malloc(size_t size) {
    void *p = malloc(size);
    ASSERT(p != NULL, "内存耗尽，程序强制停止");
    return p;
}

// 4. 通用比较函数指针
typedef int (*compare)(const void *a, const void *b);

#endif
