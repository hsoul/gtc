#pragma once

#include <functional>
#include <map>

template <typename T,
          typename Hash,
          typename Alloc = std::allocator<std::pair<const typename Hash::result_type, T>>>
class ConsistentHashMap
{
public:
    using size_type = typename Hash::result_type;
    using map_type = std::map<size_type, T, std::less<size_type>, Alloc>;
    using value_type = typename map_type::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename map_type::iterator;
    using reverse_iterator = typename map_type::reverse_iterator;
    using allocator_type = Alloc;
public:
    ConsistentHashMap() = default;
    ~ConsistentHashMap() = default;
public:
    std::size_t size() const { return nodes_.size(); }
    bool empty() const { return nodes_.empty(); }
    std::pair<iterator, bool> insert(const T& node)
    {
        size_t hash = hasher_(node);
        return nodes_.insert(value_type(hash, node));
    }
    void erase(iterator iter) { nodes_.erase(iter); }
    std::size_t erase(const T& node)
    {
        size_t hash = hasher_(node);
        return nodes_.erase(hash);
    }
    iterator find(size_type hash)
    {
        if (nodes_.empty())
            return nodes_.end();

        iterator iter = nodes_.lower_bound(hash); // 返回首个 >= hash 元素的的迭代器
        if (iter == nodes_.end())
            iter = nodes_.begin();

        return iter;
    }
    iterator begin() { return nodes_.begin(); }
    iterator end() { return nodes_.end(); }
    reverse_iterator rbegin() { return nodes_.rbegin(); }
    reverse_iterator rend() { return nodes_.rend(); }
private:
    Hash hasher_; // 用map存储，能够保证节点hash相同而冲突
    map_type nodes_;
};