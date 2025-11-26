#include "include/merkleTree.h"
#include "include/customGenerator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#ifdef USE_LIBBITCOIN
#include <bitcoin/system.hpp>
#include <algorithm>
namespace bcs = libbitcoin::system;
#endif

MerkleTree::MerkleTree(const std::vector<std::string> &data)
{
    if (data.empty())
    {
        throw std::invalid_argument("Data cannot be empty");
    }

    for (const auto &item : data)
    {
        leaves.push_back(hash(item));
    }
    root = buildTree(leaves);
}

std::string MerkleTree::hash(const std::string &data) const
{
    HashGenerator hash;
    std::string hashed = hash.generateHash(data);
    return hashed;
}

std::string MerkleTree::combineHash(const std::string &left, const std::string &right) const
{
    return hash(left + right);
}

std::shared_ptr<MerkleTree::Node> MerkleTree::buildTree(const std::vector<std::string> &hashes)
{
    if (hashes.empty())
    {
        return nullptr;
    }

    if (hashes.size() == 1)
    {
        return std::make_shared<Node>(hashes[0]);
    }

    std::vector<std::shared_ptr<Node>> level;

    // Create leaf nodes
    for (const auto &h : hashes)
    {
        level.push_back(std::make_shared<Node>(h));
    }

    while (level.size() > 1)
    {
        std::vector<std::shared_ptr<Node>> nextLevel;

        for (size_t i = 0; i < level.size(); i += 2)
        {
            if (i + 1 < level.size())
            {
                std::string combinedHash = combineHash(level[i]->hash, level[i + 1]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i + 1]);
                nextLevel.push_back(parent);
            }
            else
            {
                std::string combinedHash = combineHash(level[i]->hash, level[i]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i]);
                nextLevel.push_back(parent);
            }
        }

        level = nextLevel;
    }

    return level[0];
}

std::string MerkleTree::getRootHash() const
{
    return root ? root->hash : "";
}

void MerkleTree::getProofHelper(const std::shared_ptr<Node> &node, size_t index,
                                size_t start, size_t end, std::vector<std::string> &proof) const
{
    if (!node || start == end)
    {
        return;
    }

    size_t mid = (start + end) / 2;

    if (index <= mid)
    {
        if (node->right)
        {
            proof.push_back(node->right->hash);
        }
        getProofHelper(node->left, index, start, mid, proof);
    }
    else
    {
        if (node->left)
        {
            proof.push_back(node->left->hash);
        }
        getProofHelper(node->right, index, mid + 1, end, proof);
    }
}

std::vector<std::string> MerkleTree::getProof(size_t index) const
{
    if (index >= leaves.size())
    {
        throw std::out_of_range("Index out of range");
    }

    std::vector<std::string> proof;
    getProofHelper(root, index, 0, leaves.size() - 1, proof);
    return proof;
}

bool MerkleTree::verify(const std::string &data, const std::vector<std::string> &proof,
                        const std::string &rootHash) const
{
    std::string currentHash = hash(data);

    for (const auto &proofHash : proof)
    {
        std::string hash1 = combineHash(currentHash, proofHash);
        std::string hash2 = combineHash(proofHash, currentHash);

        currentHash = hash1;
    }

    return currentHash == rootHash;
}

void MerkleTree::printNode(const std::shared_ptr<Node> &node, int depth) const
{
    if (!node)
        return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << node->hash << " (depth: " << depth << ")" << std::endl;
    std::cout << std::endl;
    if (node->left)
        printNode(node->left, depth + 1);
    if (node->right && node->right != node->left)
        printNode(node->right, depth + 1);
}

void MerkleTree::printTree() const
{
    std::cout << "Merkle Tree Structure:" << std::endl;
    printNode(root, 0);
}

// libbitcoin implementation
#ifdef USE_LIBBITCOIN

// Helper function to create merkle root
static bcs::hash_digest create_merkle_internal(std::vector<bcs::hash_digest>& merkle)
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
            bcs::data_chunk concat_data(bcs::hash_size * 2);
            
            // Copy first hash (32 bytes)
            std::copy(it->begin(), it->end(), concat_data.begin());
            
            // Copy second hash (32 bytes)
            std::copy((it + 1)->begin(), (it + 1)->end(), concat_data.begin() + bcs::hash_size);
            
            // Hash both of the hashes using sha256 double hash
            bcs::hash_digest new_root = bcs::sha256::double_hash(concat_data);
            
            // Add this to the new list.
            new_merkle.push_back(new_root);
        }
        
        // This is the new list.
        merkle = new_merkle;
    }
    
    // Finally we end up with a single item.
    return merkle[0];
}

std::string MerkleTree::getRootHashLibbitcoin(const std::vector<std::string>& txHashes)
{
    std::vector<bcs::hash_digest> hashes;
    
    // Convert hex strings to hash_digest and reverse byte order
    for (const auto& hexHash : txHashes)
    {
        bcs::hash_digest hash;
        if (bcs::decode_base16(hash, hexHash))
        {
            // IMPORTANT: Reverse byte order (Bitcoin displays in big-endian, processes in little-endian)
            std::reverse(hash.begin(), hash.end());
            hashes.push_back(hash);
        }
    }
    
    // Calculate merkle root
    bcs::hash_digest merkle_root = create_merkle_internal(hashes);
    
    // Reverse the result back to display format (big-endian)
    std::reverse(merkle_root.begin(), merkle_root.end());
    
    // Convert back to hex string
    return bcs::encode_base16(merkle_root);
}
#else
std::string MerkleTree::getRootHashLibbitcoin(const std::vector<std::string>& txHashes)
{
    std::cerr << "Error: libbitcoin not enabled. Recompile with USE_LIBBITCOIN flag." << std::endl;
    return "";
}
#endif