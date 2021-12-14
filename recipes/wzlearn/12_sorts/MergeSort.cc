#include <climits>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

void merge(int a[], int p, int q, int r)
{
    std::vector<int> tmp(r - p + 1);
    int left_index = p;
    int right_index = q + 1;
    int insert_index = 0;
    while (left_index <= q && right_index <= r)
    {
        if (a[right_index] < a[left_index])
            tmp[insert_index++] = a[right_index++];
        else
            tmp[insert_index++] = a[left_index++];
    }

    int remain_start_index = left_index;
    int remain_end_index = q;
    if (left_index >= q)
    {
        remain_start_index = q + 1;
        remain_end_index = r;
    }

    while (remain_start_index <= remain_end_index)
    {
        tmp[insert_index++] = a[remain_start_index++];
    }

    for (int i = p; i <= r; ++i)
    {
        a[i] = tmp[i - p];
    }
}

void mergeBySentry(int a[], int p, int q, int r)
{
    int left_size = q - p + 2;
    int right_size = r - q + 1;
    std::vector<int> left_arr(left_size);
    std::vector<int> right_arr(right_size); // NT : 每个数组都多分配了一个空间用于存放sentry, 注意右边数组是从q + 1 到 r的，所以r - q 为右边数组长度

    for (int i = 0; i < left_size - 1; ++i)
    {
        left_arr[i] = a[p + i];
    }

    left_arr[left_size - 1] = INT_MAX;

    for (int i = 0; i < right_size - 1; ++i)
    {
        right_arr[i] = a[q + 1 + i];
    }

    right_arr[right_size - 1] = INT_MAX;

    int l_index = 0;
    int r_index = 0;
    int insert_index = p;

    while (insert_index <= r) // NT : 此处循环结束条件是数组元素合并完毕
    {
        if (right_arr[r_index] < left_arr[l_index]) // NT : 此处与哨兵判断相关
            a[insert_index++] = right_arr[r_index++];
        else
            a[insert_index++] = left_arr[l_index++];
    }
}

void mergeSortInternally(int a[], int p, int r)
{
    if (p >= r) // NT : 递归终止条件
        return;

    int q = p + (r - p) / 2;
    mergeSortInternally(a, p, q);
    mergeSortInternally(a, q + 1, r);
    // merge(a, p, q, r);
    mergeBySentry(a, p, q, r);
}

/*

思路：递归将待排序数组均分为两个区，直至排序数据为单个元素，然后合并拆分的数组，最终整体数组数据变为有序

排序过程：

*/

void mergeSort(int a[], int n)
{
    mergeSortInternally(a, 0, n - 1);
}

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        mergeSort(a, sizeof(a) / sizeof(int));
        fmt::print("mergeSort: {}\n", a);
    }
}

// fmt::print("p q r left_size right_size = {} {} {} {} {}\n", p, q, r, left_size, right_size);
// fmt::print("to_merge = {}\n", std::vector<int>(a + p, a + r + 1));
// fmt::print("left_arr = {}\n", left_arr);
// fmt::print("right_arr = {}\n", left_arr);
// fmt::print("a = {}\n", std::vector<int>(a + p, a + r + 1));
