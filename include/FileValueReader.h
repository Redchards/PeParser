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


class __declspec(dllexport)  FileValueReaderBase : public FileStreamBase<StreamGoal::read>
{
	using Base = FileStreamBase<StreamGoal::read>;

protected:
	FileValueReaderBase(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in | std::ios::binary);

public:
	std::unique_ptr<uint8_t> retrieveRawData(size_type position, size_type size);
	std::vector<uint8_t> retrieveRawBuffer(size_type position, size_type size);
};

template<Endianess endian = Endianess::little>
class FileValueReader;

template<>
class __declspec(dllexport) FileValueReader<Endianess::little> : public FileValueReaderBase
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

template<>
class __declspec(dllexport) FileValueReader<Endianess::big> : public FileValueReaderBase
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

using FileReader = FileValueReader<Endianess::little>;