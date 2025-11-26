#include <bitcoin/system.hpp>
#include <iostream>

namespace bcs = libbitcoin::system;

// Merkle Root Hash
bcs::hash_digest create_merkle(std::vector<bcs::hash_digest>& merkle)
{
    // Stop if hash list is empty or contains one element
    if (merkle.empty())
        return bcs::null_hash;
    else if (merkle.size() == 1)
        return merkle[0];
    
    // While there is more than 1 hash in the list, keep looping...
    while (merkle.size() > 1)
    {
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0)
            merkle.push_back(merkle.back());
        
        // List size is now even.
        assert(merkle.size() % 2 == 0);
        
        // New hash list.
        std::vector<bcs::hash_digest> new_merkle;
        
        // Loop through hashes 2 at a time.
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            // Join both current hashes together (concatenate).
            bcs::data_chunk concat_data;
            concat_data.insert(concat_data.end(), it->begin(), it->end());
            concat_data.insert(concat_data.end(), (it + 1)->begin(), (it + 1)->end());
            
            // Hash both of the hashes using sha256 double hash
            bcs::hash_digest new_root = bcs::sha256::double_hash(concat_data);
            
            // Add this to the new list.
            new_merkle.push_back(new_root);
        }
        
        // This is the new list.
        merkle = new_merkle;
        
        // DEBUG output
        std::cout << "Current merkle hash list:" << std::endl;
        for (const auto& hash: merkle)
            std::cout << "  " << bcs::encode_base16(hash) << std::endl;
        std::cout << std::endl;
    }
    
    // Finally we end up with a single item.
    return merkle[0];
}
/*
int main()
{
    // Transaction hashes from block #100000
    std::vector<bcs::hash_digest> tx_hashes;
    
    
    uzduoties duotos transakcijos
    bcs::hash_digest hash1, hash2, hash3, hash4;
    bcs::decode_base16(hash1, "8c14f0db3df150123e6f3dbbf30f8b955a8249b62ac1d1ff16284aefa3d06d87");
    bcs::decode_base16(hash2, "fff2525b8931402dd09222c50775608f75787bd2b87e56995a7bdd30f79702c4");
    bcs::decode_base16(hash3, "6359f0868171b1d194cbee1af2f16ea598ae8fad666d9b012c8ed2b79a236ec4");
    bcs::decode_base16(hash4, "e9a66845e05d5abc0ad04ec80f774a7e585c6e8db975962d069a522137b80c1d");


    //pasirinktos mano transakcijos is Bitcoin Block 925,257
    bcs::hash_digest hash1, hash2, hash3, hash4, hash5, hash6, hash7, hash8;
    bcs::decode_base16(hash1, "dbc699d2d8223c3c8a694771ca4c407b4edaec040388a43ea15d66c21347d004");
    bcs::decode_base16(hash2, "b21787d158033da4c3099c5f03b3a717cd3da374dcc37a7e1cd8d1f55e6f5c04");
    bcs::decode_base16(hash3, "abae4a65801008339a4206ae6ad02966ef23f0bb2b6495e3d477d8e3e8d6e61c");
    bcs::decode_base16(hash4, "728973d929f03d2bd2ad5c14fd588637482716d05884b7c781a0121b828ed55d");
    bcs::decode_base16(hash5, "0b58ea2f651ea99539577eef2b0585c84b68210cc8643cbf1bd2683bb8d43c57");
    bcs::decode_base16(hash6, "f90bdd04b757a70feabc94871da4a5578c7d7f807ccb0e81fb754c28af3dce5f");
    bcs::decode_base16(hash7, "c39b28b2a24c04f0b1f53406c7c90896f2bfa3c1b6d0845ac87dad4b974cb8f2");
    bcs::decode_base16(hash8, "a75ae42fa2495b1b405265ae86457b10d8c6e28fc6de5217c68d03d11dd3c1b8");

    //Bitcoin Block 925,257
    //dbc699d2d8223c3c8a694771ca4c407b4edaec040388a43ea15d66c21347d004
    //b21787d158033da4c3099c5f03b3a717cd3da374dcc37a7e1cd8d1f55e6f5c04
    //abae4a65801008339a4206ae6ad02966ef23f0bb2b6495e3d477d8e3e8d6e61c
    //728973d929f03d2bd2ad5c14fd588637482716d05884b7c781a0121b828ed55d
    //0b58ea2f651ea99539577eef2b0585c84b68210cc8643cbf1bd2683bb8d43c57
    //f90bdd04b757a70feabc94871da4a5578c7d7f807ccb0e81fb754c28af3dce5f
    //c39b28b2a24c04f0b1f53406c7c90896f2bfa3c1b6d0845ac87dad4b974cb8f2
    //a75ae42fa2495b1b405265ae86457b10d8c6e28fc6de5217c68d03d11dd3c1b8
    
    tx_hashes.push_back(hash1);
    tx_hashes.push_back(hash2);
    tx_hashes.push_back(hash3);
    tx_hashes.push_back(hash4);
    tx_hashes.push_back(hash5);
    tx_hashes.push_back(hash6);
    tx_hashes.push_back(hash7);
    tx_hashes.push_back(hash8);
    
    const bcs::hash_digest merkle_root = create_merkle(tx_hashes);
    std::cout << "Merkle Root Hash: " << bcs::encode_base16(merkle_root) << std::endl;
    
    return 0;
}*/