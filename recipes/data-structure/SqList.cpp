#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE     1
#define FALSE    0
#define OK       1
#define ERROR    0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

typedef struct
{
    ElemType* elems;
    int length;
    int capacity;
    int increment;
} SqList;

Status InitSqList(SqList& L, int capacity, int increment)
{
    L.elems = (ElemType*)malloc(capacity * sizeof(ElemType));
    if (NULL == L.elems)
        return OVERFLOW;
    L.length = 0;
    L.capacity = capacity;
    L.increment = increment;

    return OK;
}

Status DestorySqList(SqList& L)
{
    free(L.elems);
    L.elems = NULL;

    return OK;
}

Status SqListIsEmpty(SqList& L)
{
    return L.length == 0;
}

int SqListLength(SqList& L)
{
    return L.length;
}

Status SqListGetElemByIndex(SqList& L, int index, ElemType& e)
{
    if (index >= L.length || index < 0)
        return ERROR;

    e = L.elems[index];

    return OK;
}

int SqListSearch(SqList& L, ElemType e) // 在顺序表中查找元素e, 成功是返回改元素在表中第一次出现的位置，否则返回-1
{
    for (int i = 0; i < L.length; ++i)
    {
        if (e == L.elems[i])
            return i;
    }

    return -1;
}

Status visit(ElemType& e) // 遍历调用
{
    printf("%d\t", e);
    return OK;
}

Status SqListTraverse(SqList& L, Status (*visit)(ElemType& e)) // 遍历顺序表L, 依次对每个元素调用函数visit()
{
    if (0 == L.length)
        return ERROR;

    for (int i = 0; i < L.length; ++i)
    {
        visit(L.elems[i]);
    }

    return OK;
}

Status SqListAssign(SqList& L, int index, ElemType e) // 将顺序列表index位置（从0开始）的元素用e给其赋值
{
    if (index >= L.length || index < 0)
        return ERROR;

    L.elems[index] = e;

    return OK;
}

Status SqListAppend(SqList& L, ElemType e) // 在顺序表末尾添加元素
{
    ElemType* new_base;
    if (L.length >= L.capacity)
    {
        new_base = (ElemType*)realloc(L.elems, L.capacity * L.length * sizeof(ElemType));
        if (!new_base)
            return OVERFLOW;
        L.elems = new_base;
        L.capacity = L.capacity * L.length;
    }

    L.elems[L.length] = e;
    ++L.length;

    return OK;
}

Status SqListDeleteLast(SqList& L, ElemType& e) // 删除书序表末尾元素，并用参数e返回其值
{
    if (L.length == 0)
        return ERROR;

    e = L.elems[L.length - 1];
    --L.length;

    return OK;
}

int main()
{
#define LONGTH        5
#define TEST_DATA_LEN 10

    SqList L; // 创建顺序表

    int capacity = LONGTH;
    int increment = LONGTH;

    ElemType e_array[TEST_DATA_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    printf("--- [顺序表] ---\n");
    printf("\n表L的capacity为：%d\n表L的increment为：%d\n", capacity, increment);
    printf("\n待测试元素为：\n");
    for (int i = 0; i < TEST_DATA_LEN; ++i)
    {
        printf("%d\t", e_array[i]);
    }

    if (!InitSqList(L, capacity, increment))
    {
        printf("初始化顺序表失败\n");
        exit(0);
    }
    printf("\n\n已初始化顺序表\n");

    if (TRUE == SqListIsEmpty(L))
        printf("\n此表为空\n");
    else
    {
        printf("\n此表不为空\n");
    }

    printf("\n将待测元素入表：\n");
    for (int i = 0; i < TEST_DATA_LEN; ++i)
    {
        if (ERROR == SqListAppend(L, e_array[i]))
            printf("入表失败，index：%d\n", i);
    }
    printf("入表成功\n");

    printf("\n此时表内元素：\n");
    SqListTraverse(L, visit);

    ElemType e;
    printf("\n\n将元素出表到e\n");
    if (ERROR == SqListDeleteLast(L, e))
        printf("出表失败\n");
    else
        printf("出表成功，元素为%d\n", e);

    printf("\n此时表内元素：\n");
    SqListTraverse(L, visit);

    printf("\n\n销毁顺序表\n");
    if (OK == DestorySqList(L))
        printf("销毁成功\n");
    else
        printf("销毁失败");

    return 0;
}