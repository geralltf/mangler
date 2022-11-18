#ifndef BINARY_WRITER_H
#define BINARY_WRITER_H

#include <string>

using namespace std;

class BinaryWriter
{
private:
	int position = 0;
	fstream* ofs;
	std::string fileName;
public:

	BinaryWriter(string fileName);

	~BinaryWriter();

	// Set the position of the parser to the specified position in bytes.
	void SetPosition(long position);

	// Add the specified offset in bytes to the current position of the parser.
	void Advance(long offset);

	// Saves a double precision floating point value at the current position of the parser.
	void BinaryWriter::SetDouble(double value);

	// Saves a single precision floating point value at the current position of the parser.
	void BinaryWriter::SetFloat(float value);

	// Saves a 32 bit integer value at the current position of the parser.
	void BinaryWriter::SetInt32(int value);

	// Saves a 64 bit integer value at the current position of the parser.
	void BinaryWriter::SetInt64(long value);

	// Saves a string value at the current position of the parser.
	void BinaryWriter::SetString(string value, long size);
};

#endif