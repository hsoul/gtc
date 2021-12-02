#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class BucketSort
{
public:
    static void bucketSort(int a[], int n, int bucket_size) // bucket_size 每个桶预计存取多少元素
    {
        if (n < 2)
            return;

        int min_value = a[0];
        int max_value = a[1];
        for (int i = 0; i < n; ++i) // NT : 遍历一遍获取数组最大值和最小值
        {
            if (a[i] < min_value)
                min_value = a[i];
            if (a[i] > max_value)
                max_value = a[i];
        }

        int bucket_count = (max_value - min_value) / bucket_size + 1;
        std::vector<std::vector<int>> buckets(bucket_count, {bucket_size, 0});
        std::vector<int> bucket_elem_count(bucket_count, 0);

        for (int i = 0; i < n; ++i)
        {
            int bucket_index = (a[i] - min_value) / bucket_size;
            if (static_cast<size_t>(bucket_elem_count[bucket_index]) == buckets[bucket_index].size())
            {
                ensureCapacity(buckets, bucket_index);
            }
            buckets[bucket_index][bucket_elem_count[bucket_index]++] = a[i];
        }

        int push_index = 0;
        for (int i = 0; i < static_cast<int>(buckets.size()); ++i)
        {
            if (bucket_elem_count[i] == 0) // 此桶为空
                continue;
            fmt::print("{}\n", buckets[i]);
            quickSort(buckets[i].data(), 0, bucket_elem_count[i] - 1);
            for (int j = 0; j < bucket_elem_count[i]; ++j)
            {
                a[push_index++] = buckets[i][j];
            }
        }
    }
    static void ensureCapacity(std::vector<std::vector<int>>& buckets, int bucket_index)
    {
        buckets[bucket_index].resize(buckets[bucket_index].size() * 2); // NT : 其实不用写，为了方便展示排序过程
    }
    static void quickSort(int a[], int p, int r)
    {
        if (r <= p)
            return;
        int q = partition(a, p, r);
        quickSort(a, p, q - 1);
        quickSort(a, q + 1, r);
    }
    static int partition(int a[], int p, int r)
    {
        fmt::print("{}\n", std::vector<int>(a + p, a + r + 1));
        int pivot_value = a[r];
        int left_index = p;
        for (int iter_index = p; iter_index < r; ++iter_index)
        {
            if (a[iter_index] < pivot_value)
                swap(a[iter_index], a[left_index++]);
        }
        swap(a[left_index], a[r]);
        return left_index;
    }
    template <typename T>
    static void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }
};

int main()
{
    int b[] = {4, 7, 8, 9, 1, 2, 6, 4, 5, 4, 3, 9, 2, 1};
    {
        int a[sizeof(b) / sizeof(int)];
        memcpy(a, b, sizeof(b));
        BucketSort::bucketSort(a, sizeof(a) / sizeof(int), 2);
        fmt::print("bucketSort: {}\n", a);
    }
}