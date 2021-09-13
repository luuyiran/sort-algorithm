#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "external_sort.h"

/* 外部文件元素个数 */
#define ARRAY_SIZE      213ul                 

/* 可用内存，每次最多读取和排序 16 个 size_t 类型 */
/* 排序结果写入临时文件 */
#define BYTES_USE       sizeof(size_t)*16ul   


static int cmp_nums(const void *a, const void *b) {
   return ( *(size_t *)a - *(size_t *)b );
}

/* 向输出文件输出一个元素 */
static void write_to_file(FILE *fp, void *buff) {
    static int count = 0;
    if (fprintf(fp, "%lu\t", *((size_t *)buff)) < 0) {
        printf("%s error!\n", __func__);
    } else  {
        count++;
        if (count % 16 == 0)
            fprintf(fp, "\n\r");
    }
    fflush(fp);
}

/* 读取一个元素到内存 buf */
static void read_from_file(FILE *fp, void *buf) {
    if (fscanf(fp, "%lu", (size_t *)buf) < 0) {
        if (feof(fp))
            printf("all data have been read!\n");
        else if (ferror(fp))
            printf("fread error!\n");
    }
    //else  
        //printf("read %lu\n", *(size_t *)buf);
}

static void gen_input_txt(const char *file) {
    FILE *fp;
    size_t i, x, y, tmp, *nums;

    fp = fopen(file, "w");
    nums = (size_t *)malloc(ARRAY_SIZE * sizeof(size_t));
    assert(fp);
    assert(nums);

    for (i = 0; i < ARRAY_SIZE; i++)
        nums[i] = i + 1;

    srand(time(NULL));
    for (i = 0; i < ARRAY_SIZE; i++) {
        x = rand() % ARRAY_SIZE;
        y = rand() % ARRAY_SIZE;
        tmp = nums[x];
        nums[x] = nums[y];
        nums[y] = tmp;
    }

    for (i = 0; i < ARRAY_SIZE; i++) {
        if (i && i % 16 == 0)
            fprintf(fp, "\n\r");
        fprintf(fp, "%lu\t", nums[i]);
    }

    fflush(fp);
    free(nums);
    fclose(fp);
}

int main() {

    SORT_CONFIG config;
    const char *input_file = "input.txt";
    const char *output_file = "result.txt";

    gen_input_txt(input_file);

    config.es = sizeof(size_t);
    config.mem = BYTES_USE;
    config.cmp = cmp_nums;
    config.inputfile = input_file;
    config.outputfile = output_file;
    config.read_from = read_from_file;
    config.write_to = write_to_file;

    external_sort(&config);

    return 0 ;
}
