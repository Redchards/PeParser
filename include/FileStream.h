#pragma once

#include <Configuration.h>

#include <fstream>
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
struct FileStreamSelector<StreamGoal::read>
{
	using type = std::ifstream;
};

template<>
struct FileStreamSelector<StreamGoal::write>
{
	using type = std::ofstream;
};

template<StreamGoal goal>
class FileStreamBase
{
protected:
	FileStreamBase(const std::string& filename, std::ios::ios_base::openmode flags = std::ios_base::in) : filename_(filename),
																										  fstream_(filename, flags)
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

	void rewind()
	{
		fstream_.seekg(0, fstream_.beg);
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

	void init()
	{
		if (!fstream_.good())
		{
			throw std::ios_base::failure("Error when opening the file ! Please check that the file exists and is a valid Win32 file");
		}
		// So that stream will not interpret "whitespace" bytes as real whitespaces, and remove them.
		fstream_.unsetf(std::ios::skipws);
		buffer_.reserve(32768);
		//fstream_.rdbuf()->pubsetbuf(0, 0);
	}

private:
	std::string filename_;
	std::vector<unsigned char> buffer_;
	static std::unordered_map<std::string, typename FileStreamSelector<goal>::type> fileStreamMap_;

protected:
	typename FileStreamSelector<goal>::type fstream_;
};