#include <stdafx.h>

#include <FileValueReader.h>

// OPTIMIZATION NOTE : Theres two major optimizations we can perform on this class. First, the most evident one is to buffer the call.
// Then, we can have one reader by file (not multiplicating readers), by having a static undordered_map of readers, and doing a lookup each time we want to access
// a file. Supposedly, the lookup will be negated by the read, but this requires benchmark.

FileValueReaderBase::FileValueReaderBase(const std::string& filename, std::ios_base::openmode flags) : Base(filename, flags) {}

// TODO : Remove maybe ?
std::unique_ptr<unsigned char> FileValueReaderBase::retrieveRawData(size_type position, size_type size)
{
	std::unique_ptr<unsigned char> dataPtr(new unsigned char[size]);

	if (fstream_.seekg(position, fstream_.beg))
	{
		fstream_.read(reinterpret_cast<char*>(dataPtr.get()), size);
		return dataPtr;
	}
	else
	{
		throw std::ios_base::failure((std::string("Error : failed to read the file ") + getCurrentFileName()).c_str());
	}

	return dataPtr;
}

std::vector<unsigned char> FileValueReaderBase::retrieveRawBuffer(size_type position, size_type size)
{
	std::vector<unsigned char> buffer;
	buffer.resize(size);

	if(fstream_.seekg(position, fstream_.beg))
	{
		std::copy_n(std::istream_iterator<unsigned char>{fstream_}, size, buffer.begin());
	}
	else
	{
		throw std::ios_base::failure((std::string("Error : failed to read the file ") + getCurrentFileName().c_str()));
	}

	return buffer;
}