#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

int partion(int a[], int p, int r)
{
    int pivot = a[r];
    int left_index = p;
    int iter_index = p;

    for (; iter_index < r; ++iter_index)
    {
        if (a[iter_index] < pivot)
        {
            swap(a[iter_index], a[left_index]);
            ++left_index;
        }
    }

    swap(a[left_index], a[r]);

    return left_index;
}

int kthSmallestInternally(int a[], int p, int r, int target_index)
{
    int pivot_index = partion(a, p, r);
    if (pivot_index == target_index)
        return pivot_index;
    else if (pivot_index > target_index)
        return kthSmallestInternally(a, p, pivot_index - 1, target_index);
    else
        return kthSmallestInternally(a, pivot_index + 1, r, target_index);
}

/*

时间复杂度为 O(n)

分析过程：
    1. 确定最费时间的操作是什么？分区次数为 log2^n, 而分区内部比较次数为 n + n/2 + n/4 + ... + 1 = 2n - 1, 所以去除低阶式，只考虑比较此处即可分析出复杂度
    2. 时间复杂度为O(2n - 1 + log2^n) = O(n)
*/
int kthSmallest(int a[], int n, int target)
{
    if (target > n || target <= 0)
        return -1;
    return kthSmallestInternally(a, 0, n - 1, target - 1);
}

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        int targetth_num = 9;
        int index = kthSmallest(a, sizeof(a) / sizeof(int), targetth_num);
        int sorted[] = {1, 1, 2, 2, 3, 4, 4, 4, 5, 6, 7, 8, 9, 9};
        fmt::print("sorted : {} \n", sorted);
        fmt::print("kthSmallest {} : {} \n", targetth_num, a[index]);
    }
}