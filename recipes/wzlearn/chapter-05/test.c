#include "array.h"

// array* create_array();
// int init_array(array* array, int size, int type_size);

// int array_insert(array* array, int index, void* const value);
// size_t array_search_value(array* array, void* const value);
// void* array_index(array* array, size_t index);
// int array_modify(array* array, size_t index, void* const value);

// size_t array_len(array* array);
// size_t array_capacity(array* array);

// void array_destory(array* array);
// void array_del_value(array* array, void* value);
// void array_del_index(array* array, size_t index);

void dump(void* obj)
{
    printf("%d\n", *((int*)obj));
}

int main()
{
    array* a = create_array();
    if (NULL == a)
        printf("create array faild.");

    ARRAY_SET_DUMP_METHOD(a, dump);

    int ret = -1;
    if ((ret = init_array(a, 10, 4)) != 0)
        printf("int array faild error code %d.", ret);

    for (int i = 0; i < 5; ++i)
    {
        array_insert(a, 0, &i);
    }

    array_dump(a);

    return 0;
}