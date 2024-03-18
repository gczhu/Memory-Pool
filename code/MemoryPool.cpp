#include "MemoryPool.h"
#include <vector>

MemoryPool* MemoryPool::myMemoryPool = nullptr;
MemoryPool::CGabor MemoryPool::myGabor;

MemoryPool* MemoryPool::getInstance() {
	if (myMemoryPool == nullptr) {
		myMemoryPool = new MemoryPool();
	}
	return myMemoryPool;
}

MemoryPool::MemoryPool() {
	pool_header = nullptr;
}

MemoryPool::~MemoryPool() {
	if (pool_header != NULL) {
		std::vector<void*> toFree;
		Block* block = pool_header;
		if (block->preBlock == nullptr) {
			toFree.push_back((void*)block);
		}
		block = block->nextNode;
		while (block != pool_header) {
			if (block->preBlock == nullptr) {
				toFree.push_back((void*)block);
			}
			block = block->nextNode;
		}
		for(void* p : toFree){
			operator delete(p);
		}
	}
}

void* MemoryPool::allocate(size_t size) {
	if (pool_header == nullptr || pool_header->size < size) {		// if there is no enough space for this allocation
		int totalSize = size + HEADER_SIZE + sizeof(POOL_END);		// get the total size of the memory space should be allocated
		totalSize = (totalSize + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;		// the total size should be an integer multiple of BLOCK_SIZE
		unsigned char* p = allocBigBlock(totalSize);				// allocate a new big memory block
		Block* block = (Block*)p;
		block->isFree = 1;
		block->preBlock = nullptr;
		block->size = totalSize - HEADER_SIZE - sizeof(POOL_END);
		insertToFreeList(block, pool_header);
		pool_header = block;
	}
	// split the block that header points to into two parts
	// one for this allocation, the other for later use
	void* ptr = (void*)((unsigned char*)pool_header + HEADER_SIZE);				// the memory space that is allocated
	Block* block = (Block*)((unsigned char*)pool_header + HEADER_SIZE + size);	// the memory space that remains for later use
	Block* next_block = (Block*)((unsigned char*)pool_header + HEADER_SIZE + pool_header->size);  // the memory space of next block
	// the process of splitting one block
	pool_header->isFree = 0;		// the memory space that is allocated is not free anymore
	if (pool_header->size > size + HEADER_SIZE) {	// if the space is large enough to be splitted
		block->isFree = 1;			// the memory space that remains for later use is still free
		block->preBlock = pool_header;
		block->size = pool_header->size - size - HEADER_SIZE;
		pool_header->size = size;
		if (*(unsigned char*)next_block != POOL_END) {
			next_block->preBlock = block;
		}
		Block* head = pool_header;
		// insert the memory space that remains for later use to the free-list
		insertToFreeList(block, head);
		// remove the memory space that is allocated from the free-list
		removeFromFreeList(head);
	}
	else {		// if the space is too small to be splitted
		removeFromFreeList(pool_header);
	}
	// adjust the free-list to place the bigger block in the front
	adjustFreeList();
	return ptr;
}

void MemoryPool::deallocate(unsigned char* ptr) {
	Block *thisBlock, *preBlock, *nextBlock;
	thisBlock = (Block*)(ptr - HEADER_SIZE);
	preBlock = thisBlock->preBlock;
	nextBlock = (Block*)(ptr + thisBlock->size);
	thisBlock->isFree = 1;
	// if the next block is free, then merge them to a bigger block
	if (*(ptr + thisBlock->size) != POOL_END && nextBlock->isFree == 1) {
		mergeWithNextBlock(thisBlock);
		removeFromFreeList(nextBlock);
	}
	// if the previous block is free, then merge them to a bigger block
	if (preBlock != nullptr && preBlock->isFree == 1) {
		mergeWithNextBlock(preBlock);
	}
	// otherwise, insert it to the free list
	else {
		insertToFreeList(thisBlock, pool_header);
	}
	// adjust the free-list to place the bigger block in the front
	adjustFreeList();
}

unsigned char* MemoryPool::allocBigBlock(size_t size) {
	unsigned char* ptr = (unsigned char*)(operator new(size));
	*(ptr + size - sizeof(POOL_END)) = POOL_END;
	return ptr;
}

void MemoryPool::insertToFreeList(Block* to_insert, Block* block) {
	if (block != nullptr) {
		to_insert->preNode = block;
		to_insert->nextNode = block->nextNode;
		block->nextNode->preNode = to_insert;
		block->nextNode = to_insert;
	}
	else {		// if the linked list is empty
		to_insert->preNode = to_insert->nextNode = to_insert;
		pool_header = to_insert;
	}
}

void MemoryPool::removeFromFreeList(Block* to_remove) {
	if (to_remove == pool_header) {		// if the node to be removed is the header
		pool_header = pool_header->nextNode;
	}
	if (to_remove->preNode == to_remove && to_remove->nextNode == to_remove) {		// if there is only one node in the linked list
		pool_header = nullptr;
	}
	else {
		to_remove->preNode->nextNode = to_remove->nextNode;
		to_remove->nextNode->preNode = to_remove->preNode;
	}
}

void MemoryPool::mergeWithNextBlock(Block* block) {
	Block* nextBlock = (Block*)((unsigned char*)block + HEADER_SIZE + block->size);
	block->size += nextBlock->size + HEADER_SIZE;
	if (*((unsigned char*)nextBlock + nextBlock->size + HEADER_SIZE) != POOL_END) {
		nextBlock = (Block*)((unsigned char*)nextBlock + nextBlock->size + HEADER_SIZE);
		nextBlock->preBlock = block;
	}
}

void MemoryPool::adjustFreeList() {
	if (pool_header != nullptr) {
		if (pool_header->size < pool_header->nextNode->size) {		// if the block that header points to is smaller
			pool_header = pool_header->nextNode;		// move header to the next block
		}
		else {
			if (pool_header->nextNode->nextNode != pool_header) {	// otherwise, put the next block before header
				Block* block = pool_header->nextNode;
				removeFromFreeList(block);
				insertToFreeList(block, pool_header->preNode);
			}
		}
	}
}