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

int partition(int a[], int p, int r) // NT 每次分区能能将参照节点放到恰当位置
{
    int pivot = a[r]; // NT : 以pivot将数组分为两个区，左区的是 < pivot的元素，右区时 >= pivot的元素

    int left_index = p; // 右开
    int iter_index = p; // NT : iter_index 从0开始，left_index代表的是小于pivot元素的end(),开区间
    for (; iter_index < r; ++iter_index)
    {
        if (a[iter_index] < pivot)
        {
            swap(a[iter_index], a[left_index]);
            ++left_index;
        }
    }

    swap(a[left_index], a[r]);

    fmt::print("partition = {}\n", std::vector<int>(a + p, a + r + 1));

    return left_index;
}

void quickSortInternally(int a[], int p, int r)
{
    if (p >= r) // NT : p == r 时，只有一个元素要排序的不必，但是如果r = p + 1, 分完区后q = r, 然后partition（）返回的q == p, 故接下来左边排序入参为 p, p - 1显然输入错误，故不能以p == r作为递归结束条件，应该是p >= r
        return;

    int q = partition(a, p, r); // NT : 每次分区 q 位置元素（pivot）在其有序位置上
    quickSortInternally(a, p, q - 1);
    quickSortInternally(a, q + 1, r);
}

void quickSort(int a[], int n) // NT : 原地非稳定算法
{
    quickSortInternally(a, 0, n - 1);
}

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        quickSort(a, sizeof(a) / sizeof(int));
        fmt::print("quickSort: {}\n", a);
    }
}