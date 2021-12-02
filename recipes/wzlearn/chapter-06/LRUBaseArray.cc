#include <iostream>
#include <string.h>
#include <unordered_map>
#include <vector>

template <typename T>
class LRUBaseArray
{
public:
    LRUBaseArray(int capacity)
        : capacity_(capacity)
        , size_(0)
        , values_(new T*[capacity_])
    {
        // values_.reserve(capacity);
        ::memset(values_, 0, capacity_ * sizeof(T*));
    }
    void offer(T* obj)
    {
        int index = getIndex(obj);
        if (index < 0)
        {
            if (isFull())
            {
                remove();          // 删除数组最后元素
                rightShift(size_); // 数组所有元素向右移动一个位置
                cache(obj);        // 将对象指针存储在数组第一个位置
            }
            else
            {
                rightShift(size_); // 数组所有元素向右移动一个位置
                cache(obj);        // 将对象指针存储在数组第一个位置
            }
        }
        else
        {
            rightShift(index); // 从0到index - 1的元素向右移动一个位置
            cache(obj);        // 将对象指针存储在数组第一个位置
        }
    }
    void dump() const
    {
        for (int i = 0; i < size_; ++i)
        {
            std::cout << "addr:" << values_[i] << " pos:" << i << std::endl;
        }
    }
private:
    int getIndex(T* obj) const
    {
        auto iter = pos_holder_.find(obj);
        if (iter != pos_holder_.end())
            return iter->second;
        return -1;
    }
    bool isFull() const { return size_ == capacity_; }
    void cache(T* obj)
    {
        values_[0] = obj;
        ++size_;
        pos_holder_.insert({obj, 0});
    }
    void remove()
    {
        --size_;
    }
    void rightShift(int end_index) // 从0到end_index-1的元素向右移动一个位置
    {
        for (int to_index = end_index; to_index > 0; --to_index)
        {
            values_[to_index] = values_[to_index - 1];
            pos_holder_[values_[to_index]] += 1;
        }
    }
private:
    enum { DEFAULT_CAPACITY = 8 };
    int capacity_;
    int size_;
    T** values_;
    // std::vector<T*> values_;
    std::unordered_map<T*, int> pos_holder_;
};

class Obj
{
public:
    int value_;
};

int main(void)
{
    LRUBaseArray<Obj> lru(4);

    std::vector<Obj*> objs;

    for (int i = 0; i < 20; ++i)
    {
        objs.emplace_back(new Obj());
        std::cout << objs.back() << std::endl;
        lru.offer(objs.back());
    }

    lru.dump();

    // int** ptr = new int* [10] { 0 };
    // int a = 8;
    // ptr[5] = &a;
    // std::cout << &a << "\n"
    //           << ptr[5] << std::endl;

    int8_t v;

    for (int i = 0; i < 130; ++i)
    {
        v += 1;
        std::cout << static_cast<int>(v) << std::endl;
    }
}