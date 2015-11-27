#include <stdafx.h>

#include <BasicParser.h>

BasicParser::BasicParser(BasicParser&& other) : reader_(std::move(other.reader_)),
												events_(std::move(other.events_))
{}

BasicParser::BasicParser(std::string const& filename) : reader_(filename, std::ios::in | std::ios::binary),
														events_{}
{}

void BasicParser::loadFile(std::string const& filename)
{
	reader_.loadFile(filename);
	for (auto event : events_)
	{
		event();
	}
}

void BasicParser::unloadFile()
{
	reader_.unloadFile();
}

std::string BasicParser::getFileName() const noexcept
{
	return reader_.getCurrentFileName();
}

