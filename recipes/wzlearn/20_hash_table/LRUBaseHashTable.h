#pragma once

#include "fmt_include.h"
#include <unordered_map>

template <typename K, typename V>
class LRUBaseHashTable
{
public:
    class DNode
    {
    public:
        DNode()
            : prev_(nullptr)
            , next_(nullptr)
        {
        }
        DNode(K key, V value)
            : key_(key)
            , value_(value)
            , prev_(nullptr)
            , next_(nullptr)
        {
        }
        ~DNode()
        {
            fmt::print("DNode {} {} deconstruct\n", key_, value_);
        }
    public:
        K key_;
        V value_;
        DNode* prev_;
        DNode* next_;
    };
public:
    LRUBaseHashTable()
        : head_(nullptr)
        , tail_(nullptr)
        , size_(0)
        , capacity_(DEFAULT_CAPACITY)
    {
        head_ = new DNode();
        tail_ = new DNode();
        head_->next_ = tail_;
        tail_->prev_ = head_;
    }
    LRUBaseHashTable(int capacity)
        : head_(nullptr)
        , tail_(nullptr)
        , size_(0)
        , capacity_(capacity)
    {
        head_ = new DNode();
        tail_ = new DNode();
        head_->next = tail_;
        tail_->prev_ = head_;
    }
    ~LRUBaseHashTable()
    {
        if (head_)
        {
            delete head_;
            head_ = nullptr;
        }
        if (tail_)
        {
            delete tail_;
            tail_ = nullptr;
        }
    }
    void add(K key, V value) // NT : 新增节点
    {
        auto iter = hash_table_.find(key);
        if (iter == hash_table_.end())
        {
            DNode* new_node = new DNode(key, value);
            hash_table_.insert({key, new_node});
            addNode(new_node);
            if (++size_ > capacity_) // NT : 容量到达上限，删除链表末尾元素
            {
                DNode* tail = popTail();
                hash_table_.erase(tail->key_);
                --size_;
                delete tail;
            }
        }
        else
        {
            DNode* node = iter->second;
            node->value_ = value;
            moveToHead(node);
        }
    }
    DNode* get(K key)
    {
        auto iter = hash_table_.find(key);
        if (iter == hash_table_.end())
            return nullptr;
        moveToHead(iter->second);
        return iter->second;
    }
    void remove(K key)
    {
        auto iter = hash_table_.find(key);
        if (iter == hash_table_.end())
            return;
        removeNode(iter->second);
        --size_;
        delete iter->second;
        hash_table_.erase(iter);
    }
    void clear()
    {
        for (auto& elem : hash_table_)
        {
            delete elem.second;
        }
        hash_table_.clear();
        head_->next_ = tail_;
        tail_->prev_ = head_;
        size_ = 0;
    }
    void printAll()
    {
        bool is_print = false;
        DNode* node = head_->next_;
        while (node->next_) // NT : 此处判断node->next_是因为有哨兵尾结点
        {
            fmt::print("{}, ", node->value_);
            node = node->next_;
            if (!is_print)
                is_print = true;
        }
        if (is_print)
            fmt::print("\n");
    }
private:
    void addNode(DNode* new_node)
    {
        new_node->next_ = head_->next_;
        new_node->prev_ = head_;

        head_->next_->prev_ = new_node;
        head_->next_ = new_node;
    }
    DNode* popTail()
    {
        DNode* node = tail_->prev_;
        removeNode(node);
        return node;
    }
    void removeNode(DNode* node)
    {
        node->prev_->next_ = node->next_;
        node->next_->prev_ = node->prev_;
    }
    void moveToHead(DNode* node)
    {
        removeNode(node);
        addNode(node);
    }
private:
    static constexpr int DEFAULT_CAPACITY = 5;
    DNode* head_;                              // 哨兵head
    DNode* tail_;                              // 哨兵尾结点
    int size_;                                 // 链表长度
    int capacity_;                             // 容量，缓存上限
    std::unordered_map<K, DNode*> hash_table_; //
};