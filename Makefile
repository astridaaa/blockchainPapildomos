ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /Q
    RM_DIR = rmdir /S /Q
    EXE = .exe
    LIBS = -lssl -lcrypto -lws2_32 -lcrypt32
    SEP = \\
    OMPFLAG = -fopenmp
else
    # macOS/Linux
    RM = rm -f
    RM_DIR = rm -rf
    EXE = 
    LIBS = -lssl -lcrypto
    SEP = /
    OMPFLAG = -fopenmp
endif

CXX = g++
CXXFLAGS = -std=c++17 -O3 $(OMPFLAG)
INCLUDES = -Iinclude -Ihash

# libbitcoin settings
LIBBITCOIN_PREFIX = $(HOME)/libbitcoin
LIBBITCOIN_INCLUDES = -I$(LIBBITCOIN_PREFIX)/include
LIBBITCOIN_LIBS = -L$(LIBBITCOIN_PREFIX)/lib -lbitcoin-system -lboost_system -lboost_thread -lpthread
LIBBITCOIN_FLAGS = -DUSE_LIBBITCOIN
LIBBITCOIN_CXX = g++  # Changed from clang++ to g++
LIBBITCOIN_CXXFLAGS = -std=c++20 -O3 $(OMPFLAG)  # Removed -Wno-enum-constexpr-conversion

# ============================================
# Main program (custom LIDL hash merkle)
# ============================================
blockchain$(EXE): Program.o Block.o BlockHeader.o Transaction.o Functions.o merkleTree.o customGenerator.o
	$(CXX) $(CXXFLAGS) Program.o Block.o BlockHeader.o Transaction.o Functions.o merkleTree.o customGenerator.o -o blockchain$(EXE) $(LIBS)

Program.o: Program.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Program.cpp -o Program.o

Block.o: Block.cpp include/Block.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Block.cpp -o Block.o

BlockHeader.o: BlockHeader.cpp include/BlockHeader.h
	$(CXX) $(CXXFLAGS) -c BlockHeader.cpp -o BlockHeader.o

Transaction.o: Transaction.cpp include/Transaction.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Transaction.cpp -o Transaction.o

Functions.o: Functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Functions.cpp -o Functions.o

merkleTree.o: merkleTree.cpp include/merkleTree.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c merkleTree.cpp -o merkleTree.o

customGenerator.o: customGenerator.cpp include/customGenerator.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c customGenerator.cpp -o customGenerator.o

# ============================================
# Main program (libbitcoin SHA256 merkle)
# ============================================
blockchain-libbitcoin$(EXE): Program-libbitcoin.o Block-libbitcoin.o BlockHeader-libbitcoin.o Transaction-libbitcoin.o Functions-libbitcoin.o merkleTree-libbitcoin.o customGenerator-libbitcoin.o
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) Program-libbitcoin.o Block-libbitcoin.o BlockHeader-libbitcoin.o Transaction-libbitcoin.o Functions-libbitcoin.o merkleTree-libbitcoin.o customGenerator-libbitcoin.o -o blockchain-libbitcoin$(EXE) $(LIBS) $(LIBBITCOIN_LIBS)

Program-libbitcoin.o: Program.cpp
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c Program.cpp -o Program-libbitcoin.o

Block-libbitcoin.o: Block.cpp include/Block.h
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c Block.cpp -o Block-libbitcoin.o

BlockHeader-libbitcoin.o: BlockHeader.cpp include/BlockHeader.h
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c BlockHeader.cpp -o BlockHeader-libbitcoin.o

Transaction-libbitcoin.o: Transaction.cpp include/Transaction.h
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c Transaction.cpp -o Transaction-libbitcoin.o

Functions-libbitcoin.o: Functions.cpp
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c Functions.cpp -o Functions-libbitcoin.o

merkleTree-libbitcoin.o: merkleTree.cpp include/merkleTree.h
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c merkleTree.cpp -o merkleTree-libbitcoin.o

customGenerator-libbitcoin.o: customGenerator.cpp include/customGenerator.h
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_FLAGS) $(INCLUDES) $(LIBBITCOIN_INCLUDES) -c customGenerator.cpp -o customGenerator-libbitcoin.o

# ============================================
# User and Transaction Generators
# ============================================
usergen$(EXE): generators/userGenerator.o customGenerator.o
	$(CXX) $(CXXFLAGS) generators/userGenerator.o customGenerator.o -o usergen$(EXE) $(LIBS)

generators/userGenerator.o: generators/userGenerator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c generators/userGenerator.cpp -o generators/userGenerator.o

txgen$(EXE): generators/transactionGenerator.o Transaction.o customGenerator.o
	$(CXX) $(CXXFLAGS) generators/transactionGenerator.o Transaction.o customGenerator.o -o txgen$(EXE) $(LIBS)

generators/transactionGenerator.o: generators/transactionGenerator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c generators/transactionGenerator.cpp -o generators/transactionGenerator.o

# ============================================
# Standalone libbitcoin merkle demo
# ============================================
merkle$(EXE): merkleImplementation/merkle.o
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) merkleImplementation/merkle.o -o merkle$(EXE) $(LIBBITCOIN_LIBS)

merkleImplementation/merkle.o: merkleImplementation/merkle.cpp
	$(LIBBITCOIN_CXX) $(LIBBITCOIN_CXXFLAGS) $(LIBBITCOIN_INCLUDES) -c merkleImplementation/merkle.cpp -o merkleImplementation/merkle.o

# ============================================
# Clean
# ============================================
clean:
ifeq ($(OS),Windows_NT)
	-$(RM) *.o 2>nul
	-$(RM) generators$(SEP)*.o 2>nul
	-$(RM) merkleImplementation$(SEP)*.o 2>nul
	-$(RM) *.exe 2>nul
else
	$(RM) *.o *-libbitcoin.o generators/*.o merkleImplementation/*.o blockchain blockchain-libbitcoin usergen txgen merkle
endif

# ============================================
# Run targets
# ============================================
run: blockchain$(EXE)
	.$(SEP)blockchain$(EXE)

run-libbitcoin: blockchain-libbitcoin$(EXE)
	.$(SEP)blockchain-libbitcoin$(EXE)

run-merkle: merkle$(EXE)
	.$(SEP)merkle$(EXE)

setup: usergen$(EXE) txgen$(EXE)
	.$(SEP)usergen$(EXE)
	.$(SEP)txgen$(EXE)

# ============================================
# Build all targets
# ============================================
all: usergen$(EXE) txgen$(EXE) blockchain$(EXE)
	.$(SEP)usergen$(EXE)
	.$(SEP)txgen$(EXE)
	.$(SEP)blockchain$(EXE)

all-libbitcoin: usergen$(EXE) txgen$(EXE) blockchain-libbitcoin$(EXE)
	.$(SEP)usergen$(EXE)
	.$(SEP)txgen$(EXE)
	.$(SEP)blockchain-libbitcoin$(EXE)

all-with-merkle: usergen$(EXE) txgen$(EXE) blockchain$(EXE) merkle$(EXE)

everything: usergen$(EXE) txgen$(EXE) blockchain$(EXE) blockchain-libbitcoin$(EXE) merkle$(EXE)

.PHONY: clean run run-libbitcoin run-merkle setup all all-libbitcoin all-with-merkle everything