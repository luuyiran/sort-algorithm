# 外部排序(`C`语言实现)


一个可排序任意数据格式的外排程序。

用户只需要编写自己的序列化和反序列化函数。


## 用法

```c
    #include "external_sort.h"
    SORT_CONFIG config;
    external_sort(&config);
```

配置结构体
```c
typedef struct SORT_CONFIG {
    const char  *inputfile;    // 输入文件名
    const char  *outputfile;   // 输出文件名
    CMPFUNC     cmp;           // 元素比较函数
    IO_FUNC     read_from;     // 读取函数，每次读一个元素
    IO_FUNC     write_to;      // 写函数，每次写入一个元素
    size_t      es;            // sizeof(元素类型)
    size_t      mem;           // 排序内存
} SORT_CONFIG;
```

 * `cmp`：供内部`qsort`和出堆入堆用.
 * `read_from`：每次调用将解析出外部文件的一个元素到内存.
 * `write_to`：每次调用将写入结果文件一个元素，写入方式自定义.
 * `es`：每个元素内存占用的字节数.
 * `mem`：可用内存，根据此配置可得出一次最多装进内存的元素个数(`mem / es`).

具体配置见 [example.c](example.c).

`example.c` 测试排序外部`213`个元素，内存限制为`16 * sizeof(size_t)`字节，即每次限制读入`16`个元素，读入后立即排序，排序结果刷到一个临时文件，直到把外部文件遍历结束。最后归并这些临时文件。

程序开始时，数字`0, 1, 2,.., 212`随机分布在`input.txt`文件，程序结束时，排序结果放在`result.txt`中，通过观察`result.txt`验证排序效果。














