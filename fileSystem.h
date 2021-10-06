// fileSystem.h

#ifndef FILE_SYSTEM_H // include guard
#define FILE_SYSTEM_H

#include "stdc++.h"
using namespace std;

// this block resemble a actual storage data
// for now each block stores the same fileId throughtout all blocks storing 
// the data for specific file
struct Block {
	Block* prev;
	Block* next;
	int fileId;
};

class FileSystem {
	private:
		// total blocks in the storage
		int blocks;

		// total used blocks for the storage
		int usedBlocks;

		// a file information storage
		unordered_map<int, pair<int, Block*>> fileInfo;

		// actual file data is stored in a doubly linked list
		Block* fileDataCurrent;

		// a pointer to keep track of next free storage space
		Block* fileDataStart = fileDataCurrent;

	public:
		FileSystem(int blocks);
		
		pair<int, Block*> saveFile(int fileId, int bytes);

		// free the linked list nodes to free up the storage space used by given fileId
		void deleteFile(int fileId);

		// returns the starting block pointer i.e. from where the data is saved
		// and how many blocks it consumes
		// if not present it return { -1, nullptr }
		pair<int, Block*> readFile(int fileId);

		// getters
		int getBlocks() const;
		int getUsedBlocks() const;
		Block* getFileDataStart() const;

		// setters
		void setBlocks(int blocks);
		void setUsedBlocks(int usedBlocks);
		void setFileDataStart(Block* fileDataStart);

		friend ostream& operator << (ostream& o, const FileSystem* f);
		friend ostream& operator << (ostream& o, const pair<int, Block*>& b);

		~FileSystem();
};

ostream& operator << (ostream& o, const FileSystem* f);
ostream& operator << (ostream& o, const pair<int, Block*>& b);

#endif /* FILE_SYSTEM_H */
