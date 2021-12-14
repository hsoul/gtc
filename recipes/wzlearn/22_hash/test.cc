#include "ConsistentHashMap.hpp"
#include "fmt_include.h"

#include <boost/crc.hpp>
#include <boost/format.hpp>
#include <boost/functional/hash.hpp>

struct crc32_hasher
{
public:
    using result_type = uint32_t;
public:
    uint32_t operator()(const std::string& node)
    {
        boost::crc_32_type crc;
        fmt::print("node_str : {}\n", node);
        crc.process_bytes(node.c_str(), node.size());
        return crc.checksum();
    }
};

int main()
{
    using consistent_hash_t = ConsistentHashMap<std::string, crc32_hasher>;

    consistent_hash_t consistent_hash_;
    boost::format node_fmt("192.168.1.%1%");

    for (std::size_t i = 0; i < 3; ++i)
    {
        std::string node = fmt::format("192.168.1.{}", i);
        consistent_hash_.insert(node);
        fmt::print("add node {}\n", node);
    }

    {
        std::cout << "=========================================================" << std::endl;
        for (consistent_hash_t::iterator it = consistent_hash_.begin(); it != consistent_hash_.end(); ++it)
        {
            std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        }
    }

    {
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(290235110);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
    }

    {
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(2286285664);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
    }

    {
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(4282565578);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
    }

    std::cout << "=========================================================" << std::endl;
    {
        std::string node = boost::str(node_fmt % 1);
        consistent_hash_.erase(node);
        for (consistent_hash_t::iterator it = consistent_hash_.begin(); it != consistent_hash_.end(); ++it)
        {
            std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        }
    }

    std::cout << "=========================================================" << std::endl;
    {
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(4282565578);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
        consistent_hash_.erase(it);
        for (consistent_hash_t::iterator it = consistent_hash_.begin(); it != consistent_hash_.end(); ++it)
        {
            std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        }
    }

    std::cout << "=========================================================" << std::endl;
    {
        std::cout << "-------------------------------------------" << std::endl;
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(4282565578);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        it = consistent_hash_.find(4282565576);
        std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        consistent_hash_.erase(it);

        for (consistent_hash_t::iterator it = consistent_hash_.begin(); it != consistent_hash_.end(); ++it)
        {
            std::cout << boost::format("node:%1%,%2%") % it->second % it->first << std::endl;
        }
        std::cout << "-------------------------------------------" << std::endl;
    }

    std::cout << "=========================================================" << std::endl;
    {
        std::cout << "-------------------------------------------" << std::endl;
        consistent_hash_t::iterator it;
        it = consistent_hash_.find(4282565578);
        if (it == consistent_hash_.end())
        {
            std::cout << "not found, consistent_hash is empty" << std::endl;
        }
    }

    return 0;
}
