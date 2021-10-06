#include "fileSystem.h"

using namespace std;

int main () {

	// get the total storage size
	string systemStorage;
	cout << "Please enter the total storage space (a number followed immediately by MB or GB): ";
	cin >> systemStorage;

	int num = stoi(systemStorage.substr(0, systemStorage.length() - 2)); // 3
	string units = systemStorage.substr(systemStorage.length() - 2); // MB

	// convert units to uppercase
	transform(units.begin(), units.end(), units.begin(), ::toupper);

	// max storage space can be 2147483647 blocks == 2047 GB
	int totalBlocks = 0;

	// convert systemStorage to blocks
	if (units == "MB") {
		totalBlocks = num * 1024;
	} else if (units == "GB") {
		totalBlocks = num * 1024 * 1024;
	} else {
		cout << "Wrong input, try again!" << endl;
		return 1;
	}

	FileSystem * fs = new FileSystem(totalBlocks);
	cout << fs << endl;

	int input;
	bool userInput = true;
	int fileId;
	int fileBytes;
	do {
		cout << "Please enter a valid number input to access File System Manager: Save (1), Delete (2), Read (3), Exit (4)." << endl;
		cin >> input;
		if (input == 1) {
			cout << "Save a file!" << endl;
			cout << "Please enter a file ID: ";
			cin >> fileId;
			cout << "Please enter a total file bytes: ";
			cin >> fileBytes;
			auto read = fs->saveFile(fileId, fileBytes);
			cout << read << endl;
		} else if (input == 2) {
			cout << "Delete a file!" << endl;
			cout << "Please enter a file ID: ";
			cin >> fileId;
			fs->deleteFile(fileId);
		} else if (input == 3) {
			cout << "Read a file!" << endl;
			cout << "Please enter a file ID: ";
			cin >> fileId;
			auto read = fs->readFile(fileId);
			cout << read << endl;
		} else {
			userInput = false;
		}
		// print the file system state
		cout << fs << endl;
	} while (userInput);

	// exit the program, free all the acc memory
	delete fs;

	return 0;
}
