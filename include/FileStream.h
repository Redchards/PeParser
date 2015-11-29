#pragma once

#include <Configuration.h>

#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

enum class StreamGoal : flag_type
{
	read,
	write
};

// Selector, allowing us to add new class easily, along with stream goals

template<StreamGoal goal>
struct FileStreamSelector;

template<>
class FileStreamSelector<StreamGoal::read>
{
public:
	using type = std::ifstream;

public:
	FileStreamSelector(const std::string& filename, std::ios::ios_base::openmode flags) : fstream_(filename, flags)
	{}

	void read(char* buffer, std::streampos position, size_type size)
	{
		goTo(position);
		fstream_.read(buffer, size);
	}

	void read(unsigned char* buffer, std::streampos position, size_type size)
	{
		read(reinterpret_cast<char*>(buffer), position, size);
	}

	template<size_type n>
	void read(std::array<char, n>& buffer, std::streampos position, size_type size)
	{
		read(buffer.data(), position, size);
	}

	template<size_type n>
	void read(std::array<unsigned char, n>& buffer, std::streampos position, size_type size)
	{
		read(buffer.data(), position, size);
	}

	void read(std::vector<char>& buffer, std::streampos position, size_type size)
	{
		read(buffer.data(), position, size);
	}

	void read(std::vector<unsigned char>& buffer, std::streampos position, size_type size)
	{
		read(buffer.data(), position, size);
	}

	void rewind()
	{
		goTo(0);
	}

	void goTo(std::streampos position)
	{
		if (!fstream_.seekg(position))
		{
			throw std::ios_base::failure("Error when processing the file !");
		}
	}

	std::streampos getCurrentPosition()
	{
		return fstream_.tellg();
	}

protected:
	type fstream_;
};

template<>
class FileStreamSelector<StreamGoal::write>
{
public:
	using type = std::ofstream;

public:
	FileStreamSelector(const std::string& filename, std::ios::ios_base::openmode flags) : fstream_(filename, flags)
	{}

	void write(char* buffer, std::streampos position, size_type size)
	{
		goTo(position);
		fstream_.write(buffer, size);
	}

	void write(unsigned char* buffer, std::streampos position, size_type size)
	{
		write(reinterpret_cast<char*>(buffer), position, size);
	}
	template<size_type n>
	void write(std::array<char, n>& buffer, std::streampos position, size_type size)
	{
		write(buffer.data(), position, size);
	}

	template<size_type n>
	void write(std::array<unsigned char, n>& buffer, std::streampos position, size_type size)
	{
		write(buffer.data(), position, size);
	}

	void write(std::vector<char>& buffer, std::streampos position, size_type size)
	{
		write(buffer.data(), position, size);
	}

	void write(std::vector<unsigned char>& buffer, std::streampos position, size_type size)
	{
		write(buffer.data(), position, size);
	}


	void rewind()
	{
		goTo(0);
	}

	void goTo(std::streampos position)
	{
		if (!fstream_.seekp(position))
		{
			throw std::ios_base::failure("Error when processing the file !");
		}
	}

	std::streampos getCurrentPosition()
	{
		return fstream_.tellp();
	}

protected:
	type fstream_;
};

template<StreamGoal goal>
class FileStreamBase : public FileStreamSelector<goal>
{
protected:
	FileStreamBase(const std::string& filename, std::ios::ios_base::openmode flags) : FileStreamSelector(filename, flags), 
																					  filename_(filename)
	{
		init();
	}

public:
	void loadFile(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in)
	{
		unloadFile();
		fstream_.open(filename, flags);
		filename_ = filename;
		init();
	}

	void unloadFile()
	{
		if (fstream_.is_open())
		{
			fstream_.close();
			filename_ = "";
		}
	}

	const std::string& getCurrentFileName() const noexcept
	{
		return filename_;
	}

private:
	void init()
	{
		if (!fstream_.good())
		{
			throw std::ios_base::failure((std::string("Error : failed to open the file ") + getCurrentFileName() + ". Please check that the file exists, and is a valid Win32 file !").c_str());
		}
		// So that stream will not interpret "whitespace" bytes as real whitespaces, and remove them.
		fstream_.unsetf(std::ios::skipws);
	}

private:
	std::string filename_;
	static std::unordered_map<std::string, typename FileStreamSelector<goal>::type> fileStreamMap_;
};