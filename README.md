### 排序算法(`C`语言实现)

本项目实现并讨论经典排序算法：

- 冒泡排序
- 选择排序
- 插入排序
- 希尔排序
- 归并排序
- 堆排序
- 快速排序

使用标准`C`语言(`ANSI C99`).

为保证较好的泛化性，所有算法接口均和`C`标准库`qsort`保持一致：
```c
#include <stdlib.h>
void qsort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
```


最后统一测试排序性能，并选取`NetBSD`和`GNUlibc`的`qsort`作为比对标杆.

具体测试细节见[main.c](main.c)


测试结果:

```c
sorted nums:
insert_sort     0.0002620 seconds
insert_sort1    0.0002850 seconds
shell_sort      0.0004630 seconds
glibc_qsort     0.0022890 seconds
bsd_qsort       0.0037590 seconds
quick_sort      0.0046900 seconds
merge_sort      0.0060380 seconds
heap_sort       0.0175540 seconds
rbtree_sort     0.0218260 seconds
bubble_sort     6.5500369 seconds
libc_qsort      7.0386510 seconds
selec_sort     10.4721127 seconds

repeated nums:
bsd_qsort       0.0025920 seconds
glibc_qsort     0.0064080 seconds
rbtree_sort     0.0083530 seconds
merge_sort      0.0107530 seconds
heap_sort       0.0175960 seconds
libc_qsort      0.1748680 seconds
quick_sort      0.1834110 seconds
insert_sort     4.1536961 seconds
selec_sort     10.3563623 seconds
shell_sort     13.7435598 seconds
insert_sort1   16.6709728 seconds
bubble_sort    33.5384026 seconds

random nums:
glibc_qsort     0.0082520 seconds
bsd_qsort       0.0095740 seconds
quick_sort      0.0118970 seconds
libc_qsort      0.0122200 seconds
merge_sort      0.0142770 seconds
heap_sort       0.0238880 seconds
rbtree_sort     0.0253140 seconds
insert_sort     5.1789789 seconds
selec_sort     10.4028893 seconds
shell_sort     14.1937981 seconds
insert_sort1   18.2543259 seconds
bubble_sort    34.4931335 seconds
```