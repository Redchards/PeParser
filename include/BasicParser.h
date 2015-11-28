#pragma once

#include <FileValueReader.h>

#include <vector>
#include <functional>
#include <string>

class __declspec(dllexport)  BasicParser
{
public:
	BasicParser(BasicParser&& other);
	BasicParser(std::string const& filename);
	void loadFile(std::string const& filename);
	void unloadFile();
	std::string getFileName() const noexcept;
	template<typename T>
	bool hasFlag(size_type allFlags, T flag) const;

protected:
	FileValueReader<Endianess::little> reader_;
	std::vector<std::function<void()>> events_;
};

template<typename T>
bool BasicParser::hasFlag(size_type allFlags, T flag) const
{
	return ((allFlags & static_cast<size_type>(flag)) == (static_cast<size_type>(flag)));
}