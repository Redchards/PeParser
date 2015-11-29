#pragma once

#include <Configuration.h>
#include <FileStream.h>

#include <array>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

// Maybe should move all enums to Configuration.h header
// NOTE : This is the endianess of the file as one might want to analyze a file with a different endianess.
// However, PE/COFF files should always be little-endian (Windows is only present on little-endian platforms, except WinCE)

enum class BufferPolicy : flag_type
{
	nonBuffered = false,
	buffered = true
};

class __declspec(dllexport)  FileValueReaderBase : public FileStreamBase<StreamGoal::read>
{
	using Base = FileStreamBase<StreamGoal::read>;

protected:
	FileValueReaderBase(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary);

public:
	std::unique_ptr<uint8_t> retrieveRawData(std::streampos position, size_type size);
	std::vector<uint8_t> retrieveRawBuffer(std::streampos position, size_type size);
};

template<Endianess endian = Endianess::little>
class FileValueReader;

template<>
class __declspec(dllexport) FileValueReader<Endianess::little> : public FileValueReaderBase
{
public:
	FileValueReader(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary) : FileValueReaderBase(filename, flags)
	{}

	size_type retrieveValue(std::streampos position, size_type size)
	{
		if (size > sizeof(size_type))
		{
			// Throwing necessary here ?
			throw std::ios_base::failure("Error : attempt to read to much bytes at once. Check the maximal value of a field on your platform");
		}
		std::array<unsigned char, sizeof(size_type)> value{};
		size_type out = 0;
		read(value, position, size);

		for (int64_t i = size - 1; i >= 0; --i)
		{
			out |= (value[i] << (i * 8));
		}
		return out;
	}
};

template<>
class __declspec(dllexport) FileValueReader<Endianess::big> : public FileValueReaderBase
{
public:
	FileValueReader(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary) : FileValueReaderBase(filename, flags)
	{}

	size_type retrieveValue(std::streampos position, size_type size)
	{
		if (size > sizeof(size_type))
		{
			throw std::ios_base::failure("Error : attempt to read to much bytes at once. Check the maximal value of a field on your platform");
		}
		std::array<unsigned char, sizeof(size_type)> value{};
		size_type out = 0;
		read(value, position, size);

		for (uint64_t i = 0; i < size; ++i)
		{
			out |= (value[i] << (i * 8));
		}
		return out;
	}
};

using FileReader = FileValueReader<Endianess::little>;