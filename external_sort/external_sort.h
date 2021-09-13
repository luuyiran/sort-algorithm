#ifndef __EXTERNAL_SORT_H__
#define __EXTERNAL_SORT_H__
#include <stddef.h>

typedef void (*IO_FUNC)(FILE *, void *);
typedef int (*CMPFUNC)(const void *, const void *);

typedef struct SORT_CONFIG {
    const char  *inputfile;    // 输入文件名
    const char  *outputfile;   // 输出文件名
    CMPFUNC     cmp;           // 元素比较函数
    IO_FUNC     read_from;     // 读取函数，每次读一个元素
    IO_FUNC     write_to;      // 写函数，每次写入一个元素
    size_t      es;            // sizeof(元素类型)
    size_t      mem;           // 排序内存容量 bytes
} SORT_CONFIG;

void external_sort(SORT_CONFIG *config);

#endif // !__EXTERNAL_SORT_H__
