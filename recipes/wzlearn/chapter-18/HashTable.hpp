#include <functional>
#include <vector>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>

template <typename Key, typename T, typename Hash = std::hash<Key>>
class HashTable
{
public:
    using key_type = Key;
    using value_type = T;
    using hasher = Hash;
private:
    class Entry
    {
    public:
        Entry(key_type key, value_type value, Entry* next)
            : key_(key)
            , value_(value)
            , next_(next)
        {
        }
        Entry(key_type key, value_type value)
            : key_(key)
            , value_(value)
            , next_(nullptr)
        {
        }
        Entry()
            : next_(nullptr)
        {
        }
    public:
        key_type key_;
        value_type value_;
        Entry* next_;
    };
public:
    HashTable()
        : table_(DEFAULT_INITAL_CAPACITY, nullptr)
        , size_(0)
        , index_used_num_(0)
    {
    }
    void put(key_type key, value_type value)
    {
        int index = hash(key);

        if (!table_[index]) // TK : 位置未被引用创建哨兵，单链表的头结点？
            table_[index] = new Entry();

        Entry* tmp = table_[index];
        if (!tmp->next_)
        {
            tmp->next_ = new Entry(key, value);
            ++size_;
            ++index_used_num_;

            if (index_used_num_ >= table_.size() * LOAD_FACTOR) // NT : index使用数量大于load factor百分比，rehash
                resize();
        }
        else // NT : 解决散列冲突，使用链表法
        {
            do
            {
                tmp = tmp->next_;
                if (tmp->key_ == key)
                {
                    tmp->value_ = value;
                    return;
                }
            } while (tmp->next_);
            Entry* new_entry = new Entry(key, value);
            new_entry->next_ = table_[index]->next_;
            table_[index]->next_ = new_entry;
            ++size_;
        }
    }
    void resize()
    {
        index_used_num_ = 0;
        std::vector<Entry*> new_table(table_.size() * 2, nullptr);
        for (int i = 0; i < static_cast<int>(table_.size()); ++i)
        {
            if (!table_[i] || !table_[i]->next_)
                continue;

            Entry* e = table_[i];
            while (e->next_)
            {
                e = e->next_;
                int index = hasher_(e->key_);
                if (!table_[index])
                {
                    ++index_used_num_;
                    table_[index] = new Entry();
                }
                table_[index]->next_ = new Entry(e->key_, e->value_, table_[index]->next_);
            }
        }
    }
    void remove(key_type key)
    {
        int index = hasher_(key);
        Entry* e = table_[index];
        if (!e || !e->next_)
            return;

        Entry* pre;
        Entry* head_node = table_[index];
        Entry* find_node = nullptr;
        do
        {
            pre = e;
            e = e->next_;
            if (key == e->key_)
            {
                find_node = e;
                pre->next_ = e->next_;
                --size_;
                if (!head_node->next_)
                    --index_used_num_;
                delete find_node;
                return;
            }
        } while (!e->next_);
    }
    value_type get(key_type key)
    {
        int index = hasher_(key);
        Entry* e = table_[index];
        if (!e || !e->next_)
            return;

        while (!e->next_)
        {
            e = e->next_;
            if (key == e->key_)
                return e->value_;
        }

        return value_type();
    }
    void printAll() const
    {
        for (int index = 0; index < static_cast<int>(table_.size()); ++index)
        {
            Entry* e = table_[index];
            if (!e || !e->next_)
                continue;

            fmt::print("index {} ", index);

            while (e->next_)
            {
                e = e->next_;
                fmt::print("{} ", e->value_);
            }

            fmt::print("\n");
        }
    }
private:
    int hash(key_type key)
    {
        return hasher_(key) % table_.size();
    }
private:
    static constexpr int DEFAULT_INITAL_CAPACITY = 8;
    static constexpr double LOAD_FACTOR = 0.75;
private:
    const hasher hasher_ = hasher();
    std::vector<Entry*> table_;
    int size_ = 0;           // 元素数量
    int index_used_num_ = 0; // 哈希表索引使用数量
};
