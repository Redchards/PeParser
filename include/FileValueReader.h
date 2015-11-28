#pragma once

#include <Configuration.h>
#include <FileStream.h>

#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <iostream>

// Maybe should move all enums to Configuration.h header
// NOTE : This is the endianess of the file as one might want to analyze a file with a different endianess.
// However, PE/COFF files should always be little-endian (Windows is only present on little-endian platforms, except WinCE)

enum class BufferPolicy : flag_type
{
	nonBuffered = false,
	buffered = true
};

template<BufferPolicy flag>
class __declspec(dllexport)  FileValueReaderBase : public FileStreamBase<StreamGoal::read>
{
	using Base = FileStreamBase<StreamGoal::read>;

protected:
	FileValueReaderBase(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary);

public:
	std::unique_ptr<uint8_t> retrieveRawData(size_type position, size_type size);
	std::vector<uint8_t> retrieveRawBuffer(size_type position, size_type size);

protected:
	size_type lastReadOffset_;

private:
	std::vector<unsigned char> buffer_;
};

// OPTIMIZATION NOTE : Theres two major optimizations we can perform on this class. First, the most evident one is to buffer the call.
// Then, we can have one reader by file (not multiplicating readers), by having a static undordered_map of readers, and doing a lookup each time we want to access
// a file. Supposedly, the lookup will be negated by the read, but this requires benchmark.

template<BufferPolicy flag>
FileValueReaderBase<flag>::FileValueReaderBase(const std::string& filename, std::ios_base::openmode flags) : Base(filename, flags) 
{
	//buffer_.reserve(32768);
}

// TODO : Remove maybe ?
template<BufferPolicy flag>
std::unique_ptr<unsigned char> FileValueReaderBase<flag>::retrieveRawData(size_type position, size_type size)
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

template<BufferPolicy flag>
std::vector<unsigned char> FileValueReaderBase<flag>::retrieveRawBuffer(size_type position, size_type size)
{
	std::vector<unsigned char> buffer;
	buffer.resize(size);

	if (fstream_.seekg(position, fstream_.beg))
	{
		std::copy_n(std::istream_iterator<unsigned char>{fstream_}, size, buffer.begin());
	}
	else
	{
		throw std::ios_base::failure((std::string("Error : failed to read the file ") + getCurrentFileName().c_str()));
	}

	return buffer;
}

template<BufferPolicy flag, Endianess endian = Endianess::little>
class FileValueReader;

template<BufferPolicy flag>
class __declspec(dllexport) FileValueReader<flag, Endianess::little> : public FileValueReaderBase<flag>
{
public:
	FileValueReader(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary) : FileValueReaderBase(filename, flags)
	{}

	size_type retrieveValue(size_type position, size_type size)
	{
		if (size > sizeof(size_type))
		{
			// Throwing necessary here ?
			throw std::ios_base::failure("Error : attempt to read to much bytes at once. Check the maximal value of a field on your platform");
		}
		if (fstream_.seekg(position, fstream_.beg))
		{
			unsigned char value[sizeof(size_type)]{};
			size_type out = 0;
			fstream_.read(reinterpret_cast<char*>(value), size);
			for (int64_t i = size - 1; i >= 0; --i)
			{
				out |= (value[i] << (i * 8));
			}
			return out;
		}
		return 0;
	}
};

template<BufferPolicy flag>
class __declspec(dllexport) FileValueReader<flag, Endianess::big> : public FileValueReaderBase<flag>
{
public:
	FileValueReader(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary) : FileValueReaderBase(filename, flags)
	{}

	size_type retrieveValue(size_type position, size_type size)
	{
		if (size > sizeof(size_type))
		{
			throw std::ios_base::failure("Error : attempt to read to much bytes at once. Check the maximal value of a field on your platform");
		}
		if (fstream_.seekg(position, fstream_.beg))
		{
			unsigned char value[sizeof(size_type)]{};
			size_type out = 0;
			fstream_.read(reinterpret_cast<char*>(value), size);
			for (uint64_t i = 0; i < size; ++i)
			{
				out |= (value[i] << (i * 8));
			}
			return out;
		}
		return 0;
	}
};

using FileReader = FileValueReader<BufferPolicy::buffered, Endianess::little>;