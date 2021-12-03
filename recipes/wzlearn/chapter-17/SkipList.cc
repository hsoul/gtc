#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

static const int MAX_LEVEL = 16;

class Node
{
    friend class SkipList;
public:
    Node()
        : value_(-1)
        , max_level_(0)
    {
        memset(next_, 0, sizeof(next_));
    }
    Node(int v, int max_level)
        : value_(v)
        , max_level_(max_level)
    {
        memset(next_, 0, sizeof(next_));
    }
    int getValue() { return value_; }
private:
    int value_;
    Node* next_[MAX_LEVEL];
};

// 插入整数数据且不重复
/*
跳表

结构:
第k级           1               9
第k-1级         1       5       9
第k-2级         1   3   5   7   9
...
第0级(原始链表)  1 2 3 4 5 6 7 8 9
*/
class SkipList
{
public:
    SkipList()
        : max_level_(1)
        , head_(NULL)
    {
        head_ = new Node();
        rand_engine_.seed(5);
    }
    ~SkipList()
    {
        if (head_)
        {
            delete head_;
            head_ = NULL;
        }
    }
    void insert(int v)
    {
        int new_node_max_level = randomLevel();
        Node* new_node = new Node(v, new_node_max_level);

        Node* pre_node[new_node_max_level];
        for (int i = 0; i < new_node_max_level; ++i)
            pre_node[i] = head_;

        Node* cur_pre_node = head_;
        for (int i = new_node_max_level - 1; i >= 0; --i) // TK : 插入每层O(n)复杂度？
        {
            while (cur_pre_node->next_[i] && cur_pre_node->next_[i]->value_ < v) // TK : 终止时p值？ 待插入位置的前一个元素指针
            {
                cur_pre_node = cur_pre_node->next_[i];
            }
            pre_node[i] = cur_pre_node; // TK : 此处的作用？看着是给每层索引寻找合适的位置，但是p = head，它的forward_[i] 没有指定值啊
        }

        for (int i = 0; i < new_node_max_level; ++i)
        {
            new_node->next_[i] = pre_node[i]->next_[i]; // NT : 仔细看看就是单链表插入，update[i]就是要插入新节点的的前一个节点
            pre_node[i]->next_[i] = new_node;
        }

        if (max_level_ < new_node_max_level)
            max_level_ = new_node_max_level;
    }
    int erase(int v)
    {
        int ret = -1;
        Node* pre_node[max_level_];
        Node* cur_pre_node = head_;
        for (int i = max_level_ - 1; i >= 0; --i)
        {
            while (cur_pre_node->next_[i] && cur_pre_node->next_[i]->value_ < v)
            {
                cur_pre_node = cur_pre_node->next_[i];
            }
            pre_node[i] = cur_pre_node;
        }

        Node* node = NULL;
        if (pre_node[0]->next_[0] && pre_node[0]->next_[0]->value_ == v)
        {
            node = pre_node[0]->next_[0];
            for (int i = max_level_ - 1; i >= 0; --i)
            {
                if (pre_node[i]->next_[i] && pre_node[i]->next_[i]->value_ == v)
                {
                    pre_node[i]->next_[i] = pre_node[i]->next_[i]->next_[i];
                    if (!head_->next_[i])
                        --max_level_;
                }
            }
            ret = 0;
            delete node;
        }

        return ret;
    }
    Node* find(int v)
    {
        Node* pre_node = head_;
        for (int i = max_level_ - 1; i >= 0; --i) // NT : 从顶层索引向下搜索，毎循环一次向下一层，因为node有可能是在上面索引层搜索到的，上面索引层节点数量少故查找数量少，因此搜索高效
        {
            while (pre_node->next_[i] && pre_node->next_[i]->value_ < v) // NT : 1 在本层找到pre_node直接下降到
            {
                pre_node = pre_node->next_[i];
            }
        }

        if (pre_node->next_[0] != NULL && pre_node->next_[0]->value_ == v)
            return pre_node->next_[0];

        return NULL;
    }
    void printAll()
    {
        for (int i = max_level_ - 1; i >= 0; --i)
        {
            Node* node = head_->next_[i];
            fmt::print("第 {} 级 : [", i);
            while (node)
            {
                fmt::print("{}  ", node->value_);
                node = node->next_[i];
            }
            fmt::print("]\n");
        }
    }
private:
    // 理论来讲，一级索引中元素个数应该占原始数据的 50%，二级索引中元素个数占 25%，三级索引12.5% ，一直到最顶层。
    // 因为这里每一层的晋升概率是 50%。对于每一个新插入的节点，都需要调用 randomLevel 生成一个合理的层数。
    // 该 randomLevel 方法会随机生成 1~MAX_LEVEL 之间的数，且 ：
    //        50%的概率返回 1
    //        25%的概率返回 2
    //      12.5%的概率返回 3 ...
    int randomLevel()
    {
        int level = 1;
        std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);

        double ret = 0.0;
        while ((ret = uniform_dist(rand_engine_)) < 0.5 && level < MAX_LEVEL)
        {
            ++level;
        }

        return level;
    }
public:
    int max_level_;
    Node* head_; // 带头节点的链表
    std::default_random_engine rand_engine_;
};

int main()
{
    SkipList skip_list;
    for (int i = 1; i < 50; ++i)
    {
        if (i % 3 == 0)
            skip_list.insert(i);
    }
    for (int i = 1; i < 50; ++i)
    {
        if (i % 3 == 1)
            skip_list.insert(i);
    }
    skip_list.printAll();

    Node* node = skip_list.find(27);
    if (node)
        fmt::print("查找到key为27节点，节点值{}\n", node->getValue());
    else
        fmt::print("未找到查找到key为27节点\n");

    int ret = skip_list.erase(42);
    if (ret == 0)
        fmt::print("成功删除key为42节点\n");
    else
        fmt::print("删除key为42节点失败\n");

    skip_list.printAll();

    ret = skip_list.erase(22);
    if (ret == 0)
        fmt::print("成功删除key为22节点\n");
    else
        fmt::print("删除key为22节点失败\n");

    skip_list.printAll();

    return 0;
}