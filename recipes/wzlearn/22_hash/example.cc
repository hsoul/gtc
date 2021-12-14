#include "ConsistentHashMap.hpp"

#include <string>
#include <vector>

#include <boost/crc.hpp>
#include <boost/format.hpp>
#include <boost/functional/hash.hpp>

#include "fmt_include.h"

// const char* nodes[] = {
//     "192.168.1.100",
//     "192.168.1.101",
//     "192.168.1.102",
//     "192.168.1.103",
//     "192.168.1.104",
// };

constexpr int NODE_SIZE = 100;

std::vector<std::string> nodes(NODE_SIZE);

struct vnode_t
{
    vnode_t()
        : node_id_(0)
        , vnode_id_(0)
    {
    }
    vnode_t(std::size_t id, std::size_t vid)
        : node_id_(id)
        , vnode_id_(vid)
    {
    }
    std::string to_str() const
    {
        return boost::str(boost::format("%1%-%2%") % nodes[node_id_].c_str() % vnode_id_);
    }

    std::size_t node_id_;
    std::size_t vnode_id_;
};

struct crc32_hasher
{
public:
    using result_type = uint32_t;
public:
    uint32_t operator()(const vnode_t& node)
    {
        boost::crc_32_type crc;
        std::string node_str = node.to_str();
        fmt::print("node_str : {}\n", node_str);
        crc.process_bytes(node_str.c_str(), node_str.size());
        return crc.checksum();
    }
};

struct std_hasher
{
public:
    using result_type = uint32_t;
public:
    uint32_t operator()(const vnode_t& node)
    {
        boost::crc_32_type crc;
        std::string node_str = node.to_str();
        fmt::print("node_str : {}\n", node_str);
        return hasher_(node_str);
    }
public:
    std::hash<std::string> hasher_;
};

int main()
{
    using consitent_hash_map_t = ConsistentHashMap<vnode_t, std_hasher>;

    consitent_hash_map_t chash;

    for (std::size_t i = 0; i < NODE_SIZE; ++i)
    {
        std::string node = fmt::format("192.168.1.{}", i);
        fmt::print("add node {}\n", node);
        nodes[i] = node;
    }

    for (std::size_t i = 0; i < NODE_SIZE; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            chash.insert(vnode_t(i, j));
        }
    }

    {
        std::size_t sums[NODE_SIZE] = {0};
        consitent_hash_map_t::iterator iter = chash.begin();
        consitent_hash_map_t::reverse_iterator last_iter = chash.rbegin();
        std::size_t n = iter->first + UINT32_MAX - last_iter->first;

        fmt::print("vnode:{}, hash:{}, contains:{}\n", iter->second.to_str(), iter->first, n); // NT : 每个虚拟阶段所占槽位数量
        uint32_t pre_iter = iter->first;
        uint32_t cur;
        consitent_hash_map_t::iterator end = chash.end();
        while (++iter != end)
        {
            cur = iter->first;
            n = cur - pre_iter;
            fmt::print("vnode:{}, hash:{}, contains:{}\n", iter->second.to_str(), cur, n);
            sums[iter->second.node_id_] += n;
            pre_iter = cur;
        }
        fmt::print("---------------------------------------\n");
        std::size_t total_slots = 0;
        for (std::size_t i = 0; i < NODE_SIZE; ++i)
        {
            fmt::print("node:{}, contains:{}\n", nodes[i], sums[i]);
            total_slots += sums[i];
        }
        fmt::print("\n total slots {} UNIT32_MAX {}\n", total_slots, UINT32_MAX);
    }

    {
        consitent_hash_map_t::iterator iter = chash.find(290235110);
        if (iter != chash.end())
            fmt::print("\nfind node:{}, vnode:{}, hash:{}\n", nodes[iter->second.node_id_], iter->second.vnode_id_, iter->first);
        else
            fmt::print("\nnot find hash {} node\n", 290235110);
    }
}