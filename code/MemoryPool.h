#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <cstddef>

struct Block {
    size_t size;                // the size of this block
    Block* preBlock;            // poi.nter that points to the previous block
    unsigned char isFree;       // mark if this block is free
    Block* preNode;             // pointer that points to the previous node in the free-list
    Block* nextNode;            // pointer that points to the next node in the free-list
};

class MemoryPool {
public:
    // use singleton pattern to ensure class "MemoryPool" has only one instance
    static MemoryPool* getInstance();                       // get the single MemoryPool instance
    void* allocate(size_t size);                            // allocate some space from memory pool
    void deallocate(unsigned char* ptr);		            // release/free some space
                                                            // i.e. add freed space to memory pool
private:
    static MemoryPool* myMemoryPool;                        // the instance of MemoryPool
    Block* pool_header;                                     // header of memory pool
    static const size_t BLOCK_SIZE = 8192;                  // the size of one big memory block
    static const size_t HEADER_SIZE = sizeof(Block);        // the size of header of each memory block
    static const unsigned char POOL_END = 0xFF;             // the end symbol of the memory pool

    MemoryPool();                                           // private constructor
    ~MemoryPool();							                // private destructor
    unsigned char* allocBigBlock(size_t size);              // allocate a big memory block from system
    void insertToFreeList(Block* to_insert, Block* block);  // insert "to_insert" to the free-list following "block"
    void removeFromFreeList(Block* to_remove);              // remove "to_remove" from the free-list
    void mergeWithNextBlock(Block* block);                  // merge "block" with its next block to expand the size
    void adjustFreeList();                                  // adjust the free-list to place the bigger block in the front

    // define an inline garbage collection class
    class CGabor {
    public:
        ~CGabor() {
            if (MemoryPool::myMemoryPool != nullptr) {
                delete MemoryPool::myMemoryPool;
                MemoryPool::myMemoryPool = nullptr;
            }
        }
    };
    // define a static member variable of class "CGabor"
    // its destructor will be automatically called to release the singleton object at the end of the program
    static CGabor myGabor;
};

#endif