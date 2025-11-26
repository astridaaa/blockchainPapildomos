#ifndef MERKLE_TREE_LIBBITCOIN_HPP
#define MERKLE_TREE_LIBBITCOIN_HPP

#include <bitcoin/system.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace bcs = libbitcoin::system;
inline bcs::hash_digest create_merkle_libbitcoin(std::vector<bcs::hash_digest>& merkle)
{
    if (merkle.empty())
        return bcs::null_hash;
    else if (merkle.size() == 1)
        return merkle[0];
    
    while (merkle.size() > 1)
    {
        if (merkle.size() % 2 != 0)
            merkle.push_back(merkle.back());
        assert(merkle.size() % 2 == 0);
        
        std::vector<bcs::hash_digest> new_merkle;
        
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            bcs::data_chunk concat_data;
            concat_data.insert(concat_data.end(), it->begin(), it->end());
            concat_data.insert(concat_data.end(), (it + 1)->begin(), (it + 1)->end());
            
            bcs::hash_digest new_root = bcs::sha256::double_hash(concat_data);
            
            new_merkle.push_back(new_root);
        }
        merkle = new_merkle;
    }
    return merkle[0];
}

inline bool hex_to_hash(bcs::hash_digest& hash, const std::string& hex_string)
{
    return bcs::decode_base16(hash, hex_string);
}

inline std::string hash_to_hex(const bcs::hash_digest& hash)
{
    return bcs::encode_base16(hash);
}

#endif