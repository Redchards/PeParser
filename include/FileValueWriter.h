#pragma once

#include <Configuration.h>
#include <FileStream.h>

#include <fstream>
#include <string>
#include <vector>

class FileValueWriterBase : public FileStreamBase<StreamGoal::write>
{
	using Base = FileStreamBase<StreamGoal::write>;

protected:
	FileValueWriterBase(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::out);

	void checkWrite() const;

public:
	void writeRawDataAt(const std::vector<char>& data, std::streampos position);
	void writeRawDataAt(const char* const data, std::streamsize size, std::streampos position);
};

template<Endianess endian = Endianess::little>
class FileValueWriter;

template<>
class FileValueWriter<Endianess::little> : public FileValueWriterBase
{
public:
	FileValueWriter(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::out) : FileValueWriterBase(filename, flags)
	{}

	void writerValue(const std::vector<char>& data, std::streampos position)
	{
		
	}

	void writeValue(const char* const data, size_type size, std::streampos position)
	{

	}
};