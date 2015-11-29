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
struct FileStreamSelector<StreamGoal::read>
{
	using type = std::ifstream;
};

template<>
struct FileStreamSelector<StreamGoal::write>
{
	using type = std::ofstream;
};

class FileBufferBase
{
	using iterator = std::vector<unsigned char>::iterator;
	using const_iterator = std::vector<unsigned char>::const_iterator;
	using reverse_iterator = std::vector<unsigned char>::reverse_iterator;
	using const_reverse_iterator = std::vector<unsigned char>::const_reverse_iterator;

	FileBufferBase() : buffer_(32768)
	{}
	FileBufferBase(size_type bufferSize) : buffer_(bufferSize)
	{}

	iterator begin() noexcept { return buffer_.begin(); }
	const_iterator cbegin() noexcept { return buffer_.cbegin(); }
	reverse_iterator rbegin() noexcept { return buffer_.rbegin(); }
	const_reverse_iterator crbegin() noexcept { return buffer_.crbegin(); }

	iterator end() noexcept { return buffer_.end(); }
	const_iterator cend() noexcept { return buffer_.cend(); }
	reverse_iterator rend() noexcept { return buffer_.rend(); }
	const_reverse_iterator crend() noexcept { return buffer_.crend(); }

private:
	std::vector<unsigned char> buffer_;
};

template<StreamGoal goal,
		 typename = typename std::enable_if<goal == StreamGoal::read>::type>
struct FileBuffer
{

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

	void read(char* buffer, std::streampos position, size_type size)
	{
		if (!getCurrentPosition() <= position || !(static_cast<size_type>(position - getCurrentPosition()) + size) <= 32768)
		{
			std::copy_n(std::istream_iterator<unsigned char>{fstream_}, 32768, buffer_.begin());
		}
		std::copy(buffer_.begin(), buffer_.begin() + static_cast<size_type>(position - getCurrentPosition()), buffer);
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

private:
	void init()
	{
		if (!fstream_.good())
		{
			throw std::ios_base::failure("Error when opening the file ! Please check that the file exists and is a valid Win32 file");
		}
		// So that stream will not interpret "whitespace" bytes as real whitespaces, and remove them.
		fstream_.unsetf(std::ios::skipws);
		fstream_.rdbuf()->pubsetbuf(0, 0);
		buffer_.reserve(32768);

		/*if (goal == StreamGoal::read)
		{
			std::copy_n(std::istream_iterator<unsigned char>{fstream_}, 32768, buffer_.begin());
		}*/
	}

private:
	std::string filename_;
	std::vector<unsigned char> buffer_;
	static std::unordered_map<std::string, typename FileStreamSelector<goal>::type> fileStreamMap_;

protected:
	typename FileStreamSelector<goal>::type fstream_;
};