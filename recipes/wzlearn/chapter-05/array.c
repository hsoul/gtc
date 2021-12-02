#include "array.h"
#include <stdbool.h> // 布尔类型的宏
#include <string.h>  // 字符串处理

array* create_array()
{
    array* array = NULL;
    array = (struct array*)malloc(sizeof(*array));

    if (NULL == array)
        return NULL;

    array->capacity = 0;
    array->len = 0;
    array->type_size = 0;

    array->dup = NULL;
    array->free = NULL;
    array->match = NULL;
    array->dump = NULL;

    array->data = NULL;

    return array;
}

int init_array(array* array, int size, int type_size)
{
    if (array == NULL || type_size <= 0 || size < 0)
        return -1;

    void* p = calloc(1, size * type_size);
    if (p == NULL)
        return -2;

    array->data = p;
    array->len = 0;
    array->capacity = size;
    array->type_size = type_size;

    return 0;
}

int array_insert(array* array, int index, void* const value)
{
    if (array == NULL)
        return -1;

    if (array->len >= array->capacity)
        return -2;

    if (index > array->len || index < 0)
        return -3;

    char* begin = (char*)array->data;

    for (size_t i = array->len; i > index; --i) // 从最后一个元素到pos位置元素均向右移一个单位
    {
        char* new_pos = begin + i * array->type_size;
        char* cur_pos = begin + (i - 1) * array->type_size;

        if (array->dup != NULL)
            array->dup(new_pos, cur_pos);
        else
            memcpy(new_pos, cur_pos, array->type_size);
    }

    char* insert_pos = begin + index * array->type_size;

    if (array->dup != NULL)
        array->dup(insert_pos, value);
    else
        memcpy(insert_pos, value, array->type_size);

    ++array->len;

    return 0;
}

size_t array_search_value(array* array, void* const value)
{
    if (NULL == array)
        return -1;

    char* begin = (char*)array->data;

    size_t i = 0;
    for (; i < array->len; ++i)
    {
        int cmp_ret = 0;

        if (NULL != array->match)
            cmp_ret = array->match(begin + i * array->type_size, value);
        else
            cmp_ret = memcmp(begin + i * array->type_size, value, array->type_size);

        if (cmp_ret == 0)
            break;
    }

    return i;
}

void* array_index(array* array, size_t index)
{
    if (NULL == array)
        return NULL;

    if (index < 0 || index >= array->len)
        return NULL;

    return (char*)array->data + index * array->type_size;
}

int array_modify(array* array, size_t index, void* const value)
{
    if (NULL == array)
        return -1;

    if (index < 0 || index >= array->len)
        return -2;

    char* assign_pos = (char*)array->data + index * array->type_size;

    if (array->dup != NULL)
        array->dup(assign_pos, value);
    else
        memcpy(assign_pos, value, array->type_size);

    return 0;
}

size_t array_len(array* array)
{
    if (NULL == array)
        return -1;

    return array->len;
}

size_t array_capacity(array* array)
{
    if (NULL == array)
        return -1;

    return array->capacity;
}

void array_destory(array* array)
{
    if (NULL == array)
        return;

    free(array->data);
    array->data = NULL;
    free(array);
    array = NULL;
}

void array_del_value(array* array, void* value)
{
    if (NULL == array)
        return;

    char* begin = (char*)array->data;
    void* find_obj = NULL;
    bool is_find = false;

    for (size_t i = 0; i < array->len; ++i)
    {
        if (!is_find)
        {
            int cmp_ret = 0;

            if (NULL != array->match)
                cmp_ret = array->match(begin + i * array->type_size, value);
            else
                cmp_ret = memcmp(begin + i * array->type_size, value, array->type_size);

            if (cmp_ret == 0)
            {
                find_obj = begin + i * array->type_size;
                continue;
            }
        }
        else
        {
            char* pre_pos = begin + (i - 1) * array->type_size;
            char* cur_pos = begin + i * array->type_size;

            if (NULL != array->dup)
                array->dup(pre_pos, cur_pos);
            else
                memcpy(pre_pos, cur_pos, array->type_size);
        }
    }

    if (is_find)
    {
        if (array->free != NULL)
            array->free(find_obj);

        --array->len;
    }
}

void array_del_index(array* array, size_t index)
{
    if (NULL == array)
        return;

    if (index < 0 || index >= array->len)
        return;

    char* begin = (char*)array->data;
    void* delete_obj = begin + index * array->type_size;

    if (array->free != NULL)
        array->free(delete_obj);

    for (size_t i = index + 1; i < array->len; ++i)
    {
        char* pre_pos = begin + (i - 1) * array->type_size;
        char* cur_pos = begin + i * array->type_size;

        if (NULL != array->dup)
            array->dup(pre_pos, cur_pos);
        else
            memcpy(pre_pos, cur_pos, array->type_size);
    }

    --array->len;
}

void array_dump(array* array)
{
    if (NULL == array)
        return;

    printf("array len %ld\n", array->len);

    for (size_t i = 0; i < array->len; ++i)
    {
        if (array->dump)
            array->dump((char*)array->data + i * array->type_size);
    }
}