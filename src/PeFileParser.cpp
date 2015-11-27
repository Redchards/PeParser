#include <stdafx.h>

#include <PEFileParser.h>

PEFileParser::PEFileParser(const std::string& filename) : PEHeaderParser(filename)
{

};

PEFileParser::~PEFileParser()
{}

void PEFileParser::init()
{

}