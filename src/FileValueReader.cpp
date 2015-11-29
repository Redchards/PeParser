#include <stdafx.h>

#include <FileValueReader.h>

// OPTIMIZATION NOTE : 
// We can have one reader by file (not multiplicating readers), by having a static undordered_map of readers, and doing a lookup each time we want to access
// a file. Supposedly, the lookup will be negated by the read, but this requires benchmark.

FileValueReaderBase::FileValueReaderBase(const std::string& filename, std::ios_base::openmode flags) : Base(filename, flags)
{}

// TODO : Remove maybe ?
std::unique_ptr<unsigned char> FileValueReaderBase::retrieveRawData(std::streampos position, size_type size)
{
	std::unique_ptr<unsigned char> dataPtr(new unsigned char[size]);

	read(dataPtr.get(), position, size);

	return dataPtr;
}

std::vector<unsigned char> FileValueReaderBase::retrieveRawBuffer(std::streampos position, size_type size)
{
	std::vector<unsigned char> buffer;
	buffer.resize(size);

	read(buffer, position, size);

	return buffer;
}