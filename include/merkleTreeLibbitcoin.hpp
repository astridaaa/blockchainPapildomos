#ifndef MERKLE_TREE_LIBBITCOIN_HPP
#define MERKLE_TREE_LIBBITCOIN_HPP

#include <bitcoin/system.hpp>
#include <vector>
#include <string>
#include <iostream>


namespace bcs = libbitcoin::system;

bcs::hash_digest create_merkle(std::vector<bcs::hash_digest>& merkle)
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
            bcs::data_chunk concat_data(bcs::hash_size * 2);
            
            std::copy(it->begin(), it->end(), concat_data.begin());
            
            std::copy((it + 1)->begin(), (it + 1)->end(), concat_data.begin() + bcs::hash_size);
            
            bcs::hash_digest new_root = bcs::sha256::double_hash(concat_data);
            
            new_merkle.push_back(new_root);
        }
        
        merkle = new_merkle;
        
        // DEBUG output
        std::cout << "Current merkle hash list:" << std::endl;
        for (const auto& hash: merkle)
            std::cout << "  " << bcs::encode_base16(hash) << std::endl;
        std::cout << std::endl;
    }
    
    return merkle[0];
}
#endif