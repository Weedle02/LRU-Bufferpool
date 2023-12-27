#ifndef LRUBUFFERPOOL_H
#define LRUBUFFERPOOL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BufferPoolADT.h"
#include "BufferBlock.h"

//using namespace std;

// Define a class LRUBufferPool that inherits from BufferPoolADT
class LRUBufferPool : public BufferPoolADT {
private:
    char* BLOCK;                 // Pointer to the main buffer holding the data
    int poolSize;                 // Number of buffer blocks in the pool
    int blockSize;                // Size of each buffer block

    vector<BufferBlock> buffers;  // Vector to store individual buffer blocks
    vector<int> LRU;              // Vector to track the Least Recently Used (LRU) order
    vector<int> ID;               // Vector to store the IDs of buffer blocks
 
public:
    // Constructor for initializing the LRUBufferPool
    LRUBufferPool(string filename, int poolSizeInt, int blockSizeInt)
        : poolSize(poolSizeInt), blockSize(blockSizeInt) {
        // Resize vectors to match the poolSize
        LRU.resize(poolSize, 0);
        ID.resize(poolSize, 0);

        // Open the file and read its content into the main buffer
        ifstream input(filename, ios::in | ios::binary);
        input.seekg(0, ios::end);
        int length = input.tellg();
        input.seekg(0, ios::beg);

        BLOCK = new char[length];
        input.read(BLOCK, length);
        input.close();

        // Initialize the buffer blocks
        initializeBufferBlocks();
    }

    // Destructor to free the allocated memory
    ~LRUBufferPool() {
        delete[] BLOCK;
    }

    // Retrieve a sequence of bytes from the buffer pool
    void getBytes(char* space, int sz, int pos) {
        int blockIndex = findBlockIndex(pos);

        if (blockIndex != -1) {
            buffers[blockIndex].getData(pos, sz, space);
            updateLRU(blockIndex);
        } else {
            createNewBlock(pos, space, sz);
        }
    }

    // Print the buffer block order from most recently used to LRU
    void printBufferBlockOrder() {
        cout << "Buffer Block order: " << endl;
        for (int i = 0; i < poolSize; i++) {
            int count = 0;
            while (LRU[count] != i) {
                count++;
            }
            cout << ID[count] << " , ";
        }
        cout << endl;
    }

private:
    // Initialize the buffer blocks with appropriate IDs
    void initializeBufferBlocks() {
        for (int i = 0; i < poolSize; i++) {
            BufferBlock x(BLOCK + blockSize * i, blockSize);
            x.setID(i);

            LRU[i] = i;
            ID[i] = i;
            buffers.push_back(x);
        }
    }

    // Find the index of the buffer block corresponding to a given position
    int findBlockIndex(int pos) {
        for (int i = 0; i < poolSize; i++) {
            if (ID[i] == pos / blockSize) {
                return i;
            }
        }
        return -1;
    }

    // Update the LRU vector when a buffer block is accessed
    void updateLRU(int blockIndex) {
        int recentlyUsed = LRU[blockIndex];
        LRU[blockIndex] = -1;

        for (int j = 0; j < poolSize; j++) {
            if (LRU[j] < recentlyUsed) {
                LRU[j]++;
            }
        }
    }

    // Create a new buffer block for a given position and update LRU order
    void createNewBlock(int pos, char* space, int sz) {
        int leastRecentlyUsed = getLRUBlockID();

        BufferBlock x(BLOCK + blockSize * (pos / blockSize), blockSize);
        x.setID(pos / blockSize);

        buffers[leastRecentlyUsed] = x;

        ID[leastRecentlyUsed] = pos / blockSize;
        updateLRU(leastRecentlyUsed);

        // Retrieve data from the newly created block
        buffers[leastRecentlyUsed].getData(pos, sz, space);
    }

    // Get the ID of the least recently used buffer block
    int getLRUBlockID() {
        int count = 0;
        while (LRU[count] != poolSize - 1) {
            count++;
        }
        return count;
    }

    // Other members and private variables remain unchanged
};

#endif
