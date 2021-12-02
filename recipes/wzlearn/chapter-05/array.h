#pragma once

#include <stdio.h>  // 输入 / 输出
#include <stdlib.h> // 基础工具：内存管理、程序工具、字符串转换、随机数、算法

#define ARRAY_SET_DUP_METHOD(a, m)   ((a)->dup = (m))
#define ARRAY_SET_FREE_METHOD(a, m)  ((a)->free = (m))
#define ARRAY_SET_MATCH_METHOD(a, m) ((a)->match = (m))
#define ARRAY_SET_DUMP_METHOD(a, m)  ((a)->dump = (m))

#define ARRAY_GET_DUP_METHOD(a)      ((a)->dup)
#define ARRAY_GET_FREE_METHOD(a)     ((a)->free)
#define ARRAY_GET_MATCH_METHOD(a)    ((a)->match)

typedef struct array
{
    size_t capacity;                    // data 指针指向的空间大小
    size_t len;                         // data 当前元素数量
    size_t type_size;                   // 每个元素所占空间大小
    void (*dup)(void* ptr, void* key);  // 元素复制函数
    void (*free)(void* ptr);            // 元素释放函数
    int (*match)(void* ptr, void* key); // 元素比较函数
    void (*dump)(void* ptr);            // 输出元素
    void* data;                         // 指向存储元素内存的指针
} array;

array* create_array();
int init_array(array* array, int size, int type_size);

int array_insert(array* array, int index, void* const value);
size_t array_search_value(array* array, void* const value);
void* array_index(array* array, size_t index);
int array_modify(array* array, size_t index, void* const value);

size_t array_len(array* array);
size_t array_capacity(array* array);

void array_destory(array* array);
void array_del_value(array* array, void* value);
void array_del_index(array* array, size_t index);

void array_dump(array* array);