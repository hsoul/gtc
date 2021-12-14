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

void bubbleSort(int a[], int n)
{
    if (n <= 1)
        return;

    for (int i = 0; i < n; ++i)
    {
        bool is_swap = false;
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (a[j + 1] < a[j])
            {
                swap(a[j + 1], a[j]);
                is_swap = true;
            }
        }
        if (!is_swap)
            break;
    }
}

/*
思路：数组分为有序区和无序区，依次在无须区挑选最小值放到有序区末尾

排序过程：
    1.
    2.
    3.
*/
void selectSort(int a[], int n) // 因为存在跨节点交换，选择排序是非稳定排序算法
{
    if (n <= 1)
        return;

    for (int i = 0; i < n; ++i)
    {
        int min_index = i;
        for (int j = i + 1; j < n; ++j) // NT : int j = i + 1;
        {
            if (a[j] < a[min_index])
                min_index = j;
        }
        swap(a[i], a[min_index]);
    }
}

/*

思路：数组分为有序区和无序区，依次将无序区第一个元素插入（前/后遍历查找）有序区。

对比：插入排序与选择排序相比，插入排序交换元素较多，是因为无序区元素在插入有序区时需要遍历寻找位置插入，此过程需要交换平移有序元素直至插入元素到恰当位置

排序过程：
    1.
    2.
    3.
*/

void insertSort(int a[], int n) // NT : 从后向前，交换平移插入
{
    if (n <= 1)
        return;

    for (int i = 1; i < n; ++i)
    {
        for (int j = i - 1; j >= 0; --j) // NT : i为要插入元素的index
        {
            if (a[j + 1] < a[j]) // NT : 从后向前依次遍历有序元素，如果待插入元素小于有序组最后一个元素交换两个元素，重复此步骤直到插入元素大于等于前面元素停止，此时位置就是插入的位置; 保证稳定排序的要求是在寻找插入位置时从后向前用小于比较交换，从前往后用大于等于比较
                swap(a[j + 1], a[j]);
        }
    }
}

void insertSort2(int a[], int n) // NT : 从前往后查找插入位置，找到插入位置后，借用两个临时元素空间向后平移后面的有序元素
{
    for (int i = 1; i < n; ++i)
    {
        int insert_value = a[i];
        int tmp[2];
        int change_index = i;

        for (int j = 0; j < i; ++j)
        {
            if (insert_value >= a[j])
                continue;

            int index = j % 2;

            if (change_index == i) // NT : 记录无序区要插入元素将要插入有序区的位置
            {
                tmp[::abs(index - 1)] = a[j];
                change_index = j;
            }

            tmp[index] = a[j + 1]; // NT : 重要 重要 重要，本次奇，上下偶

            if (0 == index) // NT : 从元素插入的index位置其每个元素后移一个，如果本次index==0则上次即为1
                a[j + 1] = tmp[1];
            else
                a[j + 1] = tmp[0];
        }

        a[change_index] = insert_value;
    }
}

int main()
{
    int t1[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    int t2[] = {4};
    decltype(t2)& b = t2;
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        selectSort(a, sizeof(a) / sizeof(int));
        fmt::print("SelectSort: {}\n", a);
    }
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        bubbleSort(a, sizeof(a) / sizeof(int));
        fmt::print("bubbleSort: {}\n", a);
    }
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        insertSort(a, sizeof(a) / sizeof(int));
        fmt::print("insertSort: {}\n", a);
    }
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        insertSort2(a, sizeof(a) / sizeof(int));
        fmt::print("insertSort2: {}\n", a);
    }
}