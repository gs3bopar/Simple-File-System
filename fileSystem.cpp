#include "fileSystem.h"

using namespace std;

FileSystem::FileSystem(int blocks) : blocks (blocks), fileDataCurrent (nullptr), usedBlocks(0) {}

pair<int, Block*> FileSystem::saveFile(int fileId, int bytes) {
	pair<int, Block*> res { -1, nullptr };
	if (fileInfo.count(fileId)) {
		cout << "File already exists, please delete it first!" << endl;
		return res;
	}

	int blocksToBeAcc = ceil((double) bytes / 1024);
	if ((usedBlocks + blocksToBeAcc) <= blocks) {
		// update the used blocks
		int newUsedBlocks = usedBlocks + blocksToBeAcc;
		setUsedBlocks(newUsedBlocks);

		// create a linked list with required file data
		Block* fileDataLinkedList = new Block({ fileDataCurrent, nullptr, fileId });
		// store a pointer to the start of block
		Block* temp = fileDataLinkedList;
		
		// fileDataCurrent is nullptr only for the initial case
		if (fileDataCurrent == nullptr) {
			fileDataStart = temp;
		} else {
			fileDataCurrent->next = fileDataLinkedList;
		}

		// create the return object
		pair<int, Block*> info { blocksToBeAcc, temp };
		fileInfo[fileId] = info;


		while (blocksToBeAcc > 1) {
			fileDataLinkedList->next = new Block({ fileDataLinkedList, nullptr, fileId });
			fileDataLinkedList = fileDataLinkedList->next;
			blocksToBeAcc--;
		}

		// move the current ptr to last or next node
		fileDataCurrent = fileDataLinkedList;

		return info;
	} else cout << "Not enought memory, Please delete some files first!" << endl;
	return res;
}

void FileSystem::deleteFile(int fileId) {
	if (fileInfo.count(fileId)) {
		// key exist in our file information
		auto & info = fileInfo[fileId];

		// after freeing we will have info.first freed mem, which can be again
		// allocated
		int newUsedBlocks = usedBlocks - info.first;
		setUsedBlocks(newUsedBlocks);

		Block* prev = (info.second)->prev;
		Block* temp;
		while (info.first != 0) {
			temp = (info.second)->next;
			// first save the next
			free (info.second);
			info.second = temp;
			info.first--;
		}
		
		// check for first edge case, update the starting pointer
		if (prev == nullptr) {
			fileDataStart = temp;
			// cout << temp << endl;
			if (temp == nullptr) {
				// last case for fileDataCurrent
				fileDataCurrent = nullptr;
			} else {
				temp->prev = nullptr;
			}
		} else {
			prev->next = temp;
			if (temp == nullptr) {
				// last case for fileDataCurrent
				fileDataCurrent = prev;
			} else {
				temp->prev = prev;
			}
		}

		info.second = NULL;
		temp = NULL;

		// delete the entry from the map
		fileInfo.erase(fileId);

		cout << "File successfuly deleted!" << endl;
	}
	cout << "File does not exist!" << endl;
}

pair<int, Block*> FileSystem::readFile(int fileId) {
	if (fileInfo.count(fileId)) {
		cout << "File exists!" << endl;
		return fileInfo[fileId];
	}
	else {
		cout << "File does not exist!" << endl;
		return pair<int, Block*> { -1, nullptr };
	}
}

// getters
int FileSystem::getBlocks() const {
	return blocks;
}

int FileSystem::getUsedBlocks() const {
	return usedBlocks;
}

Block* FileSystem::getFileDataStart() const {
	return fileDataStart;
}

// setters
void FileSystem::setBlocks(int blocks) {
	this->blocks = blocks;
	return;
}

void FileSystem::setUsedBlocks(int usedBlocks) {
	this->usedBlocks = usedBlocks;
	return;
}

void FileSystem::setFileDataStart(Block* fileDataStart) {
	this->fileDataStart = fileDataStart;
	return;
}

ostream & operator << (ostream & o, const FileSystem* f) {
	o << "*----------------File System State-------------------*" << endl;
	o << "Total storage blocks: " << f->getBlocks() << endl;
	o << "Total storage occupied blocks: " << f->getUsedBlocks() << endl;
	Block* start = f->getFileDataStart();
	o << "Snapshot of file storage: ";
	if (!start) o << "No blocks!" << endl;
	while(start) {
		o << start->fileId;
		if (start->next) o << " -> ";
		else o << endl;
		start = start->next;
	}
	o << "*----------------------------------------------------*";
	return o;
}

ostream & operator << (ostream & o, const pair<int, Block*>& b) {
	o << "*--------------File State with Blocks----------------*" << endl;
	Block* start = b.second;
	int totBlocks = b.first;
	if (totBlocks == -1) {
		o << "Nothing in this block!" << endl;
	} else {
		while (totBlocks >= 1) {
			o << start->fileId;
			if (start->next) o << " -> ";
			else o << endl;
			start = start->next;
			totBlocks--;
		}
	}
	o << "*----------------------------------------------------*";
	return o;
}

FileSystem::~FileSystem() {
	// free all the nodes
	cout << "Freeing all Storage..." << endl;
	Block* temp = fileDataStart;
	while (temp) {
		fileDataStart = temp->next;
		free (temp);
		temp = fileDataStart;
	}

	// always good to have the pointers point to NULL to prevent,
	// undefined behaviour
	for (auto & m : fileInfo) {
		m.second.second = NULL;
	}
	fileInfo.clear();

	this->setBlocks(0);
	this->setUsedBlocks(0);
	this->setFileDataStart(nullptr);
}
