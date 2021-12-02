#include <alloca.h>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <limits.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

struct StackNode
{
    int* low_ptr_;
    int* high_ptr_;
    StackNode(int* low_ptr, int* high_ptr)
        : low_ptr_(low_ptr)
        , high_ptr_(high_ptr)
    {
    }
};

void myQsort(int a[], size_t n)
{
    int* base_ptr = a;

    if (n == 0)
        return;

    int* low_ptr = base_ptr;
    int* high_ptr = a + n - 1;

    std::stack<StackNode> stack;
    stack.push({NULL, NULL});

    fmt::print("array {} {} {}\n", std::vector<int>(low_ptr, high_ptr + 1), *low_ptr, *high_ptr);

    static int count = 0;

    while (!stack.empty())
    {
        count++;
        int* left_ptr;
        int* right_ptr;
        int* mid_ptr = low_ptr + ((high_ptr - low_ptr) >> 1);

        if (*mid_ptr < *low_ptr)
            swap(*mid_ptr, *high_ptr);
        if (*high_ptr < *mid_ptr)
            swap(*mid_ptr, *high_ptr);
        else
            goto jump_over;
        if (*mid_ptr < *low_ptr)
            swap(*mid_ptr, *low_ptr);
    jump_over:;

        left_ptr = low_ptr + 1;
        right_ptr = high_ptr - 1;

        fmt::print("mid_ptr_index {} {}\n", mid_ptr - base_ptr, *mid_ptr);
        fmt::print("left_ptr_index {}, value {}\n", left_ptr - base_ptr, *left_ptr);
        fmt::print("right_ptr_index {}, value {}\n", right_ptr - base_ptr, *right_ptr);
        fmt::print("part array {}\n", std::vector<int>(left_ptr, right_ptr + 1));
        fmt::print("array {} {} {}\n", std::vector<int>(low_ptr, high_ptr + 1), *low_ptr, *high_ptr);

        do
        {
            while (*left_ptr < *mid_ptr)
                ++left_ptr;

            while (*mid_ptr < *right_ptr)
                --right_ptr;

            if (left_ptr < right_ptr)
            {
                swap(*left_ptr, *right_ptr);
                if (mid_ptr == left_ptr)
                    mid_ptr = right_ptr;
                else if (mid_ptr == right_ptr)
                    mid_ptr = left_ptr;
                ++left_ptr;
                ++right_ptr;
            }
            else if (left_ptr == right_ptr)
            {
                left_ptr++;  // NT : left_ptr指向了右区第一个元素
                right_ptr++; // NT : right_ptr 执向了左区最后一个元素
                break;
            }
        } while (left_ptr <= right_ptr);

        fmt::print("parted array {} {} {}\n", std::vector<int>(low_ptr + 1, high_ptr), left_ptr - base_ptr, right_ptr - base_ptr);
        fmt::print("count {}\n\n", count);

        if (right_ptr - low_ptr <= 0)
        {
            if (high_ptr - left_ptr <= 0)
            {
                low_ptr = stack.top().low_ptr_;
                high_ptr = stack.top().high_ptr_;
                stack.pop();
            }
            else
            {
                low_ptr = left_ptr;
            }
        }
        else if (high_ptr - left_ptr <= 0)
            high_ptr = right_ptr;
        else if (right_ptr - low_ptr > high_ptr - left_ptr)
        {
            stack.push({low_ptr, right_ptr});
            low_ptr = left_ptr;
        }
        else
        {
            stack.push({left_ptr, high_ptr});
            high_ptr = right_ptr;
        }
    }
}

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        myQsort(a, sizeof(a) / sizeof(int));
        fmt::print("myQsort: {}\n", a);
    }
}