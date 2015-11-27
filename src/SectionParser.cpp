#include "stdafx.h"

#include <SectionParser.h>

SectionParser::SectionParser(const std::string& filename) : SectionHeaderParser(filename)
{}

SectionParser::SectionParser(PEHeaderParser& parser) : SectionHeaderParser(parser)
{}

std::vector<unsigned char> SectionParser::retrieveSectionData(size_type index)
{
	size_type addr = retrieveFieldValue(index, SectionHeaderField::PointerToRawData);
	size_type size = retrieveFieldValue(index, SectionHeaderField::SizeOfRawData);

	return reader_.retrieveRawBuffer(addr, size);
}