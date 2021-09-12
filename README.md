### 排序算法(`C`语言实现)

本项目使用标准`C`语言(`ANSI C99`)实现并测试了以下排序算法：

- 冒泡排序
- 选择排序
- 插入排序
- 希尔排序
- 归并排序
- 堆排序
- 快速排序
- 红黑树排序
- 梳排序

为保证较好的泛化性，所有算法接口均和`C`标准库`qsort`保持一致：
```c
#include <stdlib.h>
void qsort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
```

### 测试

平台：

Linux ubuntu18 x86_64 GNU/Linux

编译运行：

make && ./test

测试用例：
- 10万已排序数组.
- 10万分布在`[1-66]`的随机值，包含大量重复值.
- 10万随机值


最后统一测试排序性能，并选取`NetBSD`和`GNUlibc`的`qsort`作为比对标杆.



3个测试用例的测试输出结果，单位为`秒`.

#           |  random   | repeated    | sorted
------------|---------- | --------- | ---------
`bsd_qsort`   | 0.0112740 | 0.0035230 | 0.0050800
`glibc_qsort` | 0.0114880 | 0.0086520 | 0.0037570
quick_sort  | 0.0174430 | 0.2166480 | 0.0078600
`libc_qsort`  | 0.0179290 | 0.2136230 |`11.9077616`
merge_sort  | 0.0288600 | 0.0241070 | 0.0144490
comb_sort   | 0.0306890 | 0.0162230 | 0.0100700
heap_sort   | 0.0350740 | 0.0303020 | 0.0415440
`rbtree_sort` | 0.0421410 | 0.0128540 | 0.0340810
insert_sort | 11.9048662|11.4431257 | 0.0002800
selec_sort  | 11.9342022|11.2723885 |10.9429922
shell_sort  | 35.6656570|33.9125481 | 0.0005570
insert_sort1| 42.5716438|41.6092415 | 0.0003030
bubble_sort | 55.8134346|52.4694290 |11.8308973



几点说明

1. `bsd_qsort`函数为`NetBSD`C库的`qsort`函数，用来比对测试.
2. `glibc_qsort`函数为当前编译平台`GNUlibc`(libc.so)中的`qsort`函数，用来比对测试.
3. `libc_qsort`函数来源书籍《C标准库》，用来比对测试，该版本每次pivot选取最后一个元素，对于已排序好的数组，每次partition都会有一半为空，性能退化为O(N^2).
4. 红黑树的应用场景不是排序，顺序性只是它其中的一个特性，用来比对测试.



测试代码[main.c](main.c)，终端输出进行了自动排序，可以更方便的观察各算法在不同输入下的表现：

```c
// 输入已经排序后的数组
sorted nums:
insert_sort     0.0002800 seconds
insert_sort1    0.0003030 seconds
shell_sort      0.0005570 seconds
glibc_qsort     0.0037570 seconds
bsd_qsort       0.0050800 seconds
quick_sort      0.0078600 seconds
comb_sort       0.0100700 seconds
merge_sort      0.0144490 seconds
rbtree_sort     0.0340810 seconds
heap_sort       0.0415440 seconds
selec_sort      10.9429922 seconds
bubble_sort     11.8308973 seconds
libc_qsort      11.9077616 seconds
// 输入大量重复的随机值，顺序随机
repeated nums:
bsd_qsort       0.0035230 seconds
glibc_qsort     0.0086520 seconds
rbtree_sort     0.0128540 seconds
comb_sort       0.0162230 seconds
merge_sort      0.0241070 seconds
heap_sort       0.0303020 seconds
libc_qsort      0.2136230 seconds
quick_sort      0.2166480 seconds
selec_sort      11.2723885 seconds
insert_sort     11.4431257 seconds
shell_sort      33.9125481 seconds
insert_sort1    41.6092415 seconds
bubble_sort     52.4694290 seconds
// 输入随机值
random nums:
bsd_qsort       0.0112740 seconds
glibc_qsort     0.0114880 seconds
quick_sort      0.0174430 seconds
libc_qsort      0.0179290 seconds
merge_sort      0.0288600 seconds
comb_sort       0.0306890 seconds
heap_sort       0.0350740 seconds
rbtree_sort     0.0421410 seconds
insert_sort     11.9048662 seconds
selec_sort      11.9342022 seconds
shell_sort      35.6656570 seconds
insert_sort1    42.5716438 seconds
bubble_sort     55.8134346 seconds
```


