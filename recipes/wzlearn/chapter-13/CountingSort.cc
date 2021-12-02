#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class CountingSort
{
public:
    static void countingSort(int a[], int n) // 计数排序，排序非负整数
    {
        if (n <= 1)
            return;

        int max = a[0];
        for (int i = 1; i < n; ++i)
        {
            if (max < a[i])
                max = a[i];
        }

        std::vector<int> count_arr(max + 1, 0); // 创建数组，下标大小[0, max]
        for (int i = 0; i < n; ++i)             // NT : 将每个元素数量存入计数数组
        {
            count_arr[a[i]]++;
        }

        for (int i = 1; i < static_cast<int>(count_arr.size()); ++i)
        {
            count_arr[i] = count_arr[i - 1] + count_arr[i];
        }

        std::vector<int> tmp(n);
        for (int i = n - 1; i >= 0; --i)
        {
            int index = count_arr[a[i]] - 1;
            tmp[index] = a[i];
            count_arr[a[i]]--;
        }

        for (int i = 0; i < n; ++i)
        {
            a[i] = tmp[i];
        }
    }
};

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        CountingSort::countingSort(a, sizeof(a) / sizeof(int));
        fmt::print("countingSort: {}\n", a);
    }
}