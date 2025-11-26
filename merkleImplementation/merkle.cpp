#include <bitcoin/system.hpp>
#include <iostream>
#include <algorithm>

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

int main()
{
    // Transaction hashes from block #100000
    std::vector<bcs::hash_digest> tx_hashes;
    
    bcs::hash_digest hash1, hash2, hash3, hash4;
    bcs::decode_base16(hash1, "8c14f0db3df150123e6f3dbbf30f8b955a8249b62ac1d1ff16284aefa3d06d87");
    bcs::decode_base16(hash2, "fff2525b8931402dd09222c50775608f75787bd2b87e56995a7bdd30f79702c4");
    bcs::decode_base16(hash3, "6359f0868171b1d194cbee1af2f16ea598ae8fad666d9b012c8ed2b79a236ec4");
    bcs::decode_base16(hash4, "e9a66845e05d5abc0ad04ec80f774a7e585c6e8db975962d069a522137b80c1d");
    
    std::reverse(hash1.begin(), hash1.end());
    std::reverse(hash2.begin(), hash2.end());
    std::reverse(hash3.begin(), hash3.end());
    std::reverse(hash4.begin(), hash4.end());
    
    tx_hashes.push_back(hash1);
    tx_hashes.push_back(hash2);
    tx_hashes.push_back(hash3);
    tx_hashes.push_back(hash4);
    
    bcs::hash_digest merkle_root = create_merkle(tx_hashes);
    
    std::reverse(merkle_root.begin(), merkle_root.end());
    
    std::cout << "Merkle Root Hash: " << bcs::encode_base16(merkle_root) << std::endl;
    std::cout << "Expected:         f3e94742aca4b5ef85488dc37c06c3282295ffec960994b2c0d5ac2a25a95766" << std::endl;
    
    return 0;
}