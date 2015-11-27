#include <stdafx.h>

#include <FileValueReader.h>

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

std::vector<uint8_t> FileValueReaderBase::retrieveRawBuffer(size_type position, size_type size)
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