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
    ElemType* elements_;
    int top_;       // 栈顶
    int size_;      // 容量
    int increment_; // 扩容增量
} SqStack;

Status InitStack_Sq(SqStack& s, int size, int incremnt)
{
    s.elements_ = (ElemType*)malloc(size * sizeof(ElemType));
    if (NULL == s.elements_)
        return OVERFLOW;
    s.top_ = 0;
    s.size_ = size;
    s.increment_ = incremnt;
    return OK;
}

Status DestoryStack_Sq(SqStack& s)
{
    free(s.elements_);
    s.elements_ = 0;
    return OK;
}

Status Push_Sq(SqStack& s, ElemType e)
{
    ElemType* new_base;
    if (s.top_ >= s.size_)
    {
        new_base = (ElemType*)realloc(s.elements_, (s.size_ + s.increment_) * sizeof(ElemType));
        if (new_base == NULL)
            return OVERFLOW;
        s.elements_ = new_base;
        s.size_ += s.increment_;
    }
    s.elements_[s.top_++] = e;
    return OK;
}

Status GetTop_Sq(SqStack& s, ElemType& e)
{
    if (s.top_ == 0)
        return ERROR;
    e = s.elements_[s.top_ - 1];
    return e;
}

Status Pop_Sq(SqStack& s, ElemType& e)
{
    if (s.top_ == 0)
        return ERROR;
    e = s.elements_[s.top_ - 1];
    s.top_--;
    return e;
}

Status StackEmpty_Sq(SqStack& s)
{
    return s.top_ == 0 ? TRUE : FALSE;
}

void ClearStack_Sq(SqStack& s)
{
    if (s.top_ == 0)
        return;
    // s.size_ = 0;
    s.top_ = 0;
}

#define TEST_LENGTH 5

int main()
{
    SqStack s;

    int size = TEST_LENGTH;
    int increment = TEST_LENGTH;

    ElemType e_array[TEST_LENGTH] = {1, 2, 3, 4, 5};

    printf("---【顺序栈】---\n");
    printf("栈S的size为：%d\n栈的increment为：%d\n", size, increment);
    printf("待测试元素为：\n");

    for (int i = 0; i < TEST_LENGTH; ++i)
    {
        printf("%d\t", e_array[i]);
    }

    printf("\n");

    if (!InitStack_Sq(s, size, increment))
    {
        printf("初始化顺序栈失败\n");
        exit(0);
    }

    for (int i = 0; i < s.size_; i++)
    {
        if (!Push_Sq(s, e_array[i]))
        {
            printf("%d入栈失败\n", e_array[i]);
            exit(0);
        }
    }

    printf("已入栈\n");

    if (StackEmpty_Sq(s))
        printf("s栈为空\n");
    else
        printf("s栈非空");

    printf("栈s的栈顶元素为：\n");

    ElemType e;
    GetTop_Sq(s, e);

    printf("%d\n", e);

    printf("栈s元素出栈为：\n");

    for (int i = 0, e = 0; i < s.size_; ++i)
    {
        Pop_Sq(s, e);
        printf("%d\t", e);
    }

    printf("\n");

    ClearStack_Sq(s);

    printf("已清空栈s\n");

    DestoryStack_Sq(s);

    return 0;
}