#ifndef BUFFERBLOCK_H
#define BUFFERBLOCK_H

#include <iostream>
#include <string>
#include "BufferBlockADT.h"

using namespace std;

// Define a class BufferBlock that implements the BufferBlockADT interface
class BufferBlock : BufferblockADT {
public:
// Constructor to initialize the BufferBlock with data and size
BufferBlock(char* data, int sz) : block(data), size(sz) {}

// Destructor for freeing resources if needed
~BufferBlock() {}

// Retrieve a sequence of data from the buffer block at a specific position
void getData(int pos, int sz, char* data) {
for (int dataCount = 0; dataCount < sz && (pos + dataCount) < (size + (size * blockID)); dataCount++) {
            data[dataCount] = block[pos - (size * blockID) + dataCount];
    }
}

// Get the size of the buffer block's data
int getBlocksize() const {
    return size;
}

// Set the ID of the buffer block
void setID(int id) {
    blockID = id;
}

// Get the ID of the buffer block
int getID() const {
    return blockID;
}

// Get a pointer to the data stored in the buffer block
char* getBlock() const {
    return block;
}

// Set the data stored in the buffer block using a new pointer
void setBlock(char* blk) {
    block = blk;
}

private:
int blockID;     // Identifier for the buffer block
int size;        // Size of the buffer block's data
char* block;     // Pointer to the data stored in the buffer block

};

#endif // BUFFERBLOCK_H
