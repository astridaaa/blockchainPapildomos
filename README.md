# Libbitcoin library
### About libbitcoin
<br>
The libbitcoin toolkit is a set of cross platform C++ libraries for building bitcoin applications. The toolkit consists of several libraries, most of which depend on the base libbitcoin-system library. Each library's repository can be cloned and built using common automake 1.14+ instructions. There are no packages yet in distribution however each library includes an installation script (described below) which is regularly verified in the automated build.

### Environment
- **OS**: Ubuntu 24.04 (WSL)
- **Compiler**: Clang++ 18 / GCC 13
- **C++ Standard**: C++20
- **Boost Version**: 1.72.0
- **libbitcoin-system Version**: 4.0.0

### Set up on Windows
As it is pretty hard to set everything up on Windows OS, Ubuntu was used instead. By following intsallation instructions ([_here_](https://github.com/libbitcoin/libbitcoin-system)). I downloaded Ubuntu and used WSL for further excecution of instructions.

After following instructions, older version of Boost was needed along with other minor modifications.

1. Installing Boost 1.72
```
cd ~
wget https://sourceforge.net/projects/boost/files/boost/1.72.0/boost_1_72_0.tar.gz
tar -xzf boost_1_72_0.tar.gz
cd boost_1_72_0
./bootstrap.sh --prefix=$HOME/boost-1.72
./b2 install --prefix=$HOME/boost-1.72 -j$(nproc)
```

2. Install libbitcoin-system v4.0.0
```
cd ~
wget https://raw.githubusercontent.com/libbitcoin/libbitcoin-system/master/install.sh
chmod +x install.sh
./install.sh --prefix=$HOME/libbitcoin --build-boost --disable-shared
```
3. Set Environment Variables
```
export PKG_CONFIG_PATH="$HOME/libbitcoin/lib/pkgconfig:$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH="$HOME/libbitcoin/lib:$LD_LIBRARY_PATH"
```

4. Compilation
```
clang++ -std=c++20 -Wno-enum-constexpr-conversion -o merkle merkle.cpp $(pkg-config --cflags --libs libbitcoin-system)
```
5. Compiling on Windows using Visual Studio Code (after cloning the repo)
   
   One should open a Ubuntu (WSL) terminal instead of default one.
```
cd BlockchainPapildomos/merkleImplementation
export PKG_CONFIG_PATH="$HOME/libbitcoin/lib/pkgconfig:$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH="$HOME/libbitcoin/lib:$LD_LIBRARY_PATH"
export PATH="$HOME/libbitcoin/bin:$PATH"
clang++ -std=c++20 -Wno-enum-constexpr-conversion -o merkle merkle.cpp $(pkg-config --cflags --libs libbitcoin-system)
```
   > running only the merkle.cpp
### Running full code with given Merkle.cpp

In Ubuntu (WSL) terminal on VSC:
```
make blockchain-libbitcoin
```
```
./blockchain-libbitcoin
```
## Tasks
Firstly, after setting everying up, the code was tested with 4 transactions from the 100,000 block with their hashes being:
- 8c14f0db3df150123e6f3dbbf30f8b955a8249b62ac1d1ff16284aefa3d06d87
- fff2525b8931402dd09222c50775608f75787bd2b87e56995a7bdd30f79702c4
- 6359f0868171b1d194cbee1af2f16ea598ae8fad666d9b012c8ed2b79a236ec4
- e9a66845e05d5abc0ad04ec80f774a7e585c6e8db975962d069a522137b80c1d
  
<img width="812" height="486" alt="image" src="https://github.com/user-attachments/assets/dd1f8fb8-3714-4429-aca3-2f86292ad5f8" />

### Getting merkle root with Merkle.cpp
<img width="940" height="253" alt="image" src="https://github.com/user-attachments/assets/2a2f5995-a0af-40e6-b0f0-e367b9e63969" />

### Testing with different transactions
<img width="940" height="347" alt="image" src="https://github.com/user-attachments/assets/988cc5cb-3209-4d25-bf3d-817533d143f5" />

### Implementation 
After completing tests and set up, I implemented the code of Merkle.cpp to my previous code. Makefile gives users options to choose from running the code with implementation as it was or running the code with merkle.cpp implementation.
