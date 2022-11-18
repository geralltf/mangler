#ifndef BINARY_READER_H
#define BINARY_READER_H

#include <string>

using namespace std;

class BinaryReader
{
private:
	int position = 0;
	ifstream* ifs;
	std::string fileName;
public:

	BinaryReader(string fileName);

	~BinaryReader();

	// Set the position of the parser to the specified position in bytes.
	void SetPosition(long position);

	// Add the specified offset in bytes to the current position of the parser.
	void Advance(long offset);

	// Get the next floating point number from the current position of the parser.
	float BinaryReader::GetFloat();

	// Get the next 32 bit integer from the current position of the parser.
	int BinaryReader::GetInt32();

	// Get the next 64 bit integer from the current position of the parser.
	long BinaryReader::GetInt64();

	// Get the next string given its specified size from the current position of the parser.
	string BinaryReader::GetString(long size);
};

#endif